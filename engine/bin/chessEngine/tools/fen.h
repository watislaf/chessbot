#ifndef FEN_H
#define FEN_H

#include <string>
#include <utility>
#include <objBoard/piece.h>
#include <sstream>
#include <vector>

class FEN {
 public:
  explicit FEN(std::string fen_str="k7/p3pp2/1p6/2p5/4nP1/3PP3/PPP5/7K");


  Piece getPiece(short i, short j);
  bool getIsWhiteMowe();

  bool getWLC();
  bool getWRC();
  bool getBRC();
  bool getBLC();
  short getPassantX();
  short getMoveCount();
 private:
  std::vector<std::string> rules;
  std::vector<std::string> fen_matrix_;
};

#endif //FEN_H
