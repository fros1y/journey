//
// Created by Martin Galese on 2/13/16.
//

#include <libtcod.hpp>
#include "GraphMapGen.hpp"
#include "utils.h"
#include <boost/range/irange.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/random.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/graph/random_layout.hpp>
#include <boost/graph/make_connected.hpp>
#include <boost/graph/boyer_myrvold_planar_test.hpp>
#include <boost/graph/make_biconnected_planar.hpp>


#include <boost/graph/fruchterman_reingold.hpp>
#include <boost/graph/random_layout.hpp>
#include <boost/graph/kamada_kawai_spring_layout.hpp>
#include <boost/graph/circle_layout.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/point_traits.hpp>
#include <boost/random/linear_congruential.hpp>
#include <iostream>
#include <boost/limits.hpp>
#include <fstream>
#include <string>

#include <libavoid/libavoid.h>


enum vertex_position_t { vertex_position };
namespace boost { BOOST_INSTALL_PROPERTY(vertex, position); }
typedef boost::square_topology<>::point_type point;

typedef boost::adjacency_list<boost::vecS,
                              boost::vecS,
                              boost::undirectedS,
                              boost::property<boost::vertex_index_t, int,
                                              boost::property<vertex_position_t, point> >,
                              boost::property<boost::edge_weight_t, double> > Graph;

typedef boost::graph_traits<Graph>::edge_iterator edge_iterator;
typedef boost::graph_traits<Graph>::vertex_iterator vertex_iterator;


struct kamada_kawai_done {
  kamada_kawai_done() : last_delta() { }

  template<typename Graph>
  bool operator()(double delta_p,
                  typename boost::graph_traits<Graph>::vertex_descriptor /*p*/,
                  const Graph & /*g*/,
                  bool global) {
    if (global) {
      double diff = last_delta - delta_p;
      if (diff < 0) diff = -diff;
      last_delta = delta_p;
      return diff < 0.01;
    } else {
      return delta_p < 0.01;
    }
  }

  double last_delta;
};



std::vector<Position> _libavoid_storage;

void _libavoid_callback(void *ptr) {
  printf("In callback\n");
      Avoid::ConnRef *connRef = (Avoid::ConnRef *) ptr;
      const Avoid::PolyLine& route = connRef->route();
      for(auto i = 0; i < route.ps.size()-1; ++i) {
        int x = int(route.ps[i].x);
        int y = int(route.ps[i].y);
        TCODLine::init(x, y, int(route.ps[i+1].x), int(route.ps[i+1].y));

        printf("From (%i, %i) to (%i, %i)\n", x, y, int(route.ps[i+1].x), int(route.ps[i+1].y));

        do {
          _libavoid_storage.emplace_back(x, y);
        } while(!TCODLine::step(&x, &y));
      }
    }

void GraphMapGen::init() {
  std::unique_ptr<Graph> map_graph;
  boost::random::mt19937 rng;

  do {
    map_graph = std::make_unique<Graph>();
    boost::generate_random_graph(*map_graph, 20, 10, rng, true, true);
    boost::make_connected(*map_graph);
  } while (!boost::boyer_myrvold_planarity_test(*map_graph));

  boost::circle_graph_layout(*map_graph, get(vertex_position, *map_graph), 5.0);
  //boost::random_graph_layout(*map_graph, get(vertex_position, *map_graph), boost::square_topology<>(1.0));

//  bool ok = kamada_kawai_spring_layout(*map_graph,
//                                       get(vertex_position, *map_graph),
//                                       get(boost::edge_weight, *map_graph),
//                                       boost::square_topology<>(1.0),
//                                       boost::side_length(1.0),
//                                       kamada_kawai_done());

  auto positions = get(vertex_position, *map_graph);

  std::pair<edge_iterator, edge_iterator> ei = boost::edges(*map_graph);
  std::pair<vertex_iterator, vertex_iterator> vi = boost::vertices(*map_graph);

  std::map<int, std::shared_ptr<Room> > rooms;

  double min_x = 1e10;
  double max_x = 0;
  double min_y = 1e10;
  double max_y = 0;

  for (vertex_iterator it = vi.first; it != vi.second; ++it) {
    auto x = positions[*it][0];
    auto y = positions[*it][1];
    min_x = std::min(min_x, x);
    min_y = std::min(min_y, y);
    max_x = std::max(max_x, x);
    max_y = std::max(max_y, y);
  }

  auto router = std::make_unique<Avoid::Router>(Avoid::OrthogonalRouting);

  bool playerRoom = true;
  for (vertex_iterator it = vi.first; it != vi.second; ++it) {
    int x, y;

    if (playerRoom) {
      x = world->currLevel->width / 2;
      y = world->currLevel->height / 2;
      playerRoom = false;
    } else {
      x = maprange(positions[*it][0], min_x, max_x, 1, width-2);
      y = maprange(positions[*it][1], min_y, max_y, 1, height-2);
    }

    auto w = world->rnd->getInt(3, 20);
    auto h = world->rnd->getInt(w/3, w);
    auto room = std::make_shared<Room>(Position(x, y), w, h);
    rooms.insert(std::pair<int, std::shared_ptr<Room> >(*it, room));
    buildRoom(*room);
    Avoid::Rectangle roomRect(Avoid::Point(x,y), int(1.05*w), int(1.05*h));
    Avoid::ShapeRef *roomShape = new Avoid::ShapeRef(router.get(), roomRect);
  }

  for (edge_iterator it = ei.first; it != ei.second; ++it) {
    auto src = rooms[boost::source(*it, *map_graph)];
    auto dest = rooms[boost::target(*it, *map_graph)];
    Avoid::Point srcPt(src->center.x, src->center.y);
    Avoid::Point destPt(dest->center.x, dest->center.y);
    auto connRef = new Avoid::ConnRef(router.get(), srcPt, destPt);
    connRef->setCallback(&_libavoid_callback, connRef);
  }
  router->processTransaction();

  for(auto p : _libavoid_storage) {
    map[clamp(p.x, 1, width-2)][clamp(p.y, 1, width-2)] = Element::Floor;
  }
}
