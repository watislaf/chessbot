#ifndef CHESS_MOVEGENERATOR_H
#define CHESS_MOVEGENERATOR_H

#include <board/board.h>

class moveGenerator {
 public:
    static std::vector<Move> GenerateMove(const Board& board,
                                            const Piece& piece);
 private:
  static std::vector<Move> HorseMove();
  static std::vector<Position> CollisionCount(std::vector<Position> vector_1);

  static Board board_;
  static Piece piece_;
};
#endif //CHESS_MOVEGENERATOR_H
