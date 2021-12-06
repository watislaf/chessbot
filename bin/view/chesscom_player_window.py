import pygame


class WindowCV:
    __application_is_done = True

    def __init__(self):
        pygame.display.set_caption("bot_view")
        pygame.display.set_mode((400, 400))
        pygame.init()

    def start_loop(self):
        while self.__application_is_done:
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    self.__application_is_done = False

            pygame.display.update()
            pygame.time.delay(30)
        pygame.quit()
