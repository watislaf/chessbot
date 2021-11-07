#ifndef MOVE_H
#define MOVE_H

#include <board/piece.h>
#include <string>
#include "position.h"

class Move {
 public:
  Move(const Piece& from, const Piece& to);

  void setIsDoubleDistancePone(bool double_distance_pone, int prev_passant);
  void setCanMakeNewFigure(bool can_make_new_figure);
  void setBrakeRightCastle(bool brake_felt_castle);
  void setBrakeLeftCastle(bool brake_left_castle);
  void setNewPieceType(PieceType new_piece_type);
  void setAttackPrice(int attack_price);
  void setDefendPrice(int defend_price);
  void setIsPassant(bool is_passant);
  void setStart(const Piece& start);
  bool IsDoubleDistancePone() const;
  PieceType getNewPieceType() const;
  bool isCanMakeNewFigure() const;
  bool isBrakeRightCastle() const;
  bool isBrakeLeftCastle() const;
  bool isBrakeFeltCastle() const;
  const Piece& getStart() const;
  void setEnd(const Piece& end);
  const Piece& getEnd() const;
  int getAttackPrice() const;
  int getDefendPrice() const;
  void setIsCastle(bool b);
  bool isPassant() const;
  bool isCastle() const;

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
