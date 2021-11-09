class FEN:
    fen_str = None
    __rules = None
    __lines = None
    __array = [[]]

    def __init__(self, fen_str: str):
        self.fen_str = fen_str
        self.__rules = fen_str.split(' ')

        for index, char in reversed(list(enumerate(self.__rules[0]))):
            if char.isdigit():
                self.__rules[0] = \
                    self.__rules[0][0:index] + '_' * int(char) + \
                    self.__rules[0][index + 1:]

        self.__lines = self.__rules[0].split('/')

        self.__array = [
            *map(lambda x: [
                *map(lambda y:
                     '_' if self.__lines[y][x] == '_' else (
                         'w' + self.__lines[y][x] if self.__lines[y][
                             x].islower()
                         else
                         'b' + self.__lines[y][x].lower()),
                     range(0, 8))],
                 range(0, 8))]

    def getWhoIsMove(self):
        return self.__rules[1]

    def getMovesCount(self):
        return self.__rules[5]

    def isNotEmpty(self):
        return len(self.__array) != 0

    def __getitem__(self, item: int):
        return self.__array[item]

    def apply_move(self, move):
        self.__array[move.position_from[0]][move.position_from[1]], \
        self.__array[move.position_to[0]][move.position_to[1]] = \
            self.__array[move.position_to[0]][move.position_to[1]], \
            self.__array[move.position_from[0]][move.position_from[1]]
        self.__array[move.position_from[0]][move.position_from[1]] = "_"
        if self.__rules[1] == 'w':
            self.__rules[1] = 'b'
        else:
            self.__rules[1] = 'w'
        self.__rules[5] = str(int(self.__rules[5]) + 1)
