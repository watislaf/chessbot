#include "position.h"

void Position::SetX(int x) {
  Position::x = x;
}
void Position::SetY(int y) {
  Position::y = y;
}
int Position::GetX() const {
  return x;
}
int Position::GetY() const {
  return y;
}
Position::Position(int px, int py) : x(px), y(py) {}

Position Position::operator+(const Position& other) {
  return {GetX() + other.GetX(), GetY() + other.GetY()};
}
