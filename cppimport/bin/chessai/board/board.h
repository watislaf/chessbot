#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H

#include "piece.h"
#include <vector>

class Board {

 public:
  Piece GetPiece(const Position& position) const;
  void SetPiece(const Piece& piece);
  // Castle
  bool IsLcAvailable(const Piece& piece) const;
  bool IsRcAvailable(const Piece& piece) const;
  void BrakeLc(const Piece& piece);
  void BrakeRc(const Piece& piece);
  Position GetKingPosition(const Piece& piece) const;

  bool IsWhiteMove() const;
  bool IsBlackMove() const;
  void NextMove();
  int GetPrevLongPonMove() const;
  void SetPrevLongPonMove(int prev_long_pon_move) const;

 private:
  std::vector<std::vector<Piece>> board;
  bool is_white_move_ = true;
  int prev_long_pon_move_ = 0;

  struct Castle {
    Position king_position;
    bool LC_ = true;
    bool RC_ = true;
  } whiteCastle, blackCastle;
};

#endif // CHESS_BOARD_H
