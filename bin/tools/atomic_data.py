import threading


class AtomicData:
    locker = threading.Lock()
    is_start_ = False
    cant_find_go_button_ = True
    cant_find_board_ = True

    @property
    def is_start(self):
        with self.locker:
            return self.is_start_

    @is_start.setter
    def is_start(self, boolean):
        with self.locker:
            self.is_start_ = boolean

    @property
    def cant_find_go_button(self):
        with self.locker:
            return self.cant_find_go_button_

    @cant_find_go_button.setter
    def cant_find_go_button(self, boolean):
        with self.locker:
            self.cant_find_go_button_ = boolean

    @property
    def cant_find_board(self):
        with self.locker:
            return self.cant_find_board_

    @cant_find_board.setter
    def cant_find_board(self, boolean):
        with self.locker:
            self.cant_find_board_ = boolean
