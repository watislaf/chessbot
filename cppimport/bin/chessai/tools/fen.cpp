#include "fen.h"

FEN::FEN(std::string fen_str) {
  // rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1
  std::stringstream split_string(fen_str);
  std::string segment;
  while (std::getline(split_string, segment, ' ')) {
    rules.push_back(segment);
  }

  std::stringstream split_string_2(rules[0]);
  while (std::getline(split_string_2, segment, '/')) {
    std::string temp = "";
    for (char c: segment) {
      if (isdigit(c)) {
        for (short i = 0; i < std::stoi(std::string("") + c); i++) {
          temp.push_back('_');
        }
      } else {
        temp.push_back(c);
      }
    }
    fen_matrix_.push_back(temp);
  }

}

Piece FEN::getPiece(short i, short j) {
  auto my_piece = Piece(Position(j, 7-i));
  if (islower(fen_matrix_[i][j])) {
    my_piece.setPieceColor(PieceColor::BLACK);
  } else {
    my_piece.setPieceColor(PieceColor::WHITE);
  }
  // rnbqkbnr/pppppppp
  switch (tolower(fen_matrix_[i][j])) {
    case 'r':my_piece.setType(PieceType::tRUCK);
      break;
    case 'n':my_piece.setType(PieceType::tHORSE);
      break;
    case 'b':my_piece.setType(PieceType::tBISHOP);
      break;
    case 'q':my_piece.setType(PieceType::tQUEEN);
      break;
    case 'k':my_piece.setType(PieceType::tKING);
      break;
    case 'p':my_piece.setType(PieceType::tPONE);
      break;
  }
  return my_piece;
}

bool FEN::getIsWhiteMowe() {
  return rules[1] == "w";
}

bool FEN::getWLC() {
  return rules[2].find('Q') != std::string::npos;
}
bool FEN::getWRC() {
  return rules[2].find('K') != std::string::npos;
}
bool FEN::getBRC() {
  return rules[2].find('k') != std::string::npos;
}
bool FEN::getBLC() {
  return rules[2].find('q') != std::string::npos;
}
short FEN::getPassantX() {
  if (rules[3] == "-") {
    return -1;
  }
  return rules[3][0] - 'a';
}

short FEN::getMoveCount() {
  return std::stoi(rules[4]);
}
