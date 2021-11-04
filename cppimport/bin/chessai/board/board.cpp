#include "board.h"

Piece Board::GetPiece(Position position) const {
  auto posY = position.GetY();
  auto posX = position.GetX();
  if (posY < 0 || posY > 8 || posX < 0 || posX > 8) {
    return Piece(position);
  }
  return board[position.GetX()][position.GetY()];
}
bool Board::IsLc() const {
  return LC_;
}
void Board::SetLc(bool lc) {
  LC_ = lc;
}
bool Board::IsRc() const {
  return RC_;
}
void Board::SetRc(bool rc) {
  RC_ = rc;
}
bool Board::IsWhiteMove() const {
  return is_white_move_;
}
void Board::SetIsWhiteMove(bool is_white_move) {
  Board::is_white_move_ = is_white_move;
}
int Board::GetPrevLongPonMove() const {
  return prev_long_pon_move_;
}
void Board::SetPrevLongPonMove(int prev_long_pon_move) {
  Board::prev_long_pon_move_ = prev_long_pon_move;
}
