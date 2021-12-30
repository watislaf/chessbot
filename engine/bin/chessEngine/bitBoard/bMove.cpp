#include <iomanip>
#include "bMove.h"

BMove::BMove(uint8_t from_, uint8_t to_, uint8_t flags) {
  data_ = to_;
  data_ <<= 6;
  data_ |= from_;
  data_ <<= 4;
  data_ |= (flags);
  // to from flags
  //000000 000000 0000
  from=getFrom();
  to=getTo();
  flag = getFlags();
}

BMove& BMove::operator=(const BMove& right) = default;

uint8_t BMove::getTo() const {
  return (data_ >> 10) & 0x3F;
}
uint8_t BMove::getFrom() const {
  return (data_ >> 4) & 0x3F;
}
uint8_t BMove::getFlags() const {
  return data_ & 0xF;
}

std::pair<uint8_t, uint8_t> BMove::getToPair() const {
  return {getTo() % 8, getTo() / 8};
}
std::pair<uint8_t, uint8_t> BMove::getFromPair() const {
  return {getFrom() % 8, getFrom() / 8};
}

bool BMove::getIsFlagSet(BFlagType flag_type) const {
  return data_ == flag_type;
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
std::string BMove::toStr() const {
  int sx = getFromPair().first;
  int sy = getFromPair().second;
  int ex = getToPair().first;
  int ey = getToPair().second;
  if (isInvalid()) {
    sx = 9;
    sy = 9;
    ex = 9;
    ey = 9;
  }
  int lol = getTo();
  auto answ = std::string("((") + char('0' + sx) + ','
      + char('0' + sy) + "),("
      + char('0' + ex) + ','
      + char('0' + ey) + ")";
  if (isPromotion()) {
    answ += ",";
    switch (getNewPieceType()) {
      case PieceType::KING:answ += "k";
        break;
      case PieceType::QUEEN:answ += "q";
        break;
      case PieceType::KNIGHT:answ += "n";
        break;
      case PieceType::RUCK:answ += "r";
        break;
      case PieceType::BISHOP:answ += "b";
        break;
      default: answ += "x";
    }
  }
  return answ + ")";
}
void BMove::setNewPieceType(PieceType type) {
  data_ |= 10 + static_cast<uint8_t>(type);
  flag = getFlags();
}

bool BMove::isInvalid() const {
  return data_ == uint16_t(-1);
}
PieceType BMove::getNewPieceType() const {
  if (isPromotion()) {
    return getCurrentPieceType();
  }
  return PieceType::NONE;
}
BMove::BMove() {
  data_ = -1;
}
PieceType BMove::getCurrentPieceType() const {
  auto flags = getFlags();
  if (flags <= 11) {
    return static_cast<PieceType>(((flags) % 6) + 1);
  } else {
    return static_cast<PieceType>(((flags) - 10));
  }
}
bool BMove::isPromotion() const {
  return getFlags() >= 12;
}
bool BMove::isCapture() const {
  return getFlags() > 5 && getFlags() < 12;
}
void BMove::setFlag(BMove::BFlagType type) {
  data_ &= ~15;
  data_ |= type;
  flag = getFlags();
}
