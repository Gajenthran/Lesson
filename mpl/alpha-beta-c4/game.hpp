#ifndef GAME_H_
#define GAME_H_

#include "grid.hpp"
#include "player.hpp"

class Game {
public:
  Game(Grid g, Player player);
private:
  Grid g_;
  Player player_;
};

#endif