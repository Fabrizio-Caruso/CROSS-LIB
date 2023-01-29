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

    
        