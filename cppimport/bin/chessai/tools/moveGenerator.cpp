#include <abstracts/move.h>
#include "moveGenerator.h"
std::vector<Move> moveGenerator::GenerateMove(const Board& board,
                                              const Piece& piece) {
  {
    moveGenerator::board_ = board;
    moveGenerator::piece_ = piece;
    switch (piece.getType()) {
      case PieceType::IS_NONE:break;
        return {};
      case PieceType::IS_HORSE:break;
        return HorseMove();
      case PieceType::IS_KING:break;
      case PieceType::IS_QUEEN:break;
      case PieceType::IS_BISHOP:break;
      case PieceType::IS_RUCK:break;
      case PieceType::IS_PONE:break;
    }
  }
}

std::vector<Move> moveGenerator::HorseMove() {
  std::vector<Position> distance_to_move;
  for (int back = -1; back <= 1; back += 2) {
    for (int top = -1; top <= 2; top += 2) {
      distance_to_move.emplace_back(back * 2, top * 3);
      distance_to_move.emplace_back(back * 3, top * 2);
    }
  }
  for (auto& distance: distance_to_move) {
    distance_to_move = moveGenerator::piece_.getPosition() + distance;
  }
  return std::vector<Move>(CollisionCount(distance_to_move));
}
std::vector<Position> moveGenerator::CollisionCount(std::vector<Position> vector_1) {
  return std::vector<Position>();
}
