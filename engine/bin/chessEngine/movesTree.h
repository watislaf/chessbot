#ifndef ONLYCPP_MOVESTREE_H
#define ONLYCPP_MOVESTREE_H

#include <queue>
#include <utility>
#include "abstracts/aiAdvanceLvl.h"
#include <thread>

#include <algorithm>
#include <iostream>
#include <atomic>
#include <mutex>
#include "tools/pricer.h"
 
#include "bitBoard/bBoard.h"
#include "bitBoard/bMove.h"
#include "bitBoard/bMovesGenerator.h"  

class MovesTree {
 public:
  BMove apply(const BMove& BMove);
  explicit MovesTree(const BBoard& original_board,
                     short tree_grow);

  struct Node {
    explicit Node(BMove BMove, const int& board_sum) :
        move_to_get_here(BMove),
        board_sum(board_sum),
        best_price_(board_sum) {};
    int board_sum = 0;
    std::vector<std::shared_ptr<Node>> edges;
    BMove move_to_get_here;
    int best_price_;
  };

  BMove getBestMove();
  void makeTreeDeeper(const std::shared_ptr<Node>& current_node,
                      const short& current_height,
                      const int& grand_father_price,
                      const int& prev_node_price,
                      bool capture_only );

  bool isMoveExists();

 private:
  std::shared_ptr<Node> main_node_;
  std::shared_ptr<BBoard> board_;
  short max_height_;
  short current_tree_height_;
  Pricer pricer;
  void generateMovesForNode(const std::shared_ptr<Node>& node);

  void ProcessUntilAttacksAndShachsEnd(const std::shared_ptr<MovesTree::Node>& current_node,
                                       const short& current_height,
                                       const int& alpha,
                                       const int& grand_father_price
  );

  void ProcessUntilHightLimit(const std::shared_ptr<MovesTree::Node>& current_node,
                              const short& current_height,
                              const int& alpha,
                              const int& grand_father_price
  );

  static bool updateBestResultAndReturnReasonToContinue(
      const std::shared_ptr<MovesTree::Node>& current_node,
      const int& child_tmp, const int& alpha, bool is_white_move);

  static bool isNodeToWeak(const int& delta_moves,
                           bool is_white_turn,
                           const int& grand_father_price,
                           const int& current_price);
  static int getMinusInf(bool turn);
};

#endif //ONLYCPP_MOVESTREE_H
