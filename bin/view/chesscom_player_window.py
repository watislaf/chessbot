import os
from os import environ

environ['PYGAME_HIDE_SUPPORT_PROMPT'] = '1'

import pygame

from bin.view.button import Button
from resources.pictures import RESOURCES_PATH


class WindowCV:
    __application_is_done = True
    __py_window = None
    __button = None
    __start = False
    __atomic_data = None

    def __init__(self, atomic_data):
        self.__atomic_data = atomic_data
        self.__py_window = pygame.display.set_mode((400, 400))
        pygame.display.set_caption("bot_view")
        pygame.init()
        programIcon = pygame.image.load('{}/wq.png'.format(RESOURCES_PATH))
        pygame.display.set_icon(programIcon)

        self.__button = Button("Start", (200, 200), (200, 80))

        self.__myfont = pygame.font.SysFont('Comic Sans MS', 50)
        self.__no_board_surface = self.__myfont.render("Can't find Board",
                                                       True,
                                                       (155, 42, 42))
        self.__no_button_surface = self.__myfont.render(
            "Can't find Play button", True, (155, 32, 42))

    def start_loop(self):
        while self.__application_is_done:
            pygame.draw.rect(self.__py_window, (118, 150, 86), (0, 0, 400, 400))
            events = pygame.event.get()
            for event in events:
                if event.type == pygame.QUIT:
                    self.__application_is_done = False
            self.__button.update(self.__py_window, events)
            if self.__button.get():
                self.__atomic_data.is_start = not self.__atomic_data.is_start

            self.__start = self.__atomic_data.is_start
            if self.__start:
                self.__button.titel = "Stop"
            else:
                self.__button.titel = "Start"

            self.__button.block = False
            if self.__atomic_data.cant_find_go_button:
                self.__button.block = True
                self.__py_window.blit(self.__no_button_surface, (13, 255))

            if self.__atomic_data.cant_find_board:
                self.__button.block = True
                self.__py_window.blit(self.__no_board_surface, (54, 288))

            if self.__button.block:
                self.__atomic_data.counter = 4

            if self.__atomic_data.counter > 0:
                self.__timer_surface = self.__myfont.render(
                    str(self.__atomic_data.counter)[0:4], True, (155, 42, 42))
                self.__py_window.blit(self.__timer_surface, (24, 188))

                if not self.__button.block:
                    self.__atomic_data.counter -= 0.30
                    self.__button.block = True

            pygame.display.update()
            pygame.time.delay(70)
        pygame.quit()
        print("see you soon")
        os._exit(1)
