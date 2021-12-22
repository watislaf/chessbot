#include <algorithm>
#include <iostream>
#include "abstracts/move.h"
#include "movesGenerator.h"
#include "tools/pricer.h"

MovesGenerator::MovesGenerator(const std::shared_ptr<ObjBoard>& board,
                               const std::shared_ptr<const Piece>& piece) {
  board_ = board;
  piece_ = piece;
}

std::shared_ptr<std::list<Move>> MovesGenerator::generateMoves() {
  defende_score = 0;
  // Create simple moves
  switch (piece_->getType()) {
    case PieceType::NIGHT:horseMove();
      break;
    case PieceType::KING:kingMove();
      castleMove();
      break;
    case PieceType::RUCK:ruckMove();
      break;
    case PieceType::QUEEN:queenMove();
      break;
    case PieceType::BISHOP:bishopMove();
      break;
    case PieceType::PAWN:poneAttackMove();
      ponePacificMove();

      break;
    case PieceType::NONE:
      return std::make_shared<std::list<Move>>(std::move(moves_));
  }
  if (board_->getLastPassantX() != -1) {
    for (auto& move: moves_) {
      move.setPrevPassant(board_->getLastPassantX());
    }
  }
  auto begin_to_erase_ponter = std::remove_if(moves_.begin(),
                                              moves_.end(),
                                              [this](const Move& move) {
                                                return isShahDanger(move);
                                              });
//for (auto& move: moves_) {
//    move.setDefendScore(defende_score);
//  }
  if (begin_to_erase_ponter != moves_.end()) {
    moves_.erase(begin_to_erase_ponter, moves_.end());
  }
  return std::make_shared<std::list<Move>>(std::move(moves_));
}

std::list<Move> MovesGenerator::goByVector(Position start,
                                           Position move_vector,
                                           short max_length,
                                           bool only_tNone,
                                           bool reduce_tNone) {
  if (move_vector.getX() == 0 && move_vector.getY() == 0) {
    return {};
  }

  std::list<Move> position_to_move;
  for (short length = 1; length <= max_length; length++) {
    auto new_position = Position(
        length * move_vector.getX(), length * move_vector.getY()
    ) + start;
    auto posY = new_position.getY();
    auto posX = new_position.getX();
    if (posY < 0 || posY > 7 || posX < 0 || posX > 7) {
      break;
    }
    if (board_->getPiece(new_position)->getType() == PieceType::NONE) {
      if (!reduce_tNone) {
        position_to_move.emplace_back(Move(piece_,
                                           board_->getPiece(new_position)));
      }
      continue;
    }
    if (!only_tNone) {
      position_to_move.emplace_back(Move(piece_,
                                         board_->getPiece(new_position)));
    }
    break;
  }
  return position_to_move;
}

void MovesGenerator::bishopMove(bool reduce_tNone) {
  for (short left = -1; left <= 1; left += 2) {
    for (short top = -1; top <= 1; top += 2) {
      moves_.splice(moves_.end(), goByVector(piece_->getPosition(),
                                             Position(left, top),
                                             8,
                                             false,
                                             reduce_tNone));
    }
  }
}

void MovesGenerator::horseMove(bool reduce_tNone) {
  for (short left = -1; left <= 1; left += 2) {
    for (short top = -1; top <= 2; top += 2) {
      moves_.splice(moves_.end(), goByVector(piece_->getPosition(),
                                             Position(1 * left, 2 * top),
                                             1));
      moves_.splice(moves_.end(), goByVector(piece_->getPosition(),
                                             Position(2 * left, 1 * top),
                                             1));
    }
  }
}

void MovesGenerator::queenMove(bool reduce_tNone) {
  for (short left = -1; left <= 1; left += 1) {
    for (short top = -1; top <= 1; top += 1) {
      moves_.splice(moves_.end(), goByVector(piece_->getPosition(),
                                             Position(left, top),
                                             8,
                                             false,
                                             reduce_tNone));
    }
  }
}

void MovesGenerator::kingMove(bool reduce_tNone) {
  for (short left = -1; left <= 1; left += 1) {
    for (short top = -1; top <= 1; top += 1) {
      moves_.splice(moves_.end(), goByVector(piece_->getPosition(),
                                             Position(left, top),
                                             1,
                                             false,
                                             reduce_tNone));
    }
  }
}

