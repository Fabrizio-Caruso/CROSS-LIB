import os
from import_from_source import printc, bcolors
from modules.LoggerSingleton import LoggerSingleton

logger = LoggerSingleton.initLogger('xl', '../logs')

def run_command(option_config, command_string, verbose=False):
    logger.info("Running command: %s", command_string)
    # print("verbose: " + str(verbose))
    if verbose or option_config.terminal_config.verbose:
        print("Going to run: ")
        printc(option_config, bcolors.BOLD, command_string+"\n")
    else:
        command_string+= " > /dev/null 2>&1"

    os.system(command_string)