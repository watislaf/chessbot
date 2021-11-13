#ifndef CHESS_MOVESTREE_H
#define CHESS_MOVESTREE_H

#include "movesTree.h"

MovesTree::MovesTree(const Board& original_board,
                     int tree_grow,
                     AiAdvanceLvl mode)
    : board_(std::make_shared<Board>(original_board)), mode_(mode) {
  auto zero_piece = original_board.getPiece(Position(0, 0));
  main_node_ = std::make_shared<Node>(Move(zero_piece, zero_piece),
                                      original_board.getMoveCount(), 0);

  max_height_ += tree_grow;
  tree_grow_ = tree_grow;
  makeTreeDeeper(main_node_, board_, max_height_, false);
  if (mode == AiAdvanceLvl::A2) {
    terminate_build_loop = false;
    additional_thread =
        std::make_shared<std::thread>(&MovesTree::builLoop, this);
  }

}

void MovesTree::builLoop() {
  while (!this->terminate_build_loop) {
    board_mainnode_mtx.lock();
    std::shared_ptr<Board> board_coppy = std::make_shared<Board>(*board_);
    std::shared_ptr<Node> node = main_node_;
    board_mainnode_mtx.unlock();
    makeTreeDeeper(node, board_coppy, max_height_ + 1, false);
    max_height_ += 1;
  }
}

void MovesTree::makeTreeDeeper(std::shared_ptr<MovesTree::Node> current_node,
                               std::shared_ptr<Board> board_coppy,
                               int max_height, bool unaply) {
  nodes_mtx.lock();
  if (current_node->edges.empty()) {
    generateMovesForNode(current_node, board_coppy);
  }
  nodes_mtx.unlock();

  if (board_coppy->isWhiteMove()) {
    current_node->best_price_tmp = -1000000000;
  } else {
    current_node->best_price_tmp = 100000000;
  }
  for (const auto& child_node_pair: current_node->edges) {
    const auto& child_node = child_node_pair.second;
    if (child_node->dead) { // THREADING
      continue;
    }
    if (child_node->height == max_height) {
      child_node->best_price_tmp = child_node->board_sum;
      child_node->best_price_shure = child_node->board_sum;
    } else {
      board_coppy->apply(child_node->move_to_get_here);
      makeTreeDeeper(child_node, board_coppy, max_height, true);
    }
    if (board_coppy->isWhiteMove()) {
      current_node->best_price_tmp =
          std::max(child_node->best_price_tmp, current_node->best_price_tmp);
    } else {
      current_node->best_price_tmp =
          std::min(child_node->best_price_tmp, current_node->best_price_tmp);
    }
  }
  if (unaply) {
    board_coppy->unApply(current_node->move_to_get_here);
  } else {
    price_mtx.lock();
    ApplyRezult(current_node);
    price_mtx.unlock();
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

void MovesTree::generateMovesForNode(std::shared_ptr<MovesTree::Node> node,
                                     std::shared_ptr<Board> board_coppy) {
  const auto active =
      board_coppy->getActivePieceList(board_coppy->isWhiteMove());
  for (const auto& active_piece: active) {
    const auto
        moves = moves_generator_.generateMoves(board_coppy, active_piece);
    for (const auto& move: moves) {
      int price = Pricer::count(board_coppy, move);
      int new_board_sum = node->board_sum + price;
      if (board_coppy->isWhiteMove()) {
        price *= -1;
      }
      node->edges.emplace_back(
          price,
          std::make_shared<MovesTree::Node>(move, node->height + 1,
                                            new_board_sum));

    }
  }
  std::sort(node->edges.begin(), node->edges.end());
}
Move MovesTree::getBestMove() {
  // GET OUT MAKE TREE DEEPER AND SET TIMER
  if (mode_ != AiAdvanceLvl::A2) {
    makeTreeDeeper(main_node_, board_, max_height_, false);
  } else {
    for (int times = 20; times > 0; times--) {
      if (max_height_ - board_->getMoveCount() < tree_grow_) {
        std::this_thread::sleep_for(wait_to_tree_grow);
      }
    }
    if (max_height_ - board_->getMoveCount() < tree_grow_) {
      std::cout << "TO LONG " << max_height_ << " " << board_->getMoveCount()
                << " " << tree_grow_;
      throw 42;
    }
  }
  price_mtx.lock();
  for (const auto& node_pair: main_node_->edges) {
    const auto& node = node_pair.second;
    if (node->best_price_shure == main_node_->best_price_shure) {
      price_mtx.unlock();
      std::cout<<"deep - "<< max_height_ - board_->getMoveCount()<<std::endl;
      return node->move_to_get_here;
    }
  }
  price_mtx.unlock();
  if (main_node_->edges.size() != 0) {
    std::cout << "DOUBLESUKA ";
  }
  // IF NOT FOUND RETURN EMPTY MOVE
  auto empty_piece = std::make_shared<Piece>();
  return Move(empty_piece, empty_piece);
}

Move MovesTree::apply(const Move& move) {
  std::shared_ptr<Node> node_by_this_move;
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
    nodes_mtx.lock();
    if (node_by_this_move->edges.empty()) {
      generateMovesForNode(node_by_this_move, board_);
    }
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

void MovesTree::killNodes(std::shared_ptr<MovesTree::Node>& node) {
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
  for (auto& node_c_p: node->edges) {
    auto& node_c = node_c_p.second;
    if (!node_c->dead) {
      ApplyRezult(node_c);
    }
  }
}
bool MovesTree::isMoveExists() {
  return !main_node_->edges.empty();
}

#endif //CHESS_MOVESTREE_H
