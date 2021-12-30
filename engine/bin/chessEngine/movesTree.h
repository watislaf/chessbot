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

#if ARCH == 32
#include "objBoard/objBoard.h"
#include "objBoard/movesGenerator.h"
#else
#include "bitBoard/bBoard.h"
#include "bitBoard/bMove.h"
#include "bitBoard/bMovesGenerator.h"
#define Move BMove
#define ObjBoard BBoard
#define MovesGenerator BMovesGenerator
#endif

class MovesTree {
 public:
  Move apply(const Move& move);
  explicit MovesTree(const ObjBoard& original_board,
                     short tree_grow);

  struct Node {
    explicit Node(Move move, short height, const int& board_sum) :
        move_to_get_here(std::move(move)),
        height(height),
        board_sum(board_sum),
        best_price_(board_sum) {};
    int board_sum = 0;
    int height;
    std::vector<std::shared_ptr<Node>> edges;
    Move move_to_get_here;
    int best_price_;
  };
  Move getBestMove();
  void makeTreeDeeper(const std::shared_ptr<Node>& current_node,
                      const std::shared_ptr<ObjBoard>& board_coppy,
                      short max_height, const int& grand_father_price,
                      const int& prev_node_price,
                      const int& father_sum,
                      const int& grand_father_sum,
                      bool capture_only = false);

  bool isMoveExists();

 private:
  std::shared_ptr<Node> main_node_;
  std::shared_ptr<ObjBoard> board_;
  short max_height_;
  Pricer pricer;
  void generateMovesForNode(const std::shared_ptr<Node>& node,
                            const std::shared_ptr<ObjBoard>& board_coppy);

  void ProcessUntilAttacksAndShachsEnd(const std::shared_ptr<MovesTree::Node>& current_node,
                                       const std::shared_ptr<ObjBoard>& board_coppy,
                                       const int& max_height,
                                       const int& alpha,
                                       const int& grand_father_price,
                                       const int& father_sum,
                                       const int& grand_father_sum
  );

  void ProcessUntilHightLimit(const std::shared_ptr<MovesTree::Node>& current_node,
                              const std::shared_ptr<ObjBoard>& board_coppy,
                              short max_height,
                              const int& alpha,
                              const int& grand_father_price,
                              const int& father_sum,
                              const int& grand_father_sum
  );
  static bool updateBestResultAndReturnReasonToContinue(const std::shared_ptr<MovesTree::Node>& current_node,
                                                        const int& child_tmp, const int& alpha, bool is_white_move);

  static bool isNodeToWeak(const int& delta_moves,
                           bool is_white_turn,
                           const int& grand_father_price,
                           const int& current_price,
                           const int& grand_father_sum,
                           const int& current_sum);
  static int getMinusInf(bool turn);
};

#endif //ONLYCPP_MOVESTREE_H
