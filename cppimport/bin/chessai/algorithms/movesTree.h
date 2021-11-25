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
                     short tree_grow,
                     AiAdvanceLvl mode);
  ~MovesTree();

  struct Node {
    bool dead = false;
    explicit Node(Move move, short height, int board_sum) :
        move_to_get_here(std::move(move)),
        height(height),
        board_sum(board_sum) {
      if (height % 2) {
        best_price_tmp = -100000000;
      } else {
        best_price_tmp = 100000000;
      }
      best_price_shure = board_sum;
      best_price_tmp = board_sum;
    }
    bool is_cut = false;
    int board_sum = 0;
    int best_price_shure;
    int height;
    std::vector<std::pair<int, std::shared_ptr<Node>>> edges;
    Move move_to_get_here;
    int best_price_tmp;
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
  //A3
//  TranspositionController transpositionController;
  // A2
  void builLoop();
  std::atomic<bool> terminate_build_loop;
  std::mutex nodes_mtx;
  std::mutex price_mtx;
  std::mutex board_mainnode_mtx;
  short tree_grow_;
  std::chrono::milliseconds wait_to_tree_grow = std::chrono::milliseconds(300);
  std::shared_ptr<std::thread> additional_thread = nullptr;
  // A1
  std::shared_ptr<Node> main_node_;
  std::shared_ptr<Board> board_;
  std::atomic<short> max_height_;
  Pricer pricer;

  MovesGenerator moves_generator_;
  void killNodes(const std::shared_ptr<MovesTree::Node>& shared_ptr);
  void generateMovesForNode(const std::shared_ptr<Node>& node,
                            const std::shared_ptr<Board>& board_coppy);

  AiAdvanceLvl mode_;
  bool undead_node = false;
  void ApplyRezult(const std::shared_ptr<Node>& shared_ptr_1);
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
