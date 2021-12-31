#include <tools/pricer.h>
#include "move.h"

Move::Move(const std::shared_ptr<const Piece>& from,
           const std::shared_ptr<const Piece>& to) : start_(from), end_(to) {
  if (to->getType() != PieceType::NONE) {
    if (to->getPieceColor() == from->getPieceColor()) {
      defend_price_ = Pricer::getPrice(to);
    } else {
      attack_price_ = Pricer::getPrice(to);
    }
  }
}

void Move::setIsCastle(bool is_castle) {
  this->is_castle_ = is_castle;
}

void Move::setIsDoubleDistancePone(bool double_distance_pone,
                                   int prev_passant) {
  this->is_double_distance_pone_ = double_distance_pone;
  if (double_distance_pone) {
    this->prev_passant_ = prev_passant;
  }
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

bool Move::isDoubleDistancePone() const {
  return is_double_distance_pone_;
}

bool Move::isPassant() const {
  return is_passant_;
}

void Move::setIsPassant(bool is_passant) {
  this->is_passant_ = is_passant;
}

std::shared_ptr<const Piece> Move::getStart() const {
  return start_;
}

std::shared_ptr<const Piece> Move::getEnd() const {
  return end_;
}

int Move::getAttackScore() const {
  return attack_price_;
}

void Move::setAttackPrice(int attack_price) {
  attack_price_ = attack_price;
}

int Move::getDefendScore() const {
  return defend_price_;
}

void Move::setDefendScore(int defend_price) {
  defend_price_ = defend_price;
}

bool Move::isBrakeRightCastle() const {
  return brake_right_castle_;
}

std::string Move::toStr() const {
  std::string answer = std::string("(") +
      getStart()->getPosition().toStr() + ","
      + getEnd()->getPosition().toStr();
  if (new_piece_type != PieceType::NONE) {
    answer += ",";
    switch (new_piece_type) {
      case PieceType::KING:answer += "k";
        break;
      case PieceType::QUEEN:answer += "q";
        break;
      case PieceType::KNIGHT:answer += "n";
        break;
      case PieceType::RUCK:answer += "r";
        break;
      case PieceType::BISHOP:answer += "b";
        break;
      default: answer += "x";
    }
  }
  answer += ")";
  return answer;
}
short Move::getPrevPassant() const {
  return prev_passant_;
}
void Move::setPrevPassant(short prev_passant) {
  prev_passant_ = prev_passant;
}
PieceType Move::getNewPieceType() const {
  return new_piece_type;
}
void Move::setNewPieceType(PieceType new_piece_type) {
  Move::new_piece_type = new_piece_type;
}
