from __future__ import print_function

import sys
import os

from print_functions import *
from project_functions import project_category
from shape_functions import read_shape, compute_shape, compute_rotated_shape
from init import NUMBER_OF_TILES



# Patterns used to rip and import tile data
ONE_REPPRESENTATIONS = ["#","*","X","x","1","+","@","$","O","o"]
ASSEMBLY_EXTENSIONS = [".s",".asm",".a99",".as", ".acme"]
BASIC_EXTENSIONS    = [".bas", ".lb", ".b", ".lst"]
BYTE_PATTERN_LIST = [".byte","!byte","defb","fcb","db","dc.b","byte",".BYTE","!BYTE","DEFB","FCB","DB","DC.B","BYTE","!Byte",".Byte"]
WORD_PATTERN_LIST = [".word","!word","defw","fcw","dw","dc.w","data",".WORD","!WORD","DEFW","FCW","DW","DC.W","DATA","!Word",".Word"]
ASSEMBLY_PATTERN_LIST = BYTE_PATTERN_LIST + WORD_PATTERN_LIST
BASIC_ONLY_NO_SKIP_PATTERN_LIST = ["dA", "BIN", "d.", "D."]
BASIC_ONLY_SKIP_PATTERN_LIST = [ "symbol", "SYMBOL", "vdu 23,", "VDU 23,", "vdu23,", "VDU23,"]
BASIC_ONLY_PATTERN_LIST = BASIC_ONLY_NO_SKIP_PATTERN_LIST + BASIC_ONLY_SKIP_PATTERN_LIST
PATTERN_LIST = ASSEMBLY_PATTERN_LIST + BASIC_ONLY_PATTERN_LIST
SKIP_PATTERN_LIST = BASIC_ONLY_SKIP_PATTERN_LIST


# Detect Assembly extension
def has_extension(string, patterns):
    for i in range(len(patterns)):
        if string.endswith(patterns[i]) or string.endswith(patterns[i].upper()):
            return True
    return False


# BASIC if nothing is found between commas in BASIC than insert a "0" character
def fill_empty_slots_with_zero(data_list):
    new_list = []
    for i in range(len(data_list)):
        if data_list[i]=="":
            new_list.append("0")
        else:
            new_list.append(data_list[i])

    return new_list


# Try to detect hex data without hex prefixes
def has_headless_hex(trimmed_lines):
    if "$" not in str(trimmed_lines) and "&" not in str(trimmed_lines):
        for i in range(len(trimmed_lines)):
            data = trimmed_lines[i].split(",")
            for j in range(len(data)):
                if len(data[j])!=2:
                    return False
        return True
    else:
        return False


def has_nine_byte_lines(trimmed_lines):
    for i in range(len(trimmed_lines)):
        length = len(trimmed_lines[i].split(","))
        if length!=9:
            return False
    return True


def normalize_basic_line(line):
    # Convert hex notation to $
    line = line.replace("&H","$").replace("&h","$")

    # Capture binary BIN UDG in Sinclair BASIC
    line = line.replace("BIN", "\nBIN@")

    if("symbolafter" in line) or ("symbol after" in line):
        line = ""
    if("SYMBOLAFTER" in line) or ("SYMBOL AFTER" in line):
        line = ""

    return line


def normalize_assembly_line(line):
    # Convert hex notation to $
    line = line.replace(">","$").replace("#","$").replace("0x","$").replace("&H","$").replace("&h","$")

    # Convert bin notation to @
    line = line.replace("%","@").replace("0b","@")

    return line


def normalize_line(line, basic_extension, assembly_extension):
    # Remove spaces and new lines
    line = line.replace(" ","").replace("\n","")

    if assembly_extension:
        return normalize_assembly_line(line)
    elif basic_extension:
        return normalize_basic_line(line)
    else:
        return normalize_assembly_line(normalize_basic_line(line))


def remove_basic_comments(line):
    line = line.split(":REM")[0]
    line = line.split(":rem")[0]
    line = line.split(": REM")[0]
    line = line.split(": rem")[0]
    line = line.split(":'")[0]
    line = line.split(": '")[0]
    line = line.split("'")[0]
    line = line.replace("{","").replace('}"',"").replace("}",",").replace('"',"")
    return line


def remove_assembly_comments(line):
    line = line.split(" ;")[0]
    line = line.split(";")[0]
    return line


def display_data_type(word_data, verbose=False):
    if verbose:

        if word_data:
            data_type = "16-bit"
        else:
            data_type = "8-bit"

        print("Data type detected   : " + data_type)


def display_code_type(basic_code, verbose=False):
    if verbose:
        if basic_code:
            code_type = "BASIC"
        else:
            code_type = "Assembly"

        print("Code type            : " + code_type)


def display_extension_type(assembly_extension, basic_code, verbose=False):
    if verbose:

        if assembly_extension:
            extension_type = "Assembly"
        elif basic_code:
            extension_type = "BASIC"
        else:
            extension_type = "Undefined"

        print("\nFile extension type  : " + extension_type)


def is_basic_code(directive,assembly_extension):
    return (directive in BASIC_ONLY_PATTERN_LIST) or ((directive=="data" or directive=="DATA") and not assembly_extension)


