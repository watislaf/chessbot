#include <algorithm>
#include "abstracts/move.h"
#include "movesGenerator.h"
#include "pricer.h"

std::vector<Move> MovesGenerator::generateMoves(std::shared_ptr<Board> board,
                                                std::shared_ptr<const Piece> piece) {

  board_ = board;
  piece_ = piece;
  moves_.clear();

  // Create simple moves
  switch (piece->getType()) {
    case PieceType::tNONE:return {};
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
  }
  std::remove_if(moves_.begin(),
                 moves_.end(),
                 [this](Move move) { return isShahDanger(move); });
  return moves_;
}

void MovesGenerator::horseMove(bool reduce_tNone) {
  for (int left = -1; left <= 1; left += 2) {
    for (int top = -1; top <= 2; top += 2) {
      insertPositionsToMoves(goByVector(Position(2, 3),
                                        1,
                                        false,
                                        0));
      insertPositionsToMoves(goByVector(Position(3, 2), 1));
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
  int length = piece_->getPieceColor() == PieceColor::WHITE ? 1 : -1;
  insertPositionsToMoves(goByVector(Position(0, length),
                                    1,
                                    false,
                                    reduce_tNone));
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
    int px = length * vector.getX();
    int py = length * vector.getY();
    auto new_position = Position(px, py) + piece_->getPosition();
    auto posY = new_position.getY();
    auto posX = new_position.getX();
    if (posY < 0 || posY > 7 || posX < 0 || posX > 7) {
      break;
    }
    const auto& piece_on_pos = board_->getPiece(new_position);
    if (piece_on_pos->getType() == PieceType::tNONE) {
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
  if (board_->isLcAvailable(*piece_)) {
    auto positions_left = goByVector(Position(-1, 0));
    if (positions_left.size() == 4) {
      for (int i = -3; i >= -2; i++) {
        moves_.emplace_back(
            *piece_, *board_->getPiece(piece_->getPosition() + Position(i, 0)));
        moves_.back().SetIsCastle(true);
        moves_.back().SetBrakeRightCastle(true);
      }
    }
    for (auto& move: moves_) {
      move.SetBrakeLeftCastle(true);
    }
  }
  if (board_->isRcAvailable(*piece_)) {
    auto positions_right = goByVector(Position(1, 0));
    if (positions_right.size() == 3) {
      moves_.emplace_back(*piece_, *board_->getPiece(
          piece_->getPosition() + Position(2, 0)));
      moves_.back().SetIsCastle(true);
      moves_.back().SetBrakeLeftCastle(true);
    }
    for (auto& move: moves_) {
      move.SetBrakeRightCastle(true);
    }
  }
}

void MovesGenerator::poneSpecialMove() {
  auto piece_color = piece_->getPieceColor();
  // DOUBLE MOVE
  if (piece_color == PieceColor::WHITE && piece_->getPosition().getY() == 1 ||
      piece_color == PieceColor::BLACK && piece_->getPosition().getY() == 6) {

    if (goByVector(Position(0, piece_color == PieceColor::WHITE ? 1 : -1),
                   2,
                   true).size() == 2) {
      moves_.emplace_back(
          *piece_,
          *board_->getPiece(piece_->getPosition()
                                + Position(0, 2)));
      moves_.back().SetIsDoubleDistancePone(true, board_->getLastPassantX());
    }
  }
  for (int i = -1; i <= 1; i += 2) {
    auto go_diagonal = goByVector(Position(i, 1), 1, false, true);
    // Atack by diagonal
    if (go_diagonal.size() == 1
        && board_->getPiece(go_diagonal[0])->getPieceColor()
            != piece_color) {
      moves_.emplace_back(*piece_, *board_->getPiece(go_diagonal[0]));
    }
    auto go_diagonal2 = goByVector(Position(i, 1), 1, true);
    // Atack passant
    if (go_diagonal.size() == 1
        && board_->getLastPassantX() == go_diagonal[0].getX()) {
      moves_.emplace_back(*piece_, *board_->getPiece(go_diagonal[0]));
      moves_.back().SetIsPassant(true);
      moves_.back().SetAttackPrice(Pricer::GetPrice(moves_.back().getStart()));
    }
  }
  for (int i = moves_.size() - 1; i >= 0; i--) {
    int pone_y = moves_[i].getEnd().getPosition().getY();
    if (pone_y == 0 || pone_y == 7) {
      moves_[i].SetCanMakeNewFigure(true);
      moves_[i].SetNewPieceType(PieceType::tRUCK);
      moves_.emplace_back(moves_[i]);
      moves_.back().SetNewPieceType(PieceType::tBISHOP);
      moves_.emplace_back(moves_[i]);
      moves_.back().SetNewPieceType(PieceType::tHORSE);;
      moves_.emplace_back(moves_[i]);
      moves_.back().SetNewPieceType(PieceType::tQUEEN);
    }
  }

}

void MovesGenerator::insertPositionsToMoves(const std::vector<Position>& positions) {
  for (const auto& pos: positions) {
    moves_.emplace_back(*piece_, *board_->getPiece(pos));
  }
}

bool MovesGenerator::isShahDanger(Move move) {
  if (move.GetDefendPrice() != 0)
    return false;
  board_->apply(move);
  bool is_under_shach = isShah();
  board_->unApply(move);
  if (is_under_shach) {
    return true;
  }
  if (move.IsCastle()) {
    move.SetIsCastle(false);
    auto start_pos = move.getStart().getPosition();
    auto end_pos = move.getEnd().getPosition();
    move.SetEnd(*board_->getPiece(Position(
        (start_pos.getX() + end_pos.getX()) / 2,
        start_pos.getY())));

    board_->apply(move);
    is_under_shach = isShah(); // check castle somehow )))))
    board_->unApply(move);
  }
  if (is_under_shach) {
    return true;
  }
  return false;
}

bool MovesGenerator::isShah() {
  auto movesTMP = moves_;
  auto pieceTMP = piece_;

  moves_.clear();
  piece_ = board_->getPiece(board_->getKingPosition(*piece_));

  horseMove(true);
  if (movesAreAttacking(PieceType::tHORSE)) {
    return true;
  }
  moves_.clear();
  kingMove(true);
  if (movesAreAttacking(PieceType::tKING)) {
    return true;
  }
  moves_.clear();
  ruckMove(true);

  if (movesAreAttacking(PieceType::tRUCK)) {
    return true;
  }
  moves_.clear();
  queenMove(true);
  if (movesAreAttacking(PieceType::tQUEEN)) {
    return true;
  }
  moves_.clear();
  bishopMove(true);
  if (movesAreAttacking(PieceType::tBISHOP)) {
    return true;
  }
  moves_.clear();
  poneMove(true);
  if (movesAreAttacking(PieceType::tPONE)) {
    return true;
  }
  moves_ = movesTMP;
  piece_ = pieceTMP;
  return false;
}

bool MovesGenerator::movesAreAttacking(PieceType type) {
  for (const auto& move: moves_) {
    auto piece_here = move.getEnd();
    if (piece_here.getPieceColor() != piece_->getPieceColor()
        && piece_here.getType() == type) {
      return true;
    }
  }
  return false;
}

