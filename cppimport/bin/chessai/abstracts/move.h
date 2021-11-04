//
// Created by watislaf on 04/11/2021.
//

#ifndef MOVE_H
#define MOVE_H

#include "position.h"
class Move {
 public:
  Move(const Position& from, const Position& to);
 private:
  Position from;
  Position to;
};

#endif //MOVE_H