# Which if any pattern is the line matching?
def line_pattern(line):
    for i in range(len(PATTERN_LIST)):
        if PATTERN_LIST[i] in line:
            return i

    return -1


def remove_comments(line,basic_code):
    if basic_code:
        line = remove_basic_comments(line)
    else:
        # Remove comments
        line = remove_assembly_comments(line)

    return line


def split(option_config, params):
    import_split_tiles(option_config, params[1])


def rip_tiles(option_config, filename, xsize, ysize, rip = False, rotate = False):

    try:
        fin = open(filename, "rt")

        assembly_extension = has_extension(filename,ASSEMBLY_EXTENSIONS)

        basic_extension = has_extension(filename,BASIC_EXTENSIONS)

        lines = fin.readlines()

        # Only take patten lines if Magellan extension is detected
        if has_extension(filename, ['a99','A99']):
            filtered_lines = []
            for line in lines:
                if line.startswith("PAT"):
                    filtered_lines.append(line)
            lines = filtered_lines

        return aux_rip_tiles(option_config, lines, assembly_extension, basic_extension, xsize, ysize, rip, rotate)
    except ValueError as valueError:
        print(str(valueError.args[0]))
    except Exception as exception:
        print("Sorry! Failed to extract tile data from file: \n" + str(exception.args))


# It rips `xsize` X `ysize` tiles from an Assembly or BASIC source file
def aux_rip_tiles(option_config, lines, assembly_extension, basic_extension, xsize, ysize, rip = False, rotate = False, verbose = False):

        # fin = open(filename, "rt")

        # assembly_extension = has_extension(filename,ASSEMBLY_EXTENSIONS)

        # basic_extension = has_extension(filename,BASIC_EXTENSIONS)

        # lines = fin.readlines()
        # print("DEBUG"+str(lines))
        trimmed_lines = []
        filtered_lines = []

        pattern_count = [0] * len(PATTERN_LIST)

        # Do some filtering and count most common candidate directive
        for line in lines:
            line = normalize_line(line, basic_extension, assembly_extension)

            trimmed_lines.append(line)
            pattern_index = line_pattern(line)
            if pattern_index>=0:
                pattern_count[pattern_index]+=1

        max_directive_count = max(pattern_count)

        if max_directive_count==0:
            raise ValueError("No pattern found")

        display_extension_type(assembly_extension,basic_extension)

        max_index = pattern_count.index(max_directive_count)
        directive = PATTERN_LIST[max_index]

        # Guess if file is BASIC and if data are 8-bit or 16-bit values
        # Remark: DATA can be both an Assembly (TMS9900) directive or a BASIC command
        basic_code = is_basic_code(directive, assembly_extension)
        word_data =  directive in WORD_PATTERN_LIST and not basic_code

        # print("Pattern count: " + str(pattern_count))
        if verbose:
            print("Detected pattern     : " + directive)

        display_data_type(word_data)
        display_code_type(basic_code)

        for line in trimmed_lines:
            if (directive in line or directive.upper() in line):
                line = remove_comments(line,basic_code)
                filtered_lines.append(line)

        res = ""
        tiles = []
        tile_count = 0
        line_index = 0
        new_tile=""
        single_byte_count = 0

        trimmed_lines = []

        while line_index<len(filtered_lines):
            trimmed_lines.append(filtered_lines[line_index].split(directive)[1])
            line_index+=1

        if(rip):
            # Heuristic to guess BASIC code with hex data without a hex prefix
            headless_hex = has_headless_hex(trimmed_lines)

            # Heuristic to guess the presence of an initial header byte (to be skipped)
            header_byte = has_nine_byte_lines(trimmed_lines)
            skip_first = header_byte or (directive in SKIP_PATTERN_LIST)
        else:
            headless_hex = False
            header_byte = False
            skip_first = False

        if verbose:
            print("Skip first item      : " + str(skip_first))
            print("Headless hex data    : " + str(headless_hex))

        line_index = 0

        if verbose:
            print("")

        while tile_count<NUMBER_OF_TILES and line_index<len(filtered_lines):
            trimmed_line = trimmed_lines[line_index]

            line_index+=1
            data_list = trimmed_line.split(",")

            if skip_first:
                data_list = data_list[1:]

            if basic_code:
                data_list = fill_empty_slots_with_zero(data_list)

            # Remark: It only works for hex notation: Split word into two bytes
            if word_data:
                new_data_list = []
                for i in range(len(data_list)):
                    new_data_list += [data_list[i][0:3],"$"+data_list[i][3:5]]
                data_list = new_data_list

            for data in data_list:
                # TODO: This should be done at the beginning: Interpret as hex
                if headless_hex and data[0]!= "$":
                    data = "$" + data
                if data.startswith("@"):
                    data = str(int(data[1:],2))
                new_tile+=data
                single_byte_count+=1
                if single_byte_count==ysize:
                    single_byte_count=0
                    tiles.append(new_tile)

                    shape = compute_shape(new_tile,xsize)


                    if rotate:
                        new_tile = str(compute_rotated_shape(shape)).replace('[','').replace(']','')
                        shape = compute_shape(new_tile,ysize)

                    if not option_config.terminal_config.test:
                        print(new_tile)
                        print_shape(option_config, shape)

                    tile_count+=1
                    new_tile=""
                else:
                    new_tile+=","
        return tiles


