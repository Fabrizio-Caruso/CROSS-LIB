#!/bin/python

import os
import sys

# sys.path.append("..")

from LoggerSingleton import *

global logger

if len(sys.argv)<2:
    game_dir = "chase"
else:
    game_dir = sys.argv[1]

if len(sys.argv)<3:
    parent_dir = "games"
else:
    parent_dir = sys.argv[2]

MAX_NUMBER_OF_TILES = 27

tile=[]


# Read tile files from a given directory and store them into the tile variable
def read_tiles_from_dir(dir_name):
    logger.info("read_tiles_from_dir")
    global tile
    tile = []
    for i in range(MAX_NUMBER_OF_TILES):
        file_to_open = "../"+parent_dir+"/"+game_dir+"/tiles/"+dir_name+"/tile"+str(i)+".txt"
        if os.path.exists(file_to_open):
            with open(file_to_open, 'r') as myfile:
                print("Opening file tile"+file_to_open)
                tile.append(myfile.read().replace('\n',''))
        else:
            y = int(dir_name[2])
            zerolist = str([0]*y)
            tile.append(zerolist[1:len(zerolist)-1])


def read_templates_from_dir(dir_name):

    global stripped_template_file_names

    template_file_names = os.listdir("./templates/"+dir_name)

    stripped_template_file_names = []
    for template_file_name in template_file_names:
        if template_file_name.endswith(".template"):
            stripped_template_file_name = template_file_name[:-9]
            print("template_file_name: "+template_file_name)
            print("stripped file name: "+stripped_template_file_name)
            stripped_template_file_names.append(stripped_template_file_name)
    print("Templates found: "+str(len(stripped_template_file_names)))
    print("")


# --------------------------------------------------------------------------------------
# Atari 7800 routines
# --------------------------------------------------------------------------------------

