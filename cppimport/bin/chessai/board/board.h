#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H

#include "piece.h"
#include <vector>

class Board {

 public:
  const Piece& getPiece(int x, int y);

 private:
  std::vector<std::vector<Piece>> board;
};

#endif // CHESS_BOARD_H
