from bin.player.player import Player


class AiPlayer(Player):
    __chess_ai = None

    def __init__(self, chess_ai):
        self.__chess_ai = chess_ai

    def start_game(self, str_fen: str):
        self.__chess_ai.start_new_game(str_fen)

    def apply_move(self, move):
        self.is_my_move = not self.is_my_move
        self.__chess_ai.apply_move(move)

    def get_move(self):
        return self.__chess_ai.get_best_move()

    def get_board_str(self):
        return self.__chess_ai.get_board_str()
