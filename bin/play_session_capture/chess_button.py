import numpy as np
import cv2

from resources.pictures import RESOURCES_PATH


class ChessButton:
    image = [None, None]  # white and black version
    size = 52
    threshold = 0.80
    mode = cv2.TM_CCOEFF_NORMED
    positions = None
    last_len = 0
    found = False

    def __init__(self, images):
        self.positions = []
        self.image = []
        for k, name in enumerate(images):
            self.image.append(cv2.imread(
                RESOURCES_PATH + "/{}.png".format(name)))
            self.image[k] = cv2.cvtColor(np.uint8(self.image[k]),
                                         cv2.COLOR_BGR2GRAY)
            self.image[k] = cv2.resize(self.image[k],
                                       np.array(self.image[k].shape)[::-1] // 2)

    def update(self, sct_img, zero=[0, 0]):
        self.found = False
        self.positions.clear()
        for scale in range(5):
            for k, images in enumerate(self.image):
                true_scale = (1 + (scale - 1) * 0.5)
                img = cv2.resize(self.image[k], (
                        np.array(self.image[k].shape)[
                        ::-1] // true_scale).astype(int))
                if sct_img.shape[0] <= img.shape[0] and \
                        sct_img.shape[1] <= img.shape[1]:
                    res = np.array([])
                else:
                    res = cv2.matchTemplate(sct_img, img, self.mode)
                loc = np.where(res >= self.threshold)  ## FOUND TAble
                for pt in zip(*loc[::-1]):  # Switch collumns and rows
                    self.positions.append((np.array(pt)).astype(int))
                for i in range(len(self.positions) - 1, -1, -1):
                    j = i - 1
                    while j >= 0:
                        if abs(self.positions[j][0] - self.positions[i][
                            0]) + abs(
                            self.positions[j][1] - self.positions[i][1]) < 10:
                            break
                        j -= 1
                    if j != -1:
                        del self.positions[i]
                    if len(self.positions) > 0:
                        self.last_len = img.shape
                        self.found = True
                        self.positions[0][0] += zero[0]
                        self.positions[0][1] += zero[1]
                        return True

    def write(self, screen):
        for pt in self.positions:
            screen = cv2.rectangle(screen, pt,
                                   (pt[0] + self.last_len[1],
                                    pt[1] + self.last_len[0]),
                                   (12, 122, 199), 2)
