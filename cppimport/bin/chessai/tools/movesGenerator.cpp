#include <algorithm>
#include "abstracts/move.h"
#include "movesGenerator.h"
#include "pricer.h"

std::vector<Move> MovesGenerator::generateMoves(
    std::shared_ptr<Board> board, const std::shared_ptr<const Piece>& piece) {
  board_ = board;
  current_piece_ = piece;
  moves_.clear();

  // Create simple moves
  switch (piece->getType()) {
    case PieceType::tHORSE: horseMove();
      break;
    case PieceType::tKING: kingMove();
      castleMove();
      break;
    case PieceType::tRUCK: ruckMove();
      break;
    case PieceType::tQUEEN: queenMove();
      break;
    case PieceType::tBISHOP: bishopMove();
      break;
    case PieceType::tPONE: poneMove();
      poneSpecialMove();
      break;
    case PieceType::tNONE:return {};
  }

  moves_.erase(
      std::remove_if(moves_.begin(),
                     moves_.end(),
                     [this](Move move) { return isShahDanger(move); }),
      moves_.end());
  return moves_;
}

void MovesGenerator::horseMove(bool reduce_tNone) {
  for (int left = -1; left <= 1; left += 2) {
    for (int top = -1; top <= 2; top += 2) {
      insertPositionsToMoves(goByVector(Position(1 * left, 2 * top),
                                        1));
      insertPositionsToMoves(goByVector(Position(2 * left, 1 * top), 1));
    }
  }
}

void MovesGenerator::kingMove(bool reduce_tNone) {
  for (int left = -1; left <= 1; left += 1) {
    for (int top = -1; top <= 1; top += 1) {
      insertPositionsToMoves(goByVector(Position(left, top),
                                        1,
                                        false,
                                        reduce_tNone));
    }
  }
}

void MovesGenerator::ruckMove(bool reduce_tNone) {
  for (int top = -1; top <= 1; top += 2) {
    insertPositionsToMoves(goByVector(Position(0, top),
                                      8,
                                      false,
                                      reduce_tNone));
    insertPositionsToMoves(goByVector(Position(top, 0),
                                      8,
                                      false,
                                      reduce_tNone));
  }
}

void MovesGenerator::queenMove(bool reduce_tNone) {
  for (int left = -1; left <= 1; left += 1) {
    for (int top = -1; top <= 1; top += 1) {
      insertPositionsToMoves(goByVector(Position(left, top),
                                        1,
                                        false,
                                        reduce_tNone));
    }
  }
}

void MovesGenerator::bishopMove(bool reduce_tNone) {
  for (int left = -1; left <= 1; left += 2) {
    for (int top = -1; top <= 1; top += 2) {
      insertPositionsToMoves(goByVector(Position(left, top),
                                        1,
                                        false,
                                        reduce_tNone));
    }
  }
}

void MovesGenerator::poneMove(bool reduce_tNone) {
  int length = current_piece_->getPieceColor() == PieceColor::WHITE ? 1 : -1;
  insertPositionsToMoves(goByVector(Position(0, length),
                                    1,
                                    false,
                                    reduce_tNone));
  if (moves_.back().getEnd().getPosition().getY() % 7 == 0) {
    moves_.back().setCanMakeNewFigure(true);
  }
}

std::vector<Position> MovesGenerator::goByVector(Position vector,
                                                 int max_length,
                                                 bool only_tNone,
                                                 bool reduce_tNone) {
  if (vector.getX() == 0 && vector.getY() == 0) {
    return {};
  }

  std::vector<Position> position_to_move;
  for (int length = 1; length <= max_length; length++) {
    auto new_position = Position(
        length * vector.getX(), length * vector.getY()
    ) + current_piece_->getPosition();
    auto posY = new_position.getY();
    auto posX = new_position.getX();
    if (posY < 0 || posY > 7 || posX < 0 || posX > 7) {
      break;
    }
    if (board_->getPiece(new_position)->getType() == PieceType::tNONE) {
      if (!reduce_tNone) {
        position_to_move.emplace_back(new_position);
      }
      continue;
    }
    if (!only_tNone) {
      position_to_move.emplace_back(new_position);
    }
    break;
  }
  return position_to_move;
}

