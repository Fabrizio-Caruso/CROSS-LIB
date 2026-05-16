import os 
import sys

from project_functions import project_category
from file_functions import files_in_path
from print_functions import display_shape, print_shape
from init import NUMBER_OF_TILES
# from import_from_source import import_split_tiles


# General strategies:
# xsize or ysize < 8 -> cut left/right or top/bottom columns/rows
# ysize > 8          -> repeat top/bottom rows 
def resize_shape(tile_vect,xsize,ysize, extend_algorithm):

    if xsize==7:
        tile_vect = [vect[:-1] for vect in tile_vect]
    if xsize==6:
        tile_vect = [(vect[:-1])[1:] for vect in tile_vect]

    if(ysize==6):
        tile_vect = (tile_vect[:-1])[1:]

    # TODO: Implement zero
    if extend_algorithm=="duplicate":
        if(ysize==9):
            tile_vect = tile_vect + [ tile_vect[-1]]
        if(ysize==10):
            tile_vect = tile_vect[0:1] + tile_vect + [ tile_vect[-1]]
    elif extend_algorithm=="zero":
        zero_row = [ "." * xsize ]
        if(ysize==9):
            tile_vect = tile_vect + zero_row
        if(ysize==10):
            tile_vect = zero_row + tile_vect + zero_row
    return tile_vect



def trim_newline_from_shape(lines):
    tile = ""

    filtered_lines = []
    for line in lines:
        if not(line=="\n" or line=="\r" or line== "\r\n"):
            filtered_lines.append(line.replace('\n','').replace('\r',''))

    xsize = 8
    for line in filtered_lines:
        xsize = min(xsize,len(line))

    trimmed_lines = []
    for line in filtered_lines:
        trimmed_lines.append(line[:xsize])
    return trimmed_lines,xsize,len(trimmed_lines)


def read_shape(file_name):
    fin = open(file_name, "rt")
    lines = fin.readlines()

    # print(lines)

    trimmed_lines = trim_newline_from_shape(lines)
    fin.close()
    return trimmed_lines

# It returns the path to the shape directory for a given project and dimension
def path_to_shapes(project, xsize, ysize):

    project_cat = project_category(project)
    # print(project_cat)
    return project_cat + "s/" + project + "/shapes/" + str(xsize) + "x" + str(ysize) + "/"


# replace_shapes:
# 0 -> write in _shapes directory,
# 1 -> write in shapes directory but avoid overwriting existing shapes or tiles
def write_shapes(option_config, project, shapes, xsize,ysize):
    verbose = option_config.terminal_config.verbose
    replace_shapes = option_config.extend_config.replace_shapes
    path = path_to_shapes(project,xsize,ysize)
    if not replace_shapes:
        path = path.replace("shapes","_shapes")
    if verbose:
        print("Writing shapes in: " + path)
    if os.path.exists(path):
        if verbose:
            print("Directory exists")
    else:
        if verbose:
            print("Directory does not exist")
        os.makedirs(path)
    for tile_number, shape in shapes.items():
        # path_to_shape = path+"shape"+tile_number+".txt"
        path_to_shape = path+"shape_"+format(int(tile_number),"03d") + ".txt"

        if verbose:
            print("Path to shape file: " + path_to_shape)
        path_to_tile = path.replace("shapes","tiles")+"tile"+tile_number+".txt"
        if verbose:
            print("Path to tile file: " + path_to_tile)
        if replace_shapes and (os.path.exists(path_to_shape) or \
           os.path.exists(path_to_tile)):
            if os.path.exists(path_to_shape):
                if verbose:
                    print("shape "+ tile_number +" already exists")
            elif os.path.exists(path_to_tile):
                if verbose:
                    print("tile "+ tile_number +" already exists")
        else:
            with open(path_to_shape,"w") as file:
                for row in shape:
                    if verbose:
                        print(row)
                    file.write(row+"\n")
        if verbose:
            print("")


