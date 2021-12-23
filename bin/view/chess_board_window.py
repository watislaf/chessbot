"""
 Creates window with chess on it, also implements ChessAi
"""

import operator
import os
import threading

import pygame
from pygame.surface import Surface
from bin.tools.FEN import FEN
from bin.tools.chess_exception import ChessException
from bin.tools.pieceMove import PieceMove
from bin.view.piece_ask import PieceAsk
from engine.output.chessengine import ChessEngine
from resources.pictures import RESOURCES_PATH


class ChessBoardWindow:
    __chessAi = ChessEngine("random")

    __BLACK_BOARD_COLOR: tuple = (118, 150, 86)
    __WHITE_BOARD_COLOR: tuple = (238, 238, 210)
    __ACTIVE_COLOR: tuple = (10, 10, -80)

    __WINDOW_SIZE: int = 800
    __CELL_SIZE: float = __WINDOW_SIZE // 8

    __py_window: pygame.Surface = None
    __application_is_done: bool = True
    __is_dragging: bool = False

    __pos_piece_dragging: tuple = (0, 0)
    __possible_moves: list = None
    __pieces_img: dict = {}
    __fen: FEN = None
    __mouse_x: int = 0
    __mouse_y: int = 0
    last_move: list = [PieceMove().getInvalid()]

    def get_window_board_str(self):
        """
        :return:  string representation of current board
        """
        return self.__fen.getBoardStr()

    def start_new_game_from_fen_str(self, str_fen: str):
        try:
            self.__fen = FEN(str_fen)
        except IndexError:
            raise ChessException("FEN IS WRONG")

        self.__chessAi.start_new_game(self.__fen.fen_str)
        self.__is_dragging = False
        self.last_move[0] = PieceMove().getInvalid()

    def __init__(self, window_event_obj: threading.Event):
        """
        :param window_event_obj: is used to tell controller that user
        ended the move
        """
        self.__window_event_obj = window_event_obj
        pygame.display.set_caption("CHESS")
        pygame.init()
        programIcon = pygame.image.load('{}/wq.png'.format(RESOURCES_PATH))
        pygame.display.set_icon(programIcon)

        self.__py_window = pygame.display.set_mode(
            (self.__WINDOW_SIZE, self.__WINDOW_SIZE))
        for symbol in 'rnbqkp':
            for color in 'wb':
                self.__pieces_img[str(color + symbol)] = pygame.transform.scale(
                    pygame.image.load(
                        '{}/{}{}.png'.format(RESOURCES_PATH, color, symbol)),
                    (self.__CELL_SIZE, self.__CELL_SIZE))

    def start_loop(self):
        while self.__application_is_done:
            for event in pygame.event.get():
                self.__mouse_x, self.__mouse_y = pygame.mouse.get_pos()
                if event.type == pygame.QUIT:
                    self.__application_is_done = False
                elif event.type == pygame.MOUSEBUTTONDOWN:
                    self.start_dragging(event)
                elif event.type == pygame.MOUSEBUTTONUP:
                    self.end_dragging(event)
            self.__draw_board()
            pygame.display.update()
            pygame.time.delay(30)
        pygame.quit()
        print("See you soon")
        os._exit(1)

    def start_dragging(self, event):
        if not self.__fen:
            return
        if event.button != 1:
            return
        position_x, position_y = event.pos
        self.__pos_piece_dragging = (position_x // self.__CELL_SIZE,
                                     7 - position_y // self.__CELL_SIZE)
        piece = self.__fen[self.__pos_piece_dragging[0]][
            self.__pos_piece_dragging[1]]

        if piece != '_':
            self.__is_dragging = True
            self.__possible_moves = \
                self.__chessAi.get_possible_moves_for_position(
                    self.__pos_piece_dragging[0],
                    self.__pos_piece_dragging[1])

    def apply_move(self, move):
        self.__chessAi.apply_move(move)
        self.__fen.apply_move(move)
        if not self.__chessAi.is_move_exists():
            print("NOT EXIST!")
            self.__window_event_obj.set()

    def end_dragging(self, event):

        self.__is_dragging = False

        if event.button != 1 or self.__possible_moves is None:
            return
        position_x, position_y = event.pos
        self.__pos_piece_dragging = (position_x // self.__CELL_SIZE,
                                     7 - position_y // self.__CELL_SIZE)
        for move in self.__possible_moves:
            if move.position_to == self.__pos_piece_dragging:
                if self.__fen[move.position_from[0]][
                    move.position_from[1]][0] == self.__fen.getWhoIsMove():
                    if self.__fen[move.position_from[0]][move.position_from[1]][
                        1] == "p" and (move.position_to[1] % 7 == 0):
                        pieceType = PieceAsk().Ask(self.__WINDOW_SIZE,
                                                   self.__py_window)
                        if pieceType == "exit":
                            self.__application_is_done = False
                        if pieceType == "_":
                            return
                        move.new_piece = pieceType
                    self.last_move[0] = move
                    self.__window_event_obj.set()
                    pygame.time.delay(50)
                    return

    def __draw_board(self):
        for x in range(8):
            for y in range(8):
                self.__draw_rect(((x + y) % 2 == 1), x, y)
        if not self.__fen:
            return

        for x in range(8):
            for y in range(8):
                position_x = x * self.__CELL_SIZE
                position_y = (7 - y) * self.__CELL_SIZE

                if self.__is_dragging and (x, y) == self.__pos_piece_dragging:
                    self.__draw_pieces(
                        self.__pieces_img.get(self.__fen[x][y], None),
                        self.__mouse_x - self.__CELL_SIZE // 2,
                        self.__mouse_y - self.__CELL_SIZE // 2)
                else:
                    self.__draw_pieces(
                        self.__pieces_img.get(self.__fen[x][y], None),
                        position_x, position_y)

    def __draw_rect(self, is_black: bool, x: int, y: int):
        position_x = x * self.__CELL_SIZE
        position_y = y * self.__CELL_SIZE
        if is_black:
            color = self.__BLACK_BOARD_COLOR
        else:
            color = self.__WHITE_BOARD_COLOR
        if self.__is_dragging:
            for move in self.__possible_moves:
                if move.position_to == (x, 7 - y):
                    color = tuple(map(operator.add, color, self.__ACTIVE_COLOR))
                    break

        pygame.draw.rect(self.__py_window, color, (
            position_x, position_y, self.__CELL_SIZE, self.__CELL_SIZE))

    def __draw_pieces(self, image: Surface, position_x: int, position_y: int):
        if image is None:
            return
        self.__py_window.blit(image, (position_x, position_y))
