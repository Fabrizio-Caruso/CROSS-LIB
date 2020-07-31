#!/bin/python


import shutil

import os,sys


if len(sys.argv)<2:
    game_dir = "chase"
else:
    game_dir = sys.argv[1]

file_names = os.listdir("./generated_assets/"+game_dir)

print("Files to copy: "+str(file_names))

for file_name in file_names:
    print("Copying: ", file_name)
    source_path = "./generated_assets/"+game_dir+"/"+file_name
    print("from: " + source_path)
    dest_path = "../games/"+game_dir+"/generated_assets"
    
    if not os.path.exists(dest_path):
        print("creating: " + dest_path)
        os.makedirs(dest_path)
    print("to: " + dest_path)
    shutil.copy(source_path, dest_path )

    