#ifndef CHESS_MOVEGENERATOR_H
#define CHESS_MOVEGENERATOR_H

#include <board/board.h>

class moveGenerator {
 public:
  static std::vector<Move> GenerateMove(const Board& board,
                                        const Piece& piece);
 private:
  static std::vector<Position> GoByVector(Position vector);
  static std::vector<Position> HorseMove();
  static std::vector<Position> KingMove();
  static std::vector<Position> RuckMove();
  static std::vector<Position> QueenMove();

  static std::vector<Move> CollisionRecount(const std::vector<Position>& position_to_move);
  static std::vector<Move> RulesValidation(const std::vector<Move>& possible_moves);
  static Board board_;
  static Piece piece_;
  static std::vector<Position> BishopMove();
  static std::vector<Position> PoneMove();

  static void LeftCastleCheck(std::vector<Move>* p_vector);
  static void RightCastleCheck(std::vector<Move>* p_vector);
  static bool NoShahCheck(const Move& move);
};
#endif //CHESS_MOVEGENERATOR_H
