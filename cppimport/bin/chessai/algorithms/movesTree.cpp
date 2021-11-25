#ifndef CHESS_MOVESTREE_H
#define CHESS_MOVESTREE_H

#include "movesTree.h"

MovesTree::MovesTree(const Board& original_board,
                     short tree_grow,
                     AiAdvanceLvl mode)
    : board_(std::make_shared<Board>(original_board)), mode_(mode) {
  auto zero_piece = original_board.getPiece(Position(0, 0));
  main_node_ = std::make_shared<Node>(Move(zero_piece, zero_piece),
                                      original_board.getMoveCount(), 0);
  max_height_ = original_board.getMoveCount();
  max_height_ += tree_grow;
  tree_grow_ = tree_grow;

  generateMovesForNode(main_node_, board_);
  if (mode == AiAdvanceLvl::A2) {
    terminate_build_loop = false;
    additional_thread =
        std::make_shared<std::thread>(&MovesTree::builLoop, this);
  }

}
void MovesTree::builLoop() {
  while (!terminate_build_loop) {
    board_mainnode_mtx.lock();
    std::shared_ptr<Board> board_coppy = std::make_shared<Board>(*board_);
    std::shared_ptr<Node> node = main_node_;
    board_mainnode_mtx.unlock();
    makeTreeDeeper(node, board_coppy, max_height_ + 1, false);
    max_height_ += 1;
  }
}

MovesTree::~MovesTree() {
  terminate_build_loop = true;
  if (additional_thread != nullptr) {
    nodes_mtx.lock();
    killNodes(main_node_);
    nodes_mtx.unlock();
    additional_thread->join();
  }
}

void MovesTree::generateMovesForNode(const std::shared_ptr<MovesTree::Node>& node,
                                     const std::shared_ptr<Board>& board_coppy) {
  if (node->edges.size() != 0) {
    return;
  }

  const auto active =
      board_coppy->getActivePieceList(board_coppy->isWhiteMove());
  for (const auto& active_piece: active) {
    const auto
        moves = moves_generator_.generateMoves(board_coppy, active_piece);

    for (const auto& move: moves) {
      int price = pricer.countOrder(board_coppy, move);
      int new_board_sum = node->board_sum + price;
      int key = new_board_sum;
      if (board_->isWhiteMove()) {
        key *= -1;
      }
      node->edges.emplace_back(
          key,
          std::make_shared<MovesTree::Node>(move, node->height + 1,
                                            new_board_sum));

    }
  }
  std::sort(node->edges.begin(), node->edges.end());
}

