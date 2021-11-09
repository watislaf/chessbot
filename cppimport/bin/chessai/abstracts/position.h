#ifndef POSITION_H
#define POSITION_H

#include <string>
class Position {

 public:
  Position(int px, int py);

  std::string toStr() const;

  int getX() const;
  int getY() const;

  Position operator+(const Position& other) const;
  bool operator==(const Position& other) const;

 private:
  int x;
  int y;
};

#endif //POSITION_H
