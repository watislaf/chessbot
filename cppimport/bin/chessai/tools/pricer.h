#ifndef PRICER_H
#define PRICER_H

#include "memory"
#include <board/piece.h>

class Pricer {
 public:
  static int getPrice(const std::shared_ptr<const Piece>& piece) ;

};

#endif //PRICER_H
