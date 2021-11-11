import pygame

from graphics.pictures import PICTURES_PATH


class PieceAsk():
    __pieces_img = {}
    __rect_size = (200, 50)


    def Ask(self, wondow_size, py_window):
        self.__rect_size  = (wondow_size/2, wondow_size/8)
        for symbol in 'rnbq':
            self.__pieces_img[str(symbol)] = pygame.transform.scale(
                pygame.image.load(
                    '{}/{}{}.png'.format(PICTURES_PATH, "w", symbol)),
                (self.__rect_size[0] / 4, self.__rect_size[0] / 4))
        while True:
            for event in pygame.event.get():
                if event.type == pygame.MOUSEBUTTONUP:
                    __mouse_x, __mouse_y = pygame.mouse.get_pos()
                    __mouse_x -= wondow_size / 2 - self.__rect_size[0] / 2
                    __mouse_y -= wondow_size / 2 - self.__rect_size[1] / 2
                    __mouse_x //= self.__rect_size[0] / 4
                    __mouse_y //= self.__rect_size[0] / 4
                    if __mouse_y != 0:
                        return "_"
                    return {0: "r", 1: "n", 2: "b", 3: "q"}.get(__mouse_x, "_")

                if event.type == pygame.QUIT:
                    return "exit"
                # elif event.type == pygame.MOUSEBUTTONDOWN:
                #    self.start_dragging(event)

            pygame.draw.rect(py_window, (99, 129, 189), (
                wondow_size / 2 - self.__rect_size[0] / 2,
                wondow_size / 2 - self.__rect_size[1] / 2, self.__rect_size[0],
                self.__rect_size[1]))
            x = 0
            for symbol in 'rnbq':
                py_window.blit(self.__pieces_img.get(symbol, None),
                               (wondow_size / 2 - self.__rect_size[0] / 2 + x,
                                wondow_size / 2 - self.__rect_size[1] / 2))
                x += self.__rect_size[0] / 4
            pygame.display.update()
            pygame.time.delay(30)
