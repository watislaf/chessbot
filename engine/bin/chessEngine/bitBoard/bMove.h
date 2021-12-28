#ifndef BMOVE_H
#define BMOVE_H

#include <cstdint>
#include "abstracts/pieceType.h"

class BMove {
 public:
  explicit BMove(uint8_t from , uint8_t to , uint8_t flags = 0);
  explicit BMove();
  BMove(const BMove&) = default;
  BMove& operator=(const BMove& left);
  bool operator==(const BMove& left) const;
  bool operator!=(const BMove& left) const;
  uint8_t getTo() const;
  uint8_t getFrom() const;
  uint8_t getFlags() const;
  BMove makeInvalid() ;
  bool isInvalid() const ;
  enum BFlagType {
      KNIGHT_PROMOTION=2, //or
      BISHOP_PROMOTION=3,//or
      ROOK_PROMOTION=4,//or
      QUEEN_PROMOTION=5,//or
    KING_PROMOTION=6,//or means that this is GG

    DOUBLE_PAWN_PUSH=8,//or
    KING_CASTLE=16,//or
    QUEEN_CASTLE=24,//or
    CAPTURES=32, // or
    BRAKE_QUEEN_CASTLE=48,//or
    BRAKE_KING_CASTLE=64,//or
    EP_CAPTURE=128,

    // empty 9
    // empty 10
  };
  void xorFlags(BFlagType flag);
  uint16_t data() const;
  std::string toStr() const ;
  void setTo(uint8_t to);
  void setFrom(uint8_t from);

  bool getIsFlagSet(BFlagType flag_type) const;
  std::pair<uint8_t, uint8_t> getToPair() const;
  std::pair<uint8_t, uint8_t> getFromPair() const;

  void setNewPieceType(PieceType type);
  PieceType getNewPieceType() const;
 private:
  /// 4bits flags, 6bits from, 6bits to
  uint16_t data_ = 0; // or short or template type
};

#endif //BMOVE_H
