#ifndef B_MOVE_H
#define B_MOVE_H

#include <cstdint>

class BMove {
 public:
  explicit BMove(uint8_t from = 0, uint8_t to = 0, uint8_t flags = 0);
  BMove& operator=(const BMove& left);
  bool operator==(const BMove& left) const;
  bool operator!=(const BMove& left) const;
  uint8_t getTo() const;
  uint8_t getFrom() const;
  uint8_t getFlags() const;
  enum BFlagType {
    QUIET_MOVES,
    DOUBLE_PAWN_PUSH,
    KING_CASTLE,
    QUEEN_CASTLE,
    CAPTURES,
    EP_CAPTURE,
    KNIGHT_PROMOTION,
    BISHOP_PROMOTION,
    ROOK_PROMOTION,
    QUEEN_PROMOTION,
    KNIGHT_PROMO_CAPTURE,
    BISHOP_PROMO_CAPTURE,
    ROOK_PROMO_CAPTURE,
    QUEEN_PROMO_CAPTURE,
  };
  uint16_t data() const;
 private:
  /// 4bits flags, 6bits from, 6bits to
  uint16_t data_ = 0; // or short or template type
  void setTo(uint8_t to);
  void setFrom(uint8_t from);
  uint16_t getButterflyIndex() const;
  bool getIsFlagSet(BFlagType flag_type) const;
  uint16_t data() const;
};

#endif //B_MOVE_H
