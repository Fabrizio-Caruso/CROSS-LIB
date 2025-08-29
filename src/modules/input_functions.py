import sys

def generic_input(string):
    if sys.version_info[0] < 3:
        return raw_input(string)
    return input(string)