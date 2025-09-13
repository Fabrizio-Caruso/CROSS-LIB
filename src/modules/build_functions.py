#!/usr/bin/env python

from __future__ import print_function

import datetime
import shutil
import os
import sys
import time

from os import walk

sys.path.append("modules")

from modules.strings import *
from modules.import_from_source import *
from modules.tiles import *
from modules.LoggerSingleton import LoggerSingleton
from modules.init import *
from modules.default_values import insert_default_sizes
from modules.params import \
    handle_two_letter_params, full_params, COMMANDS_LIST, get_size_params
from modules.help_functions import help_command, manual
from modules.file_functions import files, convert_makefile, files_in_path, dirs_in_path, built_files_in_path, list_of_projects, list_projects
from modules.tests import *
from modules.split_projects import *
from modules.run import run_command, run
from modules.commands import commands
from modules.input_functions import generic_input, are_you_sure
from modules.option_functions import *
from modules.project_functions import rename, clone, create, delete
from modules.target_defs import *
from modules.shape_functions import *
from modules.clean_functions import clean, clean_test

DEFAULT_COMPILATION_THREADS = 4

logger = LoggerSingleton.initLogger('xl', '../logs')

logger.info('Started')



def multiple_project_reset(option_config, mypath):
    projects = []
    for (_, dirnames, _) in walk(mypath):
        projects.extend(dirnames)
        break

    for project_name in projects:
        print("PROJECT: " + project_name)
        reset(option_config, [mypath, project_name])
        print("\n")


def multiple_size_build(option_config, mypath,target,xsize,ysize,debug):

    GNU_MAKE, \
    compilation_threads, \
    compilation_threads_string, \
    parallelize_multi_build, \
    z88dk_compiler, \
    z88dk_compiler_opts, \
    sccz80_compiler_opts, \
    zsdcc_compiler_opts, \
    cmoc_compiler_opts, \
    cc65_compiler_opts, \
    lcc1802_compiler_opts, \
    gcc4ti99_compiler_opts, \
    vbcc_compiler_opts, \
    native_compiler_opts, \
    native_compiler, \
    tool_compiler, \
    use_tools \
    = option_config.build_config.get_opts()
    
    if option_config.build_config.parallelize_multi_build:
        # print("Parallelize_multi_build is ON")
        logger.info("Parallelize_multi_build is ON")
        from multiprocessing import Pool
    else:
        logger.info("Parallelize_multi_build is OFF")

    projects = []
    for (_, dirnames, _) in walk(mypath):
        projects.extend(dirnames)
        break

    if use_tools:
        use_tools_str = " USE_TOOLS=1 "
    else:
        use_tools_str = ""

    target_to_check = target.replace("_sized", "")
    if (parallelize_multi_build==1) and (target_to_check in PARALLEL_TARGETS):
        logger.info("Parallelize multi build with %s", compilation_threads)
        pool = Pool(processes=int(compilation_threads))
        
        for project_name in projects:

            if is_project_split(project_name):
                project_type = project_category(project_name)
                create_main(project_name,project_type)
            make_command = \
                GNU_MAKE + " " + target + use_tools_str + " XSIZE=" + xsize +  \
                " YSIZE=" + ysize + " _DEBUG_FLAG=" + str(debug) + \
                " _NATIVE_CC=" + native_compiler + " " + all_compilers_opts(option_config, "","") + \
                " GNU_MAKE=" + GNU_MAKE + \
                " TOOL_CC=" + tool_compiler + \
                " -f " + mypath+"/"+project_name+"/Makefile."+project_name

            # run_command(option_config, make_command)
            # if is_project_split(project_name):
                # delete_main(option_config, project_name,project_type)
            pool.apply_async(run_command, [option_config, make_command])

        pool.close()
        pool.join()
        for project_name in projects:
            if is_project_split(project_name):
                delete_main(option_config, project_name,project_type)
    else:
        for project_name in projects:

            if is_project_split(project_name):
                project_type = project_category(project_name)
                create_main(project_name,project_type)
            make_command = \
                GNU_MAKE + " " + target + use_tools_str + " XSIZE=" + xsize +  \
                " YSIZE=" + ysize + " _DEBUG_FLAG=" + str(debug) + \
                " _NATIVE_CC=" + native_compiler + " " + all_compilers_opts(option_config, "","") + \
                " GNU_MAKE=" + GNU_MAKE + \
                " TOOL_CC=" + tool_compiler + \
                " -f " + mypath+"/"+project_name+"/Makefile."+project_name

            run_command(option_config, make_command)
            if is_project_split(project_name):
                delete_main(option_config, project_name,project_type)


