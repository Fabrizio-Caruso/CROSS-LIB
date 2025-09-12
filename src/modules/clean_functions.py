import os
from print_functions import printc, bcolors
from project_functions import project_category
from file_functions import files_in_path
from modules.init import *
from run import run_command

def clean_test(option_config):
    clean(option_config, ["clean"])
    files_after_clean = len(files_in_path("../build"))

    if files_after_clean>1:
        # printc(option_config, bcolors.FAIL, "[xl clean]          KO\n")
        return 0
    # printc(option_config, bcolors.OKGREEN, "[xl clean]          OK\n")
    return 1
    
    

# Clean project data
def clean(option_config, params):

    GNU_MAKE = option_config.build_config.gnu_make
    
    verbose = option_config.terminal_config.verbose

    if len(params)>=2:
        if params[1]=="tools":
            make_command = GNU_MAKE + " clean_tools -f makefiles.common/auxiliary/Makefile_tools"

            run_command(option_config, make_command)
            return
        if params[1]=="log" or params[1]=="logs":
            make_command = GNU_MAKE + " clean_logs -f makefiles.common/auxiliary/Makefile_tools"

            run_command(option_config, make_command)
            return

    if len(params)<2:
        make_command = GNU_MAKE + " clean_generic -f makefiles.common/auxiliary/Makefile_common"

        if verbose:
            print("Delete all built binaries and non-project-specific temporary files")

        run_command(option_config, make_command)
        return

    game_dir = params[1]

    project_type = project_category(game_dir)

    parent_dir = project_type + "s"

    print("Project name: " + game_dir)

    parent_and_game_dir = parent_dir + "/" + game_dir

    if not os.path.exists(parent_and_game_dir):
        print("Project not found!")
        return

    print("Delete all built binaries and temporary files (also specific to '"+ \
          game_dir+"', e.g., generated assets)")

    make_command = GNU_MAKE + " clean -f " + parent_dir+"/"+game_dir+"/Makefile."+game_dir

    run_command(option_config, make_command)
