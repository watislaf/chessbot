#ifndef CHESS_CHESSAI_H
#define CHESS_CHESSAI_H

#include <memory>
#include "board/board.h"
#include "tools/movesGenerator.h"
#include "tools/movesTree.h"

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

    void startGameAnalize();

private:
  std::shared_ptr<Board> board_;
    std::shared_ptr<MovesTree> tree_moves_;

  MovesGenerator moves_generator;

    void loopStart(int tree_grow_rate);

    void makeTreeDeeper(std::shared_ptr<MovesTree::Node>);

    void generateMovesForNode(std::shared_ptr<MovesTree::Node> sharedPtr);
};

#endif //CHESS_CHESSAI_H
