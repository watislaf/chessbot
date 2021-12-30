#ifndef BMOVE_H
#define BMOVE_H

#include <cstdint>
#include "abstracts/pieceType.h"

class BMove {
 public:
  explicit BMove(uint8_t from, uint8_t to, uint8_t flags);
  explicit BMove();
  BMove(const BMove&) = default;
  BMove& operator=(const BMove& left);
  bool operator==(const BMove& left) const;
  bool operator!=(const BMove& left) const;
  uint8_t getTo() const;
  uint8_t getFrom() const;
  uint8_t getFlags() const;
  bool isInvalid() const;
  enum BFlagType {
    PAWN_MOVE = 0,
    KNIGHT_MOVE = 1,
    BISHOP_MOVE = 2,
    RUCK_MOVE = 3,
    QUEEN_MOVE = 4,
    KING_MOVE = 5,
    CAPTURE_BY_PAWN = 6,
    CAPTURE_BY_KNIGHT = 7,
    CAPTURE_BY_BISHOP = 8,
    CAPTURE_BY_RUCK = 9,
    CAPTURE_BY_QUEEN = 10,
    CAPTURE_BY_KING = 11,
    PROMOTE_TO_KNIGHT = 12,
    PROMOTE_TO_BISHOP = 13,
    PROMOTE_TO_RUCK = 14,
    PROMOTE_TO_QUEEN = 15,
  };

  uint16_t data() const;
  std::string toStr() const;

  bool getIsFlagSet(BFlagType flag_type) const;
  std::pair<uint8_t, uint8_t> getToPair() const;
  std::pair<uint8_t, uint8_t> getFromPair() const;

  void setNewPieceType(PieceType type);
  PieceType getNewPieceType() const;
  PieceType getCurrentPieceType() const;
  bool isCapture() const;
  void setFlag(BFlagType type);
  bool isPromotion() const;
 private:
  /// 4bits flags, 6bits from, 6bits to
  uint16_t data_ = 0; // or short or template type
  uint8_t from=0;
  uint8_t to=0;
  uint8_t flag=0;
};

#endif //BMOVE_H
