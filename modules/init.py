from __future__ import print_function


from os import walk
from pathlib import Path

import sys
import os

from file_functions import dirs_in_path
from LoggerSingleton import LoggerSingleton


platform = sys.platform



repo_root = Path(__file__).resolve().parent.parent


NUMBER_OF_TILES = 27


if(platform in ["cygwin", "msys"]):
    NATIVE_EXTENSION="exe"
else:
    NATIVE_EXTENSION="out"


python_version = sys.version_info[0]
python_subversion = sys.version_info[1]





# game_projects = dirs_in_path(os.getcwd() + "/games")
# example_projects = dirs_in_path(os.getcwd() + "/examples")

# print("os.getcwd() " + os.getcwd())
# print("games_projects: " + str(game_projects))

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


logger = LoggerSingleton.initLogger('xl', str(repo_root / 'logs'))


def _is_windows():
    """Return True if running on a Windows platform."""
    return platform.startswith("win")

def handle_parallelism(option_config):
    if option_config.build_config.parallelize_multi_build:
        if option_config.terminal_config.verbose:
            print("Parallelize_multi_build is ON")
        logger.info("Parallelize_multi_build is ON")
        from multiprocessing import Pool
    else:
        logger.info("Parallelize_multi_build is OFF")


def show_python_version(option_config):
    if option_config.terminal_config.verbose:
        logger.info("Verbose mode ON")
        print("----------------------------")
        print("----------------------------")
        print("Platform: " + platform)
        print("Python:   " + str(python_version) + "." + str(python_subversion))
        logger.info("Using Python version %s.%s", str(python_version), str(python_subversion))
        print("GNU MAKE command: " + option_config.build_config.gnu_make)
        print("----------------------------")
        print("")

