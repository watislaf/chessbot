class FEN:
    fen = None
    rules = None
    lines = None
    array = [[]]

    def whosMowe(self):
        return self.rules[1]

    def moveCount(self):
        return self.rules[5]

    def isInit(self):
        return len(self.array) != 0

    def __init__(self, new_fen):
        self.fen = new_fen
        self.rules = new_fen.split(' ')

        for indx, char in reversed(list(enumerate(self.rules[0]))):
            if char.isdigit():
                self.rules[0] = self.rules[0][0:indx] + '_' * int(char) + \
                                self.rules[0][indx + 1:]
        self.lines = self.rules[0].split('/')

        self.array = [
            *map(lambda x: [
                *map(lambda y:
                     '_' if self.lines[y][x] == '_' else (
                         'w' + self.lines[y][x] if self.lines[y][x].islower()
                         else
                         'b' + self.lines[y][x].lower()),
                     range(0, 8))],
                 range(0, 8))]

    def __getitem__(self, item: int):
        return self.array[item]
