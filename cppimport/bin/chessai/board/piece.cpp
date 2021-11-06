#include "piece.h"

const Position& Piece::getPosition() const {
  return this->pos_;
}

Piece::Piece(const Position& pos, PieceType piece_type_, PieceColor piece_color)
    : pos_(pos), piece_type_(piece_type_), piece_color_(piece_color) {}

PieceType Piece::getType() const {
  return this->piece_type_;
}

PieceColor Piece::getPieceColor() const {
  return piece_color_;
}
void Piece::setType(PieceType type) {
  piece_type_ = type;
}

std::string Piece::toStr() const {
  char letter;
  switch (piece_type_) {
    case PieceType::tKING:letter = 'k';
      break;
    case PieceType::tQUEEN:letter = 'q';
      break;
    case PieceType::tPONE:letter = 'p';
      break;
    case PieceType::tHORSE:letter = 'h';
      break;
    case PieceType::tRUCK:letter = 'r';
      break;
    case PieceType::tBISHOP:letter = 'b';
      break;
    case PieceType::tNONE:letter = '_';
  };
  if (piece_color_ == PieceColor::WHITE && piece_type_ != PieceType::tNONE)
    letter = std::toupper(letter);
  return std::string("") + letter;
}
void Piece::setPieceColor(PieceColor piece_color) {
  piece_color_ = piece_color;
}

void Piece::setPosition(const Position& pos) {
  pos_ = pos;
}
