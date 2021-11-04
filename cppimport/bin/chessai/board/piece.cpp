#include "piece.h"

const Position& Piece::getPosition() const {
  return this->pos_;
}

Piece::Piece(const Position& pos, PieceType piece_type_) : pos_(pos),piece_type_(piece_type_) {}
