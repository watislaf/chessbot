import random

import cv2

from bin.play_session_capture.chess_peace import ChessPiece
from collections import deque


class ChessBoard:
    all_pieces_found = False
    position_left_top = [0, 0]
    position_right_bottom = [None, None]
    piece_size = 0
    pieces = []
    empty_spaces = None

    def __init__(self):
        self.empty_spaces = ChessPiece("")
#        for color in {"w", "b"}:
        for color in {"b"}: #TODO Upgrade see algorythm
#            for piece in {"b", "k", "p", "q", "r", "n"}:
            for piece in {"p"}:
                self.pieces.append(ChessPiece(color + piece)),

        # self.pieces = ChessPiece("")

    def update(self, screen):
        self.empty_spaces.find(screen)

        if self.find_board_pattern():
            if self.all_pieces_found is False:
                self.all_pieces_found = True
                self.apply_board_from_positions()
            else:
                self.update_board_from_positions()
            for piece in self.pieces:
                piece.size = self.piece_size
        else:
            self.all_pieces_found = False

        if self.all_pieces_found:
            for piece in self.pieces:
                piece.find(screen)

    def write(self, screen):
        if self.all_pieces_found:
            self.empty_spaces.write(screen, (244, 99, 0))
            for piece in self.pieces:
                piece.write(screen)
        else:
            self.empty_spaces.write(screen, (0, 0, 255))

    def apply_board_from_positions(self):
        min_pos = [10000, 10000]
        max_pos = [-10000, -10000]
        for empty_pos in self.empty_spaces.positions:
            if empty_pos[0] <= min_pos[0]:
                min_pos[0] = empty_pos[0]
            if empty_pos[1] <= min_pos[1]:
                min_pos[1] = empty_pos[1]
            if empty_pos[0] >= max_pos[0]:
                max_pos[0] = empty_pos[0]
            if empty_pos[1] >= max_pos[1]:
                max_pos[1] = empty_pos[1]

        self.piece_size = (max_pos[0] - min_pos[0]) // 6
        self.position_right_bottom = (
            max_pos[0] + 19 + self.piece_size,
            max_pos[1] + 19 + self.piece_size)
        self.position_left_top = (
            min_pos[0] - self.piece_size, min_pos[1] - self.piece_size)

    def find_board_pattern(self):
        if len(self.empty_spaces.positions) == 0:
            return False
        for i in range(4):
            giv_int = random.randint(0, len(self.empty_spaces.positions) - 1)
            start = self.empty_spaces.positions[giv_int]
            green_ones = deque()  # get in
            orange_ones = deque()  # need analize
            gray_ones = list()  # all others
            size = 100000
            for pos in self.empty_spaces.positions:
                if pos == start:
                    continue
                gray_ones.append(pos)
                if abs(pos[0] - start[0]) < 5:
                    size = min(abs(pos[1] - start[1]), size)
            orange_ones.append(start)

            while len(orange_ones):
                element = orange_ones.pop()
                green_ones.append(element)
                for k in range(len(gray_ones) - 1, -1, -1):
                    if (abs(gray_ones[k][0] - element[0]) < 5 and abs(
                            abs(gray_ones[k][1] - element[1]) - size) < 10) or (
                            abs(gray_ones[k][1] - element[1]) < 5 and abs(
                        abs(gray_ones[k][0] - element[0]) - size) < 10):
                        orange_ones.append(gray_ones[k])
                        del gray_ones[k]
            if len(green_ones) == 49:
                self.empty_spaces.positions = list(green_ones)
                return True
        return False

    def update_board_from_positions(self):
        min_pos = [10000, 10000]
        max_pos = [-10000, -10000]
        for empty_pos in self.empty_spaces.positions:
            if empty_pos[0] <= min_pos[0]:
                min_pos[0] = empty_pos[0]
            if empty_pos[1] <= min_pos[1]:
                min_pos[1] = empty_pos[1]
            if empty_pos[0] >= max_pos[0]:
                max_pos[0] = empty_pos[0]
            if empty_pos[1] >= max_pos[1]:
                max_pos[1] = empty_pos[1]

        self.piece_size = (max_pos[0] - min_pos[0]) // 6
        self.position_left_top = (
            self.position_left_top[0] + min_pos[0] - self.piece_size,
            self.position_left_top[1] + min_pos[1] - self.piece_size)

        self.position_right_bottom = (
            self.position_left_top[0] +
            max_pos[0] + 24 + self.piece_size,
            self.position_left_top[1] +
            max_pos[1] + 24 + self.piece_size)
