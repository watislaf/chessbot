
#include "bMovesGenerator.h"

BMove BMovesGenerator::moves_free_space[MAX_MOVES];
uint8_t  BMovesGenerator::cur_moves_count_ = 0;

std::vector<BMove> BMovesGenerator::generate(BBoard* board) {

  BBoard::BPieceType color = board->whosTurn();
  auto enemy_color = static_cast<BBoard::BPieceType>(!color);
  cur_moves_count_ = 0;
  auto empty = ~board->get(color)
      & ~board->get(enemy_color);
  auto enemy = board->get(enemy_color);
  // KNIGHT
  Knight(board, color, enemy_color, empty, enemy);
  // PAWN
  Pawn(board, color, enemy_color, empty, enemy);
  // BISHOP
  Bishop(board, color, enemy_color, empty, enemy);
  // RUCK
  Roock(board, color, enemy_color, empty, enemy);
  //QUEEN
  Queen(board, color, enemy_color, empty, enemy);
  // KING
  King(board, color, enemy_color, empty, enemy);

  std::vector<BMove> answer(cur_moves_count_);
  memcpy(answer.data(), moves_free_space, cur_moves_count_ * sizeof(BMove));
  return answer;
}

void BMovesGenerator::generateMovesFromPosition(uint8_t start_pos,
                                                uint64_t attacks,
                                                uint8_t flag) {
  uint8_t end_position;
  while (attacks) {
    end_position = BBoard::bitScanForward(attacks);
    attacks &= attacks - 1;
    moves_free_space[cur_moves_count_++] = BMove(start_pos, end_position, flag);
  }
}
void BMovesGenerator::Pawn(const BBoard* board,
                           const BBoard::BPieceType& color,
                           const BBoard::BPieceType& enemy_color,
                           const uint64_t& empty,
                           const uint64_t& enemy) {
  // PAWN
  uint8_t start_pos;
  uint64_t pawns_ready_to_push;

  auto pawns_on_board = board->get(BBoard::WHITE_PAWN, color);
  int start = cur_moves_count_;
  if (color == BBoard::WHITE_PIECES) {
    pawns_ready_to_push = BBoard::wPawnsAble2Push(pawns_on_board, empty);
    while (pawns_ready_to_push) {
      start_pos = BBoard::bitScanForward(pawns_ready_to_push);
      pawns_ready_to_push &= pawns_ready_to_push - 1;
      moves_free_space[cur_moves_count_++] =
          BMove(start_pos, start_pos + 8, BMove::BFlagType::PAWN_MOVE);
    }

    // READY TO GET WEST
    pawns_ready_to_push =
        BBoard::wPawnsAble2CaptureWest(pawns_on_board, enemy);
    while (pawns_ready_to_push) {
      start_pos = BBoard::bitScanForward(pawns_ready_to_push);
      pawns_ready_to_push &= pawns_ready_to_push - 1;
      moves_free_space[cur_moves_count_++] =
          BMove(start_pos, start_pos + 7, BMove::BFlagType::CAPTURE_BY_PAWN);
    }
    // READY TO GET EAST
    pawns_ready_to_push =
        BBoard::wPawnsAble2CaptureEast(pawns_on_board, enemy);
    while (pawns_ready_to_push) {
      start_pos = BBoard::bitScanForward(pawns_ready_to_push);
      pawns_ready_to_push &= pawns_ready_to_push - 1;
      moves_free_space[cur_moves_count_++] =
          BMove(start_pos, start_pos + 9, BMove::BFlagType::CAPTURE_BY_PAWN);
    }
    int copy = cur_moves_count_;
    for (int j = start; j < copy; j++) {
      if (moves_free_space[j].getTo() >= 56) {
        moves_free_space[j].setFlag(BMove::BFlagType::PROMOTE_TO_RUCK);
        moves_free_space[cur_moves_count_] = moves_free_space[j];
        moves_free_space[cur_moves_count_++].setFlag(BMove::BFlagType::PROMOTE_TO_QUEEN);
        moves_free_space[cur_moves_count_] = moves_free_space[j];
        moves_free_space[cur_moves_count_++].setFlag(BMove::BFlagType::PROMOTE_TO_BISHOP);
        moves_free_space[cur_moves_count_] = moves_free_space[j];
        moves_free_space[cur_moves_count_++].setFlag(BMove::BFlagType::PROMOTE_TO_KNIGHT);
      }
    }

    // DOUBLE PUSH
    pawns_ready_to_push =
        BBoard::wPawnsAble2DblPush(pawns_on_board, empty);
    while (pawns_ready_to_push) {
      start_pos = BBoard::bitScanForward(pawns_ready_to_push);
      pawns_ready_to_push &= pawns_ready_to_push - 1;
      moves_free_space[cur_moves_count_++] =
          BMove(start_pos, start_pos + 16, BMove::BFlagType::PAWN_MOVE);
    }
    // EP
    pawns_ready_to_push =
        BBoard::wPawnsAble2WestEP(pawns_on_board, board->getLastDoublePush());
    while (pawns_ready_to_push) {
      start_pos = BBoard::bitScanForward(pawns_ready_to_push);
      pawns_ready_to_push &= pawns_ready_to_push - 1;
      moves_free_space[cur_moves_count_++] =
          BMove(start_pos, start_pos + 7,
                BMove::BFlagType::CAPTURE_BY_PAWN);
    }
    pawns_ready_to_push =
        BBoard::wPawnsAble2EastEP(pawns_on_board, board->getLastDoublePush());
    while (pawns_ready_to_push) {
      start_pos = BBoard::bitScanForward(pawns_ready_to_push);
      pawns_ready_to_push &= pawns_ready_to_push - 1;
      moves_free_space[cur_moves_count_++] =
          BMove(start_pos, start_pos + 9, BMove::CAPTURE_BY_PAWN);
    }

  } else {
    pawns_ready_to_push = BBoard::bPawnsAble2Push(pawns_on_board, empty);
    while (pawns_ready_to_push) {
      start_pos = BBoard::bitScanForward(pawns_ready_to_push);
      pawns_ready_to_push &= pawns_ready_to_push - 1;
      moves_free_space[cur_moves_count_++] =
          BMove(start_pos, start_pos - 8, BMove::BFlagType::PAWN_MOVE);
    }

    //READY TO GET WEST
    pawns_ready_to_push =
        BBoard::bPawnsAble2CaptureWest(pawns_on_board, enemy);
    while (pawns_ready_to_push) {
      start_pos = BBoard::bitScanForward(pawns_ready_to_push);
      pawns_ready_to_push &= pawns_ready_to_push - 1;
      moves_free_space[cur_moves_count_++] =
          BMove(start_pos, start_pos - 9, BMove::CAPTURE_BY_PAWN);
    }
    //READTY TO GET EAST
    pawns_ready_to_push =
        BBoard::bPawnsAble2CaptureEast(pawns_on_board, enemy);
    while (pawns_ready_to_push) {
      start_pos = BBoard::bitScanForward(pawns_ready_to_push);
      pawns_ready_to_push &= pawns_ready_to_push - 1;
      moves_free_space[cur_moves_count_++] =
          BMove(start_pos, start_pos - 7, BMove::CAPTURE_BY_PAWN);
    }
    int copy = cur_moves_count_;
    for (int j = start; j < copy; j++) {
      if (moves_free_space[j].getTo() <= 7) {
        moves_free_space[j].setFlag(BMove::BFlagType::PROMOTE_TO_RUCK);
        moves_free_space[cur_moves_count_] = moves_free_space[j];
        moves_free_space[cur_moves_count_++].setFlag(BMove::BFlagType::PROMOTE_TO_QUEEN);
        moves_free_space[cur_moves_count_] = moves_free_space[j];
        moves_free_space[cur_moves_count_++].setFlag(BMove::BFlagType::PROMOTE_TO_BISHOP);
        moves_free_space[cur_moves_count_] = moves_free_space[j];
        moves_free_space[cur_moves_count_++].setFlag(BMove::BFlagType::PROMOTE_TO_KNIGHT);
      }
    }

    //DOUBLE PUSH
    pawns_ready_to_push =
        BBoard::bPawnsAble2DblPush(pawns_on_board, empty);
    while (pawns_ready_to_push) {
      start_pos = BBoard::bitScanForward(pawns_ready_to_push);
      pawns_ready_to_push &= pawns_ready_to_push - 1;
      moves_free_space[cur_moves_count_++] =
          BMove(start_pos, start_pos - 16, BMove::BFlagType::PAWN_MOVE);
    }
    // EP
    pawns_ready_to_push =
        BBoard::bPawnsAble2WestEP(pawns_on_board, board->getLastDoublePush());
    while (pawns_ready_to_push) {
      start_pos = BBoard::bitScanForward(pawns_ready_to_push);
      pawns_ready_to_push &= pawns_ready_to_push - 1;
      moves_free_space[cur_moves_count_++] =
          BMove(start_pos, start_pos - 9, BMove::BFlagType::CAPTURE_BY_PAWN);
    }
    pawns_ready_to_push =
        BBoard::bPawnsAble2EastEP(pawns_on_board, board->getLastDoublePush());
    while (pawns_ready_to_push) {
      start_pos = BBoard::bitScanForward(pawns_ready_to_push);
      pawns_ready_to_push &= pawns_ready_to_push - 1;
      moves_free_space[cur_moves_count_++] =
          BMove(start_pos, start_pos - 7, BMove::BFlagType::CAPTURE_BY_PAWN);
    }
  }
}

