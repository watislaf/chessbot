#include "pricer.h"

int Pricer::getPrice(const std::shared_ptr<const Piece>& piece) {
  int price = 0;
  price += getPrice(piece->getType());

  return price;
}

int Pricer::count(const std::shared_ptr<Board>& board, const Move& move) {
  int answer = move.getAttackScore();
  if (move.getNewPieceType() != PieceType::tNONE) {
    answer += getPrice(move.getNewPieceType());
  }
//  answer += move.getDefendScore()/2;
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
int Pricer::getPrice(PieceType type) {
  switch (type) {
    case PieceType::tKING:return 999;
    case PieceType::tQUEEN:return 90;
    case PieceType::tRUCK:return 40;
    case PieceType::tPONE:return 10;
    case PieceType::tNONE:return 0;
    case PieceType::tBISHOP: return 30;
    case PieceType::tHORSE:return 30;
  }
}
