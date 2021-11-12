#ifndef CHESS_CHESSAI_H
#define CHESS_CHESSAI_H

#include <memory>
#include <abstracts/aiAdvanceLvl.h>
#include "board/board.h"
#include "tools/movesGenerator.h"
#include "tools/movesTree.h"

class ChessAi {
 public:
  explicit ChessAi(std::string difficulty = "A1");
  void startNewGame(const std::string& fen_str);
  // START GENERATING TREE
  // GIVE BEST MOVE
  // MOVE MADE

  void applyMoveParams(int fx, int fy, int tx, int ty, char new_piece = '_');
  void applyMove(const Move&);

  std::string getPossibleMovesForPosition(int x, int y);

  std::string getBoardStr() const;
  std::string getFenStr() const;

  void startGameAnalize();
  std::string getBestMoveStr();
  bool isMoveExists();
  char whosMove() const;

  Move getBestMove();

 private:
  std::shared_ptr<Board> main_board_;

  std::shared_ptr<MovesTree> tree_moves_;

  MovesGenerator moves_generator;

  void loopStart(int tree_grow_rate);

  aiAdvanceLvl advance_ ;

};

#endif //CHESS_CHESSAI_H
