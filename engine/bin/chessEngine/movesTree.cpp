#ifndef CHESS_MOVESTREE_H
#define CHESS_MOVESTREE_H

#include "movesTree.h"

MovesTree::MovesTree(const ObjBoard& original_board,
                     short tree_grow)
    : board_(std::make_shared<ObjBoard>(original_board)) {
#if ARCH == 32
  auto zero_piece = original_board.getPiece(Position(0, 0));
  main_node_ = std::make_shared<Node>(Move(zero_piece, zero_piece),
                                      original_board.getMoveCount(), 0);
#else
  main_node_ = std::make_shared<Node>(Move(),
                                      original_board.getMoveCount(), 0);
#endif

  max_height_ = original_board.getMoveCount();
  max_height_ += tree_grow;
  generateMovesForNode(main_node_, board_);
}

void MovesTree::generateMovesForNode(const std::shared_ptr<MovesTree::Node>& node,
                                     const std::shared_ptr<ObjBoard>& board_coppy) {
  if (node->edges.size() != 0) {
    return;
  }

#if ARCH == 32
  const auto active = board_coppy->getActivePieceList(
      board_coppy->isWhiteTurn());

  for (const auto& active_piece: active) {
    const auto& moves =
        MovesGenerator(board_coppy, active_piece).generateMoves();

    for (const auto& move: *moves) {
      int price = pricer.countOrder(board_coppy, move);
      int mate = 0;
      int new_board_sum = node->board_sum + price + mate;
      node->edges.emplace_back(std::make_shared<MovesTree::Node>(move,
                                                                 node->height
                                                                     + 1,
                                                                 new_board_sum));
    }
  }
#else
  const auto& moves =
      MovesGenerator::generate(&*board_coppy);

  for (const auto& move: moves) {
    int price = pricer.countOrder(&*board_coppy, move);
    int mate = 0;
//    board_coppy->apply(move);
//    if (MovesGenerator(board_coppy).isMate()) {
//      if (board_coppy->isWhiteTurn()) {
//        mate = -100000;
//      } else {
//        mate = 100000;
    //     }
    //   }
    //   board_coppy->unApply(move);

    int new_board_sum = node->board_sum + price + mate;
    node->edges.emplace_back(std::make_shared<MovesTree::Node>(move,
                                                               node->height
                                                                   + 1,
                                                               new_board_sum));

  }
#endif

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

Move MovesTree::getBestMove() {
  makeTreeDeeper(main_node_, board_, max_height_, false, 0, 0);
  for (const auto& node: main_node_->edges) {
    if (node->best_price_ == main_node_->best_price_) {
      return node->move_to_get_here;
    }
  }

  if (main_node_->edges.size() != 0) {
    std::cerr << "DOUBLESUKA ";
  }
#if ARCH == 32
  auto empty_piece = std::make_shared<Piece>();
  return Move(empty_piece, empty_piece);
#else
  return Move();
#endif
}

Move MovesTree::apply(const Move& move) {
  std::shared_ptr<Node> node_by_this_move;
  generateMovesForNode(main_node_, board_);

#if ARCH == 32
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
#else
  for (auto& node: main_node_->edges) {
    if (node->move_to_get_here.getFrom() == move.getFrom() &&
        node->move_to_get_here.getTo() == move.getTo() &&
        ((!move.isPromotion()) ||
            node->move_to_get_here.getNewPieceType()
                == move.getNewPieceType())) {
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
#endif
  if (node_by_this_move == nullptr) {
    std::cout << "CANT APPLY MOVE, DOES NOT EXIST";
    throw 42;
  }
  return main_node_->move_to_get_here;
}

bool MovesTree::isMoveExists() {
  return !main_node_->edges.empty();
}
int shit = 0;
void MovesTree::makeTreeDeeper(const std::shared_ptr<MovesTree::Node>& current_node,
                               const std::shared_ptr<ObjBoard>& board_coppy,
                               short max_height, bool unaply,
                               int father_price, int grand_father_price,
                               int prev_node_price, bool capture_only) {
  generateMovesForNode(current_node, board_coppy);

  if (board_coppy->isWhiteTurn()) {
    current_node->best_price_ = -100000000;
  } else {
    current_node->best_price_ = 10000000;
  }

  if (prev_node_price == 10000001) { // if value unset(default) - set
    prev_node_price = -current_node->best_price_;
  }

  if (current_node->edges.empty()) {
#if ARCH == 32
    if (!MovesGenerator(board_coppy).isShah(board_coppy->isWhiteTurn())) {
      current_node->best_price_ *= -1;
    }
#else
    if (!board_coppy->isShah(board_coppy->whosTurn())) {
      current_node->best_price_ *= -1;
    }
#endif

    current_node->best_price_ /= (board_coppy->getMoveCount() + 1);
  } else {
    if (capture_only) {
      ProcessUntilAttacksAndShachsEnd(current_node,
                                      board_coppy,
                                      max_height,
                                      prev_node_price,
                                      father_price,
                                      grand_father_price);
    } else {
      ProcessUntilHightLimit(current_node,
                             board_coppy,
                             max_height,
                             prev_node_price,
                             father_price, grand_father_price);
    }
  }

  if (unaply) {
    board_coppy->unApply(current_node->move_to_get_here);
  }else{
    std::cout<<shit<<"\n";
    shit =0;
  }
}

void MovesTree::ProcessUntilAttacksAndShachsEnd(const std::shared_ptr<MovesTree::Node>& current_node,
                                                const std::shared_ptr<ObjBoard>& board_coppy,
                                                int max_height,
                                                int alpha,
                                                int father_price,
                                                int grand_father_price) {
  bool existed = false;

  for (const auto& child_node: current_node->edges) {
    existed = true;
    if (((child_node->height-main_node_->height ) >= 2)) {
      if (board_coppy->isWhiteTurn()) {
        if (grand_father_price - child_node->best_price_ > 95) {
          shit++;
          continue;
        }
      } else {
        if (child_node->best_price_ - grand_father_price > 95) {
          shit++;
          continue;
        }
      }
    }
    //TODO: GENERATE ONLY CAPTURES
#if ARCH == 32
    if (child_node->move_to_get_here.getAttackScore() == 0) {
      board_coppy->apply(child_node->move_to_get_here);
      bool is_shah =
          MovesGenerator(board_coppy).isShah(board_coppy->isWhiteTurn());
      board_coppy->unApply(child_node->move_to_get_here);
#else
      if (!child_node->move_to_get_here.isCapture()) {
        board_coppy->apply(child_node->move_to_get_here);
        bool is_shah = board_coppy->isShah(board_coppy->whosTurn());
        board_coppy->unApply(child_node->move_to_get_here);
#endif

      if (!is_shah) {
        updateBest(current_node, child_node->best_price_,
                   alpha, board_coppy->isWhiteTurn());
        continue;
      }
    }

    if (child_node->height <= max_height + 8) {
      board_coppy->apply(child_node->move_to_get_here);

      makeTreeDeeper(child_node, board_coppy, max_height, true,
                     current_node->best_price_,
                     father_price,
                     current_node->best_price_, true);
    }
    bool continue_search = updateBest(current_node, child_node->best_price_,
                                      alpha, board_coppy->isWhiteTurn());
    if (!continue_search) {
      break;
    }
  }

  if (!existed) {
    current_node->best_price_ = current_node->board_sum;
  }

}
void MovesTree::ProcessUntilHightLimit(const std::shared_ptr<MovesTree::Node>& current_node,
                                       const std::shared_ptr<ObjBoard>& board_coppy,
                                       short max_height, int alpha,
                                       int father_price,
                                       int grand_father_price) {
  for (const auto& child_node: current_node->edges) {
#if ARCH == 32
    bool capture_only = (child_node->move_to_get_here.getAttackScore() != 0);
#else
    bool capture_only = child_node->move_to_get_here.isCapture();
#endif
    if ((( child_node->height-main_node_->height) >= 2)) {
      if (board_coppy->isWhiteTurn()) {
      if (grand_father_price - child_node->best_price_ > 95) {
          shit++;
          continue;
        }
      } else {
        if (child_node->best_price_ - grand_father_price > 95) {
          shit++;
          continue;
        }
      }
    }
// 7667 23064 347684 ->  price - best_price_
// >  board_sum -- best_sum
    if (child_node->height <= max_height || capture_only) {
      board_coppy->apply(child_node->move_to_get_here);

      makeTreeDeeper(child_node,
                     board_coppy,
                     max_height,
                     true,
                     current_node->best_price_,
                     father_price,
                     current_node->best_price_,
                     capture_only);
    } else {
      child_node->best_price_ = child_node->board_sum;
    }

    bool continue_search = updateBest(current_node, child_node->best_price_,
                                      alpha, board_coppy->isWhiteTurn());
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
