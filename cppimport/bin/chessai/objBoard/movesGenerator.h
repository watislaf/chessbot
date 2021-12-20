#ifndef CHESS_MOVEGENERATOR_H
#define CHESS_MOVEGENERATOR_H

#include "objBoard.h"

class MovesGenerator {
 public:
  MovesGenerator(const std::shared_ptr<ObjBoard>& board,
                 const std::shared_ptr<const Piece>& piece =
                 std::make_shared<const Piece>());
  std::shared_ptr<std::list<Move>> generateMoves();
  bool isShah(bool is_white);
  bool isMate();
 private:
  std::list<Move> goByVector(Position start,
                             Position move_vector,
                             short max_length = 8,
                             bool only_tNone = false,
                             bool reduce_tNone = false);

  void bishopMove(bool reduce_tNone = false);
  void horseMove(bool reduce_tNone = false);
  void queenMove(bool reduce_tNone = false);
  void kingMove(bool reduce_tNone = false);
  void ponePacificMove(bool reduce_tNone = false);
  void ruckMove(bool reduce_tNone = false);
  void poneAttackMove();
  void castleMove();

  bool isShahDanger(const Move& move);

  std::shared_ptr<const Piece> piece_;
  std::shared_ptr<ObjBoard> board_;
  std::list<Move> moves_;
  int defende_score = 0;
};
#endif //CHESS_MOVEGENERATOR_H
