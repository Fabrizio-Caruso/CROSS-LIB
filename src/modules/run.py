import os
from import_from_source import printc, bcolors
from modules.LoggerSingleton import LoggerSingleton

logger = LoggerSingleton.initLogger('xl', '../logs')

def run_command(option_config, command_string):
    logger.info("Running command: %s", command_string)
    test = option_config.terminal_config.test
    show_fail = option_config.terminal_config.show_fail
    if not test:
        print("Going to run: ")
        printc(option_config, bcolors.BOLD, command_string+"\n")
    else:
        command_string+= " > /dev/null 2>&1"

    res = os.system(command_string)

    if not test and show_fail and res:
        printc(option_config, bcolors.FAIL, "return code: " + str(res)+"\n")