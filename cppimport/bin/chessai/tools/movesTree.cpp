#ifndef CHESS_MOVESTREE_H
#define CHESS_MOVESTREE_H

#include <board/board.h>
#include <algorithm>
#include <iostream>
#include "movesTree.h"
#include "pricer.h"

MovesTree::MovesTree(const Board& original_board, int tree_grow)
    : board_(std::make_shared<
    Board>(original_board)) {
  auto zero_piece = original_board.getPiece(Position(0, 0));
  main_node_ = std::make_shared<Node>(Move(zero_piece, zero_piece),
                                      original_board.getMoveCount(), 0);
  max_height += tree_grow;
  makeTreeDeeper(main_node_);

}

void MovesTree::makeTreeDeeper(std::shared_ptr<MovesTree::Node> current_node) {
  if (current_node->edges.empty()) {
    generateMovesForNode(current_node);
  }
  if (board_->isWhiteMove()) {
    current_node->best_price = -1000000000;
  } else {
    current_node->best_price = 100000000;
  }
  for (const auto& child_node_pair: current_node->edges) {
    const auto& child_node = child_node_pair.second;
    if (child_node->height == max_height || child_node->dead) {
      child_node->best_price = child_node->board_sum;
    } else {
      board_->apply(child_node->move_to_get_here);
      makeTreeDeeper(child_node);
    }
    if (board_->isWhiteMove()) {
      current_node->best_price =
          std::max(child_node->best_price, current_node->best_price);
    } else {
      current_node->best_price =
          std::min(child_node->best_price, current_node->best_price);
    }
  }
  if (current_node != main_node_) {
    board_->unApply(current_node->move_to_get_here);
  }
}

void MovesTree::generateMovesForNode(std::shared_ptr<MovesTree::Node> node) {
  const auto active =
      board_->getActivePieceList(board_->isWhiteMove());
  for (const auto& active_piece: active) {
    const auto moves = moves_generator_.generateMoves(board_, active_piece);
    for (const auto& move: moves) {
      int price = Pricer::count(board_, move);
      int new_board_sum =
          node->board_sum + price;
      if (board_->isWhiteMove()) {
        price *= -1;
      }
      node->edges.emplace_back(price,
                               std::make_shared<MovesTree::Node>(move,
                                                                 node->height
                                                                     + 1,
                                                                 new_board_sum));

    }
  }
  std::sort(node->edges.begin(), node->edges.end());
}
Move MovesTree::getBestMove() {
  makeTreeDeeper(main_node_);
  for (const auto& node_pair: main_node_->edges) {
    const auto& node = node_pair.second;
    if (node->best_price == main_node_->best_price) {
      return node->move_to_get_here;
    }
  }
  // IF NOT FOUND RETURN EMPTY MOVE
  auto empty_piece = std::make_shared<Piece>();
  return Move(empty_piece, empty_piece);
}

Move MovesTree::apply(const Move& move) {
  auto does_not_delete_shared_ptr = main_node_;
  bool move_exists;
  for (auto& node_pair: main_node_->edges) {
    auto& node = node_pair.second;

    if (node->move_to_get_here.getEnd()->getPosition()
        == move.getEnd()->getPosition() &&
        node->move_to_get_here.getStart()->getPosition()
            == move.getStart()->getPosition() &&
        node->move_to_get_here.getNewPieceType() == move.getNewPieceType()) {

      main_node_->dead = true;
      main_node_ = node;
      max_height += 1;
      board_->apply(main_node_->move_to_get_here);
      if (node->edges.empty()) {
        generateMovesForNode(node);
      }
      move_exists = true;
    } else {
      killNodes(node);
    }
  }

  if (!move_exists) {
    std::cout << "CANT APPLY MOVE, DOES NOT EXIST";
    throw 42;
  }
  return main_node_->move_to_get_here;
}

void MovesTree::killNodes(std::shared_ptr<MovesTree::Node>& node) {
  if (node->dead) {
    return;
  }
  node->dead = true;
  for (auto& node_child: node->edges) {
    killNodes(node_child.second);
  }
}
#endif //CHESS_MOVESTREE_H
