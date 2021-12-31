#ifndef POSITION_H
#define POSITION_H

#include <string>
class Position {

 public:
  Position(short px, short py);

  Position(std::pair<uint8_t, uint8_t> pair_1);
  std::string toStr() const;

  short getX() const;
  short  getY() const;

  Position operator+(const Position& other) const;
  bool operator==(const Position& other) const;

 private:
  short x;
  short y;
};

#endif //POSITION_H
