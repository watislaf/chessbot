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