void MovesGenerator::castleMove() {
  // If king move he lost castle
  if (board_->isLcAvailable(*current_piece_)) {
    auto positions_left = goByVector(Position(-1, 0));
    if (positions_left.size() == 4) {
      for (int i = -3; i >= -2; i++) {
        moves_.emplace_back(
            *current_piece_,
            *board_->getPiece(current_piece_->getPosition() + Position(i, 0)));
        moves_.back().setIsCastle(true);
        moves_.back().setBrakeRightCastle(true);
      }
    }
    for (auto& move: moves_) {
      move.setBrakeLeftCastle(true);
    }
  }
  if (board_->isRcAvailable(*current_piece_)) {
    auto positions_right = goByVector(Position(1, 0));
    if (positions_right.size() == 3) {
      moves_.emplace_back(*current_piece_, *board_->getPiece(
          current_piece_->getPosition() + Position(2, 0)));
      moves_.back().setIsCastle(true);
      moves_.back().setBrakeLeftCastle(true);
    }
    for (auto& move: moves_) {
      move.setBrakeRightCastle(true);
    }
  }
}

void MovesGenerator::poneSpecialMove() {
  auto piece_color = current_piece_->getPieceColor();
  // DOUBLE MOVE
  if (piece_color == PieceColor::WHITE
      && current_piece_->getPosition().getY() == 1 ||
      piece_color == PieceColor::BLACK
          && current_piece_->getPosition().getY() == 6) {
    if (goByVector(Position(0, piece_color == PieceColor::WHITE ? 1 : -1),
                   2,
                   true).
        size() == 2) {
      moves_.emplace_back(
          *current_piece_,
          *board_->getPiece(current_piece_->getPosition()
                                + Position(0, piece_color == PieceColor::WHITE
                                              ? 2 : -2)));
      moves_.back().setIsDoubleDistancePone(true, board_->getLastPassantX());
    }
  }

  // Atack by diagonal
  for (int i = -1; i <= 1; i += 2) {
    auto go_diagonal = goByVector(Position(i, 1), 1, false, true);
    if (go_diagonal.size() == 1
        && board_->getPiece(go_diagonal[0])->getPieceColor() != piece_color) {
      moves_.emplace_back(*current_piece_, *board_->getPiece(go_diagonal[0]));
    }
    auto go_diagonal2 = goByVector(Position(i, 1), 1, true);
    // Atack passant
    if (go_diagonal.size() == 1
        && board_->getLastPassantX() == go_diagonal[0].getX()) {
      moves_.emplace_back(*current_piece_, *board_->getPiece(go_diagonal[0]));
      moves_.back().setIsPassant(true);
      moves_.back().setAttackPrice(Pricer::getPrice(moves_.back().getStart()));
    }
  }
}

void MovesGenerator::insertPositionsToMoves(const std::vector<Position>& positions) {
  for (const auto& pos: positions) {
    moves_.emplace_back(*current_piece_, *board_->getPiece(pos));
  }
}

bool MovesGenerator::isShahDanger(Move move) {
  if (move.getDefendPrice() != 0)
    return false;

  board_->apply(move);
  bool is_under_shach = isShah();
  board_->unApply(move);
  if (is_under_shach) {
    return true;
  }
  if (move.isCastle()) {
    Move non_castle_move(move.getStart(),
                         *board_->getPiece(Position(
                             (move.getStart().getPosition().getX()
                                 + move.getEnd().getPosition().getX()) / 2,
                             move.getStart().getPosition().getY())));
    non_castle_move.setIsCastle(false);

    board_->apply(move);
    is_under_shach = isShah(); // check castle somehow )))))
    board_->unApply(move);
    move.setIsCastle(true);

  }
  if (is_under_shach) {
    return true;
  }
  return false;
}

bool MovesGenerator::isShah() {
  auto movesTMP = moves_;
  auto pieceTMP = current_piece_;

  moves_.clear();
  current_piece_ = board_->getPiece(board_->getKingPosition(*current_piece_));

  horseMove(true);
  if (isPieceOnMoves(PieceType::tHORSE)) {
    return true;
  }
  moves_.clear();
  kingMove(true);
  if (isPieceOnMoves(PieceType::tKING)) {
    return true;
  }
  moves_.clear();
  ruckMove(true);

  if (isPieceOnMoves(PieceType::tRUCK)) {
    return true;
  }
  moves_.clear();
  queenMove(true);
  if (isPieceOnMoves(PieceType::tQUEEN)) {
    return true;
  }
  moves_.clear();
  bishopMove(true);
  if (isPieceOnMoves(PieceType::tBISHOP)) {
    return true;
  }
  moves_.clear();
  poneMove(true);
  if (isPieceOnMoves(PieceType::tPONE)) {
    return true;
  }
  moves_ = movesTMP;
  current_piece_ = pieceTMP;
  return false;
}

bool MovesGenerator::isPieceOnMoves(PieceType type) {
  for (const auto& move: moves_) {
    auto piece_here = move.getEnd();
    if (piece_here.getPieceColor() != current_piece_->getPieceColor()
        && piece_here.getType() == type) {
      return true;
    }
  }
  return false;
}

