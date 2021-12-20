import pygame

from bin.view.button import Button


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
        self.__button = Button("Start", (200, 200), (200, 80))

        self.__myfont = pygame.font.SysFont('Comic Sans MS', 50)
        self.__no_board_surface = self.__myfont.render("Can't find objBoard", True,
                                                       (155, 42, 42))
        self.__no_button_surface = self.__myfont.render(
            "Can't find Play button", True, (155, 42, 42))

    def start_loop(self):
        while self.__application_is_done:
            pygame.draw.rect(self.__py_window, (118, 150, 86), (0, 0, 400, 400))
            self.__button.update(self.__py_window, pygame.event.get())
            if self.__button.get():
                self.__atomic_data.is_start = not self.__atomic_data.is_start

            self.__start = self.__atomic_data.is_start
            if self.__start:
                self.__button.titel = "Stop"
            else:
                self.__button.titel = "Start"

            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    self.__application_is_done = False
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
                    self.__atomic_data.counter -= 0.70
                    self.__button.block = True

            pygame.display.update()
            pygame.time.delay(70)
        pygame.quit()
