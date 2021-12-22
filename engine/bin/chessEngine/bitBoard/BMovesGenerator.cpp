#include <memory>
#include <cstring>
#include "BMovesGenerator.h"

BMove BMovesGenerator::moves_free_space[MAX_MOVES];

std::vector<BMove> BMovesGenerator::generate(BitBoard* board,
                                             BitBoard::BPieceType color) {
  cur_moves_count_ = 0;
  auto night_on_board = board->get(BitBoard::BLACK_NIGHT, color);
  auto free_spaces = ~board->get(color);
  while (night_on_board) {
    uint8_t start_pos = BitBoard::bitScanReverse(night_on_board);
    night_on_board ^= 1 << start_pos;
    generateMovesFromPosition(start_pos, BitBoard::knightAttacks(start_pos));
  }

  std::vector<BMove> answer(cur_moves_count_);
  memcpy(answer.data(), moves_free_space, cur_moves_count_ * sizeof(BMove));
  return answer;
}

void BMovesGenerator::generateMovesFromPosition(uint8_t start_pos,
                                                uint64_t attacks,
                                                uint8_t flag) {
  while (attacks) {
    uint8_t end_position = BitBoard::bitScanReverse(attacks);
    attacks ^= 1 << end_position;
    moves_free_space[cur_moves_count_++] = BMove(start_pos, end_position, flag);
  }
}
