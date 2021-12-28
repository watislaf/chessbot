
#include "bMovesGenerator.h"

BMove BMovesGenerator::moves_free_space[MAX_MOVES];
uint8_t  BMovesGenerator::cur_moves_count_ = 0;

std::vector<BMove> BMovesGenerator::generate(BBoard* board,
                                             BBoard::BPieceType color) {
  auto enemy_color = static_cast<BBoard::BPieceType>(!color);
  cur_moves_count_ = 0;
  auto empty = ~board->get(color)
      & ~board->get(enemy_color);
  auto enemy = board->get(enemy_color);
  // KNIGHT
  Knight(board, color, enemy_color, empty, enemy);
  Pawn(board, color, enemy_color, empty, enemy);

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
  auto pawns_on_board = board->get(BBoard::WHITE_PAWN, color);
  uint64_t pawns_ready_to_push;
  uint64_t pawns_ready_to_double_push;
  uint64_t pawns_ready_to_get_west;
  uint64_t pawns_ready_to_get_east;
  uint64_t pawns_ready_to_ep;
  int start = cur_moves_count_;
  if (color == BBoard::WHITE_PIECES) {
    pawns_ready_to_push = BBoard::wPawnsAble2Push(pawns_on_board, empty);
    while (pawns_ready_to_push) {
      start_pos = BBoard::bitScanForward(pawns_ready_to_push);
      pawns_ready_to_push &= pawns_ready_to_push - 1;
      moves_free_space[cur_moves_count_++] =
          BMove(start_pos, start_pos + 8, 0);
    }
    pawns_ready_to_get_west =
        BBoard::wPawnsAble2CaptureWest(pawns_on_board, enemy);
    while (pawns_ready_to_get_west) {
      start_pos = BBoard::bitScanForward(pawns_ready_to_get_west);
      pawns_ready_to_get_west &= pawns_ready_to_get_west - 1;
      moves_free_space[cur_moves_count_++] =
          BMove(start_pos, start_pos + 7, BMove::BFlagType::CAPTURES);
    }
    pawns_ready_to_get_east =
        BBoard::wPawnsAble2CaptureEast(pawns_on_board, enemy);
    while (pawns_ready_to_get_east) {
      start_pos = BBoard::bitScanForward(pawns_ready_to_get_east);
      pawns_ready_to_get_east &= pawns_ready_to_get_east - 1;
      moves_free_space[cur_moves_count_++] =
          BMove(start_pos, start_pos + 9, BMove::BFlagType::CAPTURES);
    }
    int copy = cur_moves_count_;
    for (int j = start_pos; j < copy; j++) {
      if (moves_free_space[j].getTo() >= 56) {
        moves_free_space[j].xorFlags(BMove::BFlagType::ROOK_PROMOTION);
        moves_free_space[cur_moves_count_] = moves_free_space[j];
        moves_free_space[cur_moves_count_++].xorFlags(BMove::BFlagType::QUEEN_PROMOTION);
        moves_free_space[cur_moves_count_] = moves_free_space[j];
        moves_free_space[cur_moves_count_++].xorFlags(BMove::BFlagType::BISHOP_PROMOTION);
        moves_free_space[cur_moves_count_] = moves_free_space[j];
        moves_free_space[cur_moves_count_++].xorFlags(BMove::BFlagType::KNIGHT_PROMOTION);
      }
    }
    pawns_ready_to_double_push =
        BBoard::wPawnsAble2DblPush(pawns_on_board, empty);
    while (pawns_ready_to_double_push) {
      start_pos = BBoard::bitScanForward(pawns_ready_to_double_push);
      pawns_ready_to_double_push &= pawns_ready_to_double_push - 1;
      moves_free_space[cur_moves_count_++] =
          BMove(start_pos, start_pos + 16, BMove::BFlagType::DOUBLE_PAWN_PUSH);
    }
    pawns_ready_to_ep =
        BBoard::wPawnsAble2WestEP(pawns_on_board, board->getLastDoublePush());
    while (pawns_ready_to_ep) {
      start_pos = BBoard::bitScanForward(pawns_ready_to_ep);
      pawns_ready_to_ep &= pawns_ready_to_ep - 1;
      moves_free_space[cur_moves_count_++] =
          BMove(start_pos, start_pos + 7, BMove::BFlagType::EP_CAPTURE);
    }
    pawns_ready_to_ep =
        BBoard::wPawnsAble2EastEP(pawns_on_board, board->getLastDoublePush());
    while (pawns_ready_to_ep) {
      start_pos = BBoard::bitScanForward(pawns_ready_to_ep);
      pawns_ready_to_ep &= pawns_ready_to_ep - 1;
      moves_free_space[cur_moves_count_++] =
          BMove(start_pos, start_pos + 9, BMove::BFlagType::EP_CAPTURE);
    }

  } else {
    pawns_ready_to_push = BBoard::bPawnsAble2Push(pawns_on_board, empty);
    while (pawns_ready_to_push) {
      start_pos = BBoard::bitScanForward(pawns_ready_to_push);
      pawns_ready_to_push &= pawns_ready_to_push - 1;
      moves_free_space[cur_moves_count_++] =
          BMove(start_pos, start_pos - 8, 0);
    }
    pawns_ready_to_get_west =
        BBoard::bPawnsAble2CaptureWest(pawns_on_board, enemy);
    while (pawns_ready_to_get_west) {
      start_pos = BBoard::bitScanForward(pawns_ready_to_get_west);
      pawns_ready_to_get_west &= pawns_ready_to_get_west - 1;
      moves_free_space[cur_moves_count_++] =
          BMove(start_pos, start_pos - 9, BMove::BFlagType::CAPTURES);
    }
    pawns_ready_to_get_east =
        BBoard::bPawnsAble2CaptureEast(pawns_on_board, enemy);
    while (pawns_ready_to_get_east) {
      start_pos = BBoard::bitScanForward(pawns_ready_to_get_east);
      pawns_ready_to_get_east &= pawns_ready_to_get_east - 1;
      moves_free_space[cur_moves_count_++] =
          BMove(start_pos, start_pos - 7, BMove::BFlagType::CAPTURES);
    }
    int copy = cur_moves_count_;
    for (int j = start_pos; j < copy; j++) {
      if (moves_free_space[j].getTo() <= 7) {
        moves_free_space[j].xorFlags(BMove::BFlagType::ROOK_PROMOTION);
        moves_free_space[cur_moves_count_] = moves_free_space[j];
        moves_free_space[cur_moves_count_++].xorFlags(BMove::BFlagType::QUEEN_PROMOTION);
        moves_free_space[cur_moves_count_] = moves_free_space[j];
        moves_free_space[cur_moves_count_++].xorFlags(BMove::BFlagType::BISHOP_PROMOTION);
        moves_free_space[cur_moves_count_] = moves_free_space[j];
        moves_free_space[cur_moves_count_++].xorFlags(BMove::BFlagType::KNIGHT_PROMOTION);
      }
    }
    pawns_ready_to_double_push =
        BBoard::bPawnsAble2DblPush(pawns_on_board, empty);
    while (pawns_ready_to_double_push) {
      start_pos = BBoard::bitScanForward(pawns_ready_to_double_push);
      pawns_ready_to_double_push &= pawns_ready_to_double_push - 1;
      moves_free_space[cur_moves_count_++] =
          BMove(start_pos, start_pos - 16, BMove::BFlagType::DOUBLE_PAWN_PUSH);
    }
    pawns_ready_to_ep =
        BBoard::bPawnsAble2WestEP(pawns_on_board, board->getLastDoublePush());
    while (pawns_ready_to_ep) {
      start_pos = BBoard::bitScanForward(pawns_ready_to_ep);
      pawns_ready_to_ep &= pawns_ready_to_ep - 1;
      moves_free_space[cur_moves_count_++] =
          BMove(start_pos, start_pos - 9, BMove::BFlagType::EP_CAPTURE);
    }
    pawns_ready_to_ep =
        BBoard::bPawnsAble2EastEP(pawns_on_board, board->getLastDoublePush());
    while (pawns_ready_to_ep) {
      start_pos = BBoard::bitScanForward(pawns_ready_to_ep);
      pawns_ready_to_ep &= pawns_ready_to_ep - 1;
      moves_free_space[cur_moves_count_++] =
          BMove(start_pos, start_pos - 7, BMove::BFlagType::EP_CAPTURE);
    }
  }
}

void BMovesGenerator::Knight(const BBoard* board,
                             const BBoard::BPieceType& color,
                             const BBoard::BPieceType& enemy_color,
                             const uint64_t& empty,
                             const uint64_t& enemy) {

  uint8_t start_pos;
  auto knight_on_board = board->get(BBoard::WHITE_KNIGHT, color);
  uint64_t knight_atack;
  while (knight_on_board) {
    start_pos =
        BBoard::bitScanForward(knight_on_board);
    knight_on_board &= knight_on_board - 1;
    knight_atack = BBoard::knightAttacks(start_pos) & empty;
    generateMovesFromPosition(
        start_pos,
        knight_atack
    );
    knight_atack = BBoard::knightAttacks(start_pos) & enemy;
    generateMovesFromPosition(
        start_pos,
        knight_atack,
        BMove::BFlagType::CAPTURES
    );
  }
}
