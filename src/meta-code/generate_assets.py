#!/bin/python

import sys

templates = False

if len(sys.argv)<2:
    game_dir = "chase"
else:
    game_dir = sys.argv[1]
    if len(sys.argv)>1:
        templates = True 
        template_name = argv[2]


NUMBER_OF_TILES = 24

tile=[]

#read tile0
for i in range(NUMBER_OF_TILES):
    with open("./tiles/"+game_dir+"/8x8//tile"+str(i)+".txt", 'r') as myfile:
        print("Opening file tile"+str(i)+".txt")
        tile.append(myfile.read())

file_names = ["8x8_chars.h", "xchase.asm", "cpc_Chars8.asm"]


def generate_assets():
    for file_name in file_names:

        fin = open("./templates/"+file_name+".template", "rt")
        fout = open("./generated_assets/"+game_dir+"/"+file_name, "wt")

        for line in fin:
            newline = line
            # print("initial line: "+newline)
            for i in range(NUMBER_OF_TILES):
                #read replace the string
                newline = newline.replace('<tile_'+str(i)+'>', tile[i])
            fout.write(newline)
            if line != newline:
                print("changing \n"+line+"with\n"+newline)
        #close input and output files
        fin.close()
        fout.close()


def generate_template():

    fin = open("./templates/"+template_name, "rt")
    fout = open("./templates/"+template_name+".template",  "wt")

    for line in fin:
        newline = line
        # print("initial line: "+newline)
        for i in range(NUMBER_OF_TILES):
            #read replace the string
            newline = newline.replace(tile[i],'<tile_'+str(i)+'>', tile[i])
        fout.write(newline)
        if line != newline:
            print("changing \n"+line+"with\n"+newline)
    #close input and output files
    fin.close()
    fout.close()


def main():
    if not templates:
        generate_assets()
    else:
        generate_template()

if __name__ == "__main__":
    # execute only if run as a script
    main()
