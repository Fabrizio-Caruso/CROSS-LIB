#!/bin/python

import os,sys

templates = False

if len(sys.argv)<2:
    game_dir = "chase"
else:
    game_dir = sys.argv[1]
    if len(sys.argv)>2:
        templates = True 
        template_name = argv[2]


NUMBER_OF_TILES = 24

tile=[]

def read_tiles_from_dir(dir_name):
    global tile
    tile = []
    for i in range(NUMBER_OF_TILES):
        file_to_open = "./tiles/"+game_dir+"/"+dir_name+"/tile"+str(i)+".txt"
        with open(file_to_open, 'r') as myfile:
            print("Opening file tile"+file_to_open)
            tile.append(myfile.read())

def read_templates_from_dir(dir_name):

    global stripped_file_names

    file_names = os.listdir("./templates/"+dir_name)

    stripped_file_names = []
    for file_name in file_names:
        if file_name.endswith(".template"):
            stripped_file_name = file_name[:-9]
            print("file_name: "+file_name)
            print("stripped file name: "+stripped_file_name)
            stripped_file_names.append(stripped_file_name)
    print("Templates found: "+str(len(stripped_file_names)))

def generate_assets_from_dir(dir_name):

    for stripped_file_name in stripped_file_names:
        matches = 0
        print("")
        print("Handling "+stripped_file_name)
        print("")
        fin = open("./templates/"+dir_name+"/"+stripped_file_name+".template", "rt")
        fout = open("./generated_assets/"+game_dir+"/"+stripped_file_name, "wt")

        for line in fin:
            newline = line
            # print("initial line: "+newline)
            for i in range(NUMBER_OF_TILES):
                if stripped_file_name.startswith("cmoc"):
                    newline = newline.replace('<tile_'+str(i)+'>', tile[i].replace(","," "))
                else:
                    newline = newline.replace('<tile_'+str(i)+'>', tile[i])
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


def main():

    for dir_name in ["8x8","6x8","6x9"]:
        read_tiles_from_dir(dir_name)
        read_templates_from_dir(dir_name)
        generate_assets_from_dir(dir_name)


if __name__ == "__main__":
    # execute only if run as a script
    main()
