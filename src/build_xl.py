#!/bin/python

import shutil

import os,sys


if len(sys.argv)<2:
    game_dir = "helloworld"
else:
    game_dir = sys.argv[1]

if(game_dir in ["tiles","sounds","matrix","invaders","zombies"]):
    project_type = "demo"
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


if len(sys.argv)<5:
    optimization = ""
else:
    optimization = sys.argv[4]


print("Project name       : " + game_dir)
print("Project type       : " + project_type)
print("Number of threads  : " + threads)
print("Extra optimization : " + optimization)

parent_and_game_dir = parent_dir + "/" + game_dir

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