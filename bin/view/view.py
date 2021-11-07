import threading

from bin.view.window import Window


class View:
    window_ = None
    view_thread_ = None

    def __init__(self):
        self.window_ = Window()

    def start(self):
        self.view_thread_ = threading.Thread(target=self.window_.start)
        self.view_thread_.start()
        return True

    def set_board(self, str_fen):
        self.window_.set_board(str_fen)
