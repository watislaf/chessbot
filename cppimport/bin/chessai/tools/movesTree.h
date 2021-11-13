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
#include "pricer.h"

class MovesTree {
 public:
  Move apply(const Move& move);
  explicit MovesTree(const Board& original_board,
                     int tree_grow,
                     AiAdvanceLvl mode);
  ~MovesTree();

  struct Node {
    bool dead=false;
    explicit Node(Move move, int height, int board_sum) :
        move_to_get_here(std::move(move)),
        height(height),
        board_sum(board_sum) {
      if (height % 2) {
        best_price_tmp = -1000000000;
      } else {
        best_price_tmp = 1000000000;
      }
      best_price_shure = best_price_tmp;
    }
    int board_sum = 0;
    int best_price_shure;
    int height;
    std::vector<std::pair<int, std::shared_ptr<Node>>> edges;
    Move move_to_get_here;

    int best_price_tmp;
  };
  Move getBestMove();
  void makeTreeDeeper(std::shared_ptr<Node> current_node,
                      std::shared_ptr<Board> board_coppy,
                      int max_height, bool unaply);

  bool isMoveExists();

 private:
  // A2
  void builLoop();
  std::atomic<bool> terminate_build_loop;
  std::mutex nodes_mtx;
  std::mutex price_mtx;
  std::mutex board_mainnode_mtx;
  int tree_grow_;
  std::chrono::milliseconds wait_to_tree_grow = std::chrono::milliseconds(300);
  std::shared_ptr<std::thread> additional_thread = nullptr;
  // A1
  std::shared_ptr<Node> main_node_;
  std::shared_ptr<Board> board_;
  int max_height_ = 0;

  MovesGenerator moves_generator_;
  void killNodes(std::shared_ptr<MovesTree::Node>& shared_ptr);
  void generateMovesForNode(std::shared_ptr<Node> node,
                            std::shared_ptr<Board> board_coppy);

  AiAdvanceLvl mode_;
  bool undead_node = false;
  void ApplyRezult(const std::shared_ptr<Node>& shared_ptr_1);
};

#endif //ONLYCPP_MOVESTREE_H
