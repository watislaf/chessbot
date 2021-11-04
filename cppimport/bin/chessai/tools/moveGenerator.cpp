#include <abstracts/move.h>
#include <algorithm>
#include "moveGenerator.h"
std::vector<Move> moveGenerator::GenerateMove(const Board& board,
                                              const Piece& piece) {

  moveGenerator::board_ = board;
  moveGenerator::piece_ = piece;
  std::vector<Position> possible_positions;
  switch (piece.getType()) {
    case PieceType::tNONE:possible_positions = {};
      break;
    case PieceType::tHORSE:possible_positions = HorseMove();
      break;
    case PieceType::tKING:possible_positions = KingMove();
      break;
    case PieceType::tRUCK:possible_positions = RuckMove();
      break;
    case PieceType::tQUEEN:possible_positions = QueenMove();
      break;
    case PieceType::tBISHOP:possible_positions = BishopMove();
      break;
    case PieceType::tPONE:possible_positions = PoneMove();
      break;
  }

  auto possible_moves = CollisionRecount(possible_positions);

  switch (piece.getType()) {
    case PieceType::tKING:moveGenerator::LeftCastleCheck(&possible_moves);
      moveGenerator::RightCastleCheck(&possible_moves);
      break;
    case PieceType::tPONE:possible_positions = PoneMove();
      // DoubleSize
      if (moveGenerator::piece_.getPieceColor() == PieceColor::WHITE
          && moveGenerator::piece_.getPosition().GetX() == 2
          && GoByVector(Position(0, 1)).size() > 2) {
        possible_moves.emplace_back(
            moveGenerator::piece_, moveGenerator::piece_.getPosition()
                + Position(0, 2));
        possible_moves.back().SetIsDoubleDistancePone(true);
      }
      // Atack by diagonal
      for (int i = -1; i <= 1; i += 2) {
        auto go_diagonal = GoByVector(Position(i, 1));
        if (go_diagonal.size() == 1
            && CollisionRecount(go_diagonal).size() == 1) {
          possible_moves.emplace_back(CollisionRecount(go_diagonal)[0]);
        }
      }
      break;
    default:break;
  }

  std::vector<Move> moves;
  for (const auto& move: possible_moves) {
    if (NoShahCheck(move)) {
      moves.push_back(move);
    }
  }
  // zagrożenie shachu
  return moves;
}

std::vector<Position> moveGenerator::HorseMove() {
  std::vector<Position> distance_to_move;
  for (int left = -1; left <= 1; left += 2) {
    for (int top = -1; top <= 2; top += 2) {
      distance_to_move.emplace_back(left * 2, top * 3);
      distance_to_move.emplace_back(left * 3, top * 2);
    }
  }
  for (auto& distance: distance_to_move) {
    distance = moveGenerator::piece_.getPosition() + distance;
  }
  return distance_to_move;
}

std::vector<Move> moveGenerator::CollisionRecount(const std::vector<Position>& position_to_move) {
  std::vector<Move> clear_positions;
  for (const auto& position: position_to_move) {
    auto piece_on_pos = moveGenerator::board_.GetPiece(position);
    if (piece_on_pos.getType() == PieceType::tNONE) {
      clear_positions.emplace_back(moveGenerator::piece_, position);
      continue;
    }
    auto posY = piece_on_pos.getPosition().GetY();
    auto posX = piece_on_pos.getPosition().GetX();
    if (posY < 0 || posY > 8 || posX < 0 || posX > 8) {
      continue;
    }
    if (piece_on_pos.getPieceColor() == moveGenerator::piece_.getPieceColor()) {
      continue;
    }
    clear_positions.emplace_back(moveGenerator::piece_, position);
  }
  return clear_positions;
}

std::vector<Position> moveGenerator::KingMove() {
  std::vector<Position> distance_to_move;
  for (int left = -1; left <= 1; left += 1) {
    for (int top = -1; top <= 1; top += 1) {
      if (left == top) {
        continue;
      }
      distance_to_move.emplace_back(left, top);
    }
  }
  for (auto& distance: distance_to_move) {
    distance = moveGenerator::piece_.getPosition() + distance;
  }
  return distance_to_move;
}

std::vector<Position> moveGenerator::RuckMove() {
  std::vector<Position> position_to_move;
  for (int top = -1; top <= 1; top += 2) {
    auto new_vector = GoByVector(Position(0, top));
    position_to_move.insert(position_to_move.end(), new_vector.begin(),
                            new_vector.end());
    new_vector = GoByVector(Position(top, 0));
    position_to_move.insert(position_to_move.end(), new_vector.begin(),
                            new_vector.end());

  }

  return position_to_move;
}

