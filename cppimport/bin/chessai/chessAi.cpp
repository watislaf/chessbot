#include <string>
#include <algorithm>
#include "chessAi.h"

void ChessAi::startNewGame(const std::string& fen_str) {
  board_ = std::make_shared<Board>(FEN(fen_str));
}

std::string ChessAi::getPossibleMovesForPosition(int x, int y) {
  auto moves = moves_generator.generateMoves(
      board_, board_->getPiece(Position(x, y)));

  moves.erase(
      std::remove_if(
          moves.begin(), moves.end(),
          [](const Move& move) {
            return move.getEnd()->getType() != PieceType::tNONE
                && move.getStart()->getPieceColor()
                    == move.getEnd()->getPieceColor();
          }), moves.end()
  );

  std::string answer;
  for (const auto& move: moves) {
    answer += move.toStr() + " ";
  }

  return answer;
}

std::string ChessAi::getBoardStr() const {
  return board_->toStr();
}

std::string ChessAi::getFenStr() const {
  return board_->getFen();
}

void ChessAi::applyMove(int fx, int fy, int tx, int ty) {
  Position from = Position(fx, fy);
  Position to = Position(tx, ty);
  Move move(board_->getPiece(from), board_->getPiece(to));
  board_->apply(move);
//*move.position_from, *move.position_to
}

