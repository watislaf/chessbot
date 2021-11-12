#ifndef ONLYCPP_MOVESTREE_H
#define ONLYCPP_MOVESTREE_H

#include <queue>
#include <utility>
#include "tools/movesGenerator.h"

class MovesTree {
 public:
  Move apply(const Move& move);
  explicit MovesTree(const Board& original_board, int tree_grow);
  struct Node {
    bool dead = false;
    explicit Node(Move move, int height, int board_sum) :
        move_to_get_here(std::move(move)),
        height(height),
        board_sum(board_sum) {
      if (height % 2) {
        best_price = -100000;
      } else {
        best_price = 100000;
      }
    }
    int board_sum = 0;
    int best_price;
    int height;
    std::vector<std::pair<int, std::shared_ptr<Node>>> edges;
    Move move_to_get_here;
  };
  Move getBestMove();
  void makeTreeDeeper(std::shared_ptr<Node> current_node);
 private:
  std::shared_ptr<Node> main_node_;
  std::shared_ptr<Board> board_;
  int max_height = 0;

  MovesGenerator moves_generator_;
  void killNodes(std::shared_ptr<MovesTree::Node>& shared_ptr);
  void generateMovesForNode(std::shared_ptr<Node> node);
};

#endif //ONLYCPP_MOVESTREE_H
