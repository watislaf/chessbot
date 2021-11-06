#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H

#include "piece.h"
#include <vector>
#include <list>
#include <memory>
#include "tools/fen.h"
#include "abstracts/move.h"

class Board {

 public:
  explicit Board(FEN fen);
  std::string getFen();
  void apply(const Move&);
  void unApply(const Move&);

  std::shared_ptr<Piece> getPiece(const Position& position);
  // Castle
  bool isLcAvailable(const Piece& piece) const;
  bool isRcAvailable(const Piece& piece) const;
  void setBrakeLc(const Piece& piece, bool brake);
  void setBrakeRc(const Piece& piece, bool brake);
  Position getKingPosition(const Piece& piece) const;
  bool isWhiteMove() const;

  bool isBlackMove() const;
  void nextMove();
  int getPrevLongPonMove() const;
  void setPrevLongPonMove(int prev_long_pon_move);
  int getLastPassantX() const;
  std::string toStr() const;
 private:

  void hardMove(std::shared_ptr<Piece> piece,
                std::shared_ptr<Piece> piece_1);

  std::vector<std::vector<std::shared_ptr<Piece>>> board_ =
      std::vector<std::vector<std::shared_ptr<Piece>>>(8,
                                                       std::vector<std::shared_ptr<
                                                           Piece>>(8));
  std::list<std::shared_ptr<Piece>> active_pieces;
  bool is_white_move_ = true;
  int last_passant_x_ = 0;
  struct Castle {
    Position king_position = {0, 0};
    bool LC_ = true;
    bool RC_ = true;
  } whiteCastle, blackCastle;
  int move_count_ = 0;
};

#endif // CHESS_BOARD_H
