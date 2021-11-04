from tools.FEN import FEN
from bin.view.window import Window


class View:
    window = None
    fen = FEN()

    def __init__(self):
        self.window = Window(self.fen)

    def start(self):
        self.window.start()