def store_tiles(option_config, project, tiles, xsize, ysize):

    print("project          : " + project)

    category = project_category(project)

    main_path = "./" + category + "s/" + project + "/tiles/"+str(xsize)+"x"+str(ysize)+"/tile"
    print("main_path: " + main_path)
    for index in range(len(tiles)):
        store_tile(option_config, project,tiles[index],xsize,ysize,str(index))


def store_tile(option_config, project, tile, xsize, ysize, index):

    verbose = option_config.terminal_config.verbose
    # test    = option_config.terminal_config.test
    
    if verbose:
        print("(store tile) project    : " + project)
        print("tile index : " + str(index))
    dir = str(xsize)+"x"+str(ysize)
    
    if verbose:
        print("directory: " + dir)

    category = project_category(project)

    if verbose:
        print("project category : " + category)

    dest = "./" + category + "s/" + project + "/tiles/" + dir + "/tile" + index + ".txt"
    
    if verbose:
        print("Copy/Overwrite : " + dest)

    fin = open(dest, "wt")
    fin.write(tile)
    fin.close()


def compute_split_tiles(option_config, lines,verbose=False):

    xsize = 16
    for line in lines:
        xsize = min(xsize,len(line))
    if verbose:
        print("Detected xsize: " + str(xsize))

    filtered_lines_group = [[],[]]
    for line in lines:
        if verbose:
            print("processing line: ", line, end="")

        if not(line=="\n" or line=="\r" or line== "\r\n"):
            filtered_lines_group[0].append(line.replace('\n','').replace('\r','')[0:int(xsize/2)])
        if not(line=="\n" or line=="\r" or line== "\r\n"):
            filtered_lines_group[1].append(line.replace('\n','').replace('\r','')[int(xsize/2):xsize])

    if verbose:
        print("")

    if not option_config.terminal_config.test:
        print("")
        for filtered_lines in filtered_lines_group:
            display_simple_shape(filtered_lines)
            print("")

    return filtered_lines_group


def import_split_tiles(option_config, file_name):
    fin = open(file_name, "rt")
    lines = fin.readlines()

    filtered_lines_group = compute_split_tiles(option_config, lines)
    # print(str(lines))

    fin.close()


def compute_tile(lines, verbose=False):
    # print(str(lines))

    tile = ""

    filtered_lines = []
    for line in lines:
        if verbose:
            print("processing line: ", line, end="")
        if not(line=="\n" or line=="\r" or line== "\r\n"):
            filtered_lines.append(line.replace('\n','').replace('\r',''))

    xsize = 8
    for line in filtered_lines:
        xsize = min(xsize,len(line))

    trimmed_lines = []
    for line in filtered_lines:
        trimmed_lines.append(line[:xsize])
    ysize = len(trimmed_lines)

    dir = str(xsize)+"X"+str(ysize)

    if verbose:
        print("Tile shape: " + dir)
    for line_index in range(ysize):
        number_of_bits = len(trimmed_lines[line_index])
        if number_of_bits>8:
            print("WARNING: more than 8 bits detected in line " + line_index)
            number_of_bits=8

        value = 0
        for i in range(number_of_bits):
            if trimmed_lines[line_index][i] in ONE_REPPRESENTATIONS:
                value+=2**(number_of_bits-1-i)
        tile += str(value)
        if line_index!=ysize-1:
            tile += ","
    return tile, xsize, ysize


def import_tile(file_name):
    fin = open(file_name, "rt")
    lines = fin.readlines()
    tile,xsize,ysize = compute_tile(lines)
    fin.close()

    return tile,xsize,ysize


# It should be able to import from
# - Assembly files that use byte directives with either decimal and hex notation
# - Assembly files that use word directives with ONLY hex notation
def _import_from_source(option_config, params, rotate = False, rip_option = False):
    filename = params[1]

    xsize = 8
    ysize = 8

    tiles = rip_tiles(option_config, filename, xsize, ysize, rip_option, rotate)

    try:
        if(len(params)>=3) and "-" not in params[2]:
            store_tiles(option_config, params[2],tiles, xsize, ysize)
    except Exception as exception:
        print("Sorry! Failed to store tiles: \n" + str(exception.args))


def rotate(option_config, params):
    _import_from_source(option_config, params, rotate = True, rip_option = False)
    


def import_from_source(option_config, params):
    _import_from_source(option_config, params, rotate = False, rip_option = False)
    
    

# It should be able to import from
# - Assembly files that use byte directives with either decimal and hex notation
# - Assembly files that use word directives with ONLY hex notation
# - BASIC files that use decimal, hex notation or "headless" hex notation (by guessing)
# Remark: This function is like "import_from_source" with the "-rip" option
def rip(option_config, params, rotate = False):
    _import_from_source(option_config, params, rotate = False, rip_option = True)
