#include "pricer.h"

int Pricer::getPrice(const std::shared_ptr<const Piece>& piece) {
  switch (piece->getType()) {
    case PieceType::tKING:return 999;
    case PieceType::tQUEEN:return 90;
    case PieceType::tRUCK:return 40;
    case PieceType::tPONE:return 10;
    case PieceType::tNONE:return 0;
    case PieceType::tBISHOP: return 30;
    case PieceType::tHORSE:return 30;
  }

  return 0;
}

int Pricer::count(const std::shared_ptr<Board>& board, const Move& move) {
  int answer = move.getAttackScore();
  //answer -= move.getDefendScore()/5;
  if (board->isBlackMove()) {
    answer *= -1;
  }
  return answer;
}
int Pricer::defendScore(const Move& move) {
  if (move.getEnd()->getType() == PieceType::tKING) {
    return 0;
  }
  return getPrice(move.getEnd()) / 10;
}
