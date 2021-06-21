#!/bin/python


import shutil

import os,sys


if len(sys.argv)<2:
    game_dir = "foo"
else:
    game_dir = sys.argv[1]

if len(sys.argv)<3:
    parent_dir = "games"
else:
    parent_dir = sys.argv[2]


print("New project name: " + game_dir)
print("New project parent dir: " + parent_dir) 

parent_and_game_dir = parent_dir + "/" + game_dir

if not os.path.exists(parent_and_game_dir):
    print("Creating project dir...")
    os.makedirs(parent_and_game_dir)

source_path = "./template_projects/game_code.template"
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

# for file_name in file_names:
    # print("Copying: ", file_name)
    # source_path = "./generated_assets/"+game_dir+"/"+file_name
    # print("from: " + source_path)
    # dest_path = "../"+parent_dir+"/"+game_dir+"/generated_assets"
    
    # if not os.path.exists(dest_path):
        # print("creating: " + dest_path)
        # os.makedirs(dest_path)
    # print("to: " + dest_path)
    # shutil.copy(source_path, dest_path )

    