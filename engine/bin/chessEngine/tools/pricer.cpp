#include "pricer.h"

int Pricer::getPrice(const std::shared_ptr<const Piece>& piece) {
  int price = 0;
  price += getPrice(piece->getType());

  return price;
}
int Pricer::countOrder(const BBoard* board, const BMove& move) {
  PieceType end_piece = PieceType::NONE;
  if (move.isCapture()) {
    end_piece =
        static_cast<PieceType>(board->getPiece(
            move.getTo(),
            static_cast<BBoard::BPieceType>(!board->whosTurn())) / 2);
  }
  PieceType start_piece = move.getCurrentPieceType();

  int attack_score = getPrice(end_piece);
  if (attack_score == 0 && move.isCapture()) {
    attack_score = getPrice(PieceType::PAWN); // EP
  }
  int answer = attack_score;

  if (move.getNewPieceType() != PieceType::NONE) {
    answer += getPrice(move.getNewPieceType());
  }
  auto coef = endGameCoef(*board);

  answer += valOnBoard(move.getTo(),
                       board->isWhiteTurn(), // CHECK PIECE COLOR
                       coef,
                       start_piece);
  answer -= valOnBoard(move.getFrom(),
                       board->isWhiteTurn(),
                       coef,
                       start_piece);
  if (attack_score != 0) {
    answer += valOnBoard(move.getTo(),
                         !board->isWhiteTurn(),
                         coef,
                         end_piece);
  }
  if (start_piece == PieceType::QUEEN) {
    if (coef < 0.2)
      answer -= 3;
  }

  if (start_piece == PieceType::PAWN) {
    if (coef < 0.1)
      answer += 3;
  }

  if (board->isCastle(move)) {
    answer += 20;
  }
  if (board->isBrakeCastle(move)) {
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
    if (endGameCoef(board) < 0.2)
      answer -= 3;
  }

  if (move.getStart()->getType() == PieceType::PAWN) {
    if (endGameCoef(board) < 0.1)
      answer += 3;
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
    case PieceType::KNIGHT:return 300;
    case PieceType::BISHOP: return 305;
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
  double weg = 32. / 30. - (white_count + black_count) / 30.;
  return ((weg)) * ((weg));
}

double Pricer::endGameCoef(const BBoard& board) {
  int pieces = board.countPieces();
  double weg = 32. / 30. - pieces / 30.;
  return ((weg)) * ((weg));
}

int Pricer::valOnBoard(uint8_t pos,
                       bool is_white,
                       double endgamecoef,
                       PieceType type) {
  if (is_white) {
    pos = 63 - pos;
  }

  if (type == PieceType::KING) {
    return ((1 - endgamecoef)
        * Pricer::KingTable[pos]
        + endgamecoef * KingTableEndGame[pos]);
  }

  if (type == PieceType::KNIGHT) {
    return KnightTable[pos];
  }

  if (type == PieceType::BISHOP) {
    return BishopTable[pos];
  }
  if (type == PieceType::PAWN) {
    return PawnTable[pos];
  }
  return 0;
}
int Pricer::valOnBoard(Position pos,
                       bool is_white,
                       double endGameCoef,
                       PieceType type) {
  return valOnBoard(pos.getX() + 8 * pos.getY(), is_white, endGameCoef, type);
}
int Pricer::getPrice(BBoard::BPieceType type) {
  return getPrice(static_cast<PieceType>(type / 2));
}

