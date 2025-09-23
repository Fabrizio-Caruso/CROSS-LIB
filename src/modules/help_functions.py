from __future__ import print_function


import os

from params import COMMANDS_LIST, COMMANDS_DESCRIPTIONS, commands, full_params
from import_from_source import printc, bcolors
from file_functions import *
from input_functions import generic_input

import time
import sys


def list_of_documented_routines():
    manual_files = files_in_path("../docs/manual/")
    documented_routines = []
    for manual_file in manual_files:
        documented_routines.append(manual_file.replace(".txt",""))
    return documented_routines

def list_documented_routines():
    documented_routines = list_of_documented_routines()
    for documented_routine in documented_routines:
        print(documented_routine)

def manual(option_config, params):
    if len(params)==1:
        printc(option_config, bcolors.OKCYAN, "DOCUMENTED ROUTINES\n")
        list_documented_routines()
        print("")
        print("Use")
        printc(option_config, bcolors.BOLD,"xl manual <routine>\n")
        return
    if params[1].startswith("_XL") or params[1] in ["XSize", "YSize"]:
        command_string = "cat ../docs/manual/" + params[1] + ".txt"
    else:
        command_string = "cat ../docs/manual/_XL_" + params[1] + ".txt"
    res = os.system(command_string)
    if res:
        printc(option_config, bcolors.WARNING,"\nCommand/topic not found\n")


def help_help(option_config):
    print("Possible values for <command>:")
    print(' '.join(str(cmd) for cmd in COMMANDS_LIST))
    print("")
    printc(option_config, bcolors.BOLD,"xl help <command or routine>\n")
    print("for <command>-specific help")
    print("\nExample:")
    print("\nxl help create              \n  It displays the help page for the 'create' xl script command")
    print("")
    print("\nxl help _XL_DRAW              \n  It displays the help page for the '_XL_DRAW' C routine")
    print("")
    printc(option_config, bcolors.OKCYAN,"POSSIBLE COMMANDS")
    print("")
    commands(option_config)
    print("\nUse")
    printc(option_config, bcolors.BOLD,"xl help manual")
    print("\nto get the list of documented C routines.")
    print("\nUse")
    printc(option_config, bcolors.BOLD,"xl manual")
    print("\nto get the full manual for the C routines.")
    


