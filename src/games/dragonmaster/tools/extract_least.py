import sys

if __name__ == "__main__":
    iFile = open(sys.argv[1])
    oFile = open(sys.argv[2],'w')
    counter = 1
    for line in iFile:
        line_len = len(line.strip())
        
        # print(str(counter) + ": " + line)
        
        right_part = line[int(line_len/2):]
        
        counter+=1
        
        oFile.write(right_part)