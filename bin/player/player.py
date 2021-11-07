from abc import abstractmethod


class Player:
    @abstractmethod
    def start_game(self, str_fen: str):
        pass
