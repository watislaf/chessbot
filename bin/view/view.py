import threading

from bin.view.window import Window


class View:
    window_ = None
    view_thread_ = None
    def __init__(self, fen):
        self.window_ = Window(fen)

    def start(self):
        self.window_.start()
        #self.view_thread_ = threading.Thread(target=self.window_.start)
        #self.view_thread_.start()
