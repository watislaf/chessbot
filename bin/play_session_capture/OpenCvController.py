import numpy as np
import cv2
from mss import mss

from graphics.pictures import PICTURES_PATH


class OpenCvController:

    def __init__(self):
        bounding_box = {'top': 10, 'left': 10, 'width': 470, 'height': 900}
        sct = mss()
        template = cv2.imread(PICTURES_PATH + "/wr.png")
        w, h = template.shape[:-1]

        while True:
            sct_img = np.array(sct.grab(bounding_box))
            sct_img = sct_img.astype(np.uint8)

            res = cv2.matchTemplate(sct_img, template, cv2.TM_CCOEFF_NORMED)
            # cv2.rectangle(sct_img, (0, 0), (300, 300), (255, 0, 0), 2)
            threshold = .8
            loc = np.where(res >= threshold)
            for pt in zip(*loc[::-1]):  # Switch collumns and rows
                cv2.rectangle(sct_img, pt, (pt[0] + w, pt[1] + h), (0, 0, 255),
                              2)

            cv2.imshow('screen', sct_img)
            if (cv2.waitKey(1) & 0xFF) == ord('q'):
                cv2.destroyAllWindows()
                break
