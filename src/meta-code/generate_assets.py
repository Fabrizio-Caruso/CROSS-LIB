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

#read tile0
for i in range(NUMBER_OF_TILES):
    with open("./tiles/"+game_dir+"/8x8//tile"+str(i)+".txt", 'r') as myfile:
        print("Opening file tile"+str(i)+".txt")
        tile.append(myfile.read())

file_names = os.listdir("./templates")

stripped_file_names = []
for file_name in file_names:
    if file_name.endswith(".template"):
        stripped_file_name = file_name[:-9]
        print("file_name: "+file_name)
        print("stripped file name: "+stripped_file_name)
        stripped_file_names.append(stripped_file_name)
print("Templates found: "+str(len(stripped_file_names)))

def generate_assets():

    for stripped_file_name in stripped_file_names:
        matches = 0
        fin = open("./templates/"+stripped_file_name+".template", "rt")
        fout = open("./generated_assets/"+game_dir+"/"+stripped_file_name, "wt")

        for line in fin:
            newline = line
            # print("initial line: "+newline)
            for i in range(NUMBER_OF_TILES):
                #read replace the string
                newline = newline.replace('<tile_'+str(i)+'>', tile[i])
            fout.write(newline)
            if line != newline:
                matches = matches+1
                print("Changing \n"+line+"with\n"+newline)
        print("Number of tiles found: "+str(matches)) 
        print("")
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
