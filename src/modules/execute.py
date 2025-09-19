from __future__ import print_function

try:
    from inspect import signature
except:
    # from inspect import name_of
    # import inspect2 
    # signature = inspect2.signature(initializer)

    
    # import funcsigs
    # signature = funcsigs.signature(initializer)
    
    # Dummy signature to get xl build to work under Python 2.7
    from collections import namedtuple
    Param = namedtuple('Param', 'parameters')
    
    def signature(name):
        return Param([1,2])


from modules.strings import *
from modules.import_from_source import *
from modules.tiles import *
from modules.LoggerSingleton import LoggerSingleton
from modules.init import *
from modules.params import handle_two_letter_params, full_params, COMMANDS_LIST, get_size_params, get_params
from modules.help_functions import help_command, manual
from modules.file_functions import files, convert_makefile, files_in_path, dirs_in_path, built_files_in_path, list_of_projects, list_projects
from modules.tests import *
from modules.split_projects import *
from modules.run import run_command, run
from modules.commands import commands
from modules.input_functions import generic_input, are_you_sure
from modules.option_functions import *
from modules.project_functions import rename, clone, create, delete
from modules.target_defs import *
from modules.clean_functions import clean, clean_test
from modules.build_functions import *
from modules.print_functions import bcolors, printc

def normalize_command(command_name):
    return command_name.replace("list","list_projects").replace("help", "help_command").replace("import", "import_from_source")
    
    

def execute_command(option_config, params):
    command_name = params[1]
    if command_name in COMMANDS_LIST:
        normalized_name = normalize_command(command_name)
        params_string = ", params[1:]" if len(signature(eval(normalized_name)).parameters)>1 else ""
        print(normalized_name+"(option_config" + params_string + ")")
        print(params[1:])
        eval(normalized_name+"(option_config" + params_string + ")")
    elif command_name in list_of_projects("all") + ["examples"] + ["games"] + ["projects"] + ["all"]:
        build(option_config, params)
    else:
        manual(option_config, params)


def execute_command(option_config, params):
    command_name = params[1]
    if command_name in COMMANDS_LIST:
        normalized_name = normalize_command(command_name)
        params_string = ", params[1:]" if len(signature(eval(normalized_name)).parameters)>1 else ""
        eval(normalized_name+"(option_config" + params_string + ")")
    elif command_name in list_of_projects("all") + ["examples"] + ["games"] + ["projects"] + ["all"]:
        build(option_config, params)
    else:
        manual(option_config, params)


def execute_string(option_config, command_line, silent = True):
    params = command_line.split()

    max_command_length = 0
    for command in COMMANDS_LIST:
        if len(command)>max_command_length:
            max_command_length = len(command)

    if not silent:
        spaces_before = max_command_length + 1 - len(params[1]) 
        spaces_after = 40 - len(command_line)
        printc(option_config, bcolors.OKBLUE, "["+ params[1] +"] ")
        printc(option_config, bcolors.OKCYAN, (spaces_before * " ") + command_line + (spaces_after * " ") + "\n")
    execute_command(option_config, params)


