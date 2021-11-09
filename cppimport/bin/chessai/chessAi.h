#ifndef CHESS_CHESSAI_H
#define CHESS_CHESSAI_H

#include <memory>
#include "board/board.h"
#include "tools/movesGenerator.h"

class ChessAi {
 public:
  void startNewGame(const std::string& fen_str);
  // START GENERATING TREE
  // GIVE BEST MOVE
  // MOVE MADE

  void applyMove(int fx, int fy, int tx, int ty);

  std::string getPossibleMovesForPosition(int x, int y);

  std::string getBoardStr() const;
  std::string getFenStr() const;

 private:
  std::shared_ptr<Board> board_;
  MovesGenerator moves_generator;

};

#endif //CHESS_CHESSAI_H