Move MovesTree::getBestMove() {
  if (mode_ != AiAdvanceLvl::A2) {
    makeTreeDeeper(main_node_, board_, max_height_, false);
  } else {
    int times = 20;
    for (; times > 0; times--) {
      price_mtx.lock();
      bool answ = abs(main_node_->best_price_shure) == 100000000;
      price_mtx.unlock();
      if (answ || max_height_ - board_->getMoveCount() < tree_grow_) {
        std::this_thread::sleep_for(wait_to_tree_grow);
      } else {
        break;
      }
    }
    if (times == 0) {
      std::cout << "TO LONG " << max_height_ << " " << board_->getMoveCount()
                << " " << tree_grow_;
      throw 42;
    }
  }
  std::lock_guard<std::mutex> price_guard(price_mtx);
  std::lock_guard<std::mutex> nodes_guard(nodes_mtx);
  for (const auto& node_pair: main_node_->edges) {
    const auto& node = node_pair.second;
    if (mode_ == AiAdvanceLvl::A1) {
      if (node->best_price_tmp == main_node_->best_price_tmp) {
        return node->move_to_get_here;
      }
    } else {
      if (node->best_price_shure == main_node_->best_price_shure) {
//      std::cout << max_height_ - board_->getMoveCount();
        return node->move_to_get_here;
      }
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
  for (auto& node_pair: main_node_->edges) {
    auto& node = node_pair.second;
    if (node->move_to_get_here.getEnd()->getPosition()
        == move.getEnd()->getPosition() &&
        node->move_to_get_here.getStart()->getPosition()
            == move.getStart()->getPosition() &&
        node->move_to_get_here.getNewPieceType() == move.getNewPieceType()) {
      node_by_this_move = node;
    } else {
      nodes_mtx.lock();
      killNodes(node);
      nodes_mtx.unlock();
    }
  }
  if (node_by_this_move != nullptr) {
    // SET BEST PRICE

    board_mainnode_mtx.lock();
    board_->apply(node_by_this_move->move_to_get_here);
//    transpositionController.apply(board_);
    nodes_mtx.lock();
    generateMovesForNode(node_by_this_move, board_);
    nodes_mtx.unlock();

    if (mode_ != AiAdvanceLvl::A2) {
      max_height_ += 1;
    }

    main_node_->dead = true;
    main_node_ = node_by_this_move;
    board_mainnode_mtx.unlock();
  }

  if (node_by_this_move == nullptr) {
    std::cout << "CANT APPLY MOVE, DOES NOT EXIST";
    throw 42;
  }
  return main_node_->move_to_get_here;
}

void MovesTree::killNodes(const std::shared_ptr<MovesTree::Node>& node) {
  if (node->dead) {
    return;
  }
  node->dead = true;
  for (auto& node_child: node->edges) {
    killNodes(node_child.second);
  }
}

void MovesTree::ApplyRezult(const std::shared_ptr<Node>& node) {
  node->best_price_shure = node->best_price_tmp;
  if (node->is_cut) {
    return;
  }
  for (auto& node_c_p: node->edges) {
    auto& node_c = node_c_p.second;
    if (!node_c->dead) {
      ApplyRezult(node_c);
    } else {
      node_c->best_price_shure = node_c->best_price_tmp;

      int key = node_c->best_price_tmp;
      if (node->move_to_get_here.getStart()->getPieceColor()
          == PieceColor::WHITE) {
        key *= -1;
      }
      node_c_p.first = key;
    }
  }
//   std::sort(node->edges.begin(), node->edges.end()); // new for threading
}

bool MovesTree::isMoveExists() {
  return !main_node_->edges.empty();
}
int var = 0;
void MovesTree::makeTreeDeeper(const std::shared_ptr<MovesTree::Node>& current_node,
                               const std::shared_ptr<Board>& board_coppy,
                               short max_height,
                               bool unaply,
                               int prev_node_price, bool capture_only) {
  if (unaply == false) {
    var = 0;
  }
  var++;
  current_node->is_cut = false;

//  int price_from_
//  if (transpositionController.get(board_coppy)
//      != transpositionController.empyVal())
  // CREATE HASH AND GET move FROM MAP
  nodes_mtx.lock();
  generateMovesForNode(current_node, board_coppy);
  nodes_mtx.unlock();

  if (board_coppy->isWhiteMove()) {
    current_node->best_price_tmp = -100000000;
  } else {
    current_node->best_price_tmp = 10000000;
  }

  if (prev_node_price == 10000001) { // if value unset(default) - set
    prev_node_price = -current_node->best_price_tmp;
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
    if (!moves_generator_.isShah(board_coppy, board_coppy->isWhiteMove())) {
      current_node->best_price_tmp *= -1;
    }
    current_node->best_price_tmp /= board_coppy->getMoveCount();
  }

  if (unaply) {
    board_coppy->unApply(current_node->move_to_get_here);
  } else {
    if (mode_ == AiAdvanceLvl::A2) {
      std::cout << var;
      price_mtx.lock();
      nodes_mtx.lock();
      ApplyRezult(current_node);
      nodes_mtx.unlock();
      price_mtx.unlock();
    }
  }
}
void MovesTree::ProcessUntilAttacksAndShachsEnd(const std::shared_ptr<MovesTree::Node>& current_node,
                                                const std::shared_ptr<Board>& board_coppy,
                                                int max_height,
                                                int alpha) {
  bool existed = false;

  for (const auto& child_node_pair: current_node->edges) {
    const auto& child_node = child_node_pair.second;
    if (child_node->move_to_get_here.getAttackScore()
        == 0) { // ADD IS SHACH to move
      continue;
    }
    existed = true;
    if (child_node->dead) {
      break;
    }

    if (child_node->height <= max_height + 2) {
      board_coppy->apply(child_node->move_to_get_here);

      makeTreeDeeper(child_node, board_coppy, 0, true,
                     current_node->best_price_tmp, true);
    }
    bool continue_search = updateBest(current_node, child_node->best_price_tmp,
                                      alpha, board_coppy->isWhiteMove());
    if (!continue_search) {
      break;
    }
  }

  if (!existed) {
    current_node->best_price_tmp = current_node->board_sum;
  }

}
void MovesTree::ProcessUntilHightLimit(const std::shared_ptr<MovesTree::Node>& current_node,
                                       const std::shared_ptr<Board>& board_coppy,
                                       short max_height, int alpha) {
  for (const auto& child_node_pair: current_node->edges) {
    const auto& child_node = child_node_pair.second;
    if (child_node->dead) {
      continue;
    }
    if (child_node->height <= max_height) {
      board_coppy->apply(child_node->move_to_get_here);
      bool capture_only = child_node->move_to_get_here.getAttackScore() != 0;
      makeTreeDeeper(child_node,
                     board_coppy, max_height, true,
                     current_node->best_price_tmp, capture_only);
    } else {
      child_node->best_price_tmp = child_node->board_sum;
    }

    bool continue_search = updateBest(current_node, child_node->best_price_tmp,
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
    current_node->best_price_tmp =
        std::max(child_tmp, current_node->best_price_tmp);
    if (current_node->best_price_tmp >= alpha) {
      current_node->is_cut = true;
      return false;
    }
  } else {
    current_node->best_price_tmp =
        std::min(child_tmp, current_node->best_price_tmp);
    if (current_node->best_price_tmp <= alpha) {
      current_node->is_cut = true;
      return false;
    }
  }
  return true;
}

#endif //CHESS_MOVESTREE_H
