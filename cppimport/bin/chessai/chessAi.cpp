#include <string>
#include <algorithm>
#include <tools/pricer.h>
#include <iostream>
#include "chessAi.h"

ChessAi::ChessAi(std::string advance) {
  if (advance == "random")
    advance_ = aiAdvanceLvl::RANDOM;
  if (advance == "A1")
    advance_ = aiAdvanceLvl::A1;
}

void ChessAi::startNewGame(const std::string& fen_str) {
  main_board_ = std::make_shared<Board>(FEN(fen_str));
  int tree_grow = 3;
  if (advance_ == aiAdvanceLvl::RANDOM) {
    tree_grow = 1;
  }
  tree_moves_ = std::make_shared<MovesTree>(*main_board_, tree_grow);
}

void ChessAi::startGameAnalize() {
  int tree_grow_rate = 2;
//  loopStart(tree_grow_rate);
}

void ChessAi::loopStart(int tree_grow_rate) {
  while (true) {
//    tree_moves_->max_height += tree_grow_rate;
//    makeTreeDeeper(tree_moves_->main_node_);
//    if (tree_moves_->main_node_->edges.empty()) {
//      break;
    //   }
  }
}

std::string ChessAi::getPossibleMovesForPosition(int x, int y) {
  auto moves = moves_generator.generateMoves(
      main_board_, main_board_->getPiece(Position(x, y)));
  std::string answer;
  for (const auto& move: moves) {
    answer += move.toStr() + " ";
  }
  return answer;
}

std::string ChessAi::getBoardStr() const {
  return main_board_->toStr();
}

std::string ChessAi::getFenStr() const {
  return main_board_->getFen();
}

char ChessAi::whosMove() const {
  if (main_board_->isWhiteMove()) {
    return 'w';
  } else {
    return 'b';
  }
}

bool ChessAi::isMoveExists() {
  bool answ = getBestMove().getStart()->getPosition().getX() != 9;
  if (!answ)
    std::cout << "NO MORE MOVES";
  return answ;
}

std::string ChessAi::getBestMoveStr() {
  return getBestMove().toStr();
}

Move ChessAi::getBestMove() {
  auto empty_piece = std::make_shared<Piece>();
  auto empty_move = Move(empty_piece, empty_piece);
  if (main_board_->getMoveCount() > 120) {
    return empty_move;
  }
  auto best_move = tree_moves_->getBestMove();

  if (best_move.getStart()->getPosition()
      == empty_move.getStart()->getPosition()
      && moves_generator.isShah(main_board_, main_board_->isWhiteMove())) {
    best_move.setNewPieceType(PieceType::tKING);

  }

  return best_move;
}

void ChessAi::applyMoveParams(int fx, int fy, int tx, int ty, char niew_piece) {
  PieceType new_piece_type = PieceType::tPONE;
  switch (niew_piece) {
    case 'r':new_piece_type = PieceType::tRUCK;
      break;
    case 'q':new_piece_type = PieceType::tQUEEN;
      break;
    case 'n': new_piece_type = PieceType::tHORSE;
      break;
    case 'b': new_piece_type = PieceType::tBISHOP;
      break;
    case '_':new_piece_type = PieceType::tNONE;
      break;
  }

  auto naked_move = Move(main_board_->getPiece(Position(fx, fy)),
                         main_board_->getPiece(Position(tx, ty)));
  naked_move.setNewPieceType(new_piece_type);
  applyMove(naked_move);
}

void ChessAi::applyMove(const Move& naked_move) {
  if (naked_move.getStart()->getPosition() == Position(9, 9)) {
    std::cout << "CNAT APPLY EMPTY MOVE";
    return;
  }
  bool move_exists = false;
  const auto& full_move = tree_moves_->apply(naked_move);
  main_board_->apply(full_move);
}

