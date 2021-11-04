#ifndef MOVE_H
#define MOVE_H

#include <board/piece.h>
#include "position.h"

class Move {
 public:
  Move(const Position& from, const Position& to, int price = 0);
  Move(const Piece& from, const Position& to, int price = 0);

 private:
  Position start_;
  Position end_;
  int price_of_end_piece_;

};

#endif //MOVE_H
