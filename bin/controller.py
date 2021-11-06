from bin.Player.player import Player
from bin.view.view import View


class Controller:
    view = View()
    white_player_ = None
    black_player_ = None

    def start_view(self):
        self.view.start()
        pass

    def start_game(self):
        pass

    def set_white_payer(self, player: Player):
        self.white_player_ = player

    def set_black_payer(self, player: Player):
        self.black_player_ = player
