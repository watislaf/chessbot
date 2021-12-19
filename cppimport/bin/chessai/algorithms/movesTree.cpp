#ifndef CHESS_MOVESTREE_H
#define CHESS_MOVESTREE_H

#include "movesTree.h"

MovesTree::MovesTree(const Board& original_board,
                     short tree_grow)
    : board_(std::make_shared<Board>(original_board)) {
  auto zero_piece = original_board.getPiece(Position(0, 0));
  main_node_ = std::make_shared<Node>(Move(zero_piece, zero_piece),
                                      original_board.getMoveCount(), 0);
  max_height_ = original_board.getMoveCount();
  max_height_ += tree_grow;

  generateMovesForNode(main_node_, board_);
}

void MovesTree::generateMovesForNode(const std::shared_ptr<MovesTree::Node>& node,
                                     const std::shared_ptr<Board>& board_coppy) {
  if (node->edges.size() != 0) {
    return;
  }
  //1
  const auto active = board_coppy->getActivePieceList(
      board_coppy->isWhiteMove());

  for (const auto& active_piece: active) {
    const auto& moves =
        MovesGenerator(board_coppy, active_piece).generateMoves();

    for (const auto& move: *moves) {
      int price = pricer.countOrder(board_coppy, move);
      int mate = 0;
      if (move.getEnd()->getPosition() == Position(2, 1)) {
        int k = 13;
      }
      board_coppy->apply(move);
      if (MovesGenerator(board_coppy).isMate()) {
        if (board_coppy->isWhiteMove()) {
          mate = -10000;
        } else {
          mate = 10000;
        }
      }
      board_coppy->unApply(move);

      int new_board_sum = node->board_sum + price + mate;
      node->edges.emplace_back(std::make_shared<MovesTree::Node>(move,
                                                                 node->height
                                                                     + 1,
                                                                 new_board_sum));
    }
  }
  std::sort(node->edges.begin(),
            node->edges.end(),
            [this](const std::shared_ptr<Node>& l,
                   const std::shared_ptr<Node>& r) {
              if (board_->isWhiteMove()) {
                return l->board_sum > r->board_sum;
              } else {
                return l->board_sum < r->board_sum;
              }
            });
}

Move MovesTree::getBestMove() {
  makeTreeDeeper(main_node_, board_, max_height_, false);
  for (const auto& node: main_node_->edges) {
    if (node->best_price_ == main_node_->best_price_) {
      return node->move_to_get_here;
    }
  }

  if (main_node_->edges.size() != 0) {
    std::cerr << "DOUBLESUKA ";
  }
  auto empty_piece = std::make_shared<Piece>();
  return Move(empty_piece, empty_piece);
}

Move MovesTree::apply(const Move& move) {
  std::shared_ptr<Node> node_by_this_move;

  generateMovesForNode(main_node_, board_);
  for (auto& node: main_node_->edges) {
    if (node->move_to_get_here.getEnd()->getPosition()
        == move.getEnd()->getPosition() &&
        node->move_to_get_here.getStart()->getPosition()
            == move.getStart()->getPosition() &&
        node->move_to_get_here.getNewPieceType() == move.getNewPieceType()) {
      node_by_this_move = node;
      break;
    }
  }
  if (node_by_this_move != nullptr) {
    // SET BEST PRICE
    board_->apply(node_by_this_move->move_to_get_here);
    generateMovesForNode(node_by_this_move, board_);
    max_height_ += 1;
    main_node_ = node_by_this_move;
  }

  if (node_by_this_move == nullptr) {
    std::cout << "CANT APPLY MOVE, DOES NOT EXIST";
    throw 42;
  }
  return main_node_->move_to_get_here;
}

bool MovesTree::isMoveExists() {
  return !main_node_->edges.empty();
}

void MovesTree::makeTreeDeeper(const std::shared_ptr<MovesTree::Node>& current_node,
                               const std::shared_ptr<Board>& board_coppy,
                               short max_height, bool unaply,
                               int prev_node_price, bool capture_only) {
  generateMovesForNode(current_node, board_coppy);

  if (board_coppy->isWhiteMove()) {
    current_node->best_price_ = -100000000;
  } else {
    current_node->best_price_ = 10000000;
  }

  if (prev_node_price == 10000001) { // if value unset(default) - set
    prev_node_price = -current_node->best_price_;
  }

  if (capture_only) {
    ProcessUntilAttacksAndShachsEnd(current_node,
                                    board_coppy,
                                    max_height,
                                    prev_node_price);
  } else {
    ProcessUntilHightLimit(current_node,
                           board_coppy,
                           max_height,
                           prev_node_price);
  }

  if (current_node->edges.empty()) {
    if (!MovesGenerator(board_coppy).isShah(board_coppy->isWhiteMove())) {
      current_node->best_price_ *= -1;
    }
    current_node->best_price_ /= (board_coppy->getMoveCount() + 1);
  }

  if (unaply) {
    board_coppy->unApply(current_node->move_to_get_here);
  }
}

void MovesTree::ProcessUntilAttacksAndShachsEnd(const std::shared_ptr<MovesTree::Node>& current_node,
                                                const std::shared_ptr<Board>& board_coppy,
                                                int max_height,
                                                int alpha) {
  bool existed = false;

  for (const auto& child_node: current_node->edges) {

    if (child_node->move_to_get_here.getAttackScore() == 0) {
      board_coppy->apply(child_node->move_to_get_here);
      bool is_shah =
          MovesGenerator(board_coppy).isShah(board_coppy->isWhiteMove());
      board_coppy->unApply(child_node->move_to_get_here);
      if (!is_shah) {
        continue;
      }
    }
    existed = true;

    if (child_node->height <= max_height + 5) {
      board_coppy->apply(child_node->move_to_get_here);

      makeTreeDeeper(child_node, board_coppy, 0, true,
                     current_node->best_price_, true);
    }
    bool continue_search = updateBest(current_node, child_node->best_price_,
                                      alpha, board_coppy->isWhiteMove());
    if (!continue_search) {
      break;
    }
  }

  if (!existed) {
    current_node->best_price_ = current_node->board_sum;
  }

}
void MovesTree::ProcessUntilHightLimit(const std::shared_ptr<MovesTree::Node>& current_node,
                                       const std::shared_ptr<Board>& board_coppy,
                                       short max_height, int alpha) {
  for (const auto& child_node: current_node->edges) {
    if (child_node->height <= max_height) {
      board_coppy->apply(child_node->move_to_get_here);
      bool capture_only = child_node->move_to_get_here.getAttackScore() != 0;
      makeTreeDeeper(child_node,
                     board_coppy, max_height, true,
                     current_node->best_price_, capture_only);
    } else {
      child_node->best_price_ = child_node->board_sum;
    }

    bool continue_search = updateBest(current_node, child_node->best_price_,
                                      alpha, board_coppy->isWhiteMove());
    if (!continue_search) {
      break;
    }
  }
}
bool MovesTree::updateBest(const std::shared_ptr<MovesTree::Node>& current_node,
                           int child_tmp, int alpha,
                           bool is_white_move) {
  if (is_white_move) {
    current_node->best_price_ =
        std::max(child_tmp, current_node->best_price_);
    if (current_node->best_price_ >= alpha) {
      return false;
    }
  } else {
    current_node->best_price_ =
        std::min(child_tmp, current_node->best_price_);
    if (current_node->best_price_ <= alpha) {
      return false;
    }
  }
  return true;
}

#endif //CHESS_MOVESTREE_H
