#ifndef CHESS_MOVEGENERATOR_H
#define CHESS_MOVEGENERATOR_H

#include "board/board.h"

class MovesGenerator {
 public:
  std::vector<Move> generateMoves(std::shared_ptr<Board> board,
                                  std::shared_ptr<const Piece> piece);
 private:
  std::vector<Position> goByVector(Position vector,
                                   int max_length = 8,
                                   bool only_tNone = false,
                                   bool reduce_tNone = false);
  void horseMove(bool reduce_tNone = false);
  void kingMove(bool reduce_tNone = false);
  void ruckMove(bool reduce_tNone = false);
  void queenMove(bool reduce_tNone = false);
  void bishopMove(bool reduce_tNone = false);
  void poneMove(bool reduce_tNone = false);

  void castleMove();
  bool isShahDanger(Move move);
  void poneSpecialMove();
  bool isShah();

  void insertPositionsToMoves(const std::vector<Position>& positions);

  std::shared_ptr<Board> board_;
  std::shared_ptr<const Piece> piece_;
  std::vector<Move> moves_;

  bool movesAreAttacking(PieceType type);
};
#endif //CHESS_MOVEGENERATOR_H
