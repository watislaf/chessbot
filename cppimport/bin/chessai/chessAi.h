#ifndef CHESS_CHESSAI_H
#define CHESS_CHESSAI_H

#include <memory>
#include "board/board.h"
#include "tools/movesGenerator.h"

class ChessAi {
 public:
  void startNewGame(const std::string& FEN);
  std::string getPossibleMovesForPiece(int x, int y);

  std::string getBoardStr() const;

  std::string getFen() const;
 private:
  std::shared_ptr<Board> board_;
  MovesGenerator moves_generator;
};

#endif //CHESS_CHESSAI_H
