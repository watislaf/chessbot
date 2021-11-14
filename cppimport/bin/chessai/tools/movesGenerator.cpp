#include <algorithm>
#include <iostream>
#include "abstracts/move.h"
#include "movesGenerator.h"
#include "pricer.h"

std::list<Move> MovesGenerator::generateMoves(
    std::shared_ptr<Board> board, const std::shared_ptr<const Piece>& piece,
    bool capture_only, bool non_capture_only) {
  capture_only_ = capture_only;
  non_capture_only_ = non_capture_only;

  moves_.clear();
  defende_score = 0;
  board_ = board;
  current_piece_ = piece;
  // Create simple moves
  switch (piece->getType()) {
    case PieceType::tHORSE:horseMove();
      break;
    case PieceType::tKING:kingMove();
      castleMove();
      break;
    case PieceType::tRUCK:ruckMove();
      break;
    case PieceType::tQUEEN:queenMove();
      break;
    case PieceType::tBISHOP:bishopMove();
      break;
    case PieceType::tPONE:poneAttackMove();
      ponePacificMove();

      break;
    case PieceType::tNONE:return {};
  }
  for (auto& move: moves_) {
    move.setPrevPassant(board_->getLastPassantX());
  }
  auto clear_moves = moves_;
  auto begin_to_erase_ponter = std::remove_if(clear_moves.begin(),
                                              clear_moves.end(),
                                              [this](const Move& move) {
                                                return isShahDanger(move);
                                              });
  for (auto& move: clear_moves) {
    move.setDefendScore(defende_score);
  }
  if (begin_to_erase_ponter != clear_moves.end())
    clear_moves.erase(begin_to_erase_ponter, clear_moves.end());

  current_piece_ = nullptr;
  board = nullptr;
  moves_.clear();

  return clear_moves;
}

std::list<Position> MovesGenerator::goByVector(Position move_vector,
                                               int max_length, bool only_tNone,
                                               bool reduce_tNone) {
  if (move_vector.getX() == 0 && move_vector.getY() == 0) {
    return {};
  }

  std::list<Position> position_to_move;
  for (int length = 1; length <= max_length; length++) {
    auto new_position = Position(
        length * move_vector.getX(), length * move_vector.getY()
    ) + current_piece_->getPosition();
    auto posY = new_position.getY();
    auto posX = new_position.getX();
    if (posY < 0 || posY > 7 || posX < 0 || posX > 7) {
      break;
    }
    if (board_->getPiece(new_position)->getType() == PieceType::tNONE) {
      if (!reduce_tNone) {
        position_to_move.emplace_back(new_position);
      }
      continue;
    }
    if (!only_tNone) {
      position_to_move.emplace_back(new_position);
    }
    break;
  }
  return position_to_move;
}

void MovesGenerator::bishopMove(bool reduce_tNone) {
  for (int left = -1; left <= 1; left += 2) {
    for (int top = -1; top <= 1; top += 2) {
      insertPositionsToMoves(goByVector(Position(left, top),
                                        8,
                                        false,
                                        reduce_tNone));
    }
  }
}

void MovesGenerator::horseMove(bool reduce_tNone) {
  for (int left = -1; left <= 1; left += 2) {
    for (int top = -1; top <= 2; top += 2) {
      insertPositionsToMoves(goByVector(Position(1 * left, 2 * top),
                                        1));
      insertPositionsToMoves(goByVector(Position(2 * left, 1 * top), 1));
    }
  }
}

void MovesGenerator::queenMove(bool reduce_tNone) {
  for (int left = -1; left <= 1; left += 1) {
    for (int top = -1; top <= 1; top += 1) {
      insertPositionsToMoves(goByVector(Position(left, top),
                                        8,
                                        false,
                                        reduce_tNone));
    }
  }
}

void MovesGenerator::kingMove(bool reduce_tNone) {
  for (int left = -1; left <= 1; left += 1) {
    for (int top = -1; top <= 1; top += 1) {
      insertPositionsToMoves(goByVector(Position(left, top),
                                        1,
                                        false,
                                        reduce_tNone));
    }
  }
}

