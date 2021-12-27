
#include "chessEngine.h"

ChessEngine::ChessEngine(std::string advance) {
  mode_ = AiAdvanceLvl::A1;

  if (advance == "random")
    mode_ = AiAdvanceLvl::RANDOM;

  if (advance == "bullet")
    mode_ = AiAdvanceLvl::A1;
  if (advance == "blitz")
    mode_ = AiAdvanceLvl::A2;
  if (advance == "rapid")
    mode_ = AiAdvanceLvl::A3;
}

void ChessEngine::startNewGame(const std::string& fen_str) {
  main_board_ = std::make_shared<ObjBoard>(FEN(fen_str));
  short tree_grow = 3;
  if (mode_ == AiAdvanceLvl::RANDOM) {
    tree_grow = 1;
  }
  if (mode_ == AiAdvanceLvl::A1) {
    tree_grow = 4;
  }
  if (mode_ == AiAdvanceLvl::A2) {
    tree_grow = 4;
  }
  if (mode_ == AiAdvanceLvl::A3) {
    tree_grow = 6;
  }
  tree_moves_ = std::make_shared<MovesTree>(*main_board_, tree_grow);
}

std::string ChessEngine::getPossibleMovesForPosition(short x, short y) {
  auto moves = MovesGenerator(
      main_board_, main_board_->getPiece(
          Position(x, y))).generateMoves();
  std::string answer;
  for (const auto& move: *moves) {
    answer += move.toStr() + " ";
  }
  return answer;
}

std::string ChessEngine::getBoardStr() const {
  return main_board_->toStr();
}

char ChessEngine::whosTurn() const {
  if (main_board_->isWhiteTurn()) {
    return 'w';
  } else {
    return 'b';
  }
}

bool ChessEngine::isMoveExists() {
  bool answ = tree_moves_->isMoveExists();
  if (!answ)
    std::cout << "NO MORE MOVES";
  return answ;
}

std::string ChessEngine::getBestMoveStr() {
  return getBestMove().toStr();
}

Move ChessEngine::getBestMove() {
  auto empty_piece = std::make_shared<Piece>();
  auto empty_move = Move(empty_piece, empty_piece);
  if (main_board_->getMoveCount() > 120) {
    return empty_move;
  }
  auto best_move = tree_moves_->getBestMove();

  if (best_move.getStart()->getPosition()
      == empty_move.getStart()->getPosition()
      && MovesGenerator(main_board_).isShah(main_board_->isWhiteTurn())) {
    best_move.setNewPieceType(PieceType::KING);
  }

  return best_move;
}

void ChessEngine::applyMoveParams(short fx, short fy, short tx, short ty,
                                  char niew_piece) {
  PieceType new_piece_type = PieceType::PAWN;
  switch (niew_piece) {
    case 'r':new_piece_type = PieceType::RUCK;
      break;
    case 'q':new_piece_type = PieceType::QUEEN;
      break;
    case 'n': new_piece_type = PieceType::NIGHT;
      break;
    case 'b': new_piece_type = PieceType::BISHOP;
      break;
    case '_':new_piece_type = PieceType::NONE;
      break;
  }

  auto naked_move = Move(main_board_->getPiece(Position(fx, fy)),
                         main_board_->getPiece(Position(tx, ty)));
  naked_move.setNewPieceType(new_piece_type);
  applyMove(naked_move);
}

void ChessEngine::applyMove(const Move& naked_move) {
  if (naked_move.getStart()->getPosition() == Position(9, 9)) {
    std::cout << "CNAT APPLY EMPTY MOVE";
    return;
  }
  const auto& full_move = tree_moves_->apply(naked_move);
  main_board_->apply(full_move);
}

