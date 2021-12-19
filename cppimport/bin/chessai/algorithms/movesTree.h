#ifndef ONLYCPP_MOVESTREE_H
#define ONLYCPP_MOVESTREE_H

#include <queue>
#include <utility>
#include <abstracts/aiAdvanceLvl.h>
#include <thread>
#include "tools/movesGenerator.h"
#include <board/board.h>
#include <algorithm>
#include <iostream>
#include <atomic>
#include <mutex>
#include "tools/pricer.h"
#include "transpositionController.h"

class MovesTree {
 public:
  Move apply(const Move& move);
  explicit MovesTree(const Board& original_board,
                     short tree_grow);

  struct Node {
    explicit Node(Move move, short height, int board_sum) :
        move_to_get_here(std::move(move)),
        height(height),
        board_sum(board_sum) {
      if (height % 2) {
        best_price_ = -100000000;
      } else {
        best_price_ = 100000000;
      }
      best_price_ = board_sum;
    }
    int board_sum = 0;
    int height;
    std::vector<std::shared_ptr<Node>> edges;
    Move move_to_get_here;
    int best_price_;
  };
  Move getBestMove();
  void makeTreeDeeper(const std::shared_ptr<Node>& current_node,
                      const std::shared_ptr<Board>& board_coppy,
                      short max_height,
                      bool unaply,
                      int prev_node_price = 10000001,
                      bool capture_only = false);

  bool isMoveExists();

 private:
  std::shared_ptr<Node> main_node_;
  std::shared_ptr<Board> board_;
  short max_height_;
  Pricer pricer;
  void generateMovesForNode(const std::shared_ptr<Node>& node,
                            const std::shared_ptr<Board>& board_coppy);

  void ProcessUntilAttacksAndShachsEnd(const std::shared_ptr<MovesTree::Node>& current_node,
                                       const std::shared_ptr<Board>& board_coppy,
                                       int max_height,
                                       int alpha);

  void ProcessUntilHightLimit(const std::shared_ptr<MovesTree::Node>& current_node,
                              const std::shared_ptr<Board>& board_coppy,
                              short max_height, int alpha);
  bool updateBest(const std::shared_ptr<MovesTree::Node>& current_node,
                  int child_tmp, int alpha, bool move);
};

#endif //ONLYCPP_MOVESTREE_H
