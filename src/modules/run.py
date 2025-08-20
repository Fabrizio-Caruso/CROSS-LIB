import os
from import_from_source import printc, bcolors
from modules.LoggerSingleton import LoggerSingleton

logger = LoggerSingleton.initLogger('xl', '../logs')

def run_command(option_config, command_string):
    logger.info("Running command: %s", command_string)
    test = option_config.terminal_config.test           # run instde test_self makes test      = False
    verbose = option_config.terminal_config.verbose
    native_console = option_config.terminal_config.native_console # run inside test_self makes native_console = False
    
    if verbose:
        printc(option_config, bcolors.BOLD, command_string+"\n")
    if native_console and (test or not verbose):
        command_string+= " > /dev/null 2>&1"

    res = os.system(command_string)

    if native_console and res:
        printc(option_config, bcolors.FAIL, "return code: " + str(res)+"\n")