void MovesGenerator::ponePacificMove(bool reduce_tNone) {
  int length = current_piece_->getPieceColor() == PieceColor::WHITE ? 1 : -1;
  insertPositionsToMoves(goByVector(Position(0, length),
                                    1, true, reduce_tNone));
  auto piece_color = current_piece_->getPieceColor();
  for (auto iter = moves_.begin(); iter != moves_.end(); ++iter) {
    if (iter->getEnd()->getPosition().getY() % 7 == 0) {
      iter->setNewPieceType(PieceType::tQUEEN);
      moves_.emplace_front(iter->getStart(), iter->getEnd());
      moves_.front().setNewPieceType(PieceType::tBISHOP);
      moves_.emplace_front(iter->getStart(), iter->getEnd());
      moves_.front().setNewPieceType(PieceType::tRUCK);
      moves_.emplace_front(iter->getStart(), iter->getEnd());
      moves_.front().setNewPieceType(PieceType::tHORSE);
    }
  }
  // DOUBLE MOVE
  if (piece_color == PieceColor::WHITE
      && current_piece_->getPosition().getY() == 1 ||
      piece_color == PieceColor::BLACK
          && current_piece_->getPosition().getY() == 6) {
    if (goByVector(Position(0, piece_color == PieceColor::WHITE ? 1 : -1),
                   2,
                   true).
        size() == 2) {
      moves_.emplace_back(
          current_piece_,
          board_->getPiece(current_piece_->getPosition()
                               + Position(0, piece_color == PieceColor::WHITE
                                             ? 2 : -2)));
      moves_.back().setIsDoubleDistancePone(true, board_->getLastPassantX());
    }
  }

}

void MovesGenerator::ruckMove(bool reduce_tNone) {
  for (int top = -1; top <= 1; top += 2) {
    insertPositionsToMoves(goByVector(Position(0, top),
                                      8,
                                      false,
                                      reduce_tNone));
    insertPositionsToMoves(goByVector(Position(top, 0),
                                      8,
                                      false,
                                      reduce_tNone));
  }
  if (board_->LcIsPossible(board_->isWhiteMove())) {
    if (current_piece_->getPosition().getX() == 0) {
      for (auto& move: moves_) {
        move.setBrakeLeftCastle(true);
      }
    }
  }
  if (board_->RcIsPossible(board_->isWhiteMove())) {
    if (current_piece_->getPosition().getX() == 7) {
      for (auto& move: moves_) {
        move.setBrakeRightCastle(true);
      }
    }
  }
}

void MovesGenerator::poneAttackMove() {
  // Atack by diagonal
  int direction = current_piece_->getPieceColor() == PieceColor::WHITE ? 1 : -1;
  for (int i = -1; i <= 1; i += 2) {
    auto go_diagonal = goByVector(Position(i, direction), 1, false, true);
    if (go_diagonal.size() == 1
        && board_->getPiece(go_diagonal.back())->isEnemyTo(*current_piece_)) {
      moves_.emplace_back(current_piece_, board_->getPiece(go_diagonal.back()));
    }

    // Atack passant
    int good_row = current_piece_->getPieceColor() == PieceColor::WHITE ? 4 : 3;
    go_diagonal = goByVector(Position(i, direction), 1, true, false);
    if (go_diagonal.size() == 1
        && board_->getLastPassantX() == go_diagonal.back().getX()
        && current_piece_->getPosition().getY() == good_row) {
      moves_.emplace_back(current_piece_, board_->getPiece(go_diagonal.back()));
      moves_.back().setIsPassant(true);
      moves_.back().setAttackPrice(Pricer::getPrice(moves_.back().getStart()));
    }
  }
}

