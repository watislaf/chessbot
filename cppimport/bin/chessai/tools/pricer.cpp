#include "pricer.h"

int Pricer::GetPrice(const Piece& piece) {
  switch (piece.getType()) {
    case PieceType::tKING:return 999;
    case PieceType::tQUEEN:return 7;
    case PieceType::tRUCK:return 4;
    case PieceType::tPONE:return 1;
    case PieceType::tNONE:return 0;
    case PieceType::tBISHOP: return 3;
    case PieceType::tHORSE:return 3;
  }
}
