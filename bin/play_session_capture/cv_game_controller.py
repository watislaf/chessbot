class CVGameController:
    is_play_as_white = False
    is_whites_move = True
    last_move = None
    __white_board = None
    __black_board = None

    def __init__(self, play_as_white, white_board, black_board):
        self.is_play_as_white = play_as_white
        self.is_my_move = play_as_white
        self.__white_board = white_board
        self.__black_board = black_board

    def push_board(self, isWhite, new_board):
        if isWhite:
            my_matrix = self.__white_board
            self.__white_board = new_board
        else:
            my_matrix = self.__black_board
            self.__black_board = new_board

        if my_matrix is None:
            print("NO BOARD")
            return False

        if self.is_whites_move != isWhite:
            return True

        differences = []

        for i in range(8):
            for j in range(8):
                if new_board[i][j] != my_matrix[i][j]:
                    differences.append([i, j, my_matrix[i][j]])

        if len(differences) == 4:  # castle
            try:
                king_from = next(
                    filter(lambda move: move[2] == '*' and 3 <= move[0] <= 4,
                           differences))
                ruck_from = next(filter(lambda move: move[2] == '*' and (
                        move[0] == 0 or move[0] == 7), differences))
            except StopIteration:
                return False

            if ruck_from[0] == 7:
                self.last_move = [king_from[0], king_from[1]], [2, king_from[1]]
                return True
            if ruck_from[0] == 0:
                self.last_move = [king_from[0], king_from[1]], [5, king_from[1]]
                return True

            return False
        if len(differences) == 2:
            self.last_move = (next(
                filter(lambda move: move[2] == '*',
                       differences))[:-1], next(
                filter(lambda move: move[2] == '_',
                       differences))[:-1])
            return True
        return False
