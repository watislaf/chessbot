class FEN:
    fen = None
    rules = None
    lines = None
    array = [[]]
    def whosMowe(self):
        return self.rules[1]
    def moveCount(self):
        return self.rules[5]
    def __init__(self, new_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"):
        self.fen = new_fen
        self.rules = new_fen.split(' ')

        for indx, char in reversed(list(enumerate(self.rules[0]))):
            if char.isdigit():
                self.rules[0] = self.rules[0][0:indx] + '_' * int(char) + self.rules[0][indx + 1:]
        self.lines = self.rules[0].split('/')

        self.array = [
            *map(lambda x: [
                *map(lambda y:
                    '_' if self.lines[x][y] == '_' else (
                        'w' + self.lines[x][y] if self.lines[x][y].islower()
                        else
                        'b' + self.lines[x][y].lower()),
                    range(0, 8))],
                range(0, 8))]

    def __getitem__(self, item:int):
        return self.array[item]
