#include <string>
#include "objBoard.h"

std::shared_ptr<const Piece> ObjBoard::getPiece(const Position& position) const {
  return board_[position.getX()][position.getY()];
}

bool ObjBoard::LcIsPossible(bool is_white) const {
  if (is_white) {
    return whiteCastle.LC_is_possible_;
  }
  return blackCastle.LC_is_possible_;
}

void ObjBoard::setLCIsPossible(bool is_white, bool brake) {
  if (is_white) {
    whiteCastle.LC_is_possible_ = brake;
  } else {
    blackCastle.LC_is_possible_ = brake;
  }
}
void ObjBoard::setRCIsPossible(bool is_white, bool brake) {
  if (is_white) {
    whiteCastle.RC_is_possible_ = brake;
  } else {
    blackCastle.RC_is_possible_ = brake;
  }
}

bool ObjBoard::RcIsPossible(bool is_white) const {
  if (is_white) {
    return whiteCastle.RC_is_possible_;
  }
  return blackCastle.RC_is_possible_;
}

bool ObjBoard::isWhiteMove() const {
  return is_white_move_;
}
bool ObjBoard::isBlackMove() const {
  return !is_white_move_;
}

void ObjBoard::setPiece(const Piece& piece_template_object) {
  short x = piece_template_object.getPosition().getX();
  short y = piece_template_object.getPosition().getY();
  if (board_[x][y] != NULL && board_[x][y]->getType() != PieceType::tNONE) {
    if (board_[x][y]->getPieceColor() == ColorType::WHITE) {
      active_white_pieces_.remove(board_[x][y]);
    } else {
      active_black_pieces_.remove(board_[x][y]);
    }

  }
  board_[x][y] = (std::make_shared<Piece>(piece_template_object));
  if (piece_template_object.getType() == PieceType::tNONE) {
    return;
  }
  if (board_[x][y]->getPieceColor() == ColorType::WHITE) {
    active_white_pieces_.push_back(board_[x][y]);
  } else {
    active_black_pieces_.push_back(board_[x][y]);
  }
  if (board_[x][y]->getType() == PieceType::tKING) {
    if (board_[x][y]->getPieceColor() == ColorType::WHITE) {
      whiteCastle.king_position = {x, y};
    } else {
      blackCastle.king_position = {x, y};
    }
  }
}

ObjBoard::ObjBoard(FEN fen) {
  for (short j = 0; j < 8; j++) {
    for (short i = 0; i < 8; i++) {
      auto my_piece = fen.getPiece(i, j);
      setPiece(my_piece);
    }
  }
  is_white_move_ = fen.getIsWhiteMowe();
  whiteCastle.LC_is_possible_ = fen.getWLC();
  whiteCastle.RC_is_possible_ = fen.getWRC();
  blackCastle.LC_is_possible_ = fen.getBLC();
  blackCastle.RC_is_possible_ = fen.getBRC();
  last_passant_x_ = fen.getPassantX();
  move_count_ = fen.getMoveCount();
}

Position ObjBoard::getKingPosition(bool is_white) const {
  if (is_white) {
    return whiteCastle.king_position;
  }
  return blackCastle.king_position;
}

void ObjBoard::apply(const Move& move) {
  forceMove(getPiece(move.getStart()->getPosition()),
            getPiece(move.getEnd()->getPosition()));
  if (move.isBrakeLeftCastle()) {
    setLCIsPossible(isWhiteMove(), false);
  }
  if (move.isBrakeRightCastle()) {
    setRCIsPossible(isWhiteMove(), false);
  }
  if (move.isCastle()) {
    if (move.getEnd()->getPosition().getX() < 3) {
      // LEFT RUCK PLACE
      forceMove(getPiece(Position(0, move.getEnd()->getPosition().getY())),
                getPiece(Position(move.getEnd()->getPosition().getX() + 1,
                                  move.getEnd()->getPosition().getY())));
    } else {
      // Right ruck
      forceMove(getPiece(
                    Position(7, move.getEnd()->getPosition().getY())),
                getPiece(
                    Position(move.getEnd()->getPosition().getX() - 1,
                             move.getEnd()->getPosition().getY())));
    }
  }
  if (move.isDoubleDistancePone()) {
    last_passant_x_ = move.getStart()->getPosition().getX();
  } else {
    last_passant_x_ = -1;
  }

  if (move.isPassant()) {
    short back = -1;
    if (move.getStart()->getPieceColor() == ColorType::BLACK) {
      back = 1;
    }
    forceMove(getPiece(move.getStart()->getPosition()),
              getPiece(move.getEnd()->getPosition() + Position(0, back)));
  }
  if (move.getNewPieceType() != PieceType::tNONE) {
    setPiece(Piece(move.getEnd()->getPosition(),
                   move.getNewPieceType(),
                   move.getStart()->getPieceColor()));
  }
  is_white_move_ = !is_white_move_;
  move_count_++;

}

