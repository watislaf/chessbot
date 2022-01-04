#include "pricer.h"

int Pricer::getPrice(const std::shared_ptr<const Piece>& piece) {
  int price = 0;
  price += getPrice(piece->getType());

  return price;
}
int Pricer::countOrder(BBoard* board, const BMove& move) {
  PieceType attack_piece = PieceType::NONE;
  if (move.isCapture()) {
    attack_piece =
        static_cast<PieceType>(board->getPiece(
            move.getTo(),
            static_cast<BBoard::BPieceType>(!board->whosTurn())) / 2);

  }
  PieceType start_piece = move.getCurrentPieceType();

  int attack_score = getPrice(attack_piece);
  if (attack_piece == PieceType::PAWN
      && board->isPassedPawn(move.getTo(), !board->isWhiteTurn())) {
    attack_score += 80; // if we capture free pawn == get plus scores
  }

  if (attack_score == 0 && move.isCapture()) {
    attack_score = getPrice(PieceType::PAWN); // EP
  }
  if (attack_score != 0) {
    if (start_piece == PieceType::PAWN) {
      if (!board->isolatedPawn(move.getFrom(), move.getFrom())
          &&  // was not isolated
              board->isolatedPawn(move.getFrom(),
                                  move.getTo())) { // became isolated
        attack_score -= 30;
      }
      if (board->isolatedPawn(move.getFrom(), move.getFrom())
          &&  // was   isolated
              !board->isolatedPawn(move.getFrom(),
                                   move.getTo())) { // became unisolated
        attack_score += 30;
      }
      if (board->doublePawn(move.getFrom(), move.getTo())) {
        attack_score -= 20;
      } else {
        if (board->doublePawn(move.getFrom(), move.getFrom())) {
          attack_score += 20;
        }
      }
    }
  }
  int answer = attack_score;

  if (move.getNewPieceType() != PieceType::NONE) {
    answer += getPrice(move.getNewPieceType());
    answer -= 200;
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
                         attack_piece);
  }

  if (start_piece == PieceType::PAWN) {
    if (board->isPassedPawn(move.getFrom(), board->isWhiteTurn())) {
      answer += 10 + 30 * coef; // if we move free pawn get score
    }
    if (coef < 0.7 && board->isDefendKingPawn(move.getFrom())) {
      answer -= 20;
    } else {
      if (board->isAttackKingPawn(move.getTo())) { // if we attack king side by this pawn
        answer += 5;
      }
    }
  }

  if (start_piece == PieceType::QUEEN) {
    if (coef < 0.2)
      answer -= 15;
  }

  if (start_piece == PieceType::PAWN) {
    if (coef < 0.1)
      answer += 5;
  }

  if (board->isCastle(move) && board->goodCastle(move.getFrom())) {
    answer += 30;
  }
  if (board->isBrakeCastle(move, board->whosTurn())) {
    answer -= 30;
  }

  uint8_t king_pression = board->kingPression(
      static_cast<BBoard::BPieceType>(!board->whosTurn()));
  board->apply(move);
  answer += (
      board->kingPression(static_cast<BBoard::BPieceType>(board->whosTurn())))
      - king_pression
          * 10;
  if (board->attacksToKing(
      BBoard::bitScanForward(
          board->get(BBoard::BPieceType::WHITE_KING,
                     static_cast<BBoard::BPieceType>(board->whosTurn()))),
      static_cast<BBoard::BPieceType>(!board->whosTurn()))) {
    answer += 10;
  }
  board->unApply(move);
  if (!board->isWhiteTurn()) {
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
    case PieceType::QUEEN:return 900;
    case PieceType::KING:return 99999;
    default :return 0;
  }
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
  if (type == PieceType::RUCK) {
    return RuckTable[pos];
  }
  if (type == PieceType::QUEEN) {
    return (RuckTable[pos] + BishopTable[pos]) / 2;
  }
  return 0;
}
