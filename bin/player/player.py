from abc import abstractmethod


class Player:
    @abstractmethod
    def start_game(self, str_fen: str):
        pass

    @abstractmethod
    def whosMowe(self):
        pass

    @abstractmethod
    def apply_move(self, move):
        pass

    @abstractmethod
    def get_move(self):
        pass

    @abstractmethod
    def get_board_str(self):
        pass
