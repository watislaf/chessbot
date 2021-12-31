#ifndef BMOVESGENERATOR_H
#define BMOVESGENERATOR_H

#include <cstdint>
#include "bBoard.h"
#include "bMove.h"
#include <memory>
#include <cstring>

class BMovesGenerator {
 public:
  static std::vector<BMove> generate(BBoard* board);
  static const int MAX_MOVES = 256;
  static void generateMovesFromPosition(const uint8_t& position,
                                        uint64_t attacks,
                                        const uint8_t& flag = 0);
 private:
  static BMove moves_free_space[MAX_MOVES];
  static uint8_t cur_moves_count_;
  static void Pawn(const BBoard* board,
                   const BBoard::BPieceType& color,
                   const BBoard::BPieceType& enemy_color,
                   const uint64_t& empty,
                   const uint64_t& enemy);
  static void Knight(const BBoard* board,
                     const BBoard::BPieceType& color,
                     const BBoard::BPieceType& enemy_color,
                     const uint64_t& empty,
                     const uint64_t& enemy);
  static void Bishop(BBoard* p_board,
                     const BBoard::BPieceType& type,
                     const BBoard::BPieceType& type_1,
                     const uint64_t& empty,
                     const uint64_t& enemy);
  static void Roock(BBoard* p_board,
                    const BBoard::BPieceType& type,
                    const BBoard::BPieceType& type_1,
                    const uint64_t& empty,
                    const uint64_t& enemy);

  static void Queen(BBoard* p_board,
                    const BBoard::BPieceType& type,
                    const BBoard::BPieceType& type_1,
                    const uint64_t& empty,
                    const uint64_t& enemy);

  static void King(BBoard* p_board,
                    const BBoard::BPieceType& type,
                    const BBoard::BPieceType& type_1,
                    const uint64_t& empty,
                    const uint64_t& enemy);


};
#endif //BMOVESGENERATOR_H
