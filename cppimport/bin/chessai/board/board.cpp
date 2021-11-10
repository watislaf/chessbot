#include <string>
#include "board.h"

std::shared_ptr<const Piece> Board::getPiece(const Position& position) const {
  return board_[position.getX()][position.getY()];
}

bool Board::isLcAvailable(const std::shared_ptr<const Piece>& piece) const {
  if (piece->getPieceColor() == PieceColor::WHITE) {
    return whiteCastle.LC_;
  }
  return blackCastle.LC_;
}

void Board::setLC(const std::shared_ptr<const Piece>& piece, bool brake) {
  if (piece->getPieceColor() == PieceColor::WHITE) {
    whiteCastle.LC_ = brake;
  }
  blackCastle.LC_ = brake;

}
void Board::setRc(const std::shared_ptr<const Piece>& piece, bool brake) {
  if (piece->getPieceColor() == PieceColor::WHITE) {
    whiteCastle.RC_ = brake;
  }
  blackCastle.RC_ = brake;

}

bool Board::isRcAvailable(const std::shared_ptr<const Piece>& piece) const {
  if (piece->getPieceColor() == PieceColor::WHITE) {
    return whiteCastle.RC_;
  }
  return blackCastle.RC_;
}

bool Board::isWhiteMove() const {
  return is_white_move_;
}
bool Board::isBlackMove() const {
  return !is_white_move_;
}

void Board::setPiece(const Piece& piece_template_object) {
  int x = piece_template_object.getPosition().getX();
  int y = piece_template_object.getPosition().getY();

  board_[x][y] = (std::make_shared<Piece>(piece_template_object));
  if (board_[x][y]->getType() == PieceType::tKING) {
    if (board_[x][y]->getPieceColor() == PieceColor::WHITE) {
      whiteCastle.king_position = {x, y};
    } else {
      blackCastle.king_position = {x, y};
    }
  }
}

Board::Board(FEN fen) {
  for (int j = 0; j < 8; j++) {
    for (int i = 0; i < 8; i++) {
      auto my_piece = fen.getPiece(i, j);
      setPiece(my_piece);
    }
  }
  is_white_move_ = fen.getIsWhiteMowe();
  whiteCastle.LC_ = fen.getWLC();
  whiteCastle.RC_ = fen.getWRC();
  blackCastle.LC_ = fen.getBLC();
  blackCastle.RC_ = fen.getBRC();
  last_passant_x_ = fen.getPassantX();
  move_count_ = fen.getMoveCount();
}

Position Board::getKingPosition(const std::shared_ptr<const Piece>& piece) const {
  if (piece->getPieceColor() == PieceColor::WHITE) {
    return whiteCastle.king_position;
  }
  return blackCastle.king_position;
}

void Board::apply(const Move& move) {
  is_white_move_ = !is_white_move_;
  move_count_++;
  forceMove(getPiece(move.getStart()->getPosition()),
            getPiece(move.getEnd()->getPosition()));
  if (move.isBrakeLeftCastle()) {
    setLC(move.getStart(), false);
  }
  if (move.isBrakeRightCastle()) {
    setRc(move.getStart(), false);
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
    int back = -1;
    if (move.getStart()->getPieceColor() == PieceColor::BLACK) {
      back = 1;
    }
    forceMove(getPiece(move.getStart()->getPosition()),
              getPiece(move.getEnd()->getPosition() + Position(0, back)));
  }
  if (move.isCanMakeNewFigure()) {
    setPiece(Piece(move.getEnd()->getPosition(),
                   move.getNewPieceType(),
                   move.getStart()->getPieceColor()));
  }
}

