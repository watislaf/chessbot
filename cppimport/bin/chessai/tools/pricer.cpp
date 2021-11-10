#include "pricer.h"

int Pricer::getPrice(const std::shared_ptr<const Piece>& piece) {
  switch (piece->getType()) {
    case PieceType::tKING:return 999;
    case PieceType::tQUEEN:return 7;
    case PieceType::tRUCK:return 4;
    case PieceType::tPONE:return 1;
    case PieceType::tNONE:return 0;
    case PieceType::tBISHOP: return 3;
    case PieceType::tHORSE:return 3;
  }

  return 0;
}

int Pricer::count(const Board &board, const Move &move) {
    int answer =  move.getAttackPrice();
    if(board.isBlackMove()){
        answer*=-1;
    }
    return answer;
}
