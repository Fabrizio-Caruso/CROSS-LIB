#!/bin/python

NUMBER_OF_TILES = 24

tile=[]

#read tile0
for i in range(NUMBER_OF_TILES):
    with open('./tiles/tile'+str(i)+'.txt', 'r') as myfile:
        print("Opening file tile"+str(i)+".txt")
        tile.append(myfile.read())


#input file
fin = open("./templates/template_8x8_chars.txt", "rt")

#output file to write the result to
fout = open("./generated_assets/8x8_chars.h", "wt")

#for each line in the input file
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