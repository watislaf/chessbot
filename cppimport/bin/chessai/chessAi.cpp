#include <string>
#include <algorithm>
#include <tools/pricer.h>
#include "chessAi.h"

void ChessAi::startNewGame(const std::string& fen_str) {
  board_ = std::make_shared<Board>(FEN(fen_str));
}

std::string ChessAi::getPossibleMovesForPosition(int x, int y) {
  auto moves = moves_generator.generateMoves(
      board_, board_->getPiece(Position(x, y)));

  moves.erase(
      std::remove_if(
          moves.begin(), moves.end(),
          [](const Move& move) {
            return move.getEnd()->getType() != PieceType::tNONE
                && move.getStart()->getPieceColor()
                    == move.getEnd()->getPieceColor();
          }), moves.end()
  );

  std::string answer;
  for (const auto& move: moves) {
    answer += move.toStr() + " ";
  }

  return answer;
}

std::string ChessAi::getBoardStr() const {
  return board_->toStr();
}

std::string ChessAi::getFenStr() const {
  return board_->getFen();
}

void ChessAi::applyMove(int fx, int fy, int tx, int ty, char niew_piece) {
  auto moves = moves_generator.generateMoves(
      board_, board_->getPiece(Position(fx, fy)));
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
  }
  for (const auto& move: moves) {
    if (move.getEnd()->getPosition() == Position(tx, ty)
        && move.getNewPieceType() == new_piece_type) {
      board_->apply(move);
      return;
    }
  }
  throw 42;
}

char ChessAi::whosMove() const {
  if (board_->isWhiteMove()) {
    return 'w';
  } else {
    return 'b';
  }
}

void ChessAi::startGameAnalize() {
  tree_moves_ = std::make_shared<MovesTree>(*board_);
  int tree_grow_rate = 2;
  loopStart(tree_grow_rate);
}

void ChessAi::loopStart(int tree_grow_rate) {
  while (true) {
    tree_moves_->main_node->max_height += tree_grow_rate;
    makeTreeDeeper(tree_moves_->main_node);
    if (tree_moves_->main_node->edges.empty()) {
      break;
    }
  }
}

void ChessAi::makeTreeDeeper(std::shared_ptr<MovesTree::Node> current_node) {
  board_->apply(current_node->move_to_get_here);
  if (current_node->edges.empty()) {
    generateMovesForNode(current_node);
  }
  if (current_node->max_height != tree_moves_->main_node->max_height) {
    if (board_->isWhiteMove()) {
      current_node->current_price = -10000;
    } else {
      current_node->current_price = 10000;
    }
  }
  for (auto& child_node: current_node->edges) {
    if (child_node->height == current_node->max_height) {
      child_node->current_price =
          Pricer::count(tree_moves_->board_, child_node->move_to_get_here);
    } else {
      makeTreeDeeper(child_node);
    }
    if (board_->isWhiteMove()) {
      current_node->current_price =
          std::max(child_node->current_price, current_node->current_price);
    } else {
      current_node->current_price =
          std::min(child_node->current_price, current_node->current_price);
    }
  }
  board_->unApply(current_node->move_to_get_here);
}

void ChessAi::generateMovesForNode(std::shared_ptr<MovesTree::Node> node) {
  const auto active = board_->getActivePieceList(board_->isWhiteMove());
  for (const auto& active_piece: active) {
    const auto moves = moves_generator.generateMoves(board_, active_piece);
    for (const auto& move: moves) {
      node->edges.push_back(std::make_shared<MovesTree::Node>(move,
                                                              node->height + 1,
                                                              node->max_height));
    }
  }
}

bool ChessAi::isMoveExists() {
  const auto active = board_->getActivePieceList(board_->isWhiteMove());
  for (const auto& active_piece: active) {
    auto moves = moves_generator.generateMoves(board_, active_piece);

    moves.erase(
        std::remove_if(
            moves.begin(), moves.end(),
            [](const Move& move) {
              return move.getEnd()->getType() != PieceType::tNONE
                  && move.getStart()->getPieceColor()
                      == move.getEnd()->getPieceColor();
            }), moves.end()
    );
    if (!moves.empty())
      return true;
  }
  return false;
}

std::string ChessAi::getBestMove() {
  const auto active = board_->getActivePieceList(board_->isWhiteMove());
  std::vector<Move> moveees;
  for (const auto& active_piece: active) {
    auto moves = moves_generator.generateMoves(board_, active_piece);

    moves.erase(
        std::remove_if(
            moves.begin(), moves.end(),
            [](const Move& move) {
              return move.getEnd()->getType() != PieceType::tNONE
                  && move.getStart()->getPieceColor()
                      == move.getEnd()->getPieceColor();
            }), moves.end()
    );
    moveees.insert(moveees.begin(), moves.begin(), moves.end());
  }
  if (moveees.size() == 0) {
    if (moves_generator.isShah())
      return "((9,9),(9,9),k)";
    else
      return "((9,9),(9,9))";
  }
  return moveees[random() % moveees.size()].toStr();
}

