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
  Board(const Board& board);
  std::string getFen();
  void apply(const Move&);
  void unApply(const Move&);

  int getMoveCount() const;

  void setMoveCount(int moveCount);
  std::shared_ptr<const Piece> getPiece(const Position& position) const;
  const std::list<std::shared_ptr<const Piece>>& getActivePieceList(bool is_white) const;
  bool LcIsPossible(bool is_white) const;
  bool RcIsPossible(bool is_white) const;
  void setLCIsPossible(bool is_white, bool brake);
  void setRCIsPossible(bool is_white, bool brake);

  Position getKingPosition(bool is_white) const;
  bool isWhiteMove() const;

  bool isBlackMove() const;
  int getLastPassantX() const;
  std::string toStr() const;

  bool operator==(const Board& other) const;
  bool operator!=(const Board& other) const;

  void setPiece(const Piece& piece_template_object);

 private:
  void forceMove(const std::shared_ptr<const Piece>& piece_from,
                 const std::shared_ptr<const Piece>& piece_to);

  std::vector<std::vector<std::shared_ptr<const Piece>>> board_ =
      std::vector<std::vector<std::shared_ptr<const Piece>>>
          (8, std::vector<std::shared_ptr<const Piece>>(8));
  bool is_white_move_ = true;
  int last_passant_x_ = 0;
  struct Castle {
    Position king_position = {0, 0};
    bool LC_is_possible_ = true;
    bool RC_is_possible_ = true;
  } whiteCastle, blackCastle;
  std::list<std::shared_ptr<const Piece>> active_white_pieces_;
  std::list<std::shared_ptr<const Piece>> active_black_pieces_;
  int move_count_ = 0;
};

#endif // CHESS_BOARD_H
