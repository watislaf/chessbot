#ifndef CHESS_CHESSAI_H
#define CHESS_CHESSAI_H

#include <memory>
#include "movesTree.h"

#include "bitBoard/bBoard.h"
#include "bitBoard/bMove.h"
#include "bitBoard/bMovesGenerator.h"

#include <string>
#include <algorithm>
#include <tools/pricer.h>
#include <iostream>

class ChessEngine {
 public:
  explicit ChessEngine(std::string difficulty = "A1");
  void startNewGame(const std::string& fen_str);
  void applyMoveParams(short fx, short fy, short tx, short ty,
                       char new_piece = '_');
  void applyMove(const BMove&);

  std::string getPossibleMovesForPosition(short x, short y);

  std::string getBoardStr() const;
  std::string getBestMoveStr();
  bool isMoveExists();

  char whosTurn() const;

  BMove getBestMove();

 private:
  std::shared_ptr<BBoard> main_board_;
  std::shared_ptr<MovesTree> tree_moves_;
  AiAdvanceLvl mode_;

};

#endif //CHESS_CHESSAI_H
