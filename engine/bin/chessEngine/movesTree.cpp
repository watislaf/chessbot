#ifndef CHESS_MOVESTREE_H
#define CHESS_MOVESTREE_H

#include "movesTree.h"
MovesTree::MovesTree(const BBoard& original_board)
    : board_(std::make_shared<BBoard>(original_board)) {
  main_node_ = std::make_shared<Node>(BMove(), 0);
  current_tree_height_ = original_board.getMoveCount();

  generateMovesForNode(main_node_);
}

void MovesTree::generateMovesForNode(const std::shared_ptr<MovesTree::Node>& node) {
  if (node->edges.size() != 0) {
    return;
  }

  const auto& moves =
      BMovesGenerator::generate(&*board_);

  for (const auto& BMove: moves) {
    int price = pricer.countOrder(&*board_, BMove);
    int mate = 0;

//    board_->apply(BMove);
//    if (BMovesGenerator(board_).isMate()) {
//      if (board_->isWhiteTurn()) {
//        mate = -100000;
//      } else {
//        mate = 100000;
    //     }
    //   }

    int new_board_sum = node->board_sum + price + mate;
    node->edges.emplace_back(std::make_shared<MovesTree::Node>(BMove,
                                                               new_board_sum));

  }

  std::sort(node->edges.begin(),
            node->edges.end(),
            [this](const std::shared_ptr<Node>& l,
                   const std::shared_ptr<Node>& r) {
              if (board_->isWhiteTurn()) {
                return l->board_sum > r->board_sum;
              } else {
                return l->board_sum < r->board_sum;
              }
            });
}

BMove MovesTree::getBestMove() {
  if (max_height_ != board_->getMoveCount()) {
    start_ = std::chrono::high_resolution_clock::now();

    makeTreeDeeper(
        main_node_, board_->getMoveCount(), 0,
        -getMinusInf(board_->isWhiteTurn()), false);

    auto stop = std::chrono::high_resolution_clock::now();

    auto duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(stop - start_);
    if (duration > max_time_to_make_move_) {
      if (max_increase_times != 2) {
        max_height_--;
        max_increase_times++;
        going_to_increase_ = false;
        std::cerr << "Decreased tree deep\n";
      }
    }
    if (duration < max_time_to_make_move_ / 40) {
      if (going_to_increase_ && max_increase_times > 0) {
        max_height_++;
        max_increase_times--;
        std::cerr << "Increased tree deep\n";
        going_to_increase_ = false;
      } else {
        going_to_increase_ = true;
      }
    }

  } else {
    return main_node_->edges[rand()
        % main_node_->edges.size()]->move_to_get_here;
  }
  for (const auto& node: main_node_->edges) {
    if (node->best_price_ == main_node_->best_price_) {
      return node->move_to_get_here;
    }
  }

  if (main_node_->edges.size() != 0) {
    std::cerr << "I Lost :< ";
    return main_node_->edges[0]->move_to_get_here;
  }
  return BMove();
}

BMove MovesTree::apply(const BMove& BMove) {
  std::shared_ptr<Node> node_by_this_move;

  for (auto& node: main_node_->edges) {
    if (node->move_to_get_here.getFrom() == BMove.getFrom() &&
        node->move_to_get_here.getTo() == BMove.getTo() &&
        ((!BMove.isPromotion()) ||
            node->move_to_get_here.getNewPieceType()
                == BMove.getNewPieceType())) {
      node_by_this_move = node;
      break;
    }
  }
  if (node_by_this_move != nullptr) {
    // SET BEST PRICE
    board_->apply(node_by_this_move->move_to_get_here);
    generateMovesForNode(node_by_this_move);
    max_height_ += 1;
    current_tree_height_ += 1;
    main_node_ = node_by_this_move;
  }
  if (node_by_this_move == nullptr) {
    std::cout << "CANT APPLY BMove, DOES NOT EXIST -> " << BMove.toStr();
    throw 42;
  }
  return main_node_->move_to_get_here;
}

bool MovesTree::isMoveExists() {
  return !main_node_->edges.empty();
}

void MovesTree::makeTreeDeeper(const std::shared_ptr<MovesTree::Node>& current_node,
                               const short& current_childs_height,
                               const int& grand_father_price,
                               const int& prev_node_price,
                               bool capture_only) {
  generateMovesForNode(current_node);
  current_node->best_price_ = getMinusInf(board_->isWhiteTurn());

  if (current_node->edges.empty()) {
    if (!board_->isShah(board_->whosTurn())) {
      current_node->best_price_ *= -1;
    }

    current_node->best_price_ -= 5 * (board_->getMoveCount() + 1);
  } else {
    if (capture_only) {
      ProcessUntilAttacksAndShachsEnd(current_node,
                                      current_childs_height + 1,
                                      prev_node_price,
                                      grand_father_price);
    } else {
      ProcessUntilHightLimit(current_node,
                             current_childs_height + 1,
                             prev_node_price,
                             grand_father_price);
    }
  }

  if (current_tree_height_ != board_->getMoveCount()) {
    board_->unApply(current_node->move_to_get_here);
  }
}

