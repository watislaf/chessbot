#include "position.h"

short Position::getX() const {
  return x;
}
short Position::getY() const {
  return y;
}
Position::Position(short px, short py) : x(px), y(py) {}

Position Position::operator+(const Position& other) const {
  return Position(getX() + other.getX(), getY() + other.getY());
}

std::string Position::toStr() const {
  return std::string("(") + std::to_string(getX()) + ","
      + std::to_string(getY()) + ")";
}
bool Position::operator==(const Position& other) const {
  return x == other.x && y == other.y;
}
