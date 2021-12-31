#ifndef MOVE_H
#define MOVE_H

#include "bitBoard/piece.h"
#include <string>
#include "position.h"
#include "memory"
class Move {
 public:
  Move(const std::shared_ptr<const Piece>& from,
       const std::shared_ptr<const Piece>& to);

  void setIsDoubleDistancePone(bool double_distance_pone, int prev_passant);
  void setBrakeRightCastle(bool brake_felt_castle);
  void setBrakeLeftCastle(bool brake_left_castle);
  void setAttackPrice(int attack_price);
  void setDefendScore(int defend_price);
  void setIsPassant(bool is_passant);
  bool isDoubleDistancePone() const;
  PieceType getNewPieceType() const;
  bool isBrakeRightCastle() const;
  bool isBrakeLeftCastle() const;
  bool isBrakeFeltCastle() const;
  std::shared_ptr<const Piece> getStart() const;
  std::shared_ptr<const Piece> getEnd() const;
  int getAttackScore() const;
  int getDefendScore() const;
  void setIsCastle(bool b);
  bool isPassant() const;
  bool isCastle() const;
  short getPrevPassant() const;
  void setPrevPassant(short prev_passant);
  std::string toStr() const;
  void setNewPieceType(PieceType new_piece_type);

 private:
  std::shared_ptr<const Piece> start_;
  std::shared_ptr<const Piece> end_;

  int attack_price_{};// 1 3 3 4 7 1000
  int defend_price_{};// 1 3 3 4 7 1000
  bool brake_left_castle_ = false; // -1 LC_is_possible_ 1 RC_is_possible_
  bool brake_right_castle_ = false;

  bool is_castle_ = false;

  bool is_double_distance_pone_ = false;
  bool is_passant_ = false;
  short prev_passant_ = -1;
  // -1 LC_is_possible_ 1 RC_is_possible_
  PieceType new_piece_type = PieceType::NONE;
  // new_figure

};

#endif //MOVE_H
