#ifndef CHESS_CHESSAI_H
#define CHESS_CHESSAI_H

#include <memory>
#include "movesTree.h"

#if ARCH == 32
#include "objBoard/objBoard.h"
#include "objBoard/movesGenerator.h"
#else
#include "bitBoard/bBoard.h"
#include "bitBoard/bMove.h"
#include "bitBoard/bMovesGenerator.h"
#define Move BMove
#define ObjBoard BBoard
#define MovesGenerator BMovesGenerator
#endif

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
  void applyMove(const Move&);

  std::string getPossibleMovesForPosition(short x, short y);

  std::string getBoardStr() const;
  std::string getBestMoveStr();
  bool isMoveExists();

  char whosTurn() const;

  Move getBestMove();

 private:
  std::shared_ptr<ObjBoard> main_board_;
  std::shared_ptr<MovesTree> tree_moves_;
  AiAdvanceLvl mode_;

};

#endif //CHESS_CHESSAI_H