def multiple_build(option_config, mypath,target,threads,zsdcc_extra_optimization, reset_flag=False):
    GNU_MAKE = option_config.build_config.gnu_make

    if option_config.build_config.parallelize_multi_build:
        # print("Parallelize_multi_build is ON")
        logger.info("Parallelize_multi_build is ON")
        from multiprocessing import Pool
    else:
        logger.info("Parallelize_multi_build is OFF")

    projects = []

    verbose = option_config.terminal_config.verbose

    gnu_make, \
    compilation_threads, \
    compilation_threads_string, \
    parallelize_multi_build, \
    z88dk_compiler, \
    z88dk_compiler_opts, \
    sccz80_compiler_opts, \
    zsdcc_compiler_opts, \
    cmoc_compiler_opts, \
    cc65_compiler_opts, \
    lcc1802_compiler_opts, \
    gcc4ti99_compiler_opts, \
    vbcc_compiler_opts, \
    native_compiler_opts, \
    native_compiler, \
    tool_compiler, \
    use_tools \
    = option_config.build_config.get_opts()

    if use_tools:
        use_tools_str = " USE_TOOLS=1 "
    else:
        use_tools_str = ""

    for (_, dirnames, _) in walk(mypath):
        projects.extend(dirnames)
        break

    projects_to_build = len(projects)

    if verbose:
        print("Building " + str(projects_to_build) + " projects: ", end="")
        for project_name in projects:
            printc(option_config, bcolors.OKCYAN,project_name+" ")
        print("")

    if (parallelize_multi_build==1) and (target in PARALLEL_TARGETS):
        logger.info("Parallelize multi build with %s", threads)
        pool = Pool(processes=int(threads))

        for project_name in projects:
            if verbose:
                print("project name: " + project_name)
            if reset_flag:
                reset(option_config, ["",project_name])
            if is_project_split(project_name):
                project_type = project_category(project_name)
                create_main(project_name,project_type)
            make_command = \
                GNU_MAKE + " " + target + use_tools_str + \
                    " ZSDCC_MAKEFILE_THREADS_OPTS=\'-j " + threads + "'" \
                    + all_compilers_opts(option_config, zsdcc_extra_optimization, "") + \
                    " _NATIVE_CC=" + native_compiler + \
                    " TOOL_CC=" + tool_compiler + " GNU_MAKE=" + GNU_MAKE + \
                    " -f " + mypath+"/"+project_name+"/Makefile."+project_name
            pool.apply_async(run_command, [option_config, make_command])

        pool.close()
        pool.join()

        for project_name in projects:
            if is_project_split(project_name):
                delete_main(option_config, project_name,project_type)
    else:
        for project_name in projects:
            if reset_flag:
                reset(option_config, ["",project_name])
            if verbose:
                print("project name: " + project_name)
            if is_project_split(project_name):
                project_type = project_category(project_name)
                create_main(project_name,project_type)
            make_command = \
                GNU_MAKE + " " + target + use_tools_str + \
                    " ZSDCC_MAKEFILE_THREADS_OPTS=\'-j " + threads + "'" \
                    + all_compilers_opts(option_config, zsdcc_extra_optimization, "") + \
                    " _NATIVE_CC=" + native_compiler + \
                    " TOOL_CC=" + tool_compiler + " GNU_MAKE=" + GNU_MAKE + \
                    " -f " + mypath+"/"+project_name+"/Makefile."+project_name

            run_command(option_config, make_command)
            if is_project_split(project_name):
                delete_main(option_config, project_name,project_type)