def help_command(option_config, params):
    if len(params)<2:
        printc(option_config, bcolors.BOLD,"\nxl <command> <[optional] parameters>")
        print("\n(or xl <project> <[optional] parameters> as a shorthand for \n xl build <project> <[optional] parameters>)")
        print("")
        help_help(option_config)
    elif params[1] in COMMANDS_DESCRIPTIONS.keys():

        print("Help on ", end="")
        printc(option_config, bcolors.OKCYAN, params[1]+"\n")
        print("It " + COMMANDS_DESCRIPTIONS[params[1]] + "\n")
        params = full_params(params)
        if params[1]=="assets":
            printc(option_config, bcolors.BOLD,"\nxl assets <project>\n")
            print("")
            print("It generates assets files for <project> (new or existing project) by using tile or shape files.")
            print("")
            print("Example:")
            print("Edit any shape file in ./projects/foo/shapes/8x8/")
            print("xl assets foo")
            print("xl show foo (to check the result)")
            print("The output will be in ./projects/foo/generated_assets/")

        elif params[1]=="split":
            printc(option_config, bcolors.BOLD,"\nxl split <file>\n")
            print("")
            print("It displays a shape file as two shapes, one for the left and one for the right side.")
            print("Example:")
            print("xl split games/shuriken/docs/16x12_multi_tile.txt")
            print("")
            print("...#####")
            print(".###....")
            print("##.....#")
            print("#..#####")
            print("#...#..#")
            print("#...####")
            print("##..#...")
            print(".###....")
            print("...#####")
            print("........")
            print("........")
            print("........")
            print("")
            print("#.......")
            print("###.....")
            print("..##....")
            print("...#....")
            print("...#....")
            print("#..#....")
            print("..##....")
            print("###.....")
            print("#.......")
            print("........")
            print("........")
            print("........")

        elif params[1]=="config":
            printc(option_config, bcolors.BOLD,"\nxl config\n")
            print("")
            print("It displays the configuration as read from config.ini")
            print("Example:")
            print("xl config")
        elif params[1]=="extend":
            printc(option_config, bcolors.BOLD,"\nxl extend <project>\n")
            print("")
            print("It generates candidate shapes of different sizes from the 8x8 shapes of <project>.")
            print("Example:")
            print("In order to generate candidate shapes for foo using 8x8 shapes, you can do:")
            print("xl extend foo")
        elif params[1]=="make":
            printc(option_config, bcolors.BOLD,"\nxl make <project> <target>\n")
            print("")
            print("It re-builds <project> for <target> and its assets by using the pictorial text files in the shapes directory.")
            print("It is equivalent to:")
            print("xl tiles <project> <target>")
            print("xl rebuild <project> <target>")
            print("")
            print("Example:")
            print("xl make foo vic20")
            print("builds the project foo and its assets from the pictorial text file for the Vic 20")
        elif params[1]=="tools":
            printc(option_config, bcolors.BOLD,"\nxl tools\n")
            print("\n(or xl build tools)")
            print("")
            print("It builds some post-processing tools from their source code.")
            print("")
            print("This command has to be run only once to build the tools. The tools are used by few targets to generate ready to use images.")
        elif params[1]=="rename":
            printc(option_config, bcolors.BOLD,"\nxl rename <old project name> <new project name>\n")
            print("It renames an existing user-defined project <old project name> to <new project name>.")
            print("")
            print("Remark: No matter the type of source project, the target project will be in the 'src/projects' directory")
            print("")
            print("Example:")
            print("If you have previously created a new project 'foo'")
            print("(e.g., by cloning it with 'xl clone bomber foo'),")
            print("then you can rename it to 'bar' with")
            print("xl rename foo bar")
        elif params[1]=="clone":
            printc(option_config, bcolors.BOLD,"\nxl clone <source project> <target project>\n")
            print("It clones an existing project of any type to create a new user-defined project.")
            print("")
            print("Remark: No matter the type of source project, the target project will be in the 'src/projects' directory")
            print("")
            print("Example:")
            print("If you want to clone the built-in game Cross Horde you can just do:")
            print("xl clone horde foo")
            print("")
            print("If you have previously created a user-defined project 'foo',")
            print("you can further clone it with:")
            print("xl clone foo bar")
        elif params[1]=="show":
            printc(option_config, bcolors.BOLD,"xl show <project> <[optional] TileIndex> <[optional] XTileSize> <[optional] YTileSize> .\n")
            print("It displays the shape of graphics tiles of a given project.")
            print("")
            print("<project>")
            print("Use this mandatory parameter to specify the project whose tiles you want to display")
            print("")
            print("<TileIndex>")
            print("This optional parameter specifies which tile we want to display.")
            print("When no index is specified, then all tiles of the given shape are displayed.")
            print("")
            print("<XTileSize> <YTileSize>")
            print("These optional parameters specify the size of the tiles we want to see")
            print("")
            print("Example:")
            print("xl show chase 5 8 8")
            print("Decoding file tile: ./games/chase/tiles/8x6/tile5.txt")
            print("")
            print(".######.")
            print("#.#..#.#")
            print("#..##..#")
            print("#..##..#")
            print("#.#..#.#")
            print(".######.")


        elif params[1]=="size":
            printc(option_config, bcolors.BOLD,"xl size <project> <XSize> <YSize>\n")
            print("It builds <project> for the native host with screen size provided by <XSize> and <YSize>.")
            print("The built binaries will be in the 'build' directory.")

            print("\n<project>")
            print("<project> can also be 'games'/'examples'/'projects'/'all' to build multiple projects.")

            print("\nxl size bomber 20 20       \n  It builds Cross Bomber for the native host with screen size 20x20.")
            print("\nxl size examples 16 12     \n  It builds all examples for the native host with screen size 16x12.")

        elif params[1]=="run":
            printc(option_config, bcolors.BOLD,"xl run <project> <[optional] xsize> <[optional] ysize>\n")
            printc(option_config, bcolors.BOLD,"xl run <project> <[optional] target>\n")
            print("If no target is specified, it runs the previously compiled native version of <project>.")
            print("If integer parameters <xsize> and <ysize> are provided, then it runs the version of those sizes.")
            print("")
            print("<project>")
            print("This parameter is the name of the project that we want to run.")
            print("")
            print("<xsize> <ysize>")
            print("These parameters are the size of the native version that we want to run.")
            print("")
            print("<target>")
            print("This parameter is the target for which we want to run <project>.")
            print("This parameter only works for a restricted set of targets and it requires an emulator.")
            print("")
            print("Examples:")
            print("xl run snake")
            print("It runs the previously built (e.g., with 'xl build snake') native version of Cross Snake.")
            print("")
            print("xl run snake 16 16")
            print("It runs the previously built with size 16X16 (with 'xl size snake 16 16') native version of Cross Snake.")
        elif params[1]=="manual":
            printc(option_config, bcolors.BOLD,"xl manual <[optional] routine>\n")
            print("If no optional parameter is passed it displays a list of documented CROSS-LIB APIs.")
            print("If a Cross-Lib C rouine is passed as an optional parameter, it displays its description and usage")
            print("")
            print("Documented commands:")
            list_documented_routines()
        elif params[1]=="string":
            printc(option_config, bcolors.BOLD,"xl string <string>\n")
            print("It converts a string litteral into a concatenation of")
            print('_XL_A,..., _XL_Z, _XL_a, ..., _XL_z, _XL_SPACE, "0", ..., "9"')
            print("<string>")
            print("This parameter is the string to convert. It has to match the regular expression '[A-Za-z0-9 ]*'.")
            print("")
            print("Example:")
            print('xl string "1 Hello World 2"')
            print('"1" _XL_SPACE _XL_H _XL_e _XL_l _XL_l _XL_o _XL_SPACE _XL_W _XL_o _XL_r _XL_l _XL_d _XL_SPACE "2"')
        elif params[1]=="rebuild":
            printc(option_config, bcolors.BOLD,"xl rebuild <project> <[optional] target>\n")
            print("It rebuilds <project>.")
            # print("Remark: The 'build' command can be omitted.")

            print("It is equivalent to 'xl reset' followed by 'xl build <project> <target>'")
            print("Use 'xl help reset' and 'xl help build' for more information")
        elif params[1]=="import":
            printc(option_config, bcolors.BOLD,"xl import <source_file> <[optional] project>\n")
            print("")
            print("<source_file>")
            print("It is an Assembly or BASIC file (e.g., an Assembly file exported from CharPad or VChar64).")
            print("For example in CharPad you can export the tile data with:")
            print("File->Import/Export->Text/Asm->Export All or File->Import/Export->Text/Asm->Export All")
            print("")
            print("<project>")
            print("If a project name is passed then the tiles are imported into <project> as 8x8 tiles")
            print("")
            print("Remark: For extra heuristics useful on some BASIC files use 'xl rip'")
            print("")
            print("Example:")
            print("If you create a new project 'myname' with")
            print("xl create myname")
            print("You can import a tile into it from an Assembly file with something like this:")
            print("xl import ./assets/examples/tile_sets/asm/tile_8x6_shapeA.txt myname")
        elif params[1]=="rip":
            printc(option_config, bcolors.BOLD,"xl rip <source_file> <[optional] project>\n")
            print("")
            print("<source_file>")
            print("It is an Assembly or BASIC file (e.g., an Assembly file exported from CharPad or VChar64).")
            print("For example in CharPad you can export the tile data with:")
            print("File->Import/Export->Text/Asm->Export All or File->Import/Export->Text/Asm->Export All")
            print("")
            print("<project>")
            print("If a project name is passed then the tiles are imported into <project> as 8x8 tiles")
            print("")
            print("Example:")
            print("If you create a new project 'myname' with")
            print("xl create myname")
            print("You can import a tile into it from an Assembly file with something like this:")
            print("xl rip ./assets/examples/tile_sets/asm/tile_8x6_shapeA.txt myname")
        elif params[1]=="info":
            printc(option_config, bcolors.BOLD,"xl info <target>/<project>\n")
            print("")
            print("If the parameter is a target, it displays some information on the target <target> and how it is supported in `terminal` mode.") 
            print("")
            print("If the parameter is a project, it displays some information on the project <project>.")
        elif params[1]=="rotate":
            printc(option_config, bcolors.BOLD,"xl rotate <source_file> <[optional] project>\n")
            print("")
            print("It works similarly to 'xl import <source> <[optional] project>' but rotates the result.")
            print("")
            print("<source_file>")
            print("It is an Assembly or BASIC file (e.g., an Assembly file exported from CharPad or VChar64).")
            print("For example in CharPad you can export the tile data with:")
            print("File->Import/Export->Text/Asm->Export All or File->Import/Export->Text/Asm->Export All")
            print("")
            print("<project>")
            print("If a project name is passed then the tiles are imported into <project> as 8x8 tiles")
            print("")
            print("Example:")
            print("If you create a new project 'myname' with")
            print("xl create myname")
            print("You can import a tile into it from an Assembly file with something like this:")
            print("xl rotate ./modules/examples/tile_sets/asm/tile_8x6_shapeA.txt myname")
        elif params[1]=="tile":
            printc(option_config, bcolors.BOLD,"xl tile <shape_file> <[optional] project> <[optional] tile_index>\n")
            print("It converts the pictorial text file file <shape_file> into a line that can be used as an asset file.")
            print("")
            print("<shape_file>")
            print("The file <shape_file> describes with '#' and '.' the shape of a tile.")
            print("")
            print("<project>")
            print("This optional parameter specifies the project whose tile we want to modify")
            print("")
            print("<tile_index>")
            print("This parameter is the index of the tile we want to modify")

            print("\nPress ENTER to continue...")
            time.sleep(1)

            generic_input("")

            print("\nExample in 'src/examples/tiles/tile_shape0.txt':")
            print("...##...")
            print("..#..#..")
            print("...##...")
            print(".##..##.")
            print("#.####.#")
            print("# ####.#")
            print("..#..#..")
            print("..#..#..")
            print("")
            print("xl tile ./modules/examples/single_tiles/tile_shape0.txt")
            print("produces: ")
            print("24,36,24,102,189,189,36,36")
            print("")
            print("To be copied in 'tile_<index>.txt' in '<project>/tiles/8x8' to modify the shape.")
            print("Remark: run 'xl reset <project>' before rebuilding <project> with modified tiles.")

        elif params[1]=="tiles":
            printc(option_config, bcolors.BOLD,"xl tiles <project> <[optional] xsize> <[optional] ysize>\n")
            printc(option_config, bcolors.BOLD,"xl tiles <project> <[optional] target>\n")
            print("It converts all shape files into tiles and stores them as such in the project.")
            print("It imports from files named 'shape<number>.txt' inside the directories in the 'shapes' directory of a given project")
            print("The shape file format is the one used by 'xl tile'. Run 'xl help tile' for more information.")
            print("If no optional parameter is passed, then 8x8 shape is assumed")
            print("If a target is passed as parameter it will guess xsize and ysize for that target.")
            print("")
            print("Example: ")
            print("Provided that you have a project named 'foo', the command")
            print("xl tiles foo 6 8")
            print("will import as tiles from all shape files 'shape<number>.txt' (found in './projects/foo/shapes/6x8') into './projects/foo/tiles/6x8/'")

        elif params[1]=="self":
            printc(option_config, bcolors.BOLD,"xl self\n")
            print("It tests itself by performing a sequence of 'xl' commands.")
            print("")
        elif params[1]=="unit-tests":
            printc(option_config, bcolors.BOLD,"xl self\n")
            print("It runs unit-tests on the script code.")
        elif params[1]=="compilers":
            printc(option_config, bcolors.BOLD,"xl self\n")
            print("It checks the presence of the most common compilers.")
        elif params[1]=="check":
            printc(option_config, bcolors.BOLD,"xl check <[optional] params>\n")
            print("It runs some check on the dependencies.")
            print("")
            print("<params>")
            print("If nothing is passed to <params>, then it performs several checks.")
            print("If <params> is 'compilers', then it checks the presence of the most common compilers.")
            print("If <params> is 'tools', then it checks the presence of the most common tools.")
            print("If <params> is 'emulators', then it checks the presence of the emulators used by xl.")
            print("If <params> is 'interpreters', then it checks the presence of the interpreters used by xl.")
            print("If <params> is 'libraries', then it checks the presence of the libraries used by xl.")
        elif params[1]=="test":
            printc(option_config, bcolors.BOLD,"xl test <[optional] params> <[optional] target>\n")
            print("It runs some operations to test 'xl'.")
            print("")
            print("<params>")
            print("If nothing is passed to <params>, then it performs several tests including the self-test.")
            print("If 'self' is passed to <params>, then it tests itself by performing a sequence of 'xl' commands.")
            print("If <params> is 'compilers', then it checks the presence of the most common compilers.")
            print("If <params> is 'tools', then it checks the presence of the most common tools.")
            print("If <params> is 'emulators', then it checks the presence of the emulators used by xl.")
            print("If <params> is 'interpreters', then it checks the presence of the interpreters used by xl.")
            print("If <params> is 'libraries', then it checks the presence of the libraries used by xl.")
            print("If <params> is a game/example/project, it checks if a binary for <target> (native if no <target>), can be built for the project.")
            print('If <params> is "games" or "examples", it checks if binaries for <target> (native if no <target>) can be built for all games/examples.')
            print("If 'cc65', 'z88dk, 'cmoc', or 'lcc1802' is passed to <params>, it compiles a test program using the corresponding compiler.")
            print("If 'z88dk_alt' is passed to <params>, then it compiles a simplified test program using both Z88DK compilers.")
            print("If 'unit-tests' is passed to <params>, then it runs unit-tests on the script code.")
        elif params[1]=="files" or params[1]=="f":
            printc(option_config, bcolors.BOLD,"xl files\n")
            print("It shows the built binary files (the conent of the `build` directory).\n")
            print("Remark: 'xl files' can be shorten with `xl f`.")
        elif params[1]=="slow":
            printc(option_config, bcolors.BOLD,"xl slow <project> <target> <slowdown>\n")
            print("It builds <project> for <target> with (total) slowdown given by <slowdown>")
            print("\nExamples:")
            print("\nxl slow horde vic20 800     \n  It builds Horde for the Commodore Vic 20 using CC65 with (total) slowdown equal to 800.")
        elif params[1]=="build":
            printc(option_config, bcolors.BOLD,"xl build <project> <[optional] target> or xl <project> <[optional] target>\n")
            print("It builds <project> for <target>.")
            print("The built binaries will be in the 'build' directory.")

            print("\n<project>")
            print("<project> can also be 'games'/'examples'/'projects'/'all' to build multiple projects")

            print("\n<target>")
            print("REMARK 1: If no <target> is passed, then the native target 'ncurses' (terminal console with ASCII tiles) is considered.")
            print("REMARK 2: If 'terminal' or 'terminal<N><M>' is passed as target, then the native terminal 'graphics' is considered with tiles with NxM pixels.\nThis target requires you to reduce the font size to discern the graphics.")
            print("The 'terminal<N><M>' keyword  can be followed by <xsize> and <ysize> to specify the screen shape.")
            print("\nIf '<dev-kit>_targets' is passed as <target> (e.g., 'cc65_targets'), \nthen the given project/s is/are built for all targets that use <dev-kit> to be compiled.")
            print("Possible dev-kits are: 'cc65', 'z88dk', 'cmoc', 'lcc1802'.")
            print("\n[NOT recommended] If 'all' is passed as <target>, then the given project/s is/are built for all targets (it may take very long and it requires all supported compilers.")

            print("\nPress ENTER to continue...")
            time.sleep(1)

            generic_input("")

            print("\nExamples:")
            print("\nxl build bomber vic20       \n  It builds Cross Bomber for the Commodore Vic 20 using CC65.")
            print("\nxl snake                    \n  It builds Cross Snake for the native target (terminal console).")
            print("\nxl chase cc65_targets       \n  It builds Cross Chase for all targets that use CC65 to be built.")
            print("\nxl games cpc                \n  It builds all games for the Amstrad CPC using Z88DK.")
            print("\nxl examples c64             \n  It builds all examples for the Commodore 64 using CC65.")
            print("\nxl horde all                \n  It builds Cross Horde for all its supported targets using all supported necessary compilers.")
            print("\nxl projects all             \n  It builds all built-in projects for all supported targets using all supported necessary compilers.")
            print("\nxl all c16                  \n  It builds all projects (games and examples and user-defined projects) for the Commodore 264 series using CC65.")
        elif params[1]=="create":
            printc(option_config, bcolors.BOLD,"xl create <project> <[optional] type>\n")
            print("It creates <project>.")

            print("\n<type>")
            print("If no <type> is passed, then the initial code will just display 'hello world'")
            print("If 'game' is passed as <type>, then the project is build with some initial template game code.")
            print("If 'apis' is passed as <type>, then the project is build with some code that shows how to use all APIs.")

            print("\nExamples:")
            print("\nxl create foo               \n  It builds a new project 'foo' with some initial code that display 'hello world' on the screen.")
            print("\nxl create bar game          \n  It builds a new project 'bar' with some initial game code (main loop, level loop, etc.).")
        elif params[1]=="delete":
            printc(option_config, bcolors.BOLD,"xl delete <project> <[optiona] interactive>\n")
            print("It removes <project>, i.e., it deletes its folder with its content (source code, graphics assets, makefile).")

            print("\n<project>")
            print("<project> cannot be a built-in project.")

            print("\n<interactive>")
            print("If '-y' is passed as <interactive>, then the command won't ask for confirmation.")

            print("\nExample:")
            print("\nxl delete foo               \n  It deletes the project 'foo'.")
            print("\nxl delete foo -y            \n  Same as above but no confirmation is asked.")
        elif params[1]=="reset":
            printc(option_config, bcolors.BOLD,"xl reset <[optional] project>\n")
            print("It deletes temporary files created during the build process.")
            print("\n<project>")
            print("If no <project> is passed, only non-project specific temporary files are deleted.")
            print("If the <project> parameter is used, then also project-specific temporary files are deleted (and in particular generated graphics assets).")


            print("\nExamples:")
            print("\nxl reset                    \n  It deletes non-project specific temporary files.")
            print("\nxl reset foo                \n  It deletes all temporary files (both generic and project-specific).")
        elif params[1]=="clean":
            printc(option_config, bcolors.BOLD,"xl clean <[optional] project>\n")
            print("It deletes both built binaries and temporary files created during the build process.")
            print("\n<project>")
            print("If no <project> is passed, only built binaries and non-project specific temporary files are deleted.")
            print("If the <project> parameter is used, then also project-specific temporary files are deleted (e.g., generated graphics assets).")

            print("\nExamples:")
            print("\nxl clean                    \n  It deletes all built binaries and non-project specific temporary files.")
            print("\nxl clean foo                \n  It deletes all built-in binaries and all temporary files (both generic and project-specific).")

        elif params[1]=="list" or params[1]=="l":
            printc(option_config, bcolors.BOLD,"xl list <[optional] params>\n")
            print("It lists current projects in a given category or all projects.")
            print("")
            print("<params>")
            print("If nothing is passed as <params> then all projects are built")
            print("If 'games','examples' or 'projects' is passed as <params> then only projects in the respective directory are listed")
            print("\nExamples:")
            print("xl list                       \n  It lists all projects (games, examples and new projects)")
            print("xl list projects              \n  It lists all user-defined projects")

        elif params[1]=="commands":
            printc(option_config, bcolors.BOLD,"xl commands\n")
            print("It displays the available commands.")

        elif params[1]=="help" or params[1]=="h":
            printc(option_config, bcolors.BOLD,"xl help <[optional] command>\n")
            print("It displays help instructions.")


            print("\n<command>")
            help_help(option_config)
    elif params[1] in list_of_documented_routines():
        manual(option_config, params)

    else:
        print("Command not recognized")
        help_help(option_config)

