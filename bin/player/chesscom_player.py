import threading

from bin.play_session_capture.open_cv_controller import OpenCvController
from bin.player.player import Player
from bin.view.chesscom_player_window import WindowCV


class ChesscomPlayer(Player):

    def __init__(self):
        self.controller = OpenCvController()
        self.window = WindowCV()
        threading.Thread(target=self.window.start_loop).start()

    def start_game(self, str_fen: str):
        pass
