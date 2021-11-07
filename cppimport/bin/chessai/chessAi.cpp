#include <string>
#include <algorithm>
#include "chessAi.h"

void ChessAi::startNewGame(const std::string& fen) {
  board_ = std::make_shared<Board>(FEN(fen));
}

std::string ChessAi::getPossibleMovesForPiece(int x, int y) {
  auto moves =
      moves_generator.generateMoves(board_, board_->getPiece(Position(x, y)));

  auto end = std::remove_if(moves.begin(),
                            moves.end(),
                            [](const Move& move) {
                              return move.getEnd().getType() != PieceType::tNONE
                                  && move.getStart().getPieceColor()
                                      == move.getEnd().getPieceColor();
                            });
  std::string answer = "";
  for (auto i = moves.begin(); i != end; i++) {
    answer += (*i).toStr() + " ";
  }
  return answer;
}
std::string ChessAi::getBoardStr() const {
  return board_->toStr();
}
std::string ChessAi::getFen() const {
  return board_->getFen();
}