# Extand 8x8 shapes by reshaping them into shapes with other dimensions
def extend(option_config, params):
    verbose = option_config.terminal_config.verbose
    extend_algorithm = option_config.extend_config.extend_algorithm
    project = params[1]

    path_to_8x8_files = path_to_shapes(project,8,8)
    if verbose:
        print("Path to shape files: " + path_to_8x8_files)
        print("")
    shape_files = files_in_path(path_to_8x8_files)

    dict7x8 = {}
    dict6x8 = {}
    dict8x6 = {}
    dict6x9 = {}
    for shape_file in shape_files:
        path_to_shape_file = path_to_8x8_files + "/" + shape_file

        if verbose:
            print("shape file: " + shape_file)

        tile_number_str = shape_file[5:].replace(".txt","")

        if verbose:
            print("---tile number: " + tile_number_str)

            print("")

        tile,xsize,ysize = read_shape(path_to_shape_file)
        if xsize!=8 or ysize!=8:
            print("Shape in file " + path_to_shape_file + " has wrong dimensions!")
            print(tile)
            return

        if verbose:
            print(tile)

        if verbose:
            print("")
            print("7x8")
        shape7x8 = resize_shape(tile,7,8,extend_algorithm)
        if verbose:
            display_shape(option_config, shape7x8)
        dict7x8[tile_number_str]=shape7x8

        if verbose:
            print("")
            print("6x8")
        shape6x8 = resize_shape(tile,6,8,extend_algorithm)
        if verbose:
            display_shape(option_config, shape6x8)
        dict6x8[tile_number_str]=shape6x8

        if verbose:
            print("")
            print("8x6")
        shape8x6 = resize_shape(tile,8,6,extend_algorithm)
        if verbose:
            display_shape(option_config, shape8x6)
        dict8x6[tile_number_str]=shape8x6

        if verbose:
            print("")
            print("6x9")
        shape6x9 = resize_shape(tile,6,9,extend_algorithm)
        if verbose:
            display_shape(option_config, shape6x9)
        dict6x9[tile_number_str]=shape6x9
        if verbose:
            print("")
    # print(str(dict7x8))
    write_shapes(option_config, project,dict7x8,7,8)
    write_shapes(option_config, project,dict6x8,6,8)
    write_shapes(option_config, project,dict8x6,8,6)
    write_shapes(option_config, project,dict6x9,6,9)
    return


# Show tile shapes of a given project
def show(option_config, params):
    parent_dir = project_category(params[1])+"s"

    if len(params)<3:
        xsize = "8"
        ysize = "8"
        index = ""
    elif len(params)<4:
        index = params[2]
        xsize = "8"
        ysize = "8"
    elif len(params)==4:
        xsize = params[2]
        ysize = params[3]
        index = ""
    else:
        xsize = params[3]
        ysize = params[4]
        if len(params)>=5:
            index = params[2]
        else:
            index = ""


    if index!="":
        print_shape_from_file(option_config, parent_dir, params[1], xsize, ysize, index)

    else:
        for i in range(NUMBER_OF_TILES):
            if not option_config.terminal_config.test:
                print(i)
            print_shape_from_file(option_config, parent_dir, params[1], xsize, ysize, i)


# It computes the shape from the tile file (not directly from the shapes if available)
def print_shape_from_file(option_config, parent_dir, project_name, xsize, ysize, index):
    dir = xsize+"x"+ysize
    dest = "./" + parent_dir + "/" + project_name + "/tiles/" + dir + "/tile" + str(index) + ".txt"
    
    if option_config.terminal_config.verbose:
        print("Decoding file tile: " + dest)
        print("")
    try:
        if os.path.exists(dest):
            fin = open(dest, "rt")

            tile_data = fin.read()
            # computed_shape = compute_shape(tile_data,xsize)
            # print("computed shape: " + str(computed_shape))
            print_shape(option_config, compute_shape(tile_data,xsize))
        else:
            print("NOT FOUND")
    except Exception as error:
        print("File skipped")
        print("An exception occurred:", type(error).__name__, error)
        exc_type, exc_obj, exc_tb = sys.exc_info()
        fname = os.path.split(exc_tb.tb_frame.f_code.co_filename)[1]
        print(exc_type, fname, exc_tb.tb_lineno)

# It outputs a list of strings
def compute_shape(string, xsize):
    string_items = string.split(",")
    items = []
    values = []
    for string_item in string_items:
        string_item = string_item.replace("$","0x").replace(" ","")
        if string_item.startswith("0x"):
            base = 16
        elif string_item.startswith("@"):
            string_item = string_item[1:]
            base = 2
        else:
            base = 10
        value = int(string_item,base)
        values.append(value)
        bin_string=bin(value)[2:]
        missing_zeros = int(xsize) - len(bin_string)
        padded_bin_string = ""
        for i in range(missing_zeros):
            padded_bin_string += "0"
        padded_bin_string+=bin_string

        padded_bin_string = padded_bin_string.replace("0",".").replace("1","#")

        items.append(padded_bin_string)
        # print("items :" + str(items))
    return(items)


# It takes the output of print_shape
def compute_rotated_shape(items):
    # print(str(items))
    # return(items)
    xsize = len(items[0])
    ysize = len(items)
    val = [0]*xsize
    for j in range(xsize):
        tmp = 0
        for i in range(ysize):
            if(items[i][j]=='#'):
                tmp+=2**i
        val[j] = tmp

    str_res = []
    for i in range(xsize):
        str_res.append(val[i])

    return(str_res)
