#include "piece.h"

Piece::Piece(int px, int py) : pos_(px, py) {}

const Position& Piece::getPosition() const {
  return this->pos_;
}

Piece::Piece(const Position& posi) : pos_(posi) {}
