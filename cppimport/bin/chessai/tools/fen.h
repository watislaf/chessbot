#ifndef FEN_H
#define FEN_H

#include <string>
#include <utility>
#include <board/piece.h>
#include <sstream>
#include <vector>

class FEN {
 public:
  explicit FEN(std::string fen_str);
  Piece getPiece(short i, short i_1);
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
