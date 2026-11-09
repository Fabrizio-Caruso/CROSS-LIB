from print_functions import printc, bcolors, print_shape
from project_functions import project_category
from file_functions import files_in_path
from import_from_source import import_tile, store_tile, compute_shape
from run import run_command

# global extend_algorithm


TARGETS_WITH_XSIZE_6 = {"oric", "atmos", "comx_ntsc", "comx", "pecom", "micro"}
TARGETS_WITH_XSIZE_7 = {"apple2", "apple2enh"}

TARGETS_WITH_YSIZE_6 = {"atari_lynx"}
TARGETS_WITH_YSIZE_9 = {"comx", "micro", "pecom"}
TARGETS_WITH_YSIZE_8 = {"comx_ntsc", "micro_ntsc"} # Necessary to avoid comx and micro


def detect_xsize(target):
    for target_6_xsize in TARGETS_WITH_XSIZE_6:
        if target.startswith(target_6_xsize):
            return 6
    for target_7_xsize in TARGETS_WITH_XSIZE_7:
        if target.startswith(target_7_xsize):
            return 7
    return 8


# TODO: VG-5000 has 8x10 characters but it is currently treated as 8x8
# TODO: Cidelsa has 8x6 (rotated 6x8 in reality)
# comx_ntsc : 6x8
# comx (pal): 6x9
def detect_ysize(target):
    for target_8_ysize in TARGETS_WITH_YSIZE_8: # Necessary to have this before size 9 for NTSC case (comx, micro)
        if target.startswith(target_8_ysize):
            return 8
    for target_6_ysize in TARGETS_WITH_YSIZE_6:
        if target.startswith(target_6_ysize):
            return 6
    for target_9_ysize in TARGETS_WITH_YSIZE_9:
        if target.startswith(target_9_ysize):
            return 9
    return 8



# Import a shape as single tile from a text file that describes its shape with characters
def tile(option_config, params):

    tile,xsize,ysize = import_tile(params[1])

    if option_config.terminal_config.verbose:
        print_shape(option_config, compute_shape(tile,xsize))

    if len(params)>=3:
        store_tile(option_config, params[2], tile, xsize, ysize, params[3])
    elif not option_config.terminal_config.test:
        printc(option_config, bcolors.OKCYAN, tile+"\n")


# Import as tiles all shapes from files named "shape<number>.txt" inside the directories in the "shapes" directory of a given project
def tiles(option_config, params):
    verbose = option_config.terminal_config.verbose
    project_name = params[1]

    project_cat = project_category(project_name)

    if len(params)>3:   # xsize, ysize parameters
        xsize_str,ysize_str = params[2:]
    elif len(params)==3: # target parameter
        detected_xsize = detect_xsize(params[2])
        detected_ysize = detect_ysize(params[2])
        xsize_str = str(detected_xsize)
        ysize_str = str(detected_ysize)
    else:                # no parameter
        xsize_str = "8"
        ysize_str = "8"
    if verbose:
        print("xsize: " + xsize_str)
        print("ysize: " + ysize_str)
    path_to_files = project_cat + "s/" + params[1] + \
                    "/shapes/" + xsize_str + "x" + ysize_str + "/"
    if verbose:
        print("Path to shape files: " + path_to_files)
    shape_files = files_in_path(path_to_files)
    if verbose:
        print("shape files: ", shape_files)
    for shape_file in shape_files:
        if verbose:
            print("processing file: ", shape_file)

        path_to_shape_file = path_to_files + str(shape_file)
        if verbose:
            print("processing path file: ", path_to_shape_file)
        tile_number_str = shape_file[5:].replace(".txt","")
        
        if verbose:
            print("tile number: " + tile_number_str)

        tile,xsize,ysize = import_tile(path_to_shape_file)

        if str(xsize)!=xsize_str or str(ysize)!=ysize_str:
            print("Shape in file " + path_to_shape_file + " has wrong dimensions!")
            print(tile)
            return
        
        if not option_config.terminal_config.test:
            print(tile)
        if verbose:
            print_shape(option_config, compute_shape(tile,xsize))

        store_tile(option_config, project_name, tile, xsize, ysize, tile_number_str)
        if verbose:
            print("")


def make_assets_from_tiles(option_config, params):
    GNU_MAKE = option_config.build_config.gnu_make
    game_dir = params[1]

    project_type = project_category(game_dir)
    parent_dir = project_type + "s"

    run_command(option_config, GNU_MAKE+" GAME_NAME=" + game_dir + \
                          " PARENT_DIR=" + parent_dir + \
                          " -f ./makefiles.common/auxiliary/Makefile_assets")