# Run a project natively (terminal with ncurses) with a given XSize and YSize
def size(option_config, params, debug = False):
    GNU_MAKE = option_config.build_config.gnu_make

    verbose = option_config.terminal_config.verbose
    if len(params)<2:
        game_dir = "helloworld"
    else:
        game_dir = params[1]

    project_type = project_category(game_dir)

    parent_dir = project_type + "s"

    target, xsize, ysize = get_size_params(params)

    if verbose:
        print("Project name       : " + game_dir)
        print("Project type       : " + project_type)
        print("XSize: " + xsize)
        print("YSize: " + ysize)
        print("target: " + target)

    parent_and_game_dir = parent_dir + "/" + game_dir

    files_before = len(files_in_path("../build"))-1

    if game_dir not in ["games", "examples", "projects", "all"]:
        if not os.path.exists(parent_and_game_dir):
            print("Project not found!")
            return
        native_compiler = option_config.build_config.native_compiler
        tool_compiler   = option_config.build_config.tool_compiler
        if is_project_split(game_dir):
            if verbose:
                printc(option_config, bcolors.OKBLUE,"Create main.c from split source files\n")
            create_main(game_dir, project_type)
        make_command = \
            GNU_MAKE + " " + target + " XSIZE=" + xsize +  " YSIZE=" + ysize + \
                " _DEBUG_FLAG=" + str(debug) + " " \
                + all_compilers_opts(option_config, "","") + \
                " _NATIVE_CC=" + native_compiler + \
                " TOOL_CC=" + tool_compiler + \
                " -f " + parent_dir+"/"+game_dir+"/Makefile."+game_dir
        run_command(option_config, make_command)

        if is_project_split(game_dir):
            if verbose:
                printc(option_config, bcolors.OKBLUE,"Delete main.c (because of split source files)\n")
            delete_main(option_config, game_dir, project_type)
    else:
        if game_dir in["games","examples","projects"]:
            multiple_size_build(option_config, game_dir,target,xsize,ysize,debug)
        elif game_dir in["new"]:
            multiple_size_build(option_config, "projects",target,xsize,ysize,debug)
        elif game_dir in["builtin","built-in"]:
            multiple_size_build(option_config, "games",target,xsize,ysize,debug)
            multiple_size_build(option_config, "examples",target,xsize,ysize,debug)
        elif game_dir=="all":
            multiple_size_build(option_config, "games",target,xsize,ysize,debug)
            multiple_size_build(option_config, "examples",target,xsize,ysize,debug)
            multiple_size_build(option_config, "projects",target,xsize,ysize,debug)
        else:
            return

    files_after = len(files_in_path("../build"))-1

    if verbose:
        print("New binaries built: " + str(files_after-files_before)+"\n")


# Rebuild a project, i.e., also rebuilds assets
def rebuild(option_config, params):
    build(option_config, params, reset_flag=True)


# Generate tools from source code
def tools(option_config):
    GNU_MAKE = option_config.build_config.gnu_make
    compilation_threads = option_config.build_config.compilation_threads
    tool_compiler = option_config.build_config.tool_compiler
    make_command = \
        GNU_MAKE + " -j " + compilation_threads + " tools TOOL_CC=" + tool_compiler + \
        " GNU_MAKE=" + GNU_MAKE + " -f makefiles.common/auxiliary/Makefile_tools"
    run_command(option_config, make_command)


