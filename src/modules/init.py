from os import walk

import sys
from file_functions import dirs_in_path

platform = sys.platform



if(platform in ["cygwin", "msys"]):
    NATIVE_EXTENSION="exe"
else:
    NATIVE_EXTENSION="out"


python_version = sys.version_info[0]
python_subversion = sys.version_info[1]

game_projects = dirs_in_path("./games")
example_projects = dirs_in_path("./examples")

    

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


