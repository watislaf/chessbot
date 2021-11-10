#ifndef ONLYCPP_MOVESTREE_H
#define ONLYCPP_MOVESTREE_H


#include <queue>
#include <utility>

struct MovesTree {

    explicit MovesTree(const Board &original_board) : board_(original_board) {
        auto zero_piece = original_board.getPiece(Position(0, 0));
        main_node = std::make_shared<Node>(Move(zero_piece, zero_piece),
                                           original_board.getMoveCount(),
                                           original_board.getMoveCount());
    }

    struct Node {
        explicit Node(Move move, int height, int max_height) :
                move_to_get_here(std::move(move)),
                height(height),
                max_height(max_height) {
            if (height % 2) {
                current_price = -100000;
            } else {
                current_price = 100000;
            }
        }

        int current_price;
        int height;
        int max_height;

        std::list<std::shared_ptr<Node>> edges;
        Move move_to_get_here;
    };

    std::shared_ptr<Node> main_node;
    Board board_;
};


#endif //ONLYCPP_MOVESTREE_H
