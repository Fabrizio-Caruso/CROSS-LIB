#!/bin/python


import shutil

import os,sys


if len(sys.argv)<2:
    game_dir = "helloworld"
else:
    game_dir = sys.argv[1]
    if(game_dir in ["chase", "shoot", "bomber", "snake", "horde", "tiles", "matrix", "invaders", "zombies", "sounds"]):
        print("invalid name!")
        exit()

if len(sys.argv)<3:
    project_type = "helloworld"
else:
    if(sys.argv[2]=="game"):
        project_type = "game"
    else: 
        if(sys.argv[2] in ["apis","show_apis","test","show"]):
            project_type = "demo"
        else:
            project_type = "helloworld"

parent_dir = "games" 

print("New project name: " + game_dir)
print("Project type: " + project_type)

parent_and_game_dir = parent_dir + "/" + game_dir

if not os.path.exists(parent_and_game_dir):
    print("Creating project dir...")
    os.makedirs(parent_and_game_dir)

source_path = "./template_projects/"+project_type+"_code.template"
print("source_path: " + source_path)
dest_path = parent_and_game_dir
print("dest_path: " + dest_path)

if not os.path.exists(dest_path+"/tiles"):
    print("Copying tiles...")
    shutil.copytree(source_path+"/tiles", dest_path+"/tiles")

if not os.path.exists(dest_path+"/generated_assets"):
    print("Create empty generated_assets directory...")
    os.makedirs(dest_path+"/generated_assets")

file_names = ["char_tiles.h", "images.h", "main.c"]


for file_name in file_names:
    print("Copying file_name: " + file_name)
    shutil.copy(source_path+"/"+file_name, dest_path)

templated_makefile_path = "./template_projects"

shutil.copy(templated_makefile_path+"/"+"Makefile_game.template", "./"+dest_path+"/Makefile."+game_dir)

#read input file
fin = open("./"+dest_path+"/Makefile."+game_dir, "rt")
#read file contents to string
data = fin.read()
#replace all occurrences of the required string
data = data.replace('_GAME_NAME_', game_dir)
data = data.replace('_PARENT_DIR_', parent_dir)
#close the input file
fin.close()
#open the file in write mode
fin = open("./"+dest_path+"/Makefile."+game_dir, "wt")
#overrite the input file with the resulting data
fin.write(data)
#close the file
fin.close()



    