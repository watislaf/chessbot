import threading
import time

from bin.player.player import Player
from bin.window import Window


class Controller:
    __window = None
    __white_player_ = None
    __black_player_ = None

    def start_window(self):
        self.__window = Window()
        threading.Thread(target=self.__window.start_loop, daemon=True).start()

    def start_game(self, str_fen: str):
        if self.__window is not None:
            self.__window.start_board_from_fen_str(str_fen)
        # self.white_player_.start_game(str_fen)
        # self.black_player.start_game(str_fen)

        while True:
            print("HARD PROCESS LALALLAL")
            #        while True:
            #            if (self.fen.whosMowe() == "w"):
            #                move = self.white_player_.get_move()
            #            else:
            #                move = self.black_player_.get_move()
            #            if (move.isNotValid()):
            #                print(self.fen.whosMowe(), " Lose")
            #                break
            #            self.white_player_.apply_move(move)
            #            self.black_player_.apply_move(move)
            # if self.__view__is_initialized :
            #     self.view.apply_move(move)
            time.sleep(1)
            pass

    def set_white_player(self, player: Player):
        self.__white_player_ = player

    def set_black_player(self, player: Player):
        self.__black_player_ = player
