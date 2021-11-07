from ast import literal_eval as make_tuple
import re as regex


class PieceMove:
    position_from = tuple((0, 0))
    position_to = tuple((0, 0))

    def __init__(self, *args):
        # get only 1 arg from a string
        if len(args) == 1:
            my_tur = regex.findall(r'[(][0-9],[0-9][)]', args[0][1:-1])
            self.position_from = make_tuple(my_tur[0])
            self.position_to = make_tuple(my_tur[1])
        elif len(args) == 2:
            # get two tuples
            self.position_from = args[0]
            self.position_to = args[1]

    def __str__(self):
        return "(({},{}),({},{}))".format(
            self.position_from[0],
            self.position_from[1],
            self.position_to[0],
            self.position_to[1])
