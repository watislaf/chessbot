class CVGameController:
    is_play_as_white = False
    wait_move = 0

    def __init__(self, play_as_white):
        self.is_play_as_white = play_as_white
        if play_as_white:
            self.wait_move = 1

