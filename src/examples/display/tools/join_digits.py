import sys

DEFAULT_SHAPE = "8x8"

def value(shape):
    shape_len = len(shape)
    res = 0
    for i in range(0,shape_len):
        if shape[shape_len-1-i]=='#':
            res+=2**i
    return res

def total_value(lhs, rhs):
    len_lhs = len(lhs)
    # print(len_lhs)
    # print(lhs)
    lhs_value = value(lhs)
    # print(lhs)
    print(lhs_value)
    rhs_value = value(rhs)
    # print(rhs)
    print(rhs_value)
    
    print(lhs_value*(2**(len_lhs)) + rhs_value)
    print("")
    return lhs_value*(2**(len_lhs)) + rhs_value 
    

def join_digits(lhs,rhs, shape=DEFAULT_SHAPE):
    
    lhs_file = "./digits/"+shape+"/digit_" + str(lhs) + ".txt"
    rhs_file = "./digits/"+shape+"/digit_" + str(rhs) + ".txt"
    
    shape_output_file = "./shapes/"+shape+"/shape_0" + str(lhs) + str(rhs) + ".txt"
    tile_output_file  = "./tiles/"+shape+"/tile_0"   + str(lhs) + str(rhs) + ".txt"
    
    iLhsFile = open(lhs_file)
 
    lhs = []
    lhsLength = 0
    for lhsLine in iLhsFile:
        
        lhs.append(lhsLine)
        lhsLength+=1
    
    iLhsFile.close()

    iRhsFile = open(rhs_file)

    rhs = []
    rhsLength = 0
    for rhsLine in iRhsFile:
        
        rhs.append(rhsLine)
        rhsLength+=1

    iRhsFile.close()
    
    oShapeFile = open(shape_output_file,'w+')
    oTileFile  = open(tile_output_file,'w+')

    max_range = max(lhsLength,rhsLength)
    for counter in range(0,max_range):
        oShapeFile.write(lhs[counter].strip()+rhs[counter])
        # print("lhs: " + lhs[counter] + "rhs: " + rhs[counter] )
        computed_value = total_value(lhs[counter].strip(), rhs[counter])
        # print(computed_value)
        oTileFile.write(str(total_value(lhs[counter].strip(), rhs[counter].strip())))
        if counter < max_range-1:
            oTileFile.write(",")
    oShapeFile.close()
    oTileFile.close()


if __name__ == "__main__":
    start = int(sys.argv[1])
    end   = int(sys.argv[2])
    
    if len(sys.argv)>3:
        shape = sys.argv[3]
    else:
        shape = DEFAULT_SHAPE
    
    for number in range(start,end+1):
        lhs = number // 10
        rhs = number % 10
        # print(f"lhs: {lhs}")
        # print(f"rhs: {rhs}\n")
        join_digits(lhs,rhs, shape)
