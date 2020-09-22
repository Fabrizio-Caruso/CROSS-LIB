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

NUMBER_OF_TILES = 19

tile=[]

def read_tiles_from_dir(dir_name):
    global tile
    tile = []
    for i in range(NUMBER_OF_TILES):
        file_to_open = "../"+parent_dir+"/"+game_dir+"/tiles/"+dir_name+"/tile"+str(i)+".txt"
        with open(file_to_open, 'r') as myfile:
            print("Opening file tile"+file_to_open)
            tile.append(myfile.read())


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


def generate_asset_from_template(dir_name, stripped_template_file_name):
        matches = 0
        print("")
        print("Handling "+stripped_template_file_name)
        print("")
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
            for i in range(NUMBER_OF_TILES):
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


def generate_assets_from_dir(dir_name):

    for stripped_template_file_name in stripped_template_file_names:
        generate_asset_from_template(dir_name, stripped_template_file_name)


def main():

    for dir_name in ["8x8","6x8","6x9"]:
        read_tiles_from_dir(dir_name)
        read_templates_from_dir(dir_name)
        generate_assets_from_dir(dir_name)


if __name__ == "__main__":
    # execute only if run as a script
    main()