void MovesGenerator::castleMove() {
  // If king move he lost castle
  if (board_->LcIsPossible(board_->isWhiteMove())) {
    auto positions_left = goByVector(Position(-1, 0));
    if (positions_left.size() == 4) {
      moves_.emplace_back(
          current_piece_,
          board_->getPiece(current_piece_->getPosition() + Position(-2, 0)));
      moves_.back().setIsCastle(true);
    }
  }
  if (board_->RcIsPossible(board_->isWhiteMove())) {
    auto positions_right = goByVector(Position(1, 0));
    if (positions_right.size() == 3) {
      moves_.emplace_back(current_piece_, board_->getPiece(
          current_piece_->getPosition() + Position(2, 0)));
      moves_.back().setIsCastle(true);
    }

  }
  if (board_->RcIsPossible(board_->isWhiteMove())) {
    for (auto& move: moves_) {
      move.setBrakeRightCastle(true);
    }
  }
  if (board_->LcIsPossible(board_->isWhiteMove())) {
    for (auto& move: moves_) {
      move.setBrakeLeftCastle(true);
    }
  }
}

bool MovesGenerator::isShahDanger(const Move& move) {
  if (move.getDefendScore() != 0) {
    defende_score += Pricer::defendScore(move);
    return true;
  }
//  Board board = *board_;

  board_->apply(move);
  bool is_under_shach = isShah(board_, !board_->isWhiteMove());
  board_->unApply(move);

//  if (board != *board_) {
//    std::cout << move.toStr() << "\n";
//    std::cout << move.getStart()->toStr() << "\n";
//    std::cout << move.getEnd()->toStr() << "\n";
//
//    std::cout << "!!!!!!!!!!!!";
//  }
  if (is_under_shach) {
    return true;
  }
  if (move.isCastle()) {
    Move non_castle_move(move.getStart(),
                         board_->getPiece(Position(
                             (move.getStart()->getPosition().getX()
                                 + move.getEnd()->getPosition().getX()) / 2,
                             move.getStart()->getPosition().getY())));
    non_castle_move.setIsCastle(false);
    board_->apply(non_castle_move);
    is_under_shach =
        isShah(board_, !board_->isWhiteMove()); // check castle somehow )))))
    board_->unApply(non_castle_move);
  }

  if (is_under_shach) {
    return true;
  }
  return false;

}

bool MovesGenerator::isShah(const std::shared_ptr<Board>& board,
                            bool is_white) {
  auto movesTMP = moves_;
  auto pieceTMP = current_piece_;
  auto boardTMP = board_;

  moves_.clear();
  board_ = board;
  current_piece_ = board_->getPiece(board_->getKingPosition(is_white));

  horseMove(true);
  if (isPieceOnMoves(PieceType::tHORSE)) {
    return true;
  }
  moves_.clear();
  kingMove(true);
  if (isPieceOnMoves(PieceType::tKING)) {
    return true;
  }
  moves_.clear();
  ruckMove(true);

  if (isPieceOnMoves(PieceType::tRUCK)) {
    return true;
  }
  moves_.clear();
  queenMove(true);
  if (isPieceOnMoves(PieceType::tQUEEN)) {
    return true;
  }
  moves_.clear();
  bishopMove(true);
  if (isPieceOnMoves(PieceType::tBISHOP)) {
    return true;
  }
  moves_.clear();
  poneAttackMove();
  if (isPieceOnMoves(PieceType::tPONE)) {
    return true;
  }
  moves_ = movesTMP;
  current_piece_ = pieceTMP;
  board_ = boardTMP;
  return false;
}

bool MovesGenerator::isPieceOnMoves(PieceType type) {
  for (const auto& move: moves_) {
    auto piece_here = move.getEnd();
    if (piece_here->getPieceColor() != current_piece_->getPieceColor()
        && piece_here->getType() == type) {
      return true;
    }
  }
  return false;
}

void MovesGenerator::insertPositionsToMoves(const std::list<Position>& positions) {
  for (const auto& pos: positions) {
    auto move = Move(current_piece_, board_->getPiece(pos));
    if (capture_only_ && move.getAttackScore() == 0) {
      continue;
    }
    if (non_capture_only_ && move.getAttackScore() != 0) {
      continue;
    }
    moves_.emplace_back(move);
  }
}

