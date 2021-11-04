#ifndef CHESS_PIECE_H
#define CHESS_PIECE_H

#include <abstracts/position.h>
#include <board/pieceType.cpp>

class Piece {

 public:

  explicit Piece(const Position& pos,
                 PieceType piece_type_ = PieceType::IS_NONE);

  const Position& getPosition() const;

 private:
  PieceType piece_type_;
  Position pos_;
};

#endif // CHESS_PIECE_H
