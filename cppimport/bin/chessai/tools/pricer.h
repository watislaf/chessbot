#ifndef PRICER_H
#define PRICER_H

#include "memory"
#include "movesGenerator.h"
#include <board/piece.h>
#include <board/board.h>

class Pricer {
 public:
  static int getPrice(const std::shared_ptr<const Piece>& piece);
  static int getPrice(PieceType type);

  int countOrder(const std::shared_ptr<Board>& board, const Move& move);
  static int defendScore(const Move& move);
  double endGameCoef(const std::shared_ptr<const Board>& board);
  int valOnBoard(Position pos, bool is_white,
                 const std::shared_ptr<const Board>& board, PieceType type);

  short PawnTable[64]{
      90,90, 90, 90, 90, 90, 90, 90,
      50, 50, 50, 50, 50, 50, 50, 50,
      20, 20, 30, 40, 40, 30, 20, 20,
      5, 5, 10, 30, 30, 10, 5, 5,
      0, 0, 0, 25, 25, 0, 0, 0,
      5, -5, -10, 0, 0, -10, -5, 5,
      5, 10, 10, -30, -30, 10, 10, 5,
      0, 0, 0, 0, 0, 0, 0, 0
  };

  short KnightTable[64]{
      -50, -40, -30, -30, -30, -30, -40, -50,
      -40, -20, 0, 0, 0, 0, -20, -40,
      -30, 0, 10, 15, 15, 10, 0, -30,
      -30, 5, 15, 20, 20, 15, 5, -30,
      -30, 0, 15, 20, 20, 15, 0, -30,
      -30, 5, 10, 15, 15, 10, 5, -30,
      -40, -20, 0, 5, 5, 0, -20, -40,
      -50, -30, -20, -30, -30, -20, -30, -50,
  };

  short BishopTable[64]{
      -20, -10, -10, -10, -10, -10, -10, -20,
      -10, 0, 0, 0, 0, 0, 0, -10,
      -10, 0, 5, 10, 10, 5, 0, -10,
      -10, 5, 5, 10, 10, 5, 5, -10,
      -10, 0, 10, 10, 10, 10, 0, -10,
      -10, 10, 10, 10, 10, 10, 10, -10,
      -10, 5, 0, 0, 0, 0, 5, -10,
      -20, -10, -40, -10, -10, -40, -10, -20,
  };

  short KingTable[64]{
      -30, -40, -40, -50, -50, -40, -40, -30,
      -30, -40, -40, -50, -50, -40, -40, -30,
      -30, -40, -40, -50, -50, -40, -40, -30,
      -30, -40, -40, -50, -50, -40, -40, -30,
      -20, -30, -30, -40, -40, -30, -30, -20,
      -10, -20, -20, -20, -20, -20, -20, -10,
      20, 20, 0, 0, 0, 0, 20, 20,
      20, 30, 10, 0, 0, 10, 30, 20
  };

  short KingTableEndGame[64]{
      -50, -40, -30, -20, -20, -30, -40, -50,
      -30, -20, -10, 0, 0, -10, -20, -30,
      -30, -10, 20, 30, 30, 20, -10, -30,
      -30, -10, 30, 40, 40, 30, -10, -30,
      -30, -10, 30, 40, 40, 30, -10, -30,
      -30, -10, 20, 30, 30, 20, -10, -30,
      -30, -30, 0, 0, 0, 0, -30, -30,
      -50, -30, -30, -30, -30, -30, -30, -50
  };
};

#endif //PRICER_H
