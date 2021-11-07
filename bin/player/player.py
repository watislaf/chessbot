from abc import abstractmethod


class Player:
    @abstractmethod
    def do_some_cool_stuff(self):
        ''' To override '''
        pass

    def start_game(self, str_fen):
        pass
