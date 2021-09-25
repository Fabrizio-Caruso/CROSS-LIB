#!/bin/python

import shutil

import os,sys

from os import walk

def multiple_run(mypath,target,threads,optimization):
    projects = []
    for (dirpath, dirnames, filenames) in walk(mypath):
        projects.extend(dirnames)
        break
            
    for project_name in projects:
        make_command = \
            "make " + target + \
                " ZSDCC_MAKEFILE_THREADS_OPTS=\'-j " + threads + "'" \
                " ZSDCC_MAKEFILE_COMPILATION_OPTS=" + optimization + \
                " -f " + mypath+"/"+project_name+"/Makefile."+project_name;

        print("run command : " + make_command)
        print("\n")
        
        os.system(make_command)

if len(sys.argv)<2:
    game_dir = "helloworld"
else:
    game_dir = sys.argv[1]

if(game_dir in ["tiles","sounds","matrix","invaders","zombies"]):
    project_type = "test"
else:
    project_type = "game" 

parent_dir = project_type + "s"

if len(sys.argv)<3:
    target = "ncurses"
else:
    target = sys.argv[2]

if len(sys.argv)<4:
    threads = "8"
else:
    threads = sys.argv[3]


if len(sys.argv)>=5 and sys.argv[4]=="on":
    optimization = "--max-allocs-per-node200000"
else:
    optimization = ""


print("Project name       : " + game_dir)
print("Project type       : " + project_type)
print("Number of threads  : " + threads)
print("Extra optimization : " + optimization)

parent_and_game_dir = parent_dir + "/" + game_dir

if(game_dir not in ["games", "tests", "all"]):
    if not os.path.exists(parent_and_game_dir):
        print("Project not found!")
        exit();

    make_command = \
        "make " + target + \
            " ZSDCC_MAKEFILE_THREADS_OPTS=\'-j " + threads + "'" \
            " ZSDCC_MAKEFILE_COMPILATION_OPTS=" + optimization + \
            " -f " + parent_dir+"/"+game_dir+"/Makefile."+game_dir;

    print("run command : " + make_command)
    print("\n")

    os.system(make_command)
    
else:
    if game_dir=="games":
        multiple_run("games",target,threads,optimization)
    elif game_dir=="tests":
        multiple_run("tests",target,threads,optimization)
    elif game_dir=="all":
        multiple_run("games",target,threads,optimization)
        multiple_run("tests",target,threads,optimization)
    else:
        exit()
        

