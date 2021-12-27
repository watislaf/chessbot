#ifndef BMOVESGENERATOR_H
#define BMOVESGENERATOR_H

#include <cstdint>
#include "bit_board.h"
#include "b_move.h"

class BMovesGenerator {
 public:
  static std::vector<BMove> generate(BitBoard* board,
                                     BitBoard::BPieceType color);
  static const int MAX_MOVES = 256;
  static void generateMovesFromPosition(uint8_t position,
                                        uint64_t attacks,
                                        uint8_t flag = 0);
 private:
  static BMove moves_free_space[MAX_MOVES];
  static uint8_t cur_moves_count_;
  static void Pawn(const BitBoard* board,
                   const BitBoard::BPieceType& color,
                   const BitBoard::BPieceType& enemy_color,
                   const uint64_t& empty,
                   const uint64_t& enemy);
  static void Knight(const BitBoard* board,
                     const BitBoard::BPieceType& color,
                     const BitBoard::BPieceType& enemy_color,
                     const uint64_t& empty,
                     const uint64_t& enemy);
};

#endif //BMOVESGENERATOR_H
