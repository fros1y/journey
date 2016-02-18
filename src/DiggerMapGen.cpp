//
// Created by Martin Galese on 2/16/16.
//

#include <boost/range/irange.hpp>
#include "DiggerMapGen.hpp"

void CorridorDigger::dig(const int turnCount) {
  if(!alive)
    return;
  //mapgen->rectFill(position, 2, 2, Element::Floor, true);
  place(Element::Floor);
  stepDirection();
  if(outOfBounds(buffer))
  {
    alive = false;
  }

  auto mustTurnTest = turnsSinceDirection >= maxTurnsSinceDirection;
  auto randomTurn = false;

  if(turnsSinceDirection >= minTurnsSinceDirection) {
    auto roll = mapgen->world->rnd->getDouble(0, 1);
    randomTurn = roll < turnOdds;
  }

  if(mustTurnTest || randomTurn) {
    direction = Direction(mapgen->world->rnd->getInt(0, (int)Direction::Count-1));
    turnsSinceDirection = 0;
  } else
    turnsSinceDirection++;
}

void DiggerMapGen::init() {
  for(auto i : boost::irange(0, 5)) {
    auto digger = std::make_shared<CorridorDigger>(this, int(world->currLevel->width / 2), int(world->currLevel->height / 2));
    diggers.push_back(digger);
  }

  for(auto turnCount : boost::irange(0, 100)) {
    for(auto digger : diggers) {
      digger->dig(turnCount);
    }
  }

  for(auto digger : diggers) {
    buildRoom(digger->position, 10, 10, true);
  }
}
