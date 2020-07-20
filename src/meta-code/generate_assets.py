#!/bin/python

NUMBER_OF_TILES = 1

tile=[]

#read tile0
for i in [0,NUMBER_OF_TILES-1]:
    with open('tile'+str(i)+'.txt', 'r') as myfile:
        tile.append(myfile.read())


#input file
fin = open("template_8x8_chars.txt", "rt")

#output file to write the result to
fout = open("./generated_assets/8x8_chars.h", "wt")

#for each line in the input file
for i in [0,NUMBER_OF_TILES-1]:
    for line in fin:
            #read replace the string and write to output file
            fout.write(line.replace('<tile_'+str(i)+'>', tile[i]))
#close input and output files
fin.close()
fout.close()