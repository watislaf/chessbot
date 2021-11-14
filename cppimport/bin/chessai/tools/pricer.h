#ifndef PRICER_H
#define PRICER_H

#include "memory"
#include <board/piece.h>
#include <board/board.h>

class Pricer {
 public:
  static int getPrice(const std::shared_ptr<const Piece>& piece);
  static int getPrice(PieceType type);

  static int count(const std::shared_ptr<Board>& board, const Move& move);
  static int defendScore(const Move& move);
};

#endif //PRICER_H
