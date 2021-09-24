#!/bin/python

import shutil

import os,sys

from os import walk


project_dirs = ["demos", "games"]

projects = []

print("Searching folders")
for mypath in project_dirs:
    print(mypath)

print()

for mypath in project_dirs:
    for (dirpath, dirnames, filenames) in walk(mypath):
        projects.extend(dirnames)
        break

print("Found: " + str(len(projects)))

print()

for project in projects:
    print(project)