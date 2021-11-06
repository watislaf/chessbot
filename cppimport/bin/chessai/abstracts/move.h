#ifndef MOVE_H
#define MOVE_H

#include <board/piece.h>
#include <string>
#include "position.h"

class Move {
 public:
  Move(const Piece& from, const Piece& to);

  void SetIsCastle(bool b);
  bool isCanMakeNewFigure() const;
  void SetCanMakeNewFigure(bool can_make_new_figure);
  bool IsBrakeLeftCastle() const;
  void SetBrakeLeftCastle(bool brake_left_castle);
  bool IsBrakeFeltCastle() const;
  void SetBrakeRightCastle(bool brake_felt_castle);
  bool IsCastle() const;
  bool IsDoubleDistancePone() const;
  void SetIsDoubleDistancePone(bool double_distance_pone, int prev_passant);
  bool IsPassant() const;
  void SetIsPassant(bool is_passant);
  const Piece& getStart() const;
  void SetStart(const Piece& start);
  const Piece& getEnd() const;
  void SetEnd(const Piece& end);
  int GetAttackPrice() const;
  void SetAttackPrice(int attack_price);
  int GetDefendPrice() const;
  void SetDefendPrice(int defend_price);
  bool IsBrakeRightCastle() const;
  PieceType GetNewPieceType() const;
  void SetNewPieceType(PieceType new_piece_type);

  int PrevPassant() const;
  std::string toStr() const;
 private:
  Piece start_;
  Piece end_;

  int attack_price_{};// 1 3 3 4 7 1000
  int defend_price_{};// 1 3 3 4 7 1000

  bool brake_left_castle_ = false; // -1 LC_ 1 RC_
  bool brake_right_castle_ = false;
  bool is_castle_ = false;

  bool is_double_distance_pone_ = false;
  bool is_passant_ = false;
  int prev_passant_ = -1;
  // -1 LC_ 1 RC_
  bool can_make_new_figure_ = false; // new_figure
  PieceType new_piece_type = PieceType::tNONE;
  // new_figure

};

#endif //MOVE_H
