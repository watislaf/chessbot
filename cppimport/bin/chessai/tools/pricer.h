#ifndef PRICER_H
#define PRICER_H

#include "memory"
#include <board/piece.h>
#include <board/board.h>

class Pricer {
 public:
  static int getPrice(const std::shared_ptr<const Piece>& piece) ;

    static int count(const Board& board, const Move& move);
};

#endif //PRICER_H
