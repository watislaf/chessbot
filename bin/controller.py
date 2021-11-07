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

    def __init__(self,
                 str_fen="rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"):
        self.fen = FEN(str_fen)
        self.view = View(FEN(str_fen))

    def start_view(self):
        self.view.start()
        pass

    def start_game(self):
        while True:
            print("HARD PROCESS LALALLAL")
            time.sleep(1)
            pass

    #        self.while_player_.start_prcess()
    #        self.black_player_.start_prcess()
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

    def set_white_payer(self, player: Player):
        self.white_player_ = player

    def set_black_payer(self, player: Player):
        self.black_player_ = player
