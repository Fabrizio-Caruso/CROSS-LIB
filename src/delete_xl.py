#!/bin/python


import shutil

import os,sys


if len(sys.argv)<2:
    game_dir="helloworld"
else:
    candidate_name = sys.argv[1]
    if(candidate_name in ["chase", "shoot", "bomber", "snake", "horde", "tiles", "matrix", "invaders", "zombies", "sounds"]):
        print("invalid name!")
        exit()
    else:
        game_dir = candidate_name


if len(sys.argv)<3:
    project_type = "helloworld"
else:
    if(sys.argv[2]=="game"):
        project_type = "game"
    else: 
        if(sys.argv[2]=="test"):
            project_type = "test"
        else:
            project_type = "helloworld"


parent_dir = "games"

print("Project name: " + game_dir)

parent_and_game_dir = parent_dir + "/" + game_dir

if os.path.exists(parent_and_game_dir):
    print("Deleting directory " + parent_and_game_dir)
    shutil.rmtree(parent_and_game_dir)

makefile_name = "Makefile."+game_dir
if os.path.exists(makefile_name):
    print("Deleting..." + makefile_name)
    os.remove(makefile_name)





    