#!/bin/python

#read tile0
with open('tile0.txt', 'r') as myfile:
  tile0 = myfile.read()


#input file
fin = open("template_8x8_chars.txt", "rt")

#output file to write the result to
fout = open("8x8_chars.h", "wt")

#for each line in the input file
for line in fin:
	#read replace the string and write to output file
	fout.write(line.replace('<tile_0>', tile0))
#close input and output files
fin.close()
fout.close()