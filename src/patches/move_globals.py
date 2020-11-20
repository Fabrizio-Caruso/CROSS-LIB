#!/bin/python


import shutil

import os,sys

import re

if len(sys.argv)<2:
    game_dir = "chase"
else:
    game_dir = sys.argv[1]

if len(sys.argv)<3:
    parent_dir = "games"
else:
    parent_dir = sys.argv[2]

file_name = "../"+parent_dir+"/"+game_dir+"/main.c"

file = open(file_name)

file_content = file.read()

file.close()

# print(file_content)


pattern = '<global>(.*)</global>'

init_code = re.search(pattern, file_content, re.DOTALL).group(1)

no_global_init = re.sub(pattern, '', file_content, flags=re.DOTALL)


print(no_global_init)

print(init_code)

