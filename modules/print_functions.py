from __future__ import print_function

class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKCYAN = '\033[96m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'


def printc(option_config, print_color,text):

    if(option_config.terminal_config.color_terminal):
        print(print_color + text + bcolors.ENDC, end="")
    else:
        print(text, end="")



def print_shape(option_config, items):
    if not option_config.terminal_config.test:
        for i in range(len(items)):
            printc(option_config, bcolors.BOLD,items[i]+"\n") # + "  ") # + "{:3d}".format(values[i]))
        print("")
        print("")


# It displays a shape from a list of strings (rows)
def display_shape(option_config, tile_vect):
    for row in tile_vect:
        printc(option_config, bcolors.BOLD, row)
        print("")

def display_simple_shape(tile_vect):
    for row in tile_vect:
        print(row)

