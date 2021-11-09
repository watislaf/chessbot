import threading
import time

from bin.player.player import Player
from bin.window import Window


class Controller:
    window = None
    window_event_obj = None
    __white_player = None
    __black_player = None
    __window_is_inicialized = False

    def __init__(self):
        self.window_event_obj = threading.Event()

    def start_window(self):
        self.window = Window(self.window_event_obj)
        threading.Thread(target=self.window.start_loop,
                         daemon=True).start()
        self.__window_is_inicialized = True

    def start_game(self, str_fen: str):
        if self.window is not None:
            self.window.start_board_from_fen_str(str_fen)
        self.__white_player.start_game(str_fen)
        self.__black_player.start_game(str_fen)

        while True:
            if self.__white_player.whosMowe() == "w":
                move = self.__white_player.get_move()
            else:
                move = self.__black_player.get_move()
            #            if move.isNotValid():
            #                print(self.__white_player.whosMowe(), " Lose")
            #                break
            self.__white_player.apply_move(move)
            self.__black_player.apply_move(move)
            if self.__window_is_inicialized:
                self.window.apply_move(move)

    def set_white_player(self, player: Player):
        self.__white_player = player

    def set_black_player(self, player: Player):
        self.__black_player = player
