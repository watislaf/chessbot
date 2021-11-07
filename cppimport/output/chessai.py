import os, sys
import re
from ast import literal_eval as make_tuple

import CPPchessai


class Move:
    position_from = (0, 0)
    position_to = (0, 0)

    def __init__(self, *args):
        if len(args) == 1:
            my_tur = re.findall(r'[(][0-9],[0-9][)]', args[0][1:-1])
            self.position_from = make_tuple(my_tur[0])
            self.position_to = make_tuple(my_tur[1])
        else:
            self.position_from = args[0]
            self.position_to = args[1]

    def __str__(self):
        return "(({},{}),({},{}))".format(self.position_from[0],
                                          self.position_from[1],
                                          self.position_to[0],
                                          self.position_to[1])


class ChessAi:
    wrappedChessAi = None

    def __init__(self):
        self.wrappedChessAi = CPPchessai.ChessAi()

    def start_new_game(self,
                       fen: str = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"):
        self.wrappedChessAi.startNewGame(fen)

    def get_possible_moves(self, x: int, y: int):
        moves_str = self.wrappedChessAi.getPossibleMovesForPiece(x, y)
        if len(moves_str) == 0:
            moves_str = moves_str.split(" ")[:-1]
        return tuple(map(lambda move_str: Move(move_str), moves_str))
