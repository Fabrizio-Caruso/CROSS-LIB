import os
from project_functions import project_category

from run import run_command

def create_main(game_dir, project_type):
    parent_dir_path = "./" + project_type + "s/"
    project_dir_path = parent_dir_path + game_dir + "/"
    split_dir_path = project_dir_path + "split_files/"
    os.system("cat " + split_dir_path + "*.c > " + project_dir_path + "main.c")

def delete_main(option_config, game_dir, project_type):
    parent_dir_path = "./" + project_type + "s/"
    project_dir_path = parent_dir_path + game_dir + "/"
    # TODO: Implement flag to keep main.c
    run_command(option_config, "rm -rf " + project_dir_path + "main.c")

def is_project_split(game_dir):
    project_type = project_category(game_dir)
    parent_dir_path = "./" + project_type + "s/"
    project_dir_path = parent_dir_path + game_dir + "/"
    return os.path.isdir(project_dir_path+"split_files")