void MovesGenerator::ponePacificMove(bool reduce_tNone) {
  short length = piece_->getPieceColor() == ColorType::WHITE ? 1 : -1;
  moves_.splice(moves_.end(), goByVector(piece_->getPosition(),
                                         Position(0, length),
                                         1,
                                         true,
                                         reduce_tNone));
  auto piece_color = piece_->getPieceColor();
  for (auto iter = moves_.begin(); iter != moves_.end(); ++iter) {
    if (iter->getEnd()->getPosition().getY() % 7 == 0) {
      iter->setNewPieceType(PieceType::QUEEN);
      moves_.emplace_front(iter->getStart(), iter->getEnd());
      moves_.front().setNewPieceType(PieceType::BISHOP);
      moves_.emplace_front(iter->getStart(), iter->getEnd());
      moves_.front().setNewPieceType(PieceType::RUCK);
      moves_.emplace_front(iter->getStart(), iter->getEnd());
      moves_.front().setNewPieceType(PieceType::NIGHT);
    }
  }
  // DOUBLE MOVE
  if (piece_color == ColorType::WHITE
      && piece_->getPosition().getY() == 1 ||
      piece_color == ColorType::BLACK
          && piece_->getPosition().getY() == 6) {
    if (goByVector(piece_->getPosition(),
                   Position(0, piece_color == ColorType::WHITE ? 1 : -1),
                   2,
                   true).
        size() == 2) {
      moves_.emplace_back(
          piece_,
          board_->getPiece(piece_->getPosition()
                               + Position(0, piece_color == ColorType::WHITE
                                             ? 2 : -2)));
      moves_.back().setIsDoubleDistancePone(true, board_->getLastPassantX());
    }
  }

}

void MovesGenerator::ruckMove(bool reduce_tNone) {
  for (short top = -1; top <= 1; top += 2) {
    moves_.splice(moves_.end(), goByVector(piece_->getPosition(),
                                           Position(0, top),
                                           8,
                                           false,
                                           reduce_tNone));
    moves_.splice(moves_.end(), goByVector(piece_->getPosition(),
                                           Position(top, 0),
                                           8,
                                           false,
                                           reduce_tNone));
  }
  if (board_->LcIsPossible(
      piece_->getPieceColor() == ColorType::WHITE)) {
    if (piece_->getPosition().getX() == 0) {
      for (auto& move: moves_) {
        move.setBrakeLeftCastle(true);
      }
    }
  }
  if (board_->RcIsPossible(
      piece_->getPieceColor() == ColorType::WHITE)) {
    if (piece_->getPosition().getX() == 7) {
      for (auto& move: moves_) {
        move.setBrakeRightCastle(true);
      }
    }
  }
}

void MovesGenerator::poneAttackMove() {
  short
      direction = piece_->getPieceColor() == ColorType::WHITE ? 1 : -1;
  for (short i = -1; i <= 1; i += 2) {
    auto go_diagonal = goByVector(piece_->getPosition(),
                                  Position(i, direction),
                                  1,
                                  false,
                                  true);
    if (go_diagonal.size() == 1
        && go_diagonal.back().getEnd()->isEnemyTo(*piece_)) {
      moves_.emplace_back(piece_, go_diagonal.back().getEnd());
    }

    // Atack passant
    short
        good_row = piece_->getPieceColor() == ColorType::WHITE ? 4 : 3;
    go_diagonal = goByVector(piece_->getPosition(),
                             Position(i, direction),
                             1,
                             true,
                             false);
    if (go_diagonal.size() == 1
        && board_->getLastPassantX()
            == go_diagonal.back().getEnd()->getPosition().getX()
        && piece_->getPosition().getY() == good_row) {
      moves_.emplace_back(piece_, go_diagonal.back().getEnd());
      moves_.back().setIsPassant(true);
      moves_.back().setAttackPrice(Pricer::getPrice(moves_.back().getStart()));
    }
  }
}

