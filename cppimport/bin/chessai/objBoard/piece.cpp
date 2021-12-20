#include "piece.h"

const Position& Piece::getPosition() const {
  return this->pos_;
}

Piece::Piece(const Position& pos, PieceType piece_type_, ColorType piece_color)
    : pos_(pos), piece_type_(piece_type_), piece_color_(piece_color) {}

PieceType Piece::getType() const {
  return this->piece_type_;
}

ColorType Piece::getPieceColor() const {
  return piece_color_;
}
void Piece::setType(PieceType type) {
  piece_type_ = type;
}

std::string Piece::toStr() const {
  std::string letter;
  switch (piece_type_) {
    case PieceType::tKING:letter = 'k';
      break;
    case PieceType::tQUEEN:letter = 'q';
      break;
    case PieceType::tPAWN:letter = 'p';
      break;
    case PieceType::tNIGHT:letter = 'n';
      break;
    case PieceType::tRUCK:letter = 'r';
      break;
    case PieceType::tBISHOP:letter = 'b';
      break;
    case PieceType::tNONE:letter = '_';
  };
  if (piece_color_ == ColorType::WHITE && piece_type_ != PieceType::tNONE) {
    letter = std::string("w") + std::string(letter);
  }
  if (piece_color_ == ColorType::BLACK && piece_type_ != PieceType::tNONE) {
    letter = std::string("b") + std::string(letter);
  }
  return std::string("") + letter;
}

void Piece::setPieceColor(ColorType piece_color) {
  piece_color_ = piece_color;
}

bool Piece::operator==(const Piece& other) const {
  return piece_color_ == other.piece_color_ &&
      piece_type_ == other.piece_type_ &&
      pos_ == other.pos_;
}

bool Piece::operator!=(const Piece& other) const {
  return !(*this == other);
}
bool Piece::isEnemyTo(const Piece& other) const {
  if (other.getType() == PieceType::tNONE) {
    return false;
  }
  return piece_color_ != other.piece_color_;
}