# a7800_test = [
# [0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00], # zero
# [0x18,0x3c,0x66,0x7e,0x66,0x66,0x66,0x00], # A
# [0x7c,0x66,0x66,0x7c,0x66,0x66,0x7c,0x00],
# [0x3c,0x66,0x60,0x60,0x60,0x66,0x3c,0x00],
# [0x78,0x6c,0x66,0x66,0x66,0x6c,0x78,0x00],
# [0x7e,0x60,0x60,0x78,0x60,0x60,0x7e,0x00],
# [0x7e,0x60,0x60,0x78,0x60,0x60,0x60,0x00],
# [0x3c,0x66,0x60,0x6e,0x66,0x66,0x3c,0x00],
# [0x66,0x66,0x66,0x7e,0x66,0x66,0x66,0x00],
# [0x3c,0x18,0x18,0x18,0x18,0x18,0x3c,0x00],
# [0x1e,0x0c,0x0c,0x0c,0x0c,0x6c,0x38,0x00],
# [0x66,0x6c,0x78,0x70,0x78,0x6c,0x66,0x00],
# [0x60,0x60,0x60,0x60,0x60,0x60,0x7e,0x00],
# [0x63,0x77,0x7f,0x6b,0x63,0x63,0x63,0x00],
# [0x66,0x76,0x7e,0x7e,0x6e,0x66,0x66,0x00],
# [0x3c,0x66,0x66,0x66,0x66,0x66,0x3c,0x00],
# [0x7c,0x66,0x66,0x7c,0x60,0x60,0x60,0x00],
# [0x3c,0x66,0x66,0x66,0x66,0x3c,0x0e,0x00],
# [0x7c,0x66,0x66,0x7c,0x78,0x6c,0x66,0x00],
# [0x3c,0x66,0x60,0x3c,0x06,0x66,0x3c,0x00],
# [0x7e,0x18,0x18,0x18,0x18,0x18,0x18,0x00],
# [0x66,0x66,0x66,0x66,0x66,0x66,0x3c,0x00],
# [0x66,0x66,0x66,0x66,0x66,0x3c,0x18,0x00],
# [0x63,0x63,0x63,0x6b,0x7f,0x77,0x63,0x00],
# [0x66,0x66,0x3c,0x18,0x3c,0x66,0x66,0x00],
# [0x66,0x66,0x66,0x3c,0x18,0x18,0x18,0x00],
# [0x7e,0x06,0x0c,0x18,0x30,0x60,0x7e,0x00], # Z
# [0x3c,0x66,0x6e,0x76,0x66,0x66,0x3c,0x00],
# [0x18,0x18,0x38,0x18,0x18,0x18,0x7e,0x00], # 0
# [0x3c,0x66,0x06,0x0c,0x30,0x60,0x7e,0x00],
# [0x3c,0x66,0x06,0x1c,0x06,0x66,0x3c,0x00],
# [0x06,0x0e,0x1e,0x66,0x7f,0x06,0x06,0x00],
# [0x7e,0x60,0x7c,0x06,0x06,0x66,0x3c,0x00],
# [0x3c,0x66,0x60,0x7c,0x66,0x66,0x3c,0x00],
# [0x7e,0x66,0x0c,0x18,0x18,0x18,0x18,0x00],
# [0x3c,0x66,0x66,0x3c,0x66,0x66,0x3c,0x00],
# [0x3c,0x66,0x66,0x3e,0x06,0x66,0x3c,0x00], # 9
# [0,0,0,0,0,0,0,0], # tile 0
# [1,1,1,1,1,1,1,1],
# [2,2,2,2,2,2,2,2],
# [3,3,3,3,3,3,3,3],
# [4,4,4,4,4,4,4,4],
# [5,5,5,5,5,5,5,5],
# [6,6,6,6,6,6,6,6],
# [7,7,7,7,7,7,7,7],
# [8,8,8,8,8,8,8,8],
# [9,9,9,9,9,9,9,9],
# [0,0,0,0,0,0,0,0],
# [1,1,1,1,1,1,1,1],
# [2,2,2,2,2,2,2,2],
# [3,3,3,3,3,3,3,3],
# [4,4,4,4,4,4,4,4],
# [5,5,5,5,5,5,5,5],
# [6,6,6,6,6,6,6,6],
# [7,7,7,7,7,7,7,7],
# [8,8,8,8,8,8,8,8],
# [9,9,9,9,9,9,9,9],
# [0,0,0,0,0,0,0,0],
# [1,1,1,1,1,1,1,1],
# [2,2,2,2,2,2,2,2],
# [3,3,3,3,3,3,3,3],
# [4,4,4,4,4,4,4,4],
# [5,5,5,5,5,5,5,5],
# [6,6,6,6,6,6,6,6], # tile 26
# ]


# Input : 4 bits
# Output: 8 bits with double bits, e.g. 1010 -> 11001100
double_bit_map = { 
    # 0x0:0x00, 0x1:0x03, 0x2:0x09, 0x3:0x0F, 0x4:0x30, 0x5:0x93, 0x6:0x39, 0x7:0xFF,
    # 0x8:0x90, 0x9:0x93, 0xA:0x99, 0xB:0x9F, 0xC:0xF0, 0xD:0xF3, 0xE:0xF9, 0xF:0xFF
    0x0:0x00, 0x1:0x03, 0x2:0x0C, 0x3:0x0F, 0x4:0x30, 0x5:0x33, 0x6:0x3C, 0x7:0x3F,
    0x8:0xC0, 0x9:0xC3, 0xA:0xCC, 0xB:0xCF, 0xC:0xF0, 0xD:0xF3, 0xE:0xFC, 0xF:0xFF
}

# COCO
# 0x00, 0x03, 0x0C, 0x0F, 0x30, 0x33, 0x3C, 0x3F,
# 0xC0, 0xC3, 0xCC, 0xCF, 0xF0, 0xF3, 0xFC, 0xFF

color_1 = 0x55 # red
color_2 = 0xAA # green
color_3 = 0xFF # white

# color = 0x00, 0x55, 0xAA, 0xFF
# Input : 4 bits, color
# Output: 8 bits encoding 4 pixels in 4 color mode in a given color 
def four_colored(four_bit, color):
    return double_bit_map[four_bit]&color


