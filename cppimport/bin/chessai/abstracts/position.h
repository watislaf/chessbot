#ifndef POSITION_H
#define POSITION_H

class Position {

 public:
  Position(int px,int py);

  void SetX(int x);
  void SetY(int y);
  int GetX() const;
  int GetY() const;

 private:
  int x;
  int y;
};

#endif //POSITION_H
