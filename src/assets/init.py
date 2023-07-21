from os import walk

import sys

DEFAULT_THREADS = 8

python_version = sys.version_info[0]
python_subversion = sys.version_info[1]

def dirs_in_path(mypath):
    dirs = []
    for (dirpath, dirnames, filenames) in walk(mypath):
        dirs.extend(dirnames)
        break
    return dirs

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


# verbose = True 
# compiler_opts = ""
# compilation_threads = 1
# zsdcc_compiler_opts = ""
# sccz80_compiler_opts = ""
# cmoc_compiler_opts = ""
# native_compiler = "gcc"
# tool_compiler = "gcc"  
# cc65_compiler_opts = "-O -Cl"
# lcc1802_compiler_opts = '"-Wf-volatile" -O "-Wp-D nofloats" "-Wa-D LCCNOLONG" "-Wf-mulcall"' 
# use_tools = 0
# color_terminal = 0

