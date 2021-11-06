#ifndef PRICER_H
#define PRICER_H

#include <board/piece.h>
class Pricer {
 public:
  static int GetPrice(const Piece& piece);
};

#endif //PRICER_H
