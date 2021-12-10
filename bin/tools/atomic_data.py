import threading


class AtomicData:
    __locker = threading.Lock()
    __is_start = False
    __cant_find_go_button = True
    __cant_find_board = True

    @property
    def is_start(self):
        with self.__locker:
            return self.__is_start

    @is_start.setter
    def is_start(self, boolean):
        with self.__locker:
            self.__is_start = boolean

    @property
    def cant_find_go_button(self):
        with self.__locker:
            return self.__cant_find_go_button

    @cant_find_go_button.setter
    def cant_find_go_button(self, boolean):
        with self.__locker:
            self.__cant_find_go_button = boolean

    @property
    def cant_find_board(self):
        with self.__locker:
            return self.__cant_find_board

    @cant_find_board.setter
    def cant_find_board(self, boolean):
        with self.__locker:
            self.__cant_find_board = boolean