void MovesTree::ProcessUntilAttacksAndShachsEnd(const std::shared_ptr<MovesTree::Node>& current_node,
                                                const short& current_childs_height,
                                                const int& alpha,
                                                const int& grand_father_price) {

  for (const auto& child_node: current_node->edges) {

    if (isNodeToWeak(current_childs_height - current_tree_height_,
                     board_->isWhiteTurn(),
                     grand_father_price, child_node->best_price_)
        ) {
      if (!updateBestResultAndReturnReasonToContinue(
          current_node, child_node->best_price_,
          alpha, board_->isWhiteTurn())) {
        break;
      }
      continue;
    }
    //TODO: GENERATE ONLY CAPTURES 
    if (!child_node->move_to_get_here.isCapture()
        && !child_node->move_to_get_here.isPromotion()) {
      board_->apply(child_node->move_to_get_here);
      bool is_shah = board_->isShah(board_->whosTurn());
      board_->unApply(child_node->move_to_get_here);

      if (!is_shah) {
        if (current_childs_height <= max_height_) {
          board_->apply(child_node->move_to_get_here);
          makeTreeDeeper(child_node,
                         current_childs_height,
                         alpha,
                         current_node->best_price_,
                         false);

        }
        if (!updateBestResultAndReturnReasonToContinue(
            current_node, child_node->best_price_,
            alpha, board_->isWhiteTurn())) {
          break;
        }
        continue;
      }
    }

    if (current_childs_height <= max_height_ + 8) {
      board_->apply(child_node->move_to_get_here);
      makeTreeDeeper(child_node,
                     current_childs_height,
                     alpha,
                     current_node->best_price_,
                     true);
    }
    if (!updateBestResultAndReturnReasonToContinue(
        current_node, child_node->best_price_,
        alpha, board_->isWhiteTurn())) {
      break;
    }
  }

}
void MovesTree::ProcessUntilHightLimit(const std::shared_ptr<MovesTree::Node>& current_node,
                                       const short& current_childs_height,
                                       const int& alpha,
                                       const int& grand_father_price) {
  if (current_tree_height_ == board_->getMoveCount()) {
    start_ = std::chrono::high_resolution_clock::now();
  }

  for (const auto& child_node: current_node->edges) {
    bool capture_only = child_node->move_to_get_here.isCapture()
        || child_node->move_to_get_here.isPromotion();
    if (!isNodeToWeak(current_childs_height - current_tree_height_,
                      board_->isWhiteTurn(),
                      grand_father_price,
                      child_node->best_price_) &&
        current_childs_height <= max_height_ || capture_only) {
      board_->apply(child_node->move_to_get_here);
      makeTreeDeeper(child_node,
                     current_childs_height,
                     alpha,
                     current_node->best_price_,
                     capture_only);

    }
    if (!updateBestResultAndReturnReasonToContinue(
        current_node, child_node->best_price_,
        alpha, board_->isWhiteTurn())) {
      break;
    }
    if (current_tree_height_ == board_->getMoveCount()) {
      auto stop = std::chrono::high_resolution_clock::now();
      if (std::chrono::duration_cast<std::chrono::milliseconds>(stop - start_)
          > max_time_to_make_move_) {
        return;
      }
    }
  }
}

bool MovesTree::updateBestResultAndReturnReasonToContinue(const std::shared_ptr<
    MovesTree::Node>& current_node,
                                                          const int& child_tmp,
                                                          const int& alpha,
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

bool MovesTree::isNodeToWeak(const int& delta_moves,
                             bool is_white_turn,
                             const int& grand_father_price,
                             const int& current_price) {

  if (((delta_moves) >= 2)) {
    if (is_white_turn) {
      if ((grand_father_price - current_price > 95)) {

        return true;
      }
    } else {
      if ((current_price - grand_father_price > 95)) {
        return true;
      }
    }
  }
  return false;
}
int MovesTree::getMinusInf(bool turn) {
  if (turn) {
    return -100000000;
  } else {
    return 10000000;
  }

}
void MovesTree::setTreeGrow(short tree_grow) {
  max_height_ = current_tree_height_ + tree_grow;
}
void MovesTree::setMaxTimeTomakeMove(std::chrono::milliseconds max_time_to_make_move) {
  max_time_to_make_move_ = max_time_to_make_move;
}

#endif //CHESS_MOVESTREE_H

