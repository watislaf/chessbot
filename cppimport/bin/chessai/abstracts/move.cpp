#include <tools/pricer.h>
#include "move.h"

Move::Move(const Piece& from, const Piece& to) : start_(from),
                                                 end_(to) {
  if (to.getType() != PieceType::tNONE) {
    if (to.getPieceColor() == from.getPieceColor()) {
      defend_price_ = Pricer::getPrice(to);
    } else {
      attack_price_ = Pricer::getPrice(to);
    }
  }
}
void Move::setIsCastle(bool b) {
  if (b) {
    setBrakeRightCastle(true);
    setBrakeLeftCastle(true);
  }
  this->is_castle_ = b;
}
void Move::setIsDoubleDistancePone(bool double_distance_pone,
                                   int prev_passant) {
  this->is_double_distance_pone_ = double_distance_pone;
  if (double_distance_pone) {
    this->prev_passant_ = prev_passant;
  }
}
bool Move::isCanMakeNewFigure() const {
  return can_make_new_figure_;
}
void Move::setCanMakeNewFigure(bool can_make_new_figure) {
  this->can_make_new_figure_ = can_make_new_figure;
}
bool Move::isBrakeLeftCastle() const {
  return brake_left_castle_;
}
void Move::setBrakeLeftCastle(bool brake_left_castle) {
  Move::brake_left_castle_ = brake_left_castle;
}
bool Move::isBrakeFeltCastle() const {
  return brake_right_castle_;
}
void Move::setBrakeRightCastle(bool brake_felt_castle) {
  this->brake_right_castle_ = brake_felt_castle;
}
bool Move::isCastle() const {
  return is_castle_;
}
bool Move::IsDoubleDistancePone() const {
  return is_double_distance_pone_;
}
bool Move::isPassant() const {
  return is_passant_;
}
void Move::setIsPassant(bool is_passant) {
  this->is_passant_ = is_passant;
}
const Piece& Move::getStart() const {
  return start_;
}
void Move::setStart(const Piece& start) {
  start_ = start;
}
const Piece& Move::getEnd() const {
  return end_;
}
void Move::setEnd(const Piece& end) {
  end_ = end;
}
int Move::getAttackPrice() const {
  return attack_price_;
}
void Move::setAttackPrice(int attack_price) {
  attack_price_ = attack_price;
}
int Move::getDefendPrice() const {
  return defend_price_;
}
void Move::setDefendPrice(int defend_price) {
  defend_price_ = defend_price;
}
bool Move::isBrakeRightCastle() const {
  return brake_right_castle_;
}
PieceType Move::getNewPieceType() const {
  return new_piece_type;
}
void Move::setNewPieceType(PieceType new_piece_type) {
  Move::new_piece_type = new_piece_type;
}
int Move::PrevPassant() const {
  return prev_passant_;
}

std::string Move::toStr() const {
  return std::string("(") + getStart().getPosition().toStr() + ","
      + getEnd().getPosition().toStr() + ")";
}