void BMovesGenerator::Knight(const BBoard* board,
                             const BBoard::BPieceType& color,
                             const BBoard::BPieceType& enemy_color,
                             const uint64_t& empty,
                             const uint64_t& enemy) {
  uint8_t start_pos;
  uint64_t knight_atack;

  auto knight_on_board = board->get(BBoard::WHITE_KNIGHT, color);
  while (knight_on_board) {
    start_pos =
        BBoard::bitScanForward(knight_on_board);
    knight_on_board &= knight_on_board - 1;
    knight_atack = BBoard::knightAttacks(start_pos) & empty;
    generateMovesFromPosition(
        start_pos,
        knight_atack,
        BMove::BFlagType::KNIGHT_MOVE
    );
    knight_atack = BBoard::knightAttacks(start_pos) & enemy;
    generateMovesFromPosition(
        start_pos,
        knight_atack,
        BMove::BFlagType::CAPTURE_BY_KNIGHT
    );
  }
}
void BMovesGenerator::Bishop(BBoard* board,
                             const BBoard::BPieceType& color,
                             const BBoard::BPieceType& enemy_color,
                             const uint64_t& empty,
                             const uint64_t& enemy) {

  uint8_t start_pos;
  uint64_t bishop_attacks;

  auto bishops_on_board = board->get(BBoard::WHITE_BISHOP, color);

  while (bishops_on_board) {
    start_pos =
        BBoard::bitScanForward(bishops_on_board);
    bishops_on_board &= bishops_on_board - 1;

    bishop_attacks =
        BBoard::bishopAttacks(
            ~empty ,
            start_pos);
    generateMovesFromPosition(
        start_pos,
        bishop_attacks & empty,
        BMove::BFlagType::BISHOP_MOVE
    );
    generateMovesFromPosition(
        start_pos,
        bishop_attacks & enemy,
        BMove::BFlagType::CAPTURE_BY_BISHOP
    );
  }
}
void BMovesGenerator::Queen(BBoard* board,
                            const BBoard::BPieceType& color,
                            const BBoard::BPieceType& enemy_color,
                            const uint64_t& empty,
                            const uint64_t& enemy) {

  uint8_t start_pos;
  uint64_t queen_atack;

  auto bishops_on_board = board->get(BBoard::WHITE_QUEEN, color);

  while (bishops_on_board) {
    start_pos =
        BBoard::bitScanForward(bishops_on_board);
    bishops_on_board &= bishops_on_board - 1;

    queen_atack =
        BBoard::queenAttacks(
            ~empty ,
            start_pos);
    generateMovesFromPosition(
        start_pos,
        queen_atack & empty,
        BMove::BFlagType::QUEEN_MOVE
    );
    generateMovesFromPosition(
        start_pos,
        queen_atack & enemy,
        BMove::BFlagType::CAPTURE_BY_QUEEN
    );
  }

}