void Board::unApply(const Move& move) {
  is_white_move_ = !is_white_move_;
  move_count_--;
  // King get Pone
  forceMove(getPiece(move.getEnd()->getPosition()),
            getPiece(move.getStart()->getPosition()));
  setPiece(*move.getEnd());
  if (move.isBrakeLeftCastle()) {
    setLC(move.getStart(), true);
  }
  if (move.isBrakeRightCastle()) {
    setRc(move.getStart(), true);
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
    int back = -1;
    if (move.getStart()->getPieceColor() == PieceColor::BLACK) {
      back = 1;
    }
    auto position_to_pone_be = move.getEnd()->getPosition() + Position(0, back);
    auto pone_color_reversed_color =
        board_[position_to_pone_be.getX()][position_to_pone_be.getY()]->getPieceColor();
    if (pone_color_reversed_color == PieceColor::BLACK) {
      pone_color_reversed_color = PieceColor::WHITE;
    } else {
      pone_color_reversed_color = PieceColor::BLACK;
    }
    setPiece(Piece(position_to_pone_be,
                   PieceType::tPONE,
                   pone_color_reversed_color));
  }
  if (move.isCanMakeNewFigure()) {
    setPiece(Piece(move.getStart()->getPosition(),
                   PieceType::tPONE,
                   move.getStart()->getPieceColor()));
  }
}

int Board::getLastPassantX() const {
  return last_passant_x_;
}
void Board::forceMove(const std::shared_ptr<const Piece>& piece_from,
                      const std::shared_ptr<const Piece>& piece_to) {
  setPiece(Piece(piece_to->getPosition(),
                 piece_from->getType(),
                 piece_from->getPieceColor()));
  setPiece(Piece(piece_from->getPosition(),
                 PieceType::tNONE,
                 PieceColor::WHITE));
}
std::string Board::toStr() const {
  std::string answer;
  for (int y = 0; y <= 7; y++) {
    for (int x = 0; x <= 7; x++) {
      answer += board_[y][x]->toStr() + " ";
    }
    if (y != 7) {
      answer += "\n";
    }
  }
  return answer;
}
std::string Board::getFen() {
  std::string fen;

  for (int y = 7; y >= 0; y--) {
    int k = 0;
    for (int x = 0; x <= 7; x++) {
      std::string piece_str = board_[x][y]->toStr();
      if (piece_str == "_") {
        k++;
      } else {
        if (k != 0) {
          fen += std::to_string(k);
        }
        fen += board_[x][y]->toStr();
      }
    }

    if (k != 0) {
      fen += std::to_string(k);
    }
    if (y > 0)
      fen += '/';
  }
  // w KQkq - 0 1
  if (is_white_move_) {
    fen += " w";
  } else {
    fen += " b";
  }
  fen += " ";
  if (whiteCastle.RC_)
    fen += "K";
  if (whiteCastle.LC_)
    fen += "Q";
  if (blackCastle.RC_)
    fen += "k";
  if (blackCastle.LC_)
    fen += "q";
  fen += " ";
  if (last_passant_x_ != -1) {
    fen += char('a' + last_passant_x_);
    if (is_white_move_) {
      fen += "6";
    } else {
      fen += "3";
    }
  } else {
    fen += "-";
  }
  fen += " ";
  fen += std::to_string(move_count_);

  fen += " ";
  fen += std::to_string(move_count_ * 2 + is_white_move_);
  return fen;

}

bool Board::operator==(const Board& other) const {
  return other.is_white_move_ == is_white_move_ &&
      other.last_passant_x_ == last_passant_x_ &&
      other.whiteCastle.RC_ == whiteCastle.RC_ &&
      other.whiteCastle.king_position == whiteCastle.king_position &&
      other.whiteCastle.LC_ == whiteCastle.LC_ &&
      other.blackCastle.RC_ == blackCastle.RC_ &&
      other.blackCastle.king_position == blackCastle.king_position &&
      other.blackCastle.LC_ == blackCastle.LC_ &&
      other.move_count_ == move_count_ && [this, &other]() {
    for (int i = 0; i <= 7; i++) {
      for (int j = 0; j <= 7; j++) {
        if (*(board_[i][j]) != *(other.getPiece(Position(i, j))))
          return false;
      }
    }
    return true;
  }();
}
bool Board::operator!=(const Board& other) const {
  return !(*this == other);
}

Board::Board(const Board& board) {
  *this = board;
  for (int j = 0; j < 8; j++) {
    for (int i = 0; i < 8; i++) {
      auto my_piece = *board.getPiece(Position(i, j));
      setPiece(my_piece);
    }
  }
}

int Board::getMoveCount() const {
    return move_count_;
}

void Board::setMoveCount(int moveCount) {
    move_count_ = moveCount;
}

