#include "position.h"

int Position::getX() const {
  return x;
}
int Position::getY() const {
  return y;
}
Position::Position(int px, int py) : x(px), y(py) {}

Position Position::operator+(const Position& other) const {
  return {getX() + other.getX(), getY() + other.getY()};
}

std::string Position::toStr() const {
  return std::string("(") + std::to_string(getX()) + ","
      + std::to_string(getY()) + ")";
}
bool Position::operator==(const Position& other) const {
  return x == other.x && y == other.y;
}
