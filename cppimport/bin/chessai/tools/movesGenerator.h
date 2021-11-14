#ifndef CHESS_MOVEGENERATOR_H
#define CHESS_MOVEGENERATOR_H

#include "board/board.h"

class MovesGenerator {
 public:
  std::list<Move> generateMoves(std::shared_ptr<Board> board,
                                const std::shared_ptr<const Piece>& piece );

  bool isShah(const std::shared_ptr<Board>& board, bool is_white);

 private:
  std::list<Position> goByVector(
      Position move_vector, short max_length = 8,
      bool only_tNone = false, bool reduce_tNone = false);

  void bishopMove(bool reduce_tNone = false);
  void horseMove(bool reduce_tNone = false);
  void queenMove(bool reduce_tNone = false);
  void kingMove(bool reduce_tNone = false);
  void ponePacificMove(bool reduce_tNone = false);
  void ruckMove(bool reduce_tNone = false);
  void poneAttackMove();
  void castleMove();

  bool isShahDanger(const Move& move);

  bool isPieceOnMoves(PieceType type);
  void insertPositionsToMoves(const std::list<Position>& positions);

  std::shared_ptr<const Piece> current_piece_;
  std::shared_ptr<Board> board_;
  std::list<Move> moves_;
  int defende_score = 0;
};
#endif //CHESS_MOVEGENERATOR_H
