#include <iomanip>
#include "b_move.h"

BMove::BMove(uint8_t from, uint8_t to, uint8_t flags) {
  data_ = (flags & 0xf);
  data_ <<= 6;
  data_ |= from & 0x3f;
  data_ <<= 6;
  data_ |= to & 0x3f;
}

BMove& BMove::operator=(const BMove& right) = default;

uint8_t BMove::getTo() const {
  return data_ & 0x3f;
}
uint8_t BMove::getFrom() const {
  return (data_ >> 6) & 0x3f;
}
uint8_t BMove::getFlags() const {
  return (data_ >> 12) & 0x0f;
}

void BMove::setTo(uint8_t to) {
  data_ &= ~0x3f;
  data_ |= to & 0x3f;
}

void BMove::setFrom(uint8_t from) {
  data_ &= ~0xfc0;
  data_ |= (from & 0x3f) << 6;
}

bool BMove::getIsFlagSet(BFlagType flag_type) const {
  return (data_ & flag_type) != 0;
}

uint16_t BMove::getButterflyIndex() const {
  return data_ & 0x0fff;
}

bool BMove::operator==(const BMove& a) const {
  return (data_ & 0xffff) == (a.data_ & 0xffff);
}
bool BMove::operator!=(const BMove& a) const {
  return (data_ & 0xffff) != (a.data_ & 0xffff);
}

uint16_t BMove::data() const {
  return data_;
}