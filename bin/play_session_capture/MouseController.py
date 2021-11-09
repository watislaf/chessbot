import time

import mouse


class MouseController:
    def __init__(self):
        while True:
            time.sleep(1)
            mouse.click('left')
