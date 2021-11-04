#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H

#include "piece.h"
#include <vector>

class Board {

 public:
  Piece GetPiece(Position position) const;
  bool IsLc() const;
  void SetLc(bool lc);
  bool IsRc() const;
  void SetRc(bool rc);
  bool IsWhiteMove() const;
  void SetIsWhiteMove(bool is_white_move);
  int GetPrevLongPonMove() const;
  void SetPrevLongPonMove(int prev_long_pon_move);

 private:
  std::vector<std::vector<Piece>> board;
  bool is_white_move_ = true;
  int prev_long_pon_move_ = 0;
  bool LC_ = true;
  bool RC_ = true;
};

#endif // CHESS_BOARD_H
