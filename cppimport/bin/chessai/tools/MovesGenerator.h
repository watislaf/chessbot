#ifndef CHESS_MOVEGENERATOR_H
#define CHESS_MOVEGENERATOR_H

#include <board/board.h>

class MovesGenerator {
 public:
  static std::vector<Move> GenerateMoves(const Board& board,
                                         const Piece& piece);
 private:
  static std::vector<Position> GoByVector(Position vector,
                                          int max_length = 8,
                                          bool only_tNone = false,
                                          bool reduce_tNone = false);
  static void HorseMove();
  static void KingMove();
  static void RuckMove();
  static void QueenMove();
  static void BishopMove();
  static void PoneMove();

  static void CastleMove();
  static bool IsShahFree(const Move& move);

  static void InsertPositionsToMoves(const std::vector<Position>& positions);

  static Board board_;
  static Piece piece_;
  static std::vector<Move> moves_;

  static void PoneSpecialMove();
};
#endif //CHESS_MOVEGENERATOR_H
