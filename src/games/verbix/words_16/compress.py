import sys



def letter_index(letter):
        if letter=='e':
            return 0
        elif letter=='a':
            return 1
        elif letter=='r':
            return 2
        elif letter=='i':
            return 3
        elif letter=='o':
            return 4
        elif letter=='t':
            return 5
        elif letter=='n':
            return 6
        elif letter=='s':
            return 7
        elif letter=='l':
            return 8
        elif letter=='c':
            return 9
        elif letter=='u':
            return 10
        elif letter=='d':
            return 11
        elif letter=='p':
            return 12
        elif letter=='m':
            return 13
        elif letter=='h':
            return 14
        elif letter=='y':
            return 15
        else:
            return 16


def compress(word):
    return letter_index(word[3])+letter_index(word[2])*16+letter_index(word[1])*16*16+letter_index(word[0])*16*16*16


# MAIN FUNCTION
if __name__ == '__main__':

    file_name = sys.argv[1]
    
    # Count valid words for example excluding empty lines
    word_count = 0
    
    words = []
    word_strings = []
    
    # res = ""
    
    print("file name: " + file_name)

    with open(file_name) as f:
        lines = f.readlines()
        
    for line in lines:
        word = line[1:]
        if(len(word)>=4):
            compressed_word = compress(word)
            print(word[0:4] + "->" + str(compressed_word))
            # print(compress(word))
            # res = res + ", " + str(compressed_word)
            word_count+=1
            words += [compressed_word]
            #print()

    print()   
    # print(res[2:])
    words.sort()
    
    for word in words:
        word_strings += [str(word)+"u"]
    
    print(str(word_strings)[1:-1].replace("'","")+",")
    print()
    print("Number of compressed words: " + str(word_count))
    