#ifndef CHESS_PIECE_H
#define CHESS_PIECE_H

#include <abstracts/position.h>
#include <board/pieceColor.cpp>
#include <board/pieceType.cpp>

class Piece {

 public:

  explicit Piece(const Position& pos,
                 PieceType piece_type_ = PieceType::IS_NONE,
                 PieceColor piece_color = PieceColor::WHITE);
  PieceColor GetPieceColor() const;
  PieceType getType() const;

  const Position& getPosition() const;

 private:
  PieceColor piece_color_;
  PieceType piece_type_;
  Position pos_;

};

#endif // CHESS_PIECE_H