# Build a project (for a target or multiple targets)
def build(option_config, params, reset_flag = False):
    GNU_MAKE = option_config.build_config.gnu_make

    params = insert_default_sizes(option_config, params)

    # sized terminal
    if len(params)>=4 and params[2].startswith("terminal") and params[3].isnumeric() \
                      and params[4].isnumeric():
        size(option_config, ["size"] + params[1:],0)
        return

    if len(params)<2:
        game_dir = "helloworld"
    else:
        game_dir = params[1]

    if len(params)>=1 and game_dir=="tools":
        tools(option_config)
    else:

        files_before = len(files_in_path("../build"))-1

        project_type = project_category(game_dir)
        parent_dir = project_type + "s"

        if len(params)<3:
            target = "ncurses"
        else:
            target = params[2]

        if target in ["cc65", "z88dk", "cmoc", "lcc1802", "cc6303", "gcc4ti", "vbcc"]:
            target = target + "_targets"

        gnu_make, \
        compilation_threads, \
        compilation_threads_string, \
        parallelize_multi_build, \
        z88dk_compiler, \
        z88dk_compiler_opts, \
        sccz80_compiler_opts, \
        zsdcc_compiler_opts, \
        cmoc_compiler_opts, \
        cc65_compiler_opts, \
        lcc1802_compiler_opts, \
        gcc4ti99_compiler_opts, \
        vbcc_compiler_opts, \
        native_compiler_opts, \
        native_compiler, \
        tool_compiler, \
        use_tools \
        = option_config.build_config.get_opts()


        # compilation_threads = option_config.build_config.compilation_threads
        if len(params)<4:
            threads = str(compilation_threads)
        else:
            threads = params[3]

        # zsdcc_extra_optimization = option_config.build_config.zsdcc_extra_optimization
        if len(params)>=5 and params[4]=="on":
            zsdcc_extra_optimization = "--max-allocs-per-node200000"
        else:
            zsdcc_extra_optimization = ""

        # compiler_opts = option_config.build_config.compiler_opts
        if len(params)>=6:
            compiler_opts = " " + params[5]
        else:
            compiler_opts = ""

        color_terminal = option_config.terminal_config.color_terminal

        verbose = option_config.terminal_config.verbose

        if verbose:

            print("Project name       : ", end="")
            printc(option_config, bcolors.BOLD, game_dir+"\n")

            print("Target name        : ", end="")
            printc(option_config, bcolors.UNDERLINE, target+"\n")

            print("Project type       : " + project_type)
            print("Number of threads  : " + threads)
            print("Extra optimization : " + zsdcc_extra_optimization)
            print("Compiler's options : " + compiler_opts)

            print("--------------------------")
            print("z88dk_compiler_opts     : " + z88dk_compiler_opts)
            print("zsdcc_compiler_opts     : " + zsdcc_compiler_opts)
            print("cmoc_compiler_opts      : " + cmoc_compiler_opts)
            print("cc65_compiler_opts      : " + cc65_compiler_opts)
            print("lcc1802_compiler_opts   : " + lcc1802_compiler_opts)
            print("gcc4ti99_compiler_opts  : " + gcc4ti99_compiler_opts)
            print("vbcc_compiler_opts      : " + vbcc_compiler_opts)
            print("native_compiler_opts    : " + native_compiler_opts)

            print("")

        parent_and_game_dir = parent_dir + "/" + game_dir

        if os.path.exists(parent_and_game_dir + "/config.ini"):
            print("Local config.ini detected")
            read_config(parent_and_game_dir + "/config.ini")

        if game_dir not in ["games", "examples", "projects", "all"]:
            if reset_flag:
                reset(option_config, params)
            if not os.path.exists(parent_and_game_dir):
                print("Project not found!")
                return
            if use_tools:
                use_tools_str = " USE_TOOLS=1 "
            else:
                use_tools_str = ""

            if is_project_split(game_dir):
                if verbose:
                    printc(option_config, bcolors.OKBLUE,"Create main.c from split source files\n")
                create_main(game_dir, project_type)

            make_command = \
                GNU_MAKE + " " + target + \
                    use_tools_str + \
                    " ZSDCC_MAKEFILE_THREADS_OPTS=\'-j " + threads + "' " \
                    + all_compilers_opts(option_config, zsdcc_extra_optimization, compiler_opts) +  \
                    " _NATIVE_CC=" + native_compiler + \
                    " TOOL_CC=" + tool_compiler + " GNU_MAKE=" + GNU_MAKE + \
                    " -f " + parent_dir+"/"+game_dir+"/Makefile."+game_dir

            run_command(option_config, make_command)

            if is_project_split(game_dir):
                if verbose:
                    printc(option_config, bcolors.OKBLUE,"Delete main.c because of split source files\n")
                delete_main(option_config, game_dir, project_type)

        else:
            if game_dir in ["games","examples","projects"]:
                multiple_build(option_config, game_dir,target,threads,zsdcc_extra_optimization, reset_flag)
            elif game_dir=="new":
                multiple_build(option_config, "projects",target,threads,zsdcc_extra_optimization, reset_flag)
            elif game_dir in ["builtin","built-in"]:
                multiple_build(option_config, "games",target,threads,zsdcc_extra_optimization, reset_flag)
                multiple_build(option_config, "examples",target,threads,zsdcc_extra_optimization, reset_flag)
            elif game_dir=="all":
                multiple_build(option_config, "games",target,threads,zsdcc_extra_optimization, reset_flag)
                multiple_build(option_config, "examples",target,threads,zsdcc_extra_optimization, reset_flag)
                multiple_build(option_config, "projects",target,threads,zsdcc_extra_optimization, reset_flag)
            else:
                return

        files_after = len(files_in_path("../build"))-1

        if verbose:
            printc(option_config, bcolors.OKBLUE, "New binaries built: " + str(files_after-files_before)+"\n")


