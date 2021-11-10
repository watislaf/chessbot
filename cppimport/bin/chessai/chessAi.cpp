#include <string>
#include <algorithm>
#include "chessAi.h"
#include "tools/pricer.h"

void ChessAi::startNewGame(const std::string &fen_str) {
    board_ = std::make_shared<Board>(FEN(fen_str));
}

std::string ChessAi::getPossibleMovesForPosition(int x, int y) {
    auto moves = moves_generator.generateMoves(
            board_, board_->getPiece(Position(x, y)));

    moves.erase(
            std::remove_if(
                    moves.begin(), moves.end(),
                    [](const Move &move) {
                        return move.getEnd()->getType() != PieceType::tNONE
                               && move.getStart()->getPieceColor()
                                  == move.getEnd()->getPieceColor();
                    }), moves.end()
    );

    std::string answer;
    for (const auto &move: moves) {
        answer += move.toStr() + " ";
    }

    return answer;
}

std::string ChessAi::getBoardStr() const {
    return board_->toStr();
}

std::string ChessAi::getFenStr() const {
    return board_->getFen();
}

void ChessAi::applyMove(int fx, int fy, int tx, int ty) {
    Position from = Position(fx, fy);
    Position to = Position(tx, ty);
    Move move(board_->getPiece(from), board_->getPiece(to));
    board_->apply(move);
//*move.position_from, *move.position_to
}

void ChessAi::startGameAnalize() {
    tree_moves_ = std::make_shared<MovesTree>(*board_);
    int tree_grow_rate = 2;
    loopStart(tree_grow_rate);
}

void ChessAi::loopStart(int tree_grow_rate) {
    while (true) {
        tree_moves_->main_node->max_height += tree_grow_rate;
        makeTreeDeeper(tree_moves_->main_node);
        if (tree_moves_->main_node->edges.empty()) {
            break;
        }
    }
}

void ChessAi::makeTreeDeeper(std::shared_ptr<MovesTree::Node> current_node) {
    board_->apply(current_node->move_to_get_here);
    if (current_node->edges.empty()) {
        generateMovesForNode(current_node);
    }
    if(current_node->max_height!=tree_moves_->main_node->max_height){
        if (board_->isWhiteMove()) {
            current_node->current_price = -10000;
        }else{
            current_node->current_price = 10000;
        }
    }
    for (auto &child_node: current_node->edges) {
        if (child_node->height == current_node->max_height) {
            child_node->current_price = Pricer::count(tree_moves_->board_, child_node->move_to_get_here);
        } else {
            makeTreeDeeper(child_node);
        }
        if (board_->isWhiteMove()) {
            current_node->current_price = std::max(child_node->current_price, current_node->current_price);
        } else {
            current_node->current_price = std::min(child_node->current_price, current_node->current_price);
        }
    }
    board_->unApply(current_node->move_to_get_here);
}

void ChessAi::generateMovesForNode(std::shared_ptr<MovesTree::Node> node) {
    for (const auto &active_piece: board_->getActivePieceList()) {
        if (active_piece->getPieceColor() == PieceColor::BLACK && board_->isWhiteMove() ||
            active_piece->getPieceColor() == PieceColor::WHITE && board_->isBlackMove()) {
            continue;
        }
        const auto moves = moves_generator.generateMoves(board_, active_piece));
        for (const auto &move: moves) {
            node->edges.push_back(std::make_shared<MovesTree::Node>(move, node->height + 1, node->max_height));
        }
    }
}

