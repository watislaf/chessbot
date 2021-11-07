import time
from concurrent.futures import thread

from bin.player.player import Player
from bin.tools.FEN import FEN
from bin.view.view import View


class Controller:
    fen = None
    view = None
    white_player_ = None
    black_player_ = None
    __view__is_initialized = False

    def __init__(self):
        self.view = View()

    def start_view(self):
        self.__view__is_initialized = self.view.start()
        pass

    def start_game(self, str_fen: str):
        if self.__view__is_initialized :
            self.view.set_board(str_fen)
        #self.white_player_.start_game(str_fen)
        #self.black_player.start_game(str_fen)

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

    def set_white_payer(self, player: Player):
        self.white_player_ = player

    def set_black_payer(self, player: Player):
        self.black_player_ = player