void MovesGenerator::castleMove() {
  if (board_->RcIsPossible(
      piece_->getPieceColor() == ColorType::WHITE)) {
    for (auto& move: moves_) {
      move.setBrakeRightCastle(true);
    }
  }
  if (board_->LcIsPossible(
      piece_->getPieceColor() == ColorType::WHITE)) {
    for (auto& move: moves_) {
      move.setBrakeLeftCastle(true);
    }
  }
  // If king move he lost castle
  if (isShah(piece_->getPieceColor() == ColorType::WHITE)) {
    return;
  }
  if (board_->LcIsPossible(
      piece_->getPieceColor() == ColorType::WHITE)) {
    auto positions_left =
        goByVector(piece_->getPosition(), Position(-1, 0));
    if (positions_left.size() == 4) {
      moves_.emplace_back(
          piece_,
          board_->getPiece(piece_->getPosition() + Position(-2, 0)));
      moves_.back().setIsCastle(true);
    }
  }
  if (board_->RcIsPossible(
      piece_->getPieceColor() == ColorType::WHITE)) {
    auto positions_right =
        goByVector(piece_->getPosition(), Position(1, 0));
    if (positions_right.size() == 3) {
      moves_.emplace_back(piece_, board_->getPiece(
          piece_->getPosition() + Position(2, 0)));
      moves_.back().setIsCastle(true);
    }

  }
}

bool MovesGenerator::isShahDanger(const Move& move) {
  if (move.getDefendScore() != 0) {
    defende_score += Pricer::defendScore(move);
    return true;
  }

  board_->apply(move);
  bool is_under_shach =
      isShah(move.getStart()->getPieceColor() == ColorType::WHITE);
  board_->unApply(move);

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
        isShah(move.getStart()->getPieceColor() == ColorType::WHITE);
    board_->unApply(non_castle_move);
  }

  if (is_under_shach) {
    return true;
  }
  return false;

}

bool MovesGenerator::isShah(bool is_white) {
  Position king_pos =
      board_->getPiece(board_->getKingPosition(is_white))->getPosition();
  const auto& list_of_pieces = board_->getActivePieceList(!is_white);
  for (auto piece: list_of_pieces) {
    auto piece_pos = piece->getPosition();
    if (piece_pos == king_pos) {
      return true;
    }
    switch (piece->getType()) {
      case PieceType::NIGHT:
        if (abs(king_pos.getX() - piece_pos.getX()) == 1 &&
            abs(king_pos.getY() - piece_pos.getY()) == 2 ||
            abs(king_pos.getX() - piece_pos.getX()) == 2 &&
                abs(king_pos.getY() - piece_pos.getY()) == 1) {
          return true;
        }
        break;
      case PieceType::KING:
        if (abs(king_pos.getX() - piece_pos.getX()) <= 1 &&
            abs(king_pos.getY() - piece_pos.getY()) <= 1) {
          return true;
        }
        break;
      case PieceType::QUEEN:
      case PieceType::RUCK:
        if (king_pos.getX() == piece_pos.getX() &&
            goByVector(king_pos,
                       Position(0,
                                -(king_pos.getY() - piece_pos.getY())
                                    / abs(king_pos.getY()
                                              - piece_pos.getY())),
                       8, true).size() + 1
                >= abs(king_pos.getY() - piece_pos.getY())) {
          return true;
        }
        if (king_pos.getY() == piece_pos.getY() &&
            goByVector(king_pos,
                       Position(
                           -(king_pos.getX() - piece_pos.getX())
                               / abs(king_pos.getX() - piece_pos.getX()), 0),
                       8, true).size() + 1
                >= abs(king_pos.getX() - piece_pos.getX())) {
          return true;
        }
        if (piece->getType() == PieceType::RUCK)
          break;
      case PieceType::BISHOP:
        if ((abs(king_pos.getX() - piece_pos.getX())
            == abs(king_pos.getY() - piece_pos.getY())) &&
            goByVector(king_pos,
                       Position(
                           -(abs(king_pos.getX() - piece_pos.getX()))
                               / ((king_pos.getX() - piece_pos.getX())),
                           -(abs(king_pos.getY() - piece_pos.getY()))
                               / (king_pos.getY() - piece_pos.getY())
                       ),
                       8, true).size() + 1
                == abs(king_pos.getX() - piece_pos.getX())) {
          return true;
        }
        break;
      case PieceType::PAWN:
        if (king_pos.getY() == (piece_pos.getY() - 1
            + 2 * (piece->getPieceColor() == ColorType::WHITE))
            && abs(king_pos.getX() - piece_pos.getX()) == 1) {
          return true;
        }
        break;
      case PieceType::NONE:return {};
    }
  }
  return false;
}

bool MovesGenerator::isMate() {
  if (isShah(board_->isWhiteTurn())) {

    const auto active = board_->getActivePieceList(board_->isWhiteTurn());
    for (const auto& active_piece: active) {
      piece_ = active_piece;
      const auto& moves = generateMoves();
      if (!moves->empty()) {
        return false;
      }
    }
    return true;
  }
  return false;
}


