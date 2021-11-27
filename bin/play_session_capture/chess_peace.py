import cv2
import numpy as np
from graphics.pictures import PICTURES_PATH


class ChessPiece():
    image = [None, None]  # white and black version
    name = "str"
    size = 52
    __size = 0
    threshold = 0.57
    positions = None
    mode = cv2.TM_CCOEFF_NORMED

    @property
    def size(self):
        return self.__size

    @size.setter
    def size(self, new_size):
        if (new_size == self.__size):
            return
        self.__size = new_size
        if (self.name == ""):
            prototype = np.zeros((self.size, self.size, 3), dtype=np.uint8)
        else:
            prototype = cv2.imread(PICTURES_PATH + "/{}.png".format(self.name))
            # prototype = prototype[20:-10, 15:-15]
            # prototype = prototype[5:-5, 5:-5]
            prototype = cv2.resize(prototype, (self.size, self.size),
                                   interpolation=cv2.INTER_AREA)
        empty_color = prototype[1][1]
        self.image[0] = prototype.copy()
        self.image[1] = prototype.copy()  # (238, 238, 210)
        colors = np.array(((86, 150, 118), (210, 238, 238)), dtype=np.uint8)
        for num, new_color in enumerate(colors):
            for i, elements in enumerate(prototype):
                for j, color in enumerate(elements):
                    if self.name == "":
                        summ = (int(i > self.size // 2)
                                + int(j > self.size // 2))
                        if (summ == 0 or summ == 2):
                            self.image[num][i][j] = colors[(num + 1) % 2]
                        else:
                            self.image[num][i][j] = colors[(num) % 2]
                    else:
                        if np.all(self.image[num][i][j] == empty_color):
                            self.image[num][i][j] = new_color

        self.image[0] = cv2.cvtColor(np.uint8(self.image[0]),
                                     cv2.COLOR_BGR2GRAY)
        self.image[1] = cv2.cvtColor(np.uint8(self.image[1]),
                                     cv2.COLOR_BGR2GRAY)

    def __init__(self, name):
        self.positions = []
        self.image = [None, None]
        self.name = name
        if (name == ""):
            #            self.mode = cv2.TM_CCOEFF_NORMED
            self.threshold = 0.75
            self.mode = cv2.TM_CCOEFF_NORMED
            self.size = 10
        else:
            self.size = 105

    def find(self, sct_img):
        self.positions.clear()
        for image in self.image:
            res = cv2.matchTemplate(sct_img, image, self.mode)

            loc = np.where(res >= self.threshold)  ## FOUND TAble
            for pt in zip(*loc[::-1]):  # Switch collumns and rows
                self.positions.append(pt)
        for i in range(len(self.positions) - 1, -1, -1):
            j = i - 1
            while j >= 0:
                if abs(self.positions[j][0] - self.positions[i][0]) + abs(
                        self.positions[j][1] - self.positions[i][1]) < 10:
                    break
                j -= 1
            if j != -1:
                del self.positions[i]

    def write(self, screen, color=True):
        if color == True:
            piece = self.name[1]
            if "r" == piece:
                color = (255, 233, 123)
            elif "n" == piece:
                color = (255, 123, 123)
            elif "b" == piece:
                color = (123, 233, 123)
            elif "k" == piece:
                color = (23, 154, 200)
            elif "q" == piece:
                color = (23, 154, 23)
            elif "p" == piece:
                color = (23, 23, 200)
            else:
                color = (255, 201, 42)

        for pt in self.positions:  # Switch collumns and rows
            screen = cv2.rectangle(screen, pt,
                                   (pt[0] + self.size, pt[1] + self.size),
                                   color, 4)
