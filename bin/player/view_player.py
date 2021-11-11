import time

from bin.player.player import Player
from bin.tools.FEN import FEN
from bin.tools.pieceMove import PieceMove


class ViewPlayer(Player):
    __pointer_to_last_move = None
    whos_move = None

    def __init__(self, pointer_to_last_move, event_obj, get_window_board_str):
        self.__pointer_to_last_move = pointer_to_last_move
        self.__event_obj = event_obj
        self.__get_window_board_str = get_window_board_str

    def get_board_str(self):
        return self.__get_window_board_str()

    def start_game(self, str_fen: str):
        fen = FEN(str_fen)
        self.whos_move = fen.getWhoIsMove()

    def whosMowe(self):
        return self.whos_move

    def get_move(self):
        if self.__pointer_to_last_move[0] == (-1, -1):
            self.__event_obj.wait(timeout=None)
        if self.__pointer_to_last_move[0] == (-1, -1):
            move = PieceMove((-1, -1), (-1, -1))
        else:
            move = self.__pointer_to_last_move[0]
        self.__pointer_to_last_move[0] = (-1, -1)
        self.__event_obj.clear()

        return move

    def apply_move(self, move):
        if self.whos_move == "w":
            self.whos_move = "b"
        else:
            self.whos_move = "w"
