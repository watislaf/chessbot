#include "pricer.h"

int Pricer::getPrice(const std::shared_ptr<const Piece>& piece) {
  int price = 0;
  price += getPrice(piece->getType());

  return price;
}

int Pricer::countOrder(const std::shared_ptr<Board>& board, const Move& move) {
  int answer = move.getAttackScore();
  if (move.getNewPieceType() != PieceType::tNONE) {
    answer += getPrice(move.getNewPieceType()) ;
  }
  answer += valOnBoard(move.getEnd()->getPosition(),
                       move.getStart()->getPieceColor() == PieceColor::WHITE,
                       board,
                       move.getStart()->getType());
  answer -= valOnBoard(move.getStart()->getPosition(),
                       move.getStart()->getPieceColor() == PieceColor::WHITE,
                       board,
                       move.getStart()->getType());
  if (move.getAttackScore() != 0) {
    answer += valOnBoard(move.getEnd()->getPosition(),
                         move.getEnd()->getPieceColor() == PieceColor::WHITE,
                         board,
                         move.getEnd()->getType());
  }

  if (move.getStart()->getType() == PieceType::tQUEEN) {
    if (endGameCoef(board) < 0.3)
      answer -= 3;
  }

  if (move.getStart()->getType() == PieceType::tPONE) {
    if (endGameCoef(board) < 0.2)
      answer += 2;
  }

  if (move.isCastle()) {
    answer += 4;
  }
  if (move.isBrakeLeftCastle() || move.isBrakeLeftCastle()) {
    answer -= 2;
  }

  if (board->isBlackMove()) {
    answer *= -1;
  }
  return answer;
}

int Pricer::defendScore(const Move& move) {
  if (move.getEnd()->getType() == PieceType::tKING) {
    return 0;
  }
  return getPrice(move.getEnd()) / 5;
}
int Pricer::getPrice(PieceType type) {
  switch (type) {
    case PieceType::tKING:return 999;
    case PieceType::tQUEEN:return 180;
    case PieceType::tRUCK:return 80;
    case PieceType::tPONE:return 20;
    case PieceType::tNONE:return 0;
    case PieceType::tBISHOP: return 60;
    case PieceType::tHORSE:return 60;
  }
}
//[0:1]
double Pricer::endGameCoef(const std::shared_ptr<const Board>& board) {
  int white_count = board->getActivePieceList(true).size();
  int black_count = board->getActivePieceList(false).size();
  double weg = 0.9375 - white_count / 15.;
  double beg = 0.9375 - black_count / 15.;
  return ((weg + beg) / 2) * ((weg + beg) / 2) * ((weg + beg) / 2)
      * ((weg + beg) / 2);
}

int Pricer::valOnBoard(Position pos,
                       bool is_white,
                       const std::shared_ptr<const Board>& board,
                       PieceType type) {
  int xx = pos.getX();
  int yy = pos.getY();
  int x = (8 - xx) * 8;
  int y = yy;
  if (!is_white) {
    x = xx * 8;
  }

  if (type == PieceType::tKING) {
    return ((1 - endGameCoef(board))
        * Pricer::KingTable[x + y]
        + (endGameCoef(board)) * KingTableEndGame[x + y])/10;
  }

  if (type == PieceType::tHORSE) {
    return KnightTable[x + y]/10;
  }

  if (type == PieceType::tBISHOP) {
    return BishopTable[x + y]/10;
  }
  if (type == PieceType::tPONE) {
    return PawnTable[x + y]/10;
  }
  return 0;
}

