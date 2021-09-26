#!/bin/python

import shutil

import os,sys


if len(sys.argv)<2:
    make_command = "make clean_generic -f makefiles.common/auxiliary/Makefile_common"

    print("run command : " + make_command)
    print("")
    os.system(make_command)
    exit();
else:
    game_dir = sys.argv[1]

if(game_dir in ["tiles","sounds","input","matrix","invaders","zombies"]):
    project_type = "test"
else:
    project_type = "game" 

parent_dir = project_type + "s"

print("Project name: " + game_dir)


parent_and_game_dir = parent_dir + "/" + game_dir

if not os.path.exists(parent_and_game_dir):
    print("Project not found!")
    exit();

make_command = "make clean -f " + parent_dir+"/"+game_dir+"/Makefile."+game_dir;

print("run command : " + make_command)
print("\n")

os.system(make_command)