# Input : 1 byte, color
# Output: 2 bytes encoding 8 pixels in 4 color mode in a given color
def one_to_two(source, color):
    return [four_colored(source>>4,color), four_colored(source&0x0F,color)]


# INPUT: 64 monochromatic tiles (64 x 8 bytes)
# OUTPUT: 
# First 37  -> Converted in 37 4-color tiles in color 3
# Second 27 -> Converted in 3 groups of 27 4-color tiles in color 1, 2, 3
def generate_two_bit_asset(abstract_asset):
    two_bit_asset = []
    # space, alphabet, digits (1+26+10=37)
    for i in range(0,37):
        for j in range(0,8):
            two_bit_block = one_to_two(abstract_asset[i*8+j],color_3)
            # two_bit_asset.extend(two_bit_block)
            two_bit_asset.append(two_bit_block)

        
    # 27 tiles in color 0
    for i in range(37,64):
        for j in range(0,8):
            two_bit_block = one_to_two(abstract_asset[i*8+j],color_1)
            # two_bit_asset.extend(two_bit_block)
            two_bit_asset.append(two_bit_block)
        
    # 27 tiles in color 1
    for i in range(37,64):
        for j in range(0,8):
            two_bit_block = one_to_two(abstract_asset[i*8+j],color_2)
            # two_bit_asset.extend(two_bit_block)
            two_bit_asset.append(two_bit_block)

        
    # 27 tiles in color 2
    for i in range(37,64):
        for j in range(0,8):
            two_bit_block = one_to_two(abstract_asset[i*8+j],color_3)
            # two_bit_asset.extend(two_bit_block)
            two_bit_asset.append(two_bit_block)

    # padding to get to 128
    for i in range(0,10):
        for j in range(0,8):
            two_bit_asset.append([255,255])

    return two_bit_asset




# def generate_160A_asset(two_bit_asset, two_bit_160A_asset):


def generate_160A_asset(two_bit_assets):
   row_map = {0:[], 1:[], 2:[], 3:[], 4:[], 5:[], 6:[], 7:[]}
   two_bit_160A_asset = []

   counter = 0
   for byte_couple in two_bit_assets:
       row_map[counter&7].extend(byte_couple)
       counter+=1
       
   for i in range(0,8):
       two_bit_160A_asset.extend(row_map[7-i])
       
   return two_bit_160A_asset
       

ATARI7800_FILE = "cc65_udc_atari7800_160A.s"
ATARI7800_HEADER = \
    ".export _conio_font\n" \
    ".rodata\n" \
    ".align 256\n" \
    "_conio_font:\n\n"


def process_a7800_160A_file():
    
    parent_path = "./generated_assets/"
    dest_path = parent_path+game_dir

    
    fin = open(dest_path+"/"+ATARI7800_FILE, "rt")

    res = []
    source = []
    for line in fin:
        words = line.split(",")
        for word in words:
            word = word.strip()
            if word != '' and  word != "\n" and word != "\t\n" and word != "\r\n" and word != "\r":
                trimmed_word = word.replace("\n","").replace(" ","").replace("$","0x")
                print(trimmed_word)
                source.append(trimmed_word)
                base = 16 if trimmed_word.startswith("0x") else 10
                res.append(int(trimmed_word, base))
    fin.close()
    print(res)
    # while(1):
        # pass
    
    if len(res)==512:
        print("Asset has correct length (512 bytes)")
    else:
        print("Asset has wrong length: " + str(len(res)))
        
    two_bit_assets = generate_two_bit_asset(res)
    
    print("two_bit_assets: \n" + str(two_bit_assets))
    # while(1):
        # pass
    a7800_assets = generate_160A_asset(two_bit_assets)
    
    print("length of a7800_assets: " + str(len(a7800_assets)))
    
    formatted_assets = ATARI7800_HEADER + format_asset(a7800_assets)
    
    
    print(formatted_assets)
    
    return formatted_assets
    # while(1):
        # pass




