from os import walk

import os

from print_functions import printc, bcolors

def files_in_path(mypath):
    files = []
    for (_, _, filenames) in walk(mypath):
        files.extend(filenames)
        break
    return files
    


def dirs_in_path(mypath):
    dirs = []
    for (dirpath, dirnames, filenames) in walk(mypath):
        dirs.extend(dirnames)
        break
    return dirs




def built_files_in_path(mypath):
    files = []
    for (_, _, filenames) in walk(mypath):
        built_files = []
        for filename in filenames:
            if not filename.startswith('.'):
                built_files.append(filename)

        files.extend(built_files)
        break
    return files


def list_of_projects(project_category):
    if project_category in ["examples", "projects", "games"]:
        res = dirs_in_path("./"+project_category)
    else:
        res = dirs_in_path("./examples")+dirs_in_path("./games")+dirs_in_path("./projects")
    return res


# List all projects
def list_projects(option_config, params):

    if len(params)<2:
        project_dirs = ["examples", "games", "projects"]
    elif params[1]=="all":
        project_dirs = ["examples", "games", "projects"]
    elif params[1] in ["games", "examples", "projects"]:
        project_dirs = [params[1]]
    elif params[1] in ["new"]:
        project_dirs = ["projects"]
    elif params[1] in ["built-in", "builtin"]:
        project_dirs = ["games", "examples"]
    else:
        return

    count = 0

    for mypath in project_dirs:
        if not option_config.terminal_config.test:
            printc(option_config, bcolors.BOLD, "["+mypath+"]\n")
        for (_, dirnames, _) in walk(mypath):
            # projects.extend(dirnames)
            for project in dirnames:
                if not option_config.terminal_config.test:
                    print("  "+str(project))
                count+=1
            break
        if not option_config.terminal_config.test:
            print("")

    if option_config.terminal_config.verbose:
        print("Projects found: " + str(count))
        print("")

    return count




def convert_makefile(option_config, dir,old_type,old_name,new_name):

    dest_path = "projects/"+dir
    source_game_dir = old_name
    target_game_dir = new_name
    source_parent_dir = old_type
    target_parent_dir = "projects"

    if option_config.terminal_config.verbose:
        print("- dir: " + dir)
        print("- old_type: " + old_type)
        print("- old_name: " + old_name)
        print("- new_game: " + new_name)

    fin = open("./"+dest_path+"/Makefile."+source_game_dir, "rt")
    data = fin.read()
    data = data.replace('CROSS-' + source_game_dir.upper(),'CROSS-'+target_game_dir.upper())
    data = data.replace('GAME_NAME := ' + source_game_dir, 'GAME_NAME := '+target_game_dir)
    data = data.replace('PARENT_DIR = ' + source_parent_dir + "s", \
           'PARENT_DIR = ' + target_parent_dir)

    data = data.replace('include ./' + old_type + "s/" + source_game_dir, \
           'include ./projects/' + target_game_dir)
    fin.close()
    fin = open("./"+dest_path+"/Makefile."+target_game_dir, "wt")
    fin.write(data)
    fin.close()

    os.remove("./"+dest_path+"/Makefile."+source_game_dir)


# Show all generated files
def files(option_config):
    make_command = "ls -ls ../build"
    os.system(make_command)
