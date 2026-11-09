import sys

def generic_input(string):
    if sys.version_info[0] < 3:
        return raw_input(string)
    return input(string)
    
def are_you_sure():
    return generic_input("Are you sure [Y/N]? ").lower()
