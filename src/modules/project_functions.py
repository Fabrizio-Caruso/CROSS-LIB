import os
import shutil

from file_functions import convert_makefile

from init import game_projects, example_projects


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
