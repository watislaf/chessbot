#ifndef CHESS_PIECE_H
#define CHESS_PIECE_H

#include "abstracts/position.h"
#include "abstracts/pieceColor.h"
#include "abstracts/pieceType.h"

class Piece {
 public:
  explicit Piece(const Position& pos = Position(9, 9),
                 PieceType piece_type_ = PieceType::NONE,
                 ColorType piece_color = ColorType::WHITE);
  ColorType getPieceColor() const;
  PieceType getType() const;

  std::string toStr() const;
  const Position& getPosition() const;
  void setType(PieceType type);
  void setPieceColor(ColorType piece_color);
  bool isEnemyTo(const Piece& other) const;
  bool operator==(const Piece& other) const;
  bool operator!=(const Piece& other) const;

 private:
  ColorType piece_color_;
  PieceType piece_type_;

  Position pos_;
};

#endif // CHESS_PIECE_H