void BMovesGenerator::Roock(BBoard* board,
                            const BBoard::BPieceType& color,
                            const BBoard::BPieceType& enemy_color,
                            const uint64_t& empty,
                            const uint64_t& enemy) {

  uint8_t start_pos;
  uint64_t rook_atack;

  auto bishops_on_board = board->get(BBoard::WHITE_ROOK, color);

  while (bishops_on_board) {
    start_pos =
        BBoard::bitScanForward(bishops_on_board);
    bishops_on_board &= bishops_on_board - 1;

    rook_atack =
        BBoard::rookAttacks(
            ~empty,
            start_pos);
    generateMovesFromPosition(
        start_pos,
        rook_atack & empty,
        BMove::BFlagType::RUCK_MOVE
    );
    generateMovesFromPosition(
        start_pos,
        rook_atack & enemy,
        BMove::BFlagType::CAPTURE_BY_RUCK
    );
  }
}
void BMovesGenerator::King(BBoard* board,
                           const BBoard::BPieceType& color,
                           const BBoard::BPieceType& enemy_color,
                           const uint64_t& empty,
                           const uint64_t& enemy) {

  uint8_t
      king_pos = BBoard::bitScanForward(board->get(BBoard::WHITE_KING, color));

  uint64_t king_atack = BBoard::kingAttacks(king_pos)
      & (empty | enemy);

  generateMovesFromPosition(
      king_pos,
      king_atack & empty,
      BMove::BFlagType::KING_MOVE
  );
  generateMovesFromPosition(
      king_pos,
      king_atack & enemy,
      BMove::BFlagType::CAPTURE_BY_KING
  );

  if (board->attacksToKing(king_pos, color) != 0) {
    return;
  }

  if (board->canLeftCastle()) {
    moves_free_space[cur_moves_count_++] =
        BMove(king_pos, king_pos - 2, BMove::KING_MOVE);
  }
  if (board->canRightCastle()) {
    moves_free_space[cur_moves_count_++] =
        BMove(king_pos, king_pos + 2, BMove::KING_MOVE);
  }

}