import os
import shutil

from file_functions import convert_makefile
from init import game_projects, example_projects
from strings import only_upper_digits_and_space, no_space
from commands import COMMANDS_LIST
from input_functions import are_you_sure

# It computes the type of project
def project_category(game_dir):
    if(game_dir in example_projects):
        return "example"
    elif game_dir in game_projects:
        return "game"
    else:
        return "project"


# Rename a project
def rename(option_config, params):
    if len(params)<2:
        source_game_dir = "helloworld"
    else:
        source_game_dir = params[1]

    if len(params)<3:
        print("ERROR: You need to provide more paramaters")
        return

    target_game_dir = params[2]

    source_project_type = project_category(source_game_dir)

    if source_project_type != "project":
        print("This is a built-in project.")
        return

    target_project_type = "project"
    target_parent_dir = target_project_type + "s"
    target_parent_dir_and_game_dir = target_parent_dir + "/" + target_game_dir

    if option_config.terminal_config.verbose:
        print("source_project_type: " + source_project_type)

    convert_makefile(option_config, source_game_dir, source_project_type, source_game_dir, target_game_dir)

    os.rename(target_parent_dir + "/" +source_game_dir, target_parent_dir_and_game_dir)


# Clone a project to create a new one
def clone(option_config, params):
    verbose = option_config.terminal_config.verbose
    if len(params)<2:
        source_game_dir = "helloworld"
    else:
        source_game_dir = params[1]

    if len(params)<3:
        return

    target_game_dir = params[2]

    source_project_type = project_category(source_game_dir)
    if verbose:
        print("source project category: " + source_project_type)
    source_parent_dir = source_project_type + "s"
    source_parent_and_game_dir = source_parent_dir + "/" + source_game_dir

    target_project_type = "project"
    target_parent_dir = target_project_type + "s"
    target_parent_dir_and_game_dir = target_parent_dir + "/" + target_game_dir

    if verbose:
        print("Source project name    : " + source_game_dir)
        print("Source Project type    : " + source_project_type)
        print("Path to source project : " + source_parent_and_game_dir)
        print("")
        print("Target project name    : " + target_game_dir)
        print("target Project type    : " + target_project_type)
        print("Path to target project : " + target_parent_dir_and_game_dir)

    source_path = source_parent_and_game_dir
    if verbose:
        print("source_path: " + source_path)
    dest_path = target_parent_dir_and_game_dir
    if verbose:
        print("dest_path: " + dest_path)

    if not os.path.exists(dest_path+"/tiles"):
        if verbose:
            print("Copying tiles...")
        shutil.copytree(source_path, dest_path)

    if not os.path.exists(dest_path+"/generated_assets"):
        if verbose:
            print("Create empty generated_assets directory...")
        os.makedirs(dest_path+"/generated_assets")

    convert_makefile(option_config, target_game_dir, source_project_type, source_game_dir, target_game_dir)


