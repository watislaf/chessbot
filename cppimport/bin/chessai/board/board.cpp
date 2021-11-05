#include "board.h"

Piece Board::GetPiece(const Position& position) const {
  auto posY = position.GetY();
  auto posX = position.GetX();
  if (posY < 0 || posY > 7 || posX < 0 || posX > 7) {
    return Piece(position);
  }
  return board[position.GetX()][position.GetY()];
}
bool Board::IsLcAvailable(const Piece& piece) const {
  if (piece.getPieceColor() == PieceColor::WHITE) {
    return whiteCastle.LC_;
  }
  return blackCastle.LC_;
}
void Board::BrakeLc(const Piece& piece) {
  if (piece.getPieceColor() == PieceColor::WHITE) {
    whiteCastle.LC_ = false;
  }
  blackCastle.LC_ = false;

}
void Board::BrakeRc(const Piece& piece) {
  if (piece.getPieceColor() == PieceColor::WHITE) {
    whiteCastle.RC_ = false;
  }
  blackCastle.RC_ = false;

}

bool Board::IsRcAvailable(const Piece& piece) const {
  if (piece.getPieceColor() == PieceColor::WHITE) {
    return whiteCastle.RC_;
  }
  return blackCastle.RC_;
}

bool Board::IsWhiteMove() const {
  return is_white_move_;
}
void Board::NextMove() {
  Board::is_white_move_ = !is_white_move_;
}
bool Board::IsBlackMove() const {
  return !is_white_move_;
}
int Board::GetPrevLongPonMove() const {
  return prev_long_pon_move_;
}
void Board::SetPrevLongPonMove(int prev_long_pon_move) const {
  Board::prev_long_pon_move_ = prev_long_pon_move;
}

void Board::SetPiece(const Piece& piece) {
  if (piece.getType() == PieceType::tKING) {
    if (piece.getPieceColor() == PieceColor::WHITE) {
      whiteCastle.king_position = piece.getPosition();
    }
    blackCastle.king_position = piece.getPosition();
  }
  board[piece.getPosition().GetX()][piece.getPosition().GetY()];
}

Position Board::GetKingPosition(const Piece& piece) const {
  if (piece.getType() == PieceType::tKING) {
    if (piece.getPieceColor() == PieceColor::WHITE) {
      return whiteCastle.king_position;
    }
    return blackCastle.king_position;
  }
}
