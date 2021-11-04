#include "move.h"

Move::Move(const Position& from, const Position& to, int price)
    : start_(from), end_(to), price_of_end_piece_(price) {}

Move::Move(const Piece& from, const Position& to, int price)
    : start_(from.getPosition()), end_(to), price_of_end_piece_(price) {}

