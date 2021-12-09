import pygame

from bin.view.button import Button


class WindowCV:
    __application_is_done = True
    __py_window = None
    __button = None
    __start = True

    def __init__(self):
        self.__py_window = pygame.display.set_mode((400, 400))
        pygame.display.set_caption("bot_view")
        pygame.init()
        self.__button = Button("Start", (200, 200), (200, 80))

    def start_loop(self):
        while self.__application_is_done:
            pygame.draw.rect(self.__py_window, (118, 150, 86), (0, 0, 400, 400))
            self.__button.update(self.__py_window, pygame.event.get())
            if (self.__button.get()):
                self.__start = not self.__start
                if(self.__start):
                    self.__button.titel = "Stop"
                else:
                    self.__button.titel = "Start"

            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    self.__application_is_done = False

            pygame.display.update()
            pygame.time.delay(30)
        pygame.quit()
