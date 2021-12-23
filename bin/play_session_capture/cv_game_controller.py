from copy import copy

from bin.tools.pieceMove import PieceMove


class CVGameController:
    is_play_as_white = False
    is_whites_move = True
    __last_move = None
    white_board = None
    black_board = None
    pawns_board = None

    def __init__(self, play_as_white):
        self.is_play_as_white = play_as_white
        self.is_my_move = play_as_white
        if play_as_white:
            self.white_board = [['*'] * 2 + ['_'] * 6] * 8
            self.black_board = [['_'] * 6 + ['*'] * 2] * 8
            self.pawns_board = [['_'] * 6 + ['*'] + ['_']] * 8
        else:
            self.black_board = [['*'] * 2 + ['_'] * 2] * 8
            self.white_board = [['_'] * 6 + ['*'] * 2] * 8
            self.pawns_board = [['_'] * 6 + ['*'] + ['_']] * 8

    def push_board(self, new_board):
        if not self.is_play_as_white:
            my_matrix = self.white_board
            self.white_board = new_board
        else:
            my_matrix = self.black_board
            self.black_board = new_board

        if my_matrix is None:
            print("NO BOARD")
            return False

        differences = []

        for i in range(8):
            for j in range(8):
                if new_board[i][j] != my_matrix[i][j]:
                    differences.append(
                        [self.get_index(i), self.get_index(j), my_matrix[i][j]])

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
                short_castle = 6
                self.__last_move = [king_from[0], king_from[1]], [short_castle,
                                                                  king_from[1]]

                return True
            if ruck_from[0] == 0:
                long_castle = 2
                self.__last_move = [king_from[0], king_from[1]], [long_castle,
                                                                  king_from[1]]

                return True

            return False
        if len(differences) == 2:
            self.__last_move = (next(
                filter(lambda move: move[2] == '*',
                       differences))[:-1], next(
                filter(lambda move: move[2] == '_',
                       differences))[:-1])
            self.is_last_move_was_pawn_promotion()
            return True
        return False

    def get_last_move(self):
        if self.__last_move is None:
            return PieceMove().getInvalid()

        return PieceMove(*self.__last_move)

    def defeat(self):
        self.__last_move = None

    def get_enemy_color_board(self):
        if not self.is_play_as_white:
            my_matrix = self.white_board
        else:
            my_matrix = self.black_board
        return my_matrix

    def is_last_move_was_pawn_promotion(self):
        if self.__last_move is None:
            return False
        start_i, start_j = self.__last_move[0]
        start_i, start_j = self.get_index(start_i), \
                           self.get_index(start_j)
        end_i, end_j = self.__last_move[0]
        end_i, end_j = self.get_index(end_i), \
                       self.get_index(end_j)

        if self.pawns_board[start_i][start_j] == "*":
            self.pawns_board[start_i][start_j] = "_"
            if end_j == 8:
                return True
            self.pawns_board[end_i][end_j] = "*"
        return False

    def get_index(self, i):
        ki = i
        if not (self.is_play_as_white):
            ki = 7 - i
        return ki

    def get_pawn_board(self):
        return self.pawns_board
