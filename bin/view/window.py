import threading

import pygame
from pygame.surface import Surface

from bin.tools.FEN import FEN
from cppimport.output.chessai import ChessAi, Move
from graphics.pictures import PICTURES_PATH


class Window:
    __chessAi = ChessAi()

    __black_board_color = (118, 150, 86)
    __white_board_color = (238, 238, 210)
    __active_color = (123, 12, 54)

    __size = 400
    __cell_size = __size // 8
    __window = None
    __fen = None
    __done = True
    __pieces = {}

    __is_dragging = False
    __position_dragging = (0, 0)

    __mouse_x = 0
    __mouse_y = 0

    def set_board(self, str_fen):
        self.__fen = FEN(str_fen)
        self.__chessAi.start_new_game(self.__fen.fen)

    def __init__(self):
        pygame.display.set_caption("CHESS")
        pygame.init()
        self.__window = pygame.display.set_mode((self.__size, self.__size))
        for symbol in 'rnbqkp':
            for color in 'wb':
                self.__pieces[str(color + symbol)] = pygame.transform.scale(
                    pygame.image.load(
                        '{}/{}{}.png'.format(PICTURES_PATH, color, symbol)),
                    (self.__cell_size, self.__cell_size))

    def start(self):
        while self.__done:
            for event in pygame.event.get():
                self.__mouse_x, self.__mouse_y = pygame.mouse.get_pos()
                if event.type == pygame.QUIT:
                    self.quit()
                elif event.type == pygame.MOUSEBUTTONDOWN:
                    self.drag_start(event)
                elif event.type == pygame.MOUSEBUTTONUP:
                    self.drag_end(event)
            self.__draw_board()
            pygame.display.update()
            pygame.time.delay(50)
        pygame.quit()

    def quit(self):
        self.__done = False

    def drag_start(self, event):
        if not self.__fen:
            return
        if event.button == 1:
                position_x, position_y = event.pos
                self.__position_dragging = (
                    position_x // self.__cell_size,
                    7 - position_y // self.__cell_size)

                if self.__fen[self.__position_dragging[0]][
                    self.__position_dragging[1]] != '_':
                    self.__is_dragging = True
                    self.__possible_moves = self.__chessAi.get_possible_moves(
                        self.__position_dragging[0], self.__position_dragging[1])

    def drag_end(self, event):
        self.__is_dragging = False

    def __draw_board(self):
        for x in range(0, 8):
            for y in range(0, 8):
                self.__draw_rect(((x + y) % 2 == 1), x,
                                 y)
        if not self.__fen:
            return
        for x in range(0, 8):
            for y in range(0, 8):
                position_x = x * self.__cell_size
                position_y = (7 - y) * self.__cell_size

                if self.__is_dragging and (x, y) == self.__position_dragging:
                    self.__draw_pieces(
                        self.__pieces.get(self.__fen[x][y], None),
                        self.__mouse_x - self.__cell_size // 2,
                        self.__mouse_y - self.__cell_size // 2)
                else:
                    self.__draw_pieces(
                        self.__pieces.get(self.__fen[x][y], None),
                        position_x, position_y)

    def __draw_rect(self, is_black: bool, x: int, y: int):
        position_x = x * self.__cell_size
        position_y = y * self.__cell_size
        if is_black:
            color = self.__black_board_color
        else:
            color = self.__white_board_color
        if self.__is_dragging:
            for move in self.__possible_moves:
                if move.position_to == (x, 7 - y):
                    color = self.__active_color

        pygame.draw.rect(self.__window, color, (
            position_x, position_y, self.__cell_size, self.__cell_size))

    def __draw_pieces(self, image: Surface, position_x: int, position_y: int):
        if image is None:
            return
        self.__window.blit(image, (position_x, position_y))
