#include "move.h"

Move::Move(const Position& from, const Position& to)
    : start_(from), end_(to) {}

Move::Move(const Piece& from, const Position& to)
    : start_(from.getPosition()), end_(to) {}
int Move::GetPriceOfEndPiece() const {
  return price_of_end_piece_;
}
void Move::SetPriceOfEndPiece(int price_of_end_piece) {
  price_of_end_piece_ = price_of_end_piece;
}
bool Move::IsCanMakeNewFigure() const {
  return can_make_new_figure;
}
void Move::SetCanMakeNewFigure(bool can_make_new_figure) {
  this->can_make_new_figure = can_make_new_figure;
}
bool Move::IsBrakeLeftCastle() const {
  return brake_left_castle_;
}
void Move::SetBrakeLeftCastle(bool brake_left_castle) {
  Move::brake_left_castle_ = brake_left_castle;
}
bool Move::IsBrakeFeltCastle() const {
  return brake_right_castle_;
}
void Move::SetBrakeRightCastle(bool brake_felt_castle) {
  this->brake_right_castle_ = brake_felt_castle;
}
void Move::SetIsCastle(bool b) {
  if (b) {
    SetBrakeRightCastle(true);
    SetBrakeLeftCastle(true);
  }
  this->is_castle = b;
}
bool Move::IsCastle() const {
  return is_castle;
}
bool Move::IsDoubleDistancePone() const {
  return is_double_distance_pone;
}
void Move::SetIsDoubleDistancePone(bool double_distance_pone) {
  this->is_double_distance_pone = double_distance_pone;
}
bool Move::IsPassant() const {
  return is_passant;
}
void Move::SetIsPassant(bool is_passant) {
  this->is_passant = is_passant;
}
const Position& Move::GetStart() const {
  return start_;
}
void Move::SetStart(const Position& start) {
  start_ = start;
}
const Position& Move::GetEnd() const {
  return end_;
}
void Move::SetEnd(const Position& end) {
  end_ = end;
}

