import pygame


class Button:
    __size = (0, 0)
    __center = (0, 0)
    titel = "Empty"
    __color = [238, 238, 210]
    __color_fade = [208, 208, 210]
    __click = False
    __mouse_in = False
    __mouse_down = False
    __textsurface = None
    block = False

    def get(self):
        boal = self.__click
        self.__click = False
        return boal

    def __init__(self, titel, center, size):
        self.__center = center
        self.titel = titel
        self.__size = size
        pygame.font.init()  # you have to call this at the start,
        # if you want to use this module.

    def update(self, window, events):
        mouse_x, mouse_y = pygame.mouse.get_pos()
        if self.block:
            mouse_x, mouse_y = (-100, -100)
        if (abs(mouse_x - self.__center[0]) < self.__size[0] / 2 and
                abs(mouse_y - self.__center[1]) < self.__size[1] / 2):
            self.__mouse_in = True
            for event in events:
                if event.type == pygame.MOUSEBUTTONDOWN:
                    self.__mouse_down = True
                elif event.type == pygame.MOUSEBUTTONUP:
                    self.__mouse_down = False
                    self.__click = True

        else:
            self.__mouse_in = False
            self.__mouse_down = False

        if self.__mouse_down:
            color = self.__color_fade.copy()
        else:
            color = self.__color.copy()
        if self.__mouse_in:
            color[0] = color[0] - 10
            color[1] = color[1] - 10
            color[2] = color[2] - 10

        if self.block:
            color = [30, 40, 50]
        pygame.draw.rect(window, color, (self.__center[0] - self.__size[0] // 2,
                                         self.__center[1] - self.__size[1] // 2,
                                         self.__size[0], self.__size[1]))
        letter_size = 16

        myfont = pygame.font.SysFont('Comic Sans MS', 50)
        self.__textsurface = myfont.render(self.titel, False, (1, 4, 11))
        window.blit(self.__textsurface, (self.__center[0] - letter_size * len(
            self.titel) // 2, self.__center[1] - letter_size))
