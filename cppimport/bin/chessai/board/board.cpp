#include "board.h"

const Piece& Board::getPiece(int row, int column) {
  return board[row][column];
}
