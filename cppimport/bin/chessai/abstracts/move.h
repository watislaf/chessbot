#ifndef MOVE_H
#define MOVE_H

#include <board/piece.h>
#include "position.h"

class Move {
 public:
  Move(const Position& from, const Position& to);
  Move(const Piece& from, const Position& to);
  Move(const Piece& from, const Piece& to);

  void SetIsCastle(bool b);
  int GetPriceOfEndPiece() const;
  void SetPriceOfEndPiece(int price_of_end_piece);
  bool IsCanMakeNewFigure() const;
  void SetCanMakeNewFigure(bool can_make_new_figure);
  bool IsBrakeLeftCastle() const;
  void SetBrakeLeftCastle(bool brake_left_castle);
  bool IsBrakeFeltCastle() const;
  void SetBrakeRightCastle(bool brake_felt_castle);
  bool IsCastle() const;
  bool IsDoubleDistancePone() const;
  void SetIsDoubleDistancePone(bool double_distance_pone);
  bool IsPassant() const;
  void SetIsPassant(bool is_passant);
  const Position& GetStart() const;
  void SetStart(const Position& start);
  const Position& GetEnd() const;
  void SetEnd(const Position& end);
  int GetAttackPrice() const;
  void SetAttackPrice(int attack_price);
  int GetDefendPrice() const;
  void SetDefendPrice(int defend_price);
  bool IsBrakeRightCastle() const;


 private:
  Position start_;
  Position end_;

  int attack_price_{};// 1 3 3 4 7 1000
  int defend_price_{};// 1 3 3 4 7 1000

  bool brake_left_castle_ = false; // -1 LC_ 1 RC_
  bool brake_right_castle_ = false;
  bool is_castle = false;
 private:

  bool is_double_distance_pone = false;
  bool is_passant = false;
  // -1 LC_ 1 RC_
  bool can_make_new_figure = false; // new_figure

};

#endif //MOVE_H