def slow(option_config, params):
    GNU_MAKE = option_config.build_config.gnu_make

    verbose = option_config.terminal_config.verbose
    game_dir = params[1]

    project_type = project_category(game_dir)
    parent_dir = project_type + "s"

    if len(params)>=4:
        target = params[2]
        slowdown = params[3]
    elif len(params)==3:
        target = NATIVE_TARGET
        slowdown = params[2]
    else:
        printc(option_config, bcolors.FAIL, "Wrong number of arguments")
        sys.exit(-1)
    parent_and_game_dir = parent_dir + "/" + game_dir

    if os.path.exists(parent_and_game_dir + "/config.ini"):
        print("Local config.ini detected")
        read_config(parent_and_game_dir + "/config.ini")

    if not os.path.exists(parent_and_game_dir):
        print("Project not found!")
        return

    if is_project_split(game_dir):
        if verbose:
            printc(option_config, bcolors.OKBLUE,"Create main.c from split source files\n")
        create_main(game_dir, project_type)

    make_command = \
        GNU_MAKE + " " + target + \
            " SLOWDOWN=" + slowdown + \
            all_compilers_opts(option_config, "", "") +  \
            " -f " + parent_dir+"/"+game_dir+"/Makefile."+game_dir

    run_command(option_config, make_command)

    if is_project_split(game_dir):
        if verbose:
            printc(option_config, bcolors.OKBLUE,"Delete main.c because of split source files\n")
        delete_main(option_config, game_dir, project_type)


# Delete temporary project files If a project is specified, it also deletes generated assets.
def reset(option_config, params):
    GNU_MAKE = option_config.build_config.gnu_make

    verbose = option_config.terminal_config.verbose
    par_len = len(params)

    if par_len>1:
        game_dir = params[1]
    else:
        game_dir=""

    if game_dir in ["games","examples","projects"]:
        multiple_project_reset(option_config, game_dir)
        return
    if game_dir=="new":
        multiple_project_reset(option_config, "projects")
        return
    if game_dir in ["builtin","built-in"]:
        multiple_project_reset(option_config, "games")
        multiple_project_reset(option_config, "examples")
        return
    if game_dir=="all":
        multiple_project_reset(option_config, "games")
        multiple_project_reset(option_config, "examples")
        multiple_project_reset(option_config, "projects")
        return

    if len(params)<2:
        make_command = GNU_MAKE + \
                       " clean_generic_no_built_in -f makefiles.common/auxiliary/Makefile_common"
        if verbose:
            print("Delete all non-project-specific temporary files")
        run_command(option_config, make_command)
        return

    game_dir = params[1]

    project_type = project_category(game_dir)

    parent_dir = project_type + "s"

    if verbose:

        print("Project name: " + game_dir)

    parent_and_game_dir = parent_dir + "/" + game_dir

    if not os.path.exists(parent_and_game_dir):
        print("Project not found!")
        return

    if verbose:
        print("Delete all temporary files including generated assets for '"+game_dir+"'")

    make_command = GNU_MAKE + " clean_no_built_in -f " + \
                   parent_dir+"/"+game_dir+"/Makefile."+game_dir

    run_command(option_config,make_command)


def make(option_config, params):
    tiles(option_config, params)
    rebuild(option_config, params)
    
def assets(option_config, params):
    tiles(option_config, params)
    make_assets_from_tiles(option_config, params)