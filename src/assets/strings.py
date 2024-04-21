
import re


# def only_upper_and_digits(s):
    # return "".join(c for c in s if c.isupper() or c in ["0","1","2","3","4","5","6","7","8","9"])


def only_upper_digits_and_space(s):
    return "".join(c for c in s if c.isupper() or c in [" ", "0","1","2","3","4","5","6","7","8","9"])


def no_space(s):
    return "".join(c for c in s if c!=" ")
    

# Convert a string into a sequence of macros that can be used in the code (only necessary for small letters)
def compute_string(string):
    # string = params[1]
    m = re.search('[a-zA-Z0-9 ]*',string)
    string = m.group(0)
    
    string = string.replace(" ", "@")
    converted_string = ""
    
    for ch in string:
        converted_string+= "_XL_"+ch+" "
    
    converted_string = converted_string.replace("_XL_@", "_XL_SPACE")
    
    for i in range(9):
        converted_string = converted_string.replace("_XL_"+str(i),'"'+str(i)+'"')
    
    converted_string = converted_string[:-1]
    
    return  converted_string

def string(params):
    print(compute_string(params[1]))