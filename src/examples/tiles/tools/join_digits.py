import sys

if __name__ == "__main__":
    iLhsFile = open(sys.argv[1])

    
    lhs = []
    lhsLength = 0
    for lhsLine in iLhsFile:
        
        lhs.append(lhsLine)
        lhsLength+=1
    
    iLhsFile.close()
    # print(str(lhs))


    iRhsFile = open(sys.argv[2])

    rhs = []
    rhsLength = 0
    for rhsLine in iRhsFile:
        
        rhs.append(rhsLine)
        rhsLength+=1

    iRhsFile.close()
    # print(str(rhs))
    
    oFile = open(sys.argv[3],'w')

    for counter in range(0,max(lhsLength,rhsLength)):
        # print(counter)
        oFile.write(lhs[counter].strip()+rhs[counter])
    oFile.close()