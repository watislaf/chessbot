#ifndef CHESS_PIECE_H
#define CHESS_PIECE_H

#include <abstracts/position.h>
#include <board/pieceType.cpp>

class Piece {

 public:
  Piece(int px, int py);
  explicit Piece(const Position& posi);

  const Position& getPosition() const;

 private:
  Position pos_;
  PieceType piece_type_;
};

#endif // CHESS_PIECE_H
