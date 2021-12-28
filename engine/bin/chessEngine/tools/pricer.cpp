#include "pricer.h"

int Pricer::getPrice(const std::shared_ptr<const Piece>& piece) {
  int price = 0;
  price += getPrice(piece->getType());

  return price;
}
int Pricer::countOrder(const BBoard* board, const BMove& move) {
  PieceType end_piece;
  if (move.getIsFlagSet(BMove::CAPTURES)) {
    end_piece =
        static_cast<PieceType>(board->getPiece(
            move.getTo(),
            static_cast<BBoard::BPieceType>(!board->whosTurn())) / 2);
  }
  auto tmp = board->getPiece(
      move.getFrom(),
      static_cast<BBoard::BPieceType>(!board->whosTurn()));

  bool is_white_move = tmp % 2;
  PieceType start_piece = static_cast<PieceType>(tmp / 2);

  int attack_score = getPrice(end_piece);

  int answer = attack_score;

  if (move.getNewPieceType() != PieceType::NONE) {
    answer += getPrice(move.getNewPieceType());
  }
  auto coef = endGameCoef(*board);
  int sx = move.getFromPair().first;
  int sy = move.getFromPair().second;
  int ex = move.getToPair().first;
  int ey = move.getToPair().second;
  answer += valOnBoard(ex, ey,
                       is_white_move, // CHECK PIECE COLOR
                       coef,
                       start_piece);
  answer -= valOnBoard(sx, sy,
                       is_white_move,
                       coef,
                       start_piece);
  if (attack_score != 0) {
    answer += valOnBoard(ex, ey,
                         !is_white_move,
                         coef,
                         end_piece);
  }

  if (start_piece == PieceType::QUEEN) {
    if (coef < 0.3)
      answer -= 15;
  }

  if (start_piece == PieceType::PAWN) {
    if (coef < 0.2)
      answer += 10;
  }

  if (move.getIsFlagSet(BMove::KING_CASTLE) ||
      move.getIsFlagSet(BMove::QUEEN_CASTLE)) {
    answer += 20;
  }
  if (move.getIsFlagSet(BMove::BRAKE_KING_CASTLE) ||
      move.getIsFlagSet(BMove::BRAKE_QUEEN_CASTLE)) {
    answer -= 20;
  }

  if (!board->isWhiteTurn()) {
    answer *= -1;
  }
  // check for mate
  return answer;

}

int Pricer::countOrder(const std::shared_ptr<ObjBoard>& board,
                       const Move& move) {
  int answer = move.getAttackScore();
  if (move.getNewPieceType() != PieceType::NONE) {
    answer += getPrice(move.getNewPieceType());
  }
  auto coef = endGameCoef(board);
  answer += valOnBoard(move.getEnd()->getPosition(),
                       move.getStart()->getPieceColor() == ColorType::WHITE,
                       coef,
                       move.getStart()->getType());
  answer -= valOnBoard(move.getStart()->getPosition(),
                       move.getStart()->getPieceColor() == ColorType::WHITE,
                       coef,
                       move.getStart()->getType());
  if (move.getAttackScore() != 0) {
    answer += valOnBoard(move.getEnd()->getPosition(),
                         move.getEnd()->getPieceColor() == ColorType::WHITE,
                         coef,
                         move.getEnd()->getType());
  }

  if (move.getStart()->getType() == PieceType::QUEEN) {
    if (endGameCoef(board) < 0.3)
      answer -= 15;
  }

  if (move.getStart()->getType() == PieceType::PAWN) {
    if (endGameCoef(board) < 0.2)
      answer += 10;
  }

  if (move.isCastle()) {
    answer += 20;
  }
  if (move.isBrakeLeftCastle() || move.isBrakeLeftCastle()) {
    answer -= 20;
  }

  if (board->isBlackTurn()) {
    answer *= -1;
  }
  // check for mate
  return answer;
}

int Pricer::getPrice(PieceType type) {
  switch (type) {
    case PieceType::PAWN:return 100;
    case PieceType::NIGHT:return 300;
    case PieceType::BISHOP: return 350;
    case PieceType::RUCK:return 500;
    case PieceType::QUEEN:return 9000;
    case PieceType::KING:return 99999;
    default :return 0;
  }
}
//[0:1]
double Pricer::endGameCoef(const std::shared_ptr<const ObjBoard>& board) {
  int white_count = board->getActivePieceList(true).size();
  int black_count = board->getActivePieceList(false).size();
  double weg = 0.9375 - white_count / 15.;
  double beg = 0.9375 - black_count / 15.;
  return ((weg + beg) / 2) * ((weg + beg) / 2) * ((weg + beg) / 2)
      * ((weg + beg) / 2);
}

double Pricer::endGameCoef(const BBoard& board) {
  int pieces = board.countPieces();
  double weg = 0.9375 - pieces / 15.;
  return ((weg) / 2) * ((weg) / 2) * ((weg) / 2)
      * ((weg) / 2);

}

int Pricer::valOnBoard(int xx,
                       int yy,
                       bool is_white,
                       double endgamecoef,
                       PieceType type) {
  int x = (8 - xx) * 8;
  int y = yy;
  if (!is_white) {
    x = xx * 8;
  }

  if (type == PieceType::KING) {
    return ((1 - endgamecoef)
        * Pricer::KingTable[x + y]
        + endgamecoef * KingTableEndGame[x + y]);
  }

  if (type == PieceType::NIGHT) {
    return KnightTable[x + y];
  }

  if (type == PieceType::BISHOP) {
    return BishopTable[x + y];
  }
  if (type == PieceType::PAWN) {
    return PawnTable[x + y];
  }
  return 0;
}
int Pricer::valOnBoard(Position pos,
                       bool is_white,
                       double endGameCoef,
                       PieceType type) {
  return valOnBoard(pos.getX(), pos.getY(), is_white, endGameCoef, type);
}
int Pricer::getPrice(BBoard::BPieceType type) {
  return getPrice(static_cast<PieceType>(type / 2));
}

