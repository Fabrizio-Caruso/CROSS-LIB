import os
import glob
from project_functions import project_category

from run import run_command

DELETE_MERGED_MAIN = True

def create_main(game_dir, project_type):
    game_dir = os.path.basename(game_dir)
    project_type = os.path.basename(project_type)
    base_dir = os.path.realpath(".")
    project_dir_path = os.path.realpath(os.path.join(base_dir, project_type + "s", game_dir))
    if not project_dir_path.startswith(base_dir + os.sep):
        raise ValueError("Invalid path: potential path traversal detected")
    split_dir_path = os.path.join(project_dir_path, "split_files") + os.sep
    main_c_path = os.path.join(project_dir_path, "main.c")
    with open(main_c_path, "w") as outfile:
        for c_file in sorted(glob.glob(split_dir_path + "*.c")):
            with open(c_file) as infile:
                outfile.write(infile.read())

# TODO: Implement flag to keep main.c
def delete_main(option_config, game_dir, project_type):
    if DELETE_MERGED_MAIN:
        parent_dir_path = "./" + project_type + "s/"
        project_dir_path = parent_dir_path + game_dir + "/"
        run_command(option_config, "rm -rf " + project_dir_path + "main.c")
    else:
        print("Keeping merged main.c")

def is_project_split(game_dir):
    project_type = project_category(game_dir)
    parent_dir_path = "./" + project_type + "s/"
    project_dir_path = parent_dir_path + game_dir + "/"
    return os.path.isdir(project_dir_path+"split_files")