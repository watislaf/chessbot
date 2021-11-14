#ifndef CHESS_CHESSAI_H
#define CHESS_CHESSAI_H

#include <memory>
#include "board/board.h"
#include "tools/movesGenerator.h"
#include "algorithms/movesTree.h"
#include <string>
#include <algorithm>
#include <tools/pricer.h>
#include <iostream>

class ChessAi {
 public:
  explicit ChessAi(std::string difficulty = "A1");
  void startNewGame(const std::string& fen_str);
  void applyMoveParams(short fx, short fy, short tx, short ty, char new_piece = '_');
  void applyMove(const Move&);

  std::string getPossibleMovesForPosition(short x, short y);

  std::string getBoardStr() const;
  std::string getFenStr() const;

  std::string getBestMoveStr();
  bool isMoveExists();
  char whosMove() const;

  Move getBestMove();

 private:
  std::shared_ptr<Board> main_board_;
  std::shared_ptr<MovesTree> tree_moves_;
  MovesGenerator moves_generator;
  AiAdvanceLvl mode_ ;

};

#endif //CHESS_CHESSAI_H