# Create a new project
def create(option_config, params):
    verbose = option_config.terminal_config.verbose
    if len(params)<2:
        game_dir = "helloworld"
    else:
        game_dir = params[1]
    game_dir_capital = game_dir.upper()
    game_dir_capital_without_special_chars = \
        only_upper_digits_and_space(game_dir.upper().replace("-"," ").replace("_"," "))

    game_dir = no_space(game_dir)

    if game_dir in example_projects or game_dir in game_projects or game_dir in COMMANDS_LIST:
        print("invalid name!")
        return

    if len(params)<3:
        project_type = "helloworld"
    else:
        if params[2]=="game" or params[2]=="arcade":
            project_type = "arcade_game"
        elif params[2]=="text":
            project_type = "text_game"
        elif params[2]=="test":
            project_type = "test"
        else:
            if params[2] in ["apis","show_apis","example","show","demo"]:
                project_type = "demo"
            else:
                project_type = "helloworld"

    parent_dir = "projects"

    if verbose:
        print("New project name: " + game_dir)
        print("Project type: " + project_type)

    parent_and_game_dir = parent_dir + "/" + game_dir

    if not os.path.exists(parent_and_game_dir):
        if verbose:
            print("Creating project dir...")
        os.makedirs(parent_and_game_dir)

    source_path = "./template_projects/"+project_type+"_code.template"
    if verbose:
        print("source_path: " + source_path)
    dest_path = parent_and_game_dir
    if verbose:
        print("dest_path: " + dest_path)

    if not os.path.exists(dest_path+"/tiles"):
        if verbose:
            print("Copying tiles...")
        shutil.copytree(source_path+"/tiles", dest_path+"/tiles")

    if not os.path.exists(dest_path+"/shapes"):
        if verbose:
            print("Copying shapes...")
        if not os.path.exists(source_path+"/shapes"):
            os.makedirs(dest_path+"/shapes")
        else:
            shutil.copytree(source_path+"/shapes", dest_path+"/shapes")

    if not os.path.exists(dest_path+"/generated_assets"):
        if verbose:
            print("Create empty generated_assets directory...")
        os.makedirs(dest_path+"/generated_assets")

    if not os.path.exists(dest_path+"/config"):
        if verbose:
            print("Create empty config directory...")
        os.makedirs(dest_path+"/config")

    file_names = ["main.c"]

    for file_name in file_names:
        if verbose:
            print("Copying file_name: " + file_name)
        shutil.copy(source_path+"/"+file_name, dest_path)

    # Replace _GAME_NAME_CAPITAL with the capitalized game name without special characters
    fin = open("./"+dest_path+"/main.c", "rt")
    data = fin.read()
    data = data.replace('_GAME_NAME_CAPITAL',game_dir_capital_without_special_chars)
    fin.close()
    fin = open("./"+dest_path+"/main.c", "wt")
    #overrite the input file with the resulting data
    fin.write(data)

    templated_makefile_path = "./template_projects"

    shutil.copy(templated_makefile_path+"/"+"Makefile_game.template", \
                "./"+dest_path+"/Makefile."+game_dir)

    shutil.copy(templated_makefile_path+"/"+"/config/project_config.mk.template", \
                "./"+dest_path+"/config/project_config.mk")
    shutil.copy(templated_makefile_path+"/"+"/config/game_config.mk.template", \
                "./"+dest_path+"/config/game_config.mk")

    #read input file
    fin = open("./"+dest_path+"/Makefile."+game_dir, "rt")
    #read file contents to string
    data = fin.read()
    #replace all occurrences of the required string
    data = data.replace('_GAME_NAME_CAPITAL',game_dir_capital)
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


# Delete project
def delete(option_config, params):
    if(len(params)>=2) and params[1]=="tools":
        make_command = GNU_MAKE + " clean_tools"

        run_command(option_config, make_command)
        return

    if len(params)<2:
        game_dir="helloworld"
    else:
        candidate_name = params[1]
        if candidate_name in example_projects or candidate_name in game_projects or \
            candidate_name in COMMANDS_LIST:
            print("invalid name!")
            return
        game_dir = candidate_name

    if (len(params)>2) and (params[2]=="-y"):
        interactive = False
    else:
        interactive = True

    parent_dir = "projects"
    verbose = option_config.terminal_config.verbose
    if verbose:
        print("Project name: " + game_dir)

    parent_and_game_dir = parent_dir + "/" + game_dir
    if verbose:
        print("Remove the project '"+game_dir+ \
              "' with all its files (source, graphics assets, makefile)")
    if (not interactive) or (are_you_sure()=="y"):
        if os.path.exists(parent_and_game_dir):
            if verbose:
                print("Deleting directory " + parent_and_game_dir)
            shutil.rmtree(parent_and_game_dir)

        makefile_name = "Makefile."+game_dir
        if os.path.exists(makefile_name):
            if verbose:
                print("Deleting..." + makefile_name)
            os.remove(makefile_name)
        if verbose:
            print("'" + game_dir + "' deleted")
    else:
        return

