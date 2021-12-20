
#include "chessAi.h"

ChessAi::ChessAi(std::string advance) {
  if (advance == "random")
    mode_ = AiAdvanceLvl::RANDOM;
  if (advance == "A1")
    mode_ = AiAdvanceLvl::A1;
  if (advance == "A2")
    mode_ = AiAdvanceLvl::A2;
}

void ChessAi::startNewGame(const std::string& fen_str) {
  main_board_ = std::make_shared<ObjBoard>(FEN(fen_str));
  short tree_grow = 3;
  if (mode_ == AiAdvanceLvl::RANDOM) {
    tree_grow = 1;
  }
  if (mode_ == AiAdvanceLvl::A1) {
    tree_grow = 4;
  }

  tree_moves_ = std::make_shared<MovesTree>(*main_board_, tree_grow);
}

std::string ChessAi::getPossibleMovesForPosition(short x, short y) {
  auto moves = MovesGenerator(
      main_board_, main_board_->getPiece(
          Position(x, y))).generateMoves();
  std::string answer;
  for (const auto& move: *moves) {
    answer += move.toStr() + " ";
  }
  return answer;
}

std::string ChessAi::getBoardStr() const {
  return main_board_->toStr();
}

char ChessAi::whosMove() const {
  if (main_board_->isWhiteMove()) {
    return 'w';
  } else {
    return 'b';
  }
}

bool ChessAi::isMoveExists() {
  bool answ = tree_moves_->isMoveExists();
  if (!answ)
    std::cout << "NO MORE MOVES";
  return answ;
}

std::string ChessAi::getBestMoveStr() {
  return getBestMove().toStr();
}

Move ChessAi::getBestMove() {
  auto empty_piece = std::make_shared<Piece>();
  auto empty_move = Move(empty_piece, empty_piece);
  if (main_board_->getMoveCount() > 120) {
    return empty_move;
  }
  auto best_move = tree_moves_->getBestMove();

  if (best_move.getStart()->getPosition()
      == empty_move.getStart()->getPosition()
      && MovesGenerator(main_board_).isShah(main_board_->isWhiteMove())) {
    best_move.setNewPieceType(PieceType::tKING);
  }

  return best_move;
}

void ChessAi::applyMoveParams(short fx, short fy, short tx, short ty,
                              char niew_piece) {
  PieceType new_piece_type = PieceType::tPAWN;
  switch (niew_piece) {
    case 'r':new_piece_type = PieceType::tRUCK;
      break;
    case 'q':new_piece_type = PieceType::tQUEEN;
      break;
    case 'n': new_piece_type = PieceType::tNIGHT;
      break;
    case 'b': new_piece_type = PieceType::tBISHOP;
      break;
    case '_':new_piece_type = PieceType::tNONE;
      break;
  }

  auto naked_move = Move(main_board_->getPiece(Position(fx, fy)),
                         main_board_->getPiece(Position(tx, ty)));
  naked_move.setNewPieceType(new_piece_type);
  applyMove(naked_move);
}

void ChessAi::applyMove(const Move& naked_move) {
  if (naked_move.getStart()->getPosition() == Position(9, 9)) {
    std::cout << "CNAT APPLY EMPTY MOVE";
    return;
  }
  const auto& full_move = tree_moves_->apply(naked_move);
  main_board_->apply(full_move);
}

std::string ChessAi::getFenStr() const {
  std::string fen;
/*
  for (short y = 7; y >= 0; y--) {
    short k = 0;
    for (short x = 0; x <= 7; x++) {
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
  if (whiteCastle.RC_is_possible_)
    fen += "K";
  if (whiteCastle.LC_is_possible_)
    fen += "Q";
  if (blackCastle.RC_is_possible_)
    fen += "k";
  if (blackCastle.LC_is_possible_)
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
*/
  return "42";
}

