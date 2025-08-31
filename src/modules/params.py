

from init import *
from collections import OrderedDict
from import_from_source import printc, bcolors
from commands import *
from target_defs import *

def full_params(params):
    full_command = SHORT_COMMANDS_LIST.get(params[1])
    if full_command is not None:
        return ["", full_command] + params[2:]
    return params


def handle_two_letter_params(params):
    command = params[1]
    if (len(command)==2) and command not in COMMANDS_LIST and command not in SHORT_COMMANDS_LIST:
        first_short_command = params[1][0]
        second_short_command = params[1][1]
        first_command = SHORT_COMMANDS_LIST[first_short_command]
        second_command = SHORT_COMMANDS_LIST[second_short_command]
        new_params = ['', first_command, second_command]
        if len(params)>2:
            new_params.extend(params[2:])
        # print("Interpreting two-letter command as: " + str(new_params))
        return new_params
    return params
    

def get_size_params(params):
    if len(params)<5:
        xsize = params[2]
        ysize = params[3]
        target = NATIVE_TARGET
    else:
        xsize = params[3]
        ysize = params[4]
        target = params[2]

    if target == 'terminal':
        target = 'terminal8x8'

    if target in [NATIVE_TARGET,'stdio'] or target.startswith('terminal'):
        target = target + '_sized'
        
    return target, xsize, ysize


