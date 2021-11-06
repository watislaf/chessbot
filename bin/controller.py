from bin.view.view import View


class Controller:
    view = View()
    white_player = None
    black_player = None
    def start_view(self):
        self.view.start()
        pass

    def start_game(self):
        pass
