#ifndef CHESS_PIECE_H
#define CHESS_PIECE_H

#include "abstracts/position.h"
#include "abstracts/pieceColor.h"
#include "abstracts/pieceType.h"

class Piece {
 public:
  explicit Piece(const Position& pos,
                 PieceType piece_type_ = PieceType::tNONE,
                 PieceColor piece_color = PieceColor::WHITE);
  PieceColor getPieceColor() const;
  PieceType getType() const;

  std::string toStr() const;
  const Position& getPosition() const;
  void setType(PieceType type);
  void setPieceColor(PieceColor piece_color);
  void setPosition(const Position& pos);

 private:
  PieceColor piece_color_;
 private:
  PieceType piece_type_;

  Position pos_;
};

#endif // CHESS_PIECE_H
