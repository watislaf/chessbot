#include <string>
#include "board.h"

std::shared_ptr<Piece> Board::getPiece(const Position& position) {
  return board_[position.getX()][position.getY()];
}

bool Board::isLcAvailable(const Piece& piece) const {
  if (piece.getPieceColor() == PieceColor::WHITE) {
    return whiteCastle.LC_;
  }
  return blackCastle.LC_;
}

void Board::setBrakeLc(const Piece& piece, bool brake) {
  if (piece.getPieceColor() == PieceColor::WHITE) {
    whiteCastle.LC_ = brake;
  }
  blackCastle.LC_ = brake;

}
void Board::setBrakeRc(const Piece& piece, bool brake) {
  if (piece.getPieceColor() == PieceColor::WHITE) {
    whiteCastle.RC_ = brake;
  }
  blackCastle.RC_ = brake;

}

bool Board::isRcAvailable(const Piece& piece) const {
  if (piece.getPieceColor() == PieceColor::WHITE) {
    return whiteCastle.RC_;
  }
  return blackCastle.RC_;
}

bool Board::isWhiteMove() const {
  return is_white_move_;
}
void Board::nextMove() {
  is_white_move_ = !is_white_move_;
  move_count_++;
}
bool Board::isBlackMove() const {
  return !is_white_move_;
}
int Board::getPrevLongPonMove() const {
  return last_passant_x_;
}
void Board::setPrevLongPonMove(int prev_long_pon_move) {
  last_passant_x_ = prev_long_pon_move;
}

Position Board::getKingPosition(const Piece& piece) const {
  if (piece.getPieceColor() == PieceColor::WHITE) {
    return whiteCastle.king_position;
  }
  return blackCastle.king_position;
}

void Board::apply(const Move& move) {
  hardMove(getPiece(move.getStart().getPosition()),
           getPiece(move.getEnd().getPosition()));
  if (move.isBrakeLeftCastle()) {
    setBrakeLc(move.getStart(), true);
  }
  if (move.isBrakeRightCastle()) {
    setBrakeRc(move.getStart(), true);
  }
  if (move.isCastle()) {
    if (move.getEnd().getPosition().getX() < 3) {
      // LEFT RUCK PLACE
      hardMove(getPiece(
                   Position(0, move.getEnd().getPosition().getY())),
               getPiece(
                   Position(move.getEnd().getPosition().getX() + 1,
                            move.getEnd().getPosition().getY())));
    } else {
      // Right ruck
      hardMove(getPiece(
                   Position(7, move.getEnd().getPosition().getY())),
               getPiece(
                   Position(move.getEnd().getPosition().getX() - 1,
                            move.getEnd().getPosition().getY())));
    }
  }
  if (move.IsDoubleDistancePone()) {
    last_passant_x_ = move.getStart().getPosition().getX();
  } else {
    last_passant_x_ = -1;
  }
  if (move.isPassant()) {
    int back = -1;
    if (move.getStart().getPieceColor() == PieceColor::BLACK) {
      back = 1;
    }
    hardMove(getPiece(move.getStart().getPosition()),
             getPiece(move.getEnd().getPosition() + Position(0, back)));
  }
  if (move.isCanMakeNewFigure()) {
    getPiece(move.getEnd().getPosition())->setType(
        move.getNewPieceType());
  }
}

void Board::unApply(const Move& move) {
  // King get Pone
  hardMove(getPiece(move.getEnd().getPosition()),
           getPiece(move.getStart().getPosition()));

  if (move.isBrakeLeftCastle()) {
    setBrakeLc(move.getStart(), false);
  }
  if (move.isBrakeRightCastle()) {
    setBrakeRc(move.getStart(), false);
  }
  if (move.isCastle()) {
    if (move.getEnd().getPosition().getX() < 3) {
      // LEFT RUCK PLACE
      hardMove(getPiece(
                   Position(move.getEnd().getPosition().getX() + 1,
                            move.getEnd().getPosition().getY())),
               getPiece(
                   Position(0, move.getEnd().getPosition().getY())));
    } else {
      // Right ruck
      hardMove(getPiece(
                   Position(move.getEnd().getPosition().getX() - 1,
                            move.getEnd().getPosition().getY())),
               getPiece(Position(7, move.getEnd().getPosition().getY())));
    }
  }
  last_passant_x_ = move.PrevPassant();

  if (move.isPassant()) {
    int back = -1;
    if (move.getStart().getPieceColor() == PieceColor::BLACK) {
      back = 1;
    }
    hardMove(getPiece(move.getEnd().getPosition() + Position(0, back)),
             getPiece(move.getStart().getPosition()));
    auto position_to_pone_be = move.getEnd().getPosition() + Position(0, back);
    board_[position_to_pone_be.getX()][position_to_pone_be.getY()]->setPieceColor(
        move.getStart().getPieceColor());
  }
  if (move.isCanMakeNewFigure()) {
    getPiece(move.getStart().getPosition())->setType(
        PieceType::tPONE);
  }
}

int Board::getLastPassantX() const {
  return last_passant_x_;
}

void Board::hardMove(std::shared_ptr<Piece> piece,
                     std::shared_ptr<Piece> piece_1) {
  std::swap(board_[piece_1->getPosition().getX()][piece_1->getPosition().getY()],
            board_[piece->getPosition().getX()][piece->getPosition().getY()]);
  auto tmp = piece->getPosition();
  piece->setPosition(piece_1->getPosition());
  piece_1->setPosition(tmp);
}

Board::Board(FEN fen) {
  for (int j = 0; j < 8; j++) {
    for (int i = 0; i < 8; i++) {
      auto my_piece = fen.getPiece(7 - j, i);
      board_[i][j] = (std::make_shared<Piece>(Position(i, j),
                                              my_piece.getType(),
                                              my_piece.getPieceColor()));
      if (board_[i][j]->getType() == PieceType::tKING) {
        if (board_[i][j]->getPieceColor() == PieceColor::WHITE) {
          whiteCastle.king_position = {i, j};
        } else {
          blackCastle.king_position = {i, j};
        }
      }
      if (board_[i][j]->getType() != PieceType::tNONE) {
        active_pieces.emplace_back(board_[i][j]);
      }
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
std::string Board::toStr() const {
  std::string answer;
  // y
  // 4
  // 3
  // 2
  // 1
  // 0 1 2 3 4 : x
  for (int y = 7; y >= 0; y--) {
    for (int x = 0; x <= 7; x++) {
      answer += board_[x][y]->toStr() + " ";
    }
    answer += "\n";
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

