#!/bin/python


import shutil

import os,sys


if len(sys.argv)<2:
    project_type = "helloworld"
else:
    if(sys.argv[1]=="game"):
        project_type = "game"
    else: 
        if(sys.argv[1]=="demo"):
            project_type = "demo"
        else:
            project_type = "helloworld"

if len(sys.argv)<3:
    if(project_type=="helloworld"):
        game_dir=project_type
    else:
        game_dir = "foo_"+project_type
else:
    candidate_name = sys.argv[2]
    if(candidate_name in ["bar", "chase", "shoot", "bomber", "snake", "horde", "tiles", "matrix", "invaders", "zombies"]):
        print("invalid name!")
        exit()
    else:
        game_dir = candidate_name

if(project_type=="helloworld"):
    parent_dir = "demos"
else:
    parent_dir = project_type+"s"

print("Project name: " + game_dir)
print("Project type: " + project_type)

print("parent_and_game_dir: " + parent_and_game_dir)

if os.path.exists(parent_and_game_dir):
    print("Deleting directory " + parent_and_game_dir)
    shutil.rmtree(parent_and_game_dir)

makefile_name = "Makefile."+game_dir
if os.path.exists(makefile_name):
    print("Deleting..." + makefile_name)
    os.remove(makefile_name)





    