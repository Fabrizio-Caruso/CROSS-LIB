#!/bin/python

import os,sys


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



a7800_test = [
[0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00], # zero
[0x18,0x3c,0x66,0x7e,0x66,0x66,0x66,0x00], # A
[0x7c,0x66,0x66,0x7c,0x66,0x66,0x7c,0x00],
[0x3c,0x66,0x60,0x60,0x60,0x66,0x3c,0x00],
[0x78,0x6c,0x66,0x66,0x66,0x6c,0x78,0x00],
[0x7e,0x60,0x60,0x78,0x60,0x60,0x7e,0x00],
[0x7e,0x60,0x60,0x78,0x60,0x60,0x60,0x00],
[0x3c,0x66,0x60,0x6e,0x66,0x66,0x3c,0x00],
[0x66,0x66,0x66,0x7e,0x66,0x66,0x66,0x00],
[0x3c,0x18,0x18,0x18,0x18,0x18,0x3c,0x00],
[0x1e,0x0c,0x0c,0x0c,0x0c,0x6c,0x38,0x00],
[0x66,0x6c,0x78,0x70,0x78,0x6c,0x66,0x00],
[0x60,0x60,0x60,0x60,0x60,0x60,0x7e,0x00],
[0x63,0x77,0x7f,0x6b,0x63,0x63,0x63,0x00],
[0x66,0x76,0x7e,0x7e,0x6e,0x66,0x66,0x00],
[0x3c,0x66,0x66,0x66,0x66,0x66,0x3c,0x00],
[0x7c,0x66,0x66,0x7c,0x60,0x60,0x60,0x00],
[0x3c,0x66,0x66,0x66,0x66,0x3c,0x0e,0x00],
[0x7c,0x66,0x66,0x7c,0x78,0x6c,0x66,0x00],
[0x3c,0x66,0x60,0x3c,0x06,0x66,0x3c,0x00],
[0x7e,0x18,0x18,0x18,0x18,0x18,0x18,0x00],
[0x66,0x66,0x66,0x66,0x66,0x66,0x3c,0x00],
[0x66,0x66,0x66,0x66,0x66,0x3c,0x18,0x00],
[0x63,0x63,0x63,0x6b,0x7f,0x77,0x63,0x00],
[0x66,0x66,0x3c,0x18,0x3c,0x66,0x66,0x00],
[0x66,0x66,0x66,0x3c,0x18,0x18,0x18,0x00],
[0x7e,0x06,0x0c,0x18,0x30,0x60,0x7e,0x00], # Z
[0x3c,0x66,0x6e,0x76,0x66,0x66,0x3c,0x00],
[0x18,0x18,0x38,0x18,0x18,0x18,0x7e,0x00], # 0
[0x3c,0x66,0x06,0x0c,0x30,0x60,0x7e,0x00],
[0x3c,0x66,0x06,0x1c,0x06,0x66,0x3c,0x00],
[0x06,0x0e,0x1e,0x66,0x7f,0x06,0x06,0x00],
[0x7e,0x60,0x7c,0x06,0x06,0x66,0x3c,0x00],
[0x3c,0x66,0x60,0x7c,0x66,0x66,0x3c,0x00],
[0x7e,0x66,0x0c,0x18,0x18,0x18,0x18,0x00],
[0x3c,0x66,0x66,0x3c,0x66,0x66,0x3c,0x00],
[0x3c,0x66,0x66,0x3e,0x06,0x66,0x3c,0x00], # 9
[0,0,0,0,0,0,0,0], # tile 0
[1,1,1,1,1,1,1,1],
[2,2,2,2,2,2,2,2],
[3,3,3,3,3,3,3,3],
[4,4,4,4,4,4,4,4],
[5,5,5,5,5,5,5,5],
[6,6,6,6,6,6,6,6],
[7,7,7,7,7,7,7,7],
[8,8,8,8,8,8,8,8],
[9,9,9,9,9,9,9,9],
[0,0,0,0,0,0,0,0],
[1,1,1,1,1,1,1,1],
[2,2,2,2,2,2,2,2],
[3,3,3,3,3,3,3,3],
[4,4,4,4,4,4,4,4],
[5,5,5,5,5,5,5,5],
[6,6,6,6,6,6,6,6],
[7,7,7,7,7,7,7,7],
[8,8,8,8,8,8,8,8],
[9,9,9,9,9,9,9,9],
[0,0,0,0,0,0,0,0],
[1,1,1,1,1,1,1,1],
[2,2,2,2,2,2,2,2],
[3,3,3,3,3,3,3,3],
[4,4,4,4,4,4,4,4],
[5,5,5,5,5,5,5,5],
[6,6,6,6,6,6,6,6], # tile 26
]


# Input : 4 bits
# Output: 8 bits with double bits, e.g. 1010 -> 11001100
double_bit_map = { 
    0x0:0x00, 0x1:0x03, 0x2:0x0C, 0x3:0x0F, 0x4:0x30, 0x5:0x33, 0x6:0x3C, 0x7:0x3F,
    0x8:0xC0, 0x9:0xC3, 0xA:0xCC, 0xB:0xCF, 0xC:0xF0, 0xD:0xF3, 0xE:0xFC, 0xF:0xFF
}

color_1 = 0x55
color_2 = 0xAA
color_3 = 0xFF

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
    two_bit_asset = [];
    # space, alphabet, digits (1+26+10=37)
    for i in range(0,37):
        for j in range(0,8):
            two_bit_asset.append(one_to_two(abstract_asset[i][j],color_3))
        
    # 27 tiles in color 0
    for i in range(37,63):
        for j in range(0,8):
            two_bit_asset.append(one_to_two(abstract_asset[i][j],color_1))
        
    # 27 tiles in color 1
    for i in range(37,63):
        for j in range(0,8):
            two_bit_asset.append(one_to_two(abstract_asset[i][j],color_2)) 
        
    # 27 tiles in color 2
    for i in range(37,63):
        for j in range(0,8):
            two_bit_asset.append(one_to_two(abstract_asset[i][j],color_3))
    return two_bit_asset

# def generate_160A_asset(two_bit_asset, two_bit_160A_asset):


ATARI7800_FILE_NAME = "cc65_udc_atari7800_160A.s"


def process_a7800_160A_file():
    
    parent_path = "./generated_assets/"
    dest_path = parent_path+game_dir

    
    fin = open(dest_path+"/"+ATARI7800_FILE_NAME, "rt")

    source = []
    result = []
    for line in fin:
        words = line.split(",")
        for word in words:
            if word != "\n" and word != "\t\n":
                trimmed_word = word.replace("\n","").replace(" ","")
                print(trimmed_word)
                source.append(trimmed_word)
    fin.close()
    # print(source)
    
    if len(source)==512:
        print("Asset has correct length (512 bytes)")
    else:
        print("Asset has wrong length: " + str(len(source)))

    
def generate_asset_from_template(dir_name, stripped_template_file_name):
        matches = 0
        print("")
        print("Handling "+stripped_template_file_name)
        print("")
        
        
        # print("DEBUG")
        
        # print(generate_two_bit_asset(a7800_test))
        
        # foo()
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
        if stripped_template_file_name==ATARI7800_FILE_NAME:
            process_a7800_160A_file()



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
    main()

