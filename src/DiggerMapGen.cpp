//
// Created by Martin Galese on 2/16/16.
//

#include <boost/range/irange.hpp>
#include "DiggerMapGen.hpp"

void Digger::dig(const int turnCount) {
  if(!alive)
    return;
  mapgen->map[position.x][position.y] = Element::Floor;
  switch(direction) {
    case Direction::Up:
      position.y--;
      break;
    case Direction::Down:
      position.y++;
      break;
    case Direction::Left:
      position.x--;
      break;
    case Direction::Right:
      position.x++;
      break;
    default:
      break;
  }
  if(position.x < 1 || position.y < 1 || position.x >= mapgen->width-1 || position.y >= mapgen->height-1)
    alive=false;
//  if(mapgen->map[position.x][position.y] == Element::Floor)
//    alive=false;

  if(turnsSinceDirection >= maxTurnsSinceDirection) {
    direction = Direction(mapgen->world->rnd->getInt(0, (int)Direction::Count-1));
    turnsSinceDirection = 0;
  } else
    turnsSinceDirection++;

  if(turnsSinceRoom >= maxTurnsSinceRoom) {
    mapgen->buildRoom(position, 5, 5);
    turnsSinceRoom = 0;
  } else
    turnsSinceRoom++;
}

void DiggerMapGen::init() {
  for(auto i : boost::irange(0, 5)) {
    auto digger = std::make_shared<Digger>(this, int(world->currLevel->width / 2), int(world->currLevel->height / 2));
    digger->maxTurnsSinceDirection = 5;
    diggers.push_back(digger);
    //diggers.push_back(std::make_shared<Digger>(this));
  }

  for(auto turnCount : boost::irange(0, 100)) {
    for(auto digger : diggers) {
      digger->dig(turnCount);
    }
  }
}