# --------------------------------------------------------------------------------------


def format_asset(assets):
    res = ""
    counter = 0
    for asset in assets:
        # print("[format_asset] asset: " + str(asset))
        if counter==0:
            line = ".byte "
        line = line + str(asset)
        counter=counter+1
        if counter<8:
            line = line + ", "
        else:
            res = res + line + "\n"
            # print("line: " + line)
            # print("res: " + res)
            counter = 0
    if counter>0:     
        res = res + line + "\n"
        # print("line: " + line)
        # print("res: " + res)   
    # if res.endswith(', '):
        res = res[:-3] + "\n"
    return res

    
def generate_asset_from_template(dir_name, stripped_template_file_name):
    matches = 0
    print("")
    print("Handling "+stripped_template_file_name)
    print("")
    
    
    # print("DEBUG")
    
    # print("Length of test: " + str(len(a7800_test)))
    # two_bit_assets = generate_two_bit_asset(a7800_test)
    # print("two bit asset:\n" + str(two_bit_assets))
    
    # print("Length of two_bit_assets: " + str(len(two_bit_assets)))

    # a7800_assets = generate_160A_asset(two_bit_assets)
    
    # print("Length of a7800_assets: " + str(len(a7800_assets)))
    
    # print("formatted a7800_assets:\n" + format_asset(a7800_assets))
    
    
    
    # while(1):
        # pass
    # exit()
    
    # print("END DEBUG")
    
    fin = open("./templates/"+dir_name+"/"+stripped_template_file_name+".template", "rt")
    
    parent_path = "./generated_assets/"
    
    if not os.path.exists(parent_path):
        print("creating: " + parent_path)
        os.makedirs(parent_path)
    
    dest_path = parent_path+game_dir
    
    if not os.path.exists(dest_path):
        print("creating: " + dest_path)
        os.makedirs(dest_path)
    
    fout = open(dest_path+"/"+stripped_template_file_name, "wt")

    for line in fin:
        newline = line
        # print("initial line: "+newline)
        for i in range(len(tile)):
            if stripped_template_file_name.startswith("cmoc"):
                tile_data = tile[i].replace(","," \n    FCB ")
            else:
                if stripped_template_file_name.endswith(".h"):
                    tile_data = tile[i].replace("$","0x")
                else:
                    tile_data = tile[i]
            if stripped_template_file_name.startswith("cmoc"):
                tile_data = tile_data + "\n"
            newline = newline.replace('<tile_'+str(i)+'>', tile_data)
        fout.write(newline)
        if line != newline:
            matches = matches+1
            print("Changing \n"+line+"with\n"+newline)
            
    print("Number of tiles found: "+str(matches)) 
    print("")
    print("")
    #close input and output files
    fin.close()
    fout.close()
    if stripped_template_file_name==ATARI7800_FILE:
        formatted_assets = process_a7800_160A_file()
    
        fout = open(dest_path+"/formatted_"+ATARI7800_FILE, "wt")
        fout.write(formatted_assets)
        fout.close()
    # process_a7800_160A_file()



def generate_assets_from_dir(dir_name):

    for stripped_template_file_name in stripped_template_file_names:
        generate_asset_from_template(dir_name, stripped_template_file_name)


def main():

    for dir_name in ["8x8", "7x8", "6x8", "6x9", "8x6"]:
        print("dir name: " + dir_name)
        read_tiles_from_dir(dir_name)
        read_templates_from_dir(dir_name)
        generate_assets_from_dir(dir_name)



if __name__ == "__main__":
    # execute only if run as a script
    logger = LoggerSingleton.initLogger('xl','../../logs')
    
    # logger.setLevel(logging.DEBUG) 
    # logging.basicConfig(filename='../logs/log_'+datetime.datetime.now().strftime("%Y-%m-%d_%H_%M_%S")+'.log', level = logging.DEBUG)
    # logging.basicConfig(filename='../../logs/xl_log_.log', level = logging.DEBUG)

    logger.info("Started generate_assets")
    main()
    logger.error("Finished generate_assets")