std::vector<Position> moveGenerator::GoByVector(Position vector) {
  if (vector.GetX() == 0 && vector.GetY() == 0) {
    return {};
  }

  std::vector<Position> position_to_move;
  for (int length = 1; length <= 8; length++) {
    int px = length * vector.GetX();
    int py = length * vector.GetY();
    auto new_position =
        Position(px, py) + moveGenerator::piece_.getPosition();
    auto posY = new_position.GetY();
    auto posX = new_position.GetX();
    if (posY < 0 || posY > 8 || posX < 0 || posX > 8) {
      break;
    }
    const auto& piece_on_pos = moveGenerator::board_.GetPiece(new_position);
    if (piece_on_pos.getType() == PieceType::tNONE) {
      position_to_move.emplace_back(new_position);
      continue;
    }
    if (piece_on_pos.getPieceColor()
        != moveGenerator::piece_.getPieceColor()) {
      position_to_move.emplace_back(new_position);
    }
    break;
  }
  return position_to_move;
}

std::vector<Position> moveGenerator::QueenMove() {
  std::vector<Position> position_to_move;
  for (int left = -1; left <= 1; left += 1) {
    for (int top = -1; top <= 1; top += 1) {
      if (left == 0 && top == 0) {
        continue;
      }
      auto new_vector = GoByVector(Position(left, top));
      position_to_move.insert(position_to_move.end(), new_vector.begin(),
                              new_vector.end());
    }
  }
  return position_to_move;

}
std::vector<Position> moveGenerator::BishopMove() {
  std::vector<Position> position_to_move;
  for (int left = -1; left <= 1; left += 2) {
    for (int top = -1; top <= 1; top += 2) {
      auto new_vector = GoByVector(Position(left, top));
      position_to_move.insert(position_to_move.end(), new_vector.begin(),
                              new_vector.end());
    }
  }
  return position_to_move;
}
std::vector<Position> moveGenerator::PoneMove() {
  std::vector<Position> position_to_move;
  int length =
      moveGenerator::piece_.getPieceColor() == PieceColor::WHITE ? 1 : -1;
  auto new_position = Position(0, length) + moveGenerator::piece_.getPosition();
  auto posY = new_position.GetY();
  if (posY < 0 || posY > 8) {
    return {};
  }
  const auto& piece_on_pos = moveGenerator::board_.GetPiece(new_position);
  if (piece_on_pos.getType() == PieceType::tNONE) {
    return {new_position};
  }
  return position_to_move;
}

std::vector<Move> moveGenerator::RulesValidation(const std::vector<Move>& possible_moves) {
  std::vector<Move> moves;

  return moves;
}
void moveGenerator::LeftCastleCheck(std::vector<Move>* p_vector) {
  if (moveGenerator::board_.IsLc()) {
    for (auto& move: *p_vector) {
      move.SetBrakeLeftCastle(true);
    }
    auto position_left = GoByVector(Position(-1, 0));
    if (position_left.size() == 4
        && std::find_if(p_vector->begin(),
                        p_vector->end(),
                        [](const Move& move) {
                          return move.GetEnd().GetX() == 0;
                        }) != p_vector->end()) {
      for (int i = -3; i >= -2; i++) {
        p_vector->emplace_back(
            moveGenerator::piece_, moveGenerator::piece_.getPosition()
                + Position(i, 0));
        p_vector->back().SetIsCastle(true);
      }
    }
  }
}
void moveGenerator::RightCastleCheck(std::vector<Move>* p_vector) {
  if (moveGenerator::board_.IsRc()) {
    for (auto& move: *p_vector) {
      move.SetBrakeRightCastle(true);
    }
    auto position_left = GoByVector(Position(1, 0));
    if (position_left.size() == 3
        && std::find_if(p_vector->begin(),
                        p_vector->end(),
                        [](const Move& move) {
                          return move.GetEnd().GetX() == 7;
                        }) != p_vector->end()) {
      p_vector->emplace_back(
          moveGenerator::piece_, moveGenerator::piece_.getPosition()
              + Position(2, 0));
      p_vector->back().SetIsCastle(true);
    }
  }

}

bool moveGenerator::NoShahCheck(const Move& move) {
  std::vector<Position> position_to_move;
  for (int top = -1; top <= 1; top += 2) {
    auto new_vector = CollisionRecount(GoByVector(Position(0, top)));
    if (!new_vector.empty()
        && moveGenerator::board_.GetPiece(new_vector.back().GetEnd()).getType()
            == PieceType::tRUCK)
      new_vector = GoByVector(Position(top, 0));
  }
  return false;
}
