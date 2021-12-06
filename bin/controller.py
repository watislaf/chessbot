import threading
import time

from bin.player.player import Player
from bin.view.window import Window


class Controller:
    window = None
    window_event_obj = None
    __first_player = None
    __second_player = None
    __window_is_inicialized = False

    def start_window(self):
        self.window_event_obj = threading.Event()
        self.window = Window(self.window_event_obj)
        threading.Thread(target=self.window.start_loop).start()
        self.__window_is_inicialized = True

    def start_game(self, str_fen: str):
        if self.window is not None:
            self.window.start_board_from_fen_str(str_fen)

        self.__first_player.start_game(str_fen)
        self.__second_player.start_game(str_fen)
        if self.__first_player.is_my_move is None and self.__second_player.is_my_move is None:
            self.__first_player.is_my_move = True
            self.__second_player.is_my_move = False
        else:
            if self.__first_player.is_my_move is None:
                self.__first_player.is_my_move = not self.__second_player.is_my_move
            else:
                self.__second_player.is_my_move = not self.__first_player.is_my_move

        while True:
            if self.__first_player.is_my_move:
                move = self.__first_player.get_move()
            else:
                move = self.__second_player.get_move()
            if move.isInvalid():
                self.__first_player.is_my_move = None
                self.__second_player.is_my_move = None
                if move.new_piece != "k":
                    return "DRAW"
                else:
                    if self.__first_player.is_my_move:
                        return "FIRST PLAYER LOSE"
                    else:
                        return "FIRST PLAYER WIN"

            self.__first_player.apply_move(move)
            self.__second_player.apply_move(move)
            time.sleep(0.05)

            if self.__window_is_inicialized:
                self.window.apply_move(move)
                if self.__first_player.get_board_str().replace(" ", "") != \
                        self.__second_player.get_board_str().replace(" ", ""):
                    print("DIFFERENT BOARDS BETWEEN PLAYERS")
                    print(self.__first_player.get_board_str())
                    print(self.__second_player.get_board_str())

    def set_players(self, first_player, second_player):
        self.__first_player = first_player
        self.__second_player = second_player
