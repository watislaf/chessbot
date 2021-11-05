#include <abstracts/move.h>
#include <algorithm>
#include "MovesGenerator.h"
std::vector<Move> MovesGenerator::GenerateMoves(const Board& board,
                                                const Piece& piece) {

  MovesGenerator::board_ = board;
  MovesGenerator::piece_ = piece;
  MovesGenerator::moves_.clear();

  // Create simple moves
  switch (piece.getType()) {
    case PieceType::tNONE:return {};
    case PieceType::tHORSE: HorseMove();
      break;
    case PieceType::tKING: KingMove();
      CastleMove();
      break;
    case PieceType::tRUCK: RuckMove();
      break;
    case PieceType::tQUEEN: QueenMove();
      break;
    case PieceType::tBISHOP: BishopMove();
      break;
    case PieceType::tPONE: PoneMove();
      PoneSpecialMove();
      break;
  }
  std::remove_if(MovesGenerator::moves_.begin(),
                 MovesGenerator::moves_.end(),
                 IsShahFree);
}

void MovesGenerator::HorseMove() {
  for (int left = -1; left <= 1; left += 2) {
    for (int top = -1; top <= 2; top += 2) {
      InsertPositionsToMoves(GoByVector(Position(2, 3), 1));
      InsertPositionsToMoves(GoByVector(Position(3, 2), 1));
    }
  }
}

void MovesGenerator::KingMove() {
  for (int left = -1; left <= 1; left += 1) {
    for (int top = -1; top <= 1; top += 1) {
      InsertPositionsToMoves(GoByVector(Position(left, top), 1));
    }
  }
}

void MovesGenerator::RuckMove() {
  for (int top = -1; top <= 1; top += 2) {
    InsertPositionsToMoves(GoByVector(Position(0, top)));
    InsertPositionsToMoves(GoByVector(Position(top, 0)));
  }
}

void MovesGenerator::QueenMove() {
  for (int left = -1; left <= 1; left += 1) {
    for (int top = -1; top <= 1; top += 1) {
      InsertPositionsToMoves(GoByVector(Position(left, top), 1));
    }
  }
}

void MovesGenerator::BishopMove() {
  for (int left = -1; left <= 1; left += 2) {
    for (int top = -1; top <= 1; top += 2) {
      InsertPositionsToMoves(GoByVector(Position(left, top), 1));
    }
  }
}

void MovesGenerator::PoneMove() {
  int length =
      MovesGenerator::piece_.getPieceColor() == PieceColor::WHITE ? 1 : -1;
  InsertPositionsToMoves(GoByVector(Position(0, length), 1));
}

std::vector<Position> MovesGenerator::GoByVector(Position vector,
                                                 int max_length,
                                                 bool only_tNone,
                                                 bool reduce_tNone) {
  if (vector.GetX() == 0 && vector.GetY() == 0) {
    return {};
  }

  std::vector<Position> position_to_move;
  for (int length = 1; length <= max_length; length++) {
    int px = length * vector.GetX();
    int py = length * vector.GetY();
    auto new_position = Position(px, py) + MovesGenerator::piece_.getPosition();
    auto posY = new_position.GetY();
    auto posX = new_position.GetX();
    if (posY < 0 || posY > 7 || posX < 0 || posX > 7) {
      break;
    }
    const auto& piece_on_pos = MovesGenerator::board_.GetPiece(new_position);
    if (piece_on_pos.getType() == PieceType::tNONE) {
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

void MovesGenerator::CastleMove() {
  // If king move he lost castle
  if (MovesGenerator::board_.IsLcAvailable(MovesGenerator::piece_)) {
    auto positions_left = GoByVector(Position(-1, 0));
    if (positions_left.size() == 4) {
      for (int i = -3; i >= -2; i++) {
        MovesGenerator::moves_.emplace_back(
            MovesGenerator::piece_, MovesGenerator::piece_.getPosition()
                + Position(i, 0));
        MovesGenerator::moves_.back().SetIsCastle(true);
        MovesGenerator::moves_.back().SetBrakeRightCastle(true);
      }
    }
    for (auto& move: MovesGenerator::moves_) {
      move.SetBrakeLeftCastle(true);
    }
  }
  if (MovesGenerator::board_.IsRcAvailable(MovesGenerator::piece_)) {
    auto positions_right = GoByVector(Position(1, 0));
    if (positions_right.size() == 3) {
      MovesGenerator::moves_.emplace_back(
          MovesGenerator::piece_, MovesGenerator::piece_.getPosition()
              + Position(2, 0));
      MovesGenerator::moves_.back().SetIsCastle(true);
      MovesGenerator::moves_.back().SetBrakeLeftCastle(true);
    }
    for (auto& move: MovesGenerator::moves_) {
      move.SetBrakeRightCastle(true);
    }
  }
}

void MovesGenerator::PoneSpecialMove() {
  auto piece_color = MovesGenerator::piece_.getPieceColor();
  //
  if (piece_color == PieceColor::WHITE
      && MovesGenerator::piece_.getPosition().GetY() == 1 ||
      piece_color == PieceColor::BLACK
          && MovesGenerator::piece_.getPosition().GetY() == 6) {

    if (GoByVector(Position(0, piece_color == PieceColor::WHITE ? 1 : -1),
                   2,
                   true).size() == 2) {
      MovesGenerator::moves_.emplace_back(
          MovesGenerator::piece_, MovesGenerator::piece_.getPosition()
              + Position(0, 2));
      MovesGenerator::moves_.back().SetIsDoubleDistancePone(true);
    }
  }
  // Atack by diagonal
  for (int i = -1; i <= 1; i += 2) {
    auto go_diagonal = GoByVector(Position(i, 1));
    if (go_diagonal.size() == 1
        && MovesGenerator::board_.GetPiece(go_diagonal[0]).getPieceColor()
            != piece_color) {
      MovesGenerator::moves_.emplace_back(MovesGenerator::piece_,
                                          MovesGenerator::board_.GetPiece(
                                              go_diagonal[0]));
    }
  }
}

void MovesGenerator::InsertPositionsToMoves(const std::vector<Position>& positions) {
  MovesGenerator::moves_.insert(MovesGenerator::moves_.begin(),
                                positions.begin(),
                                positions.end());
}

bool MovesGenerator::IsShahFree(const Move& move) {
  auto piece_tmp = MovesGenerator::board_.GetPiece(move.GetEnd());
    MovesGenerator::board_.(move.GetEnd());

  return false;
}