void ObjBoard::unApply(const Move& move) {
  is_white_move_ = !is_white_move_;
  move_count_--;
  // King get Pone
  forceMove(getPiece(move.getEnd()->getPosition()),
            getPiece(move.getStart()->getPosition()));
  setPiece(*move.getEnd());
  if (move.isBrakeLeftCastle()) {
    setLCIsPossible(isWhiteMove(), true);
  }
  if (move.isBrakeRightCastle()) {
    setRCIsPossible(isWhiteMove(), true);
  }
  if (move.isCastle()) {
    if (move.getEnd()->getPosition().getX() < 3) {
      // LEFT RUCK PLACE
      forceMove(getPiece(
                    Position(move.getEnd()->getPosition().getX() + 1,
                             move.getEnd()->getPosition().getY())),
                getPiece(
                    Position(0, move.getEnd()->getPosition().getY())));
    } else {
      // Right ruck
      forceMove(getPiece(
                    Position(move.getEnd()->getPosition().getX() - 1,
                             move.getEnd()->getPosition().getY())),
                getPiece(Position(7, move.getEnd()->getPosition().getY())));
    }
  }
  last_passant_x_ = move.getPrevPassant();

  if (move.isPassant()) {
    short back = -1;
    if (move.getStart()->getPieceColor() == ColorType::BLACK) {
      back = 1;
    }
    auto position_to_pone_be = move.getEnd()->getPosition() + Position(0, back);
    auto pone_color_reversed_color = move.getStart()->getPieceColor();
    if (pone_color_reversed_color == ColorType::BLACK) {
      pone_color_reversed_color = ColorType::WHITE;
    } else {
      pone_color_reversed_color = ColorType::BLACK;
    }
    setPiece(Piece(position_to_pone_be,
                   PieceType::tPAWN,
                   pone_color_reversed_color));
  }
  if (move.getNewPieceType() != PieceType::tNONE) {
    setPiece(Piece(move.getStart()->getPosition(),
                   PieceType::tPAWN,
                   move.getStart()->getPieceColor()));
  }
}

short ObjBoard::getLastPassantX() const {
  return last_passant_x_;
}
void ObjBoard::forceMove(const std::shared_ptr<const Piece>& piece_from,
                         const std::shared_ptr<const Piece>& piece_to) {
  setPiece(Piece(piece_to->getPosition(),
                 piece_from->getType(),
                 piece_from->getPieceColor()));
  setPiece(Piece(piece_from->getPosition(),
                 PieceType::tNONE,
                 ColorType::WHITE));
}

std::string ObjBoard::toStr() const {
  std::string answer;
  for (short y = 0; y <= 7; y++) {
    for (short x = 0; x <= 7; x++) {
      answer += board_[y][x]->toStr() + " ";
    }
    if (y != 7) {
      answer += "\n";
    }
  }
  return answer;
}

bool ObjBoard::operator==(const ObjBoard& other) const {
  return other.is_white_move_ == is_white_move_ &&
      other.last_passant_x_ == last_passant_x_ &&
      other.whiteCastle.RC_is_possible_ == whiteCastle.RC_is_possible_ &&
      other.whiteCastle.king_position == whiteCastle.king_position &&
      other.whiteCastle.LC_is_possible_ == whiteCastle.LC_is_possible_ &&
      other.blackCastle.RC_is_possible_ == blackCastle.RC_is_possible_ &&
      other.blackCastle.king_position == blackCastle.king_position &&
      other.blackCastle.LC_is_possible_ == blackCastle.LC_is_possible_ &&
      other.move_count_ == move_count_ && [this, &other]() {
    for (short i = 0; i <= 7; i++) {
      for (short j = 0; j <= 7; j++) {
        if (*(board_[i][j]) != *(other.getPiece(Position(i, j))))
          return false;
      }
    }
    return true;
  }();
}
bool ObjBoard::operator!=(const ObjBoard& other) const {
  return !(*this == other);
}

ObjBoard::ObjBoard(const ObjBoard& board) {
  *this = board;
  for (short j = 0; j < 8; j++) {
    for (short i = 0; i < 8; i++) {
      auto my_piece = *board.getPiece(Position(i, j));
      setPiece(my_piece);
    }
  }
}

short ObjBoard::getMoveCount() const {
  return move_count_;
}


const std::list<std::shared_ptr<const Piece>>& ObjBoard::getActivePieceList(bool is_white) const {
  if (is_white) {
    return active_white_pieces_;
  } else {
    return active_black_pieces_;
  }
}

