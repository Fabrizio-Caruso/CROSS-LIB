#!/usr/bin/env python

from __future__ import print_function

# TODO: define and assemble macro blocks
# added FILESIZE("filename")
# defs doesn't cause bytes to be written to output unless real data follows

def printusage():
    print("pyz80 by Andrew Collier, modified by Simon Owen")
    print(" https://github.com/simonowen/pyz80/")
    print("Usage:")
    print("     pyz80 (options) inputfile(s)")
    print("Options:")
    print("-o outputfile")
    print("   save the resulting disk image at the given path")
    print("--nozip")
    print("   do not compress the resulting disk image")
    print("-I filepath")
    print("   Add this file to the disk image before assembling")
    print("   May be used multiple times to add multiple files")
    print("--obj=outputfile")
    print("   save the output code as a raw binary file at the given path")
    print("-D symbol")
    print("-D symbol=value")
    print("   Define a symbol before parseing the source")
    print("   (value is integer; if omitted, assume 1)")
    print("--exportfile=filename")
    print("   Save all symbol information into the given file")
    print("--importfile=filename")
    print("   Define symbols before assembly, from a file previously exported")
    print("--mapfile=filename")
    print("   Save address-to-symbol map into the given file")
    print("--lstfile=filename")
    print("   Produce assembly listing into given file")
    print("--case")
    print("   treat source labels as case sensitive (as COMET itself did)")
    print("--nobodmas")
    print("   treat arithmetic operators without precedence (as COMET itself did)")
    print("-s regexp")
    print("   print the value of any symbols matching the given regular expression")
    print("   This may be used multiple times to output more than one subset")
    print("-e")
    print("   use python's own error handling instead of trying to catch parse errors")
    

def printlicense():
    print("This program is free software; you can redistribute it and/or modify")
    print("it under the terms of the GNU General Public License as published by")
    print("the Free Software Foundation; either version 2 of the License, or")
    print("(at your option) any later version.")
    print(" ")
    print("This program is distributed in the hope that it will be useful,")
    print("but WITHOUT ANY WARRANTY; without even the implied warranty of")
    print("MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the")
    print("GNU General Public License for more details.")
    print(" ")
    print("You should have received a copy of the GNU General Public License")
    print("along with this program; if not, write to the Free Software")
    print("Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA")

import getopt
import sys, os
import array
import fileinput
import re
import gzip
import math # for use by expressions in source files
import random

# Try for native pickle (2.x), fall back on Python version (3.x)
try:
    import cPickle as pickle
except ImportError:
    import pickle

def new_disk_image():
    
    image = array.array('B')
    image.append(0)
    targetsize = 80*10*2*512
    # disk image is arranged as: tr 0 s 1-10, tr 128 s 1-10, tr 1 s 1-10, tr 129 s 1-10 etc
    
    while len(image) < targetsize:
        image.extend(image)
    while len(image) > targetsize:
        image.pop()

    return image

def dsk_at(track,side,sector):
    return (track*20+side*10+(sector-1))*512
    # uses numbering 1-10 for sectors, because SAMDOS internal format also does


def add_file_to_disk_image(image, filename, codestartpage, codestartoffset, execpage=0, execoffset=0, filelength=None, fromfile=None ):
    global firstpageoffset

    if fromfile != None:
        fromfilefile = open(fromfile,'rb')
        fromfilefile.seek(0,2)
        filelength = fromfilefile.tell()
        
        fromfilefile.seek(0)
        fromfile = array.array('B')
        fromfile.fromfile(fromfilefile, filelength)

    sectors_already_used = 0
    # we're writing the whole image, so we can take a bit of a shortcut
    # instead of reading the entire sector map to find unused space, we can assume all files are contiguous
    # and place new files just at the end of the used space

    #find an unused directory entry
    for direntry in range(80):
        dirpos = dsk_at(direntry//20,0,1+(direntry%20)//2) + 256*(direntry%2)
        if image[dirpos] == 0:
            break;
        else:
            sectors_already_used += image[dirpos+11]*256 +image[dirpos+12]
            
    else:
        fatal ("Too many files for dsk format")

    image[dirpos] = 19 # code file

    for i in range(10):
        image[dirpos+1+i]  = ord((filename+"          ")[i])

    nsectors = 1 + (filelength+9)//510
    image[dirpos+11] = nsectors // 256 # MSB number of sectors used
    image[dirpos+12] = nsectors % 256 # LSB number of sectors used
    
    starting_side =  (4 + sectors_already_used//10)//80
    starting_track = (4 + sectors_already_used//10)%80
    starting_sector = sectors_already_used%10 + 1
    
    image[dirpos+13] = starting_track + 128*starting_side # starting track
    image[dirpos+14] = starting_sector # starting sector

    # 15 - 209 sector address map
 
    # write table of used sectors (can precalculate from number of used bits)
    while nsectors > 0:
        image[dirpos+15 + sectors_already_used//8] |= (1 << (sectors_already_used & 7))
        sectors_already_used += 1
        nsectors -= 1
    
    # 210-219 MGT future and past (reserved)
        
    image[dirpos+220] = 0 # flags (reserved)

    # 221-231 File type information (n/a for code files)
    # 232-235 reserved
    
    image[dirpos+236] = codestartpage # start page number
    image[dirpos+237] = (codestartoffset%256) # page offset (in section C, 0x8000 - 0xbfff)
    image[dirpos+238] = 128 + (codestartoffset // 256)
    
    image[dirpos+239] = filelength//16384 # pages in length
    image[dirpos+240] = filelength%256 # file length % 16384
    image[dirpos+241] = (filelength%16384)//256
    
    if (execpage>0) :
        image[dirpos+242] = execpage # execution address or 255 255 255 (basicpage, L, H - offset in page C)
        image[dirpos+243] = execoffset % 256;
        image[dirpos+244] = (execoffset%16384)//256 + 128
    else:
        image[dirpos+242] = 255 # execution address or 255 255 255 (basicpage, L, H - offset in page C)
        image[dirpos+243] = 255
        image[dirpos+244] = 255


    side = starting_side
    track = starting_track
    sector = starting_sector
    fpos = 0

# write file's 9 byte header and file
    imagepos = dsk_at(track,side,sector)
    
# 0       File type               19 for a code file
    image[imagepos + 0] = 19
# 1-2     Modulo length           Length of file % 16384
    image[imagepos + 1] = filelength%256
    image[imagepos + 2] = (filelength%16384)//256
# 3-4     Offset start            Start address
    image[imagepos + 3] = (codestartoffset%256)
    image[imagepos + 4] = 128 + (codestartoffset // 256)
# 5-6     Unused
# 7       Number of pages
    image[imagepos + 7] = filelength//16384
# 8       Starting page number
    image[imagepos + 8] = codestartpage

    start_of_file = True
    while fpos < filelength:
        imagepos = dsk_at(track,side,sector)
        unadjustedimagepos = imagepos
        if start_of_file:
            if filelength > 500:
                copylen = 501
            else:
                copylen = filelength
            imagepos += 9
            start_of_file = False
        else:
            if (filelength-fpos) > 509:
                copylen = 510
            else:
                copylen = (filelength-fpos)
        
        if fromfile != None:
            image[imagepos:imagepos+copylen] = fromfile[fpos:fpos+copylen]
        else:
            if ((fpos+firstpageoffset)//16384) == (((fpos+codestartoffset)+copylen-1)//16384):
                if memory[codestartpage+(fpos+codestartoffset)//16384] != '':
                    image[imagepos:imagepos+copylen] = memory[codestartpage+(fpos+firstpageoffset)//16384][(fpos+codestartoffset)%16384 : (fpos+codestartoffset)%16384+copylen]
            else:
                copylen1 = 16384 - ((fpos+codestartoffset)%16384)
                page1 = (codestartpage+(fpos+codestartoffset)//16384)
                if memory[page1] != '':
                    image[imagepos:imagepos+copylen1] = memory[page1][(fpos+codestartoffset)%16384 : ((fpos+codestartoffset)%16384)+copylen1]
                if (page1 < 31) and memory[page1+1] != '':
                    image[imagepos+copylen1:imagepos+copylen] = memory[page1+1][0 : ((fpos+codestartoffset)+copylen)%16384]
        
        fpos += copylen
        
        sector += 1
        if sector == 11:
            sector = 1
            track += 1
            if track == 80:
                track = 0
                side += 1
                if side==2:
                    fatal("Disk full writing "+filename)
        
        # pointers to next sector and track
        if (fpos < filelength):
            image[unadjustedimagepos+510] = track + 128*side
            image[unadjustedimagepos+511] = sector


def save_disk_image(image, pathname):
    imagestr = image.tostring()
    if ZIP:
        dskfile = gzip.open(pathname, 'wb')
    else:
        dskfile = open(pathname, 'wb')
    dskfile.write(imagestr)
    dskfile.close()
    

def save_memory(memory, image=None, filename=None):
    global firstpage,firstpageoffset
    
    if firstpage==32:
        # code was assembled without using a DUMP directive
        firstpage = 1
        firstpageoffset = 0
    
    if memory[firstpage] != '':
        # check that something has been assembled at all
    
        filelength = (lastpage - firstpage + 1) * 16384
        
        filelength -= firstpageoffset
        filelength -= 16384-lastpageoffset
        
        if (autoexecpage>0) :
            savefilename = ("AUTO" + filename + "    ")[:8]+".O"
        else:
            savefilename = (filename + "    ")[:8]+".O"

        if image:
            add_file_to_disk_image(image,savefilename,firstpage, firstpageoffset, autoexecpage, autoexecorigin, filelength)
        else:
            save_memory_to_file(filename, firstpage, firstpageoffset, filelength)

def save_file_to_image(image, pathname):
    sam_filename = os.path.basename(pathname)
    
    if len(sam_filename)>10:
        if sam_filename.count("."):
            extpos = sam_filename.rindex(".")
            extlen = len(sam_filename)-extpos
            sam_filename = sam_filename[:10-extlen] + sam_filename[extpos:]
        else:
            sam_filename = sam_filename[:10]
    
    add_file_to_disk_image(image,sam_filename, 1, 0, fromfile=pathname)

def save_memory_to_file(filename, firstusedpage, firstpageoffset, filelength):
    objfile = open(filename, 'wb')
    
    flen = filelength
    page = firstusedpage
    offset = firstpageoffset
    
    while flen:
        wlen = min(16384-offset, flen)
        
        if memory[page] != "":
            pagestr = memory[page].tostring()
            objfile.write(pagestr[offset:offset+wlen])
            
            
        else:
            # write wlen nothings into the file
            objfile.seek(wlen,1)
        
        flen -= wlen
        page += 1
        offset=0
    
    objfile.close()

def warning(message):
    print(global_currentfile, 'warning:', message)
    print('\t', global_currentline.strip())

def fatal(message):
    print(global_currentfile, 'error:', message)
    print ('\t', global_currentline.strip())
    sys.exit(1)

def expand_symbol(sym):
    while 1:
        match = re.search('\{([^\{\}]*)\}', sym)
        if match:            
            value = parse_expression(match.group(1))
            sym = sym.replace(match.group(0),str(value))
        else:
            break
    return sym

def file_and_stack(explicit_currentfile=None):
    
    if explicit_currentfile==None:
        explicit_currentfile = global_currentfile
    
    f,l = explicit_currentfile.split(':')
    s=''
    for i in forstack:
        s=s+"^"+str(i[2])
    return f+s+':'+l

def set_symbol(sym, value, explicit_currentfile=None, is_label=False):
    symorig = expand_symbol(sym)
    sym = symorig if CASE else symorig.upper()

    if sym[0]=='@':
        sym = sym + '@' + file_and_stack(explicit_currentfile=explicit_currentfile)
    symboltable[sym] = value 
    symbolcase[sym] = symorig

    if is_label:
        labeltable[sym] = value

def get_symbol(sym):
    symorig = expand_symbol(sym)
    sym = symorig if CASE else symorig.upper()
 
    if sym[0]=='@':
        if (sym + '@' + file_and_stack()) in symboltable:
            return symboltable[sym + '@' + file_and_stack()]
        else:
            if len(sym) > 1 and (sym[1]=='-' or sym[1]=='+'):
                directive = sym[1]
                sym = sym[0]+sym[2:]
            else:
                directive=''
            
            reqfile,reqline = file_and_stack().split(':')
            reqline = int(reqline)
 
            closestKey = None
            for key in symboltable:
                if (sym+'@'+reqfile+":").startswith(key.split(":",1)[0]+":") or (sym+'@'+reqfile+":").startswith(key.split(":",1)[0]+"^"):
                    # key is allowed fewer layers of FOR stack, but any layers it has must match
                    # ensure a whole number (ie 1 doesn't match 11) by forceing a colon or hat
                    symfile,symline = key.split(':')
                    symline=int(symline)
                    
                    difference = reqline - symline
                    
                    if (difference < 0 or directive != '+') and (difference > 0 or directive != '-') and ((closestKey == None) or (abs(difference) < closest)):
                        closest = abs(difference)
                        closestKey = key

            if (not closestKey) and (directive == '-'):
                global include_stack
                use_include_stack = include_stack
                use_include_stack.reverse()
                # try searching up the include stack
                for include_item in use_include_stack:
                    include_file, include_line = include_item[1].split(":",1)
                    if not closestKey:
                        for key in symboltable:
                            if (sym+'@'+include_file+":").startswith(key.split(":",1)[0]+":") or (sym+'@'+include_file+":").startswith(key.split(":",1)[0]+"^"):
                                # key is allowed fewer layers of FOR stack, but any layers it has must match
                                # ensure a whole number (ie 1 doesn't match 11) by forceing a colon or hat
                                symfile,symline = key.split(':')
                                symline=int(symline)

                                difference = int(include_line) - symline
                                
                                if (difference < 0 or directive != '+') and (difference > 0 or directive != '-') and ((closestKey == None) or (abs(difference) < closest)):
                                    closest = abs(difference)
                                    closestKey = key
    
    

            if closestKey != None:
                sym = closestKey

            
    
    
    if sym in symboltable:
        symusetable[sym] = symusetable.get(sym,0)+1
        return symboltable[sym]

    return None




def parse_expression(arg, signed=0, byte=0, word=0, silenterror=0):
    if ',' in arg:
        if silenterror:
            return ''
        fatal("Erroneous comma in expression"+arg)
    
    while 1:
        match = re.search('"(.)"', arg)
        if match:
            arg = arg.replace('"'+match.group(1)+'"',str(ord(match.group(1))))
        else:
            break

    while 1:
        match = re.search('defined\s*\(\s*(.*)\s*\)', arg, re.IGNORECASE)
        if match:
            result = (get_symbol(match.group(1)) != None)
            arg = arg.replace(match.group(0),str(int(result)))
            
        else:
            break
    
    arg = arg.replace('$','('+str(origin)+')')
    arg = arg.replace('%','0b') # COMET syntax for binary literals (parsed later, change to save confusion with modulus)
    arg = arg.replace('\\','%') # COMET syntax for modulus
    arg = arg.replace('&','0x') # COMET syntax for hex numbers
    
    #    don't do these except at the start of a token:    
    arg = re.sub('\\b0X', '0x', arg) # darnit, this got capitalized
    arg = re.sub('\\b0B', '0b', arg) # darnit, this got capitalized
        
# if the argument contains letters at this point,
# it's a symbol which needs to be replaced
    
    testsymbol=''
    argcopy = ''
    
    incurly = 0
    inquotes = False
    
    for c in arg+' ':
        if c.isalnum() or c in '"_.@{}' or (c=="+" and testsymbol=='@') or (c=="-" and testsymbol=='@') or incurly or inquotes:
            testsymbol += c
            if c=='{':
                incurly += 1
            elif c=='}':
                incurly -= 1
            elif c=='"':
                inquotes = not inquotes


        else:
            if (testsymbol != ''):
                if not testsymbol[0].isdigit():
                    result = get_symbol(testsymbol)
                    if (result != None):
                        testsymbol = str(result)
                    elif testsymbol[0] == '"' and testsymbol[-1]=='"':
                        # string literal used in some expressions
                        pass
                    else:
                        understood = 0
                        # some of python's math expressions should be available to the parser
                        if not understood and testsymbol.lower() != 'e':
                            parsestr = 'math.'+testsymbol.lower()
                            try:
                                eval(parsestr)
                                understood = 1
                            except:
                                understood = 0
                        
                        if not understood:
                            parsestr = 'random.'+testsymbol.lower()
                            try:
                                eval(parsestr)
                                understood = 1
                            except:
                                understood = 0
                        
                        if testsymbol in ["FILESIZE"]:
                            parsestr = 'os.path.getsize'
                            understood = 1
                        
                        if not understood :
                            if silenterror:
                                return ''
                            fatal("Error in expression "+arg+": Undefined symbol "+expand_symbol(testsymbol))
                        
                        testsymbol = parsestr
                        
                elif testsymbol[0]=='0' and len(testsymbol)>2 and testsymbol[1]=='b':
                # binary literal
                    literal = 0
                    for digit in testsymbol[2:]:
                        literal *= 2
                        if digit == '1':
                            literal += 1
                    testsymbol = str(literal)
                
                elif testsymbol[0]=='0' and len(testsymbol)>1 and testsymbol[1]!='x':
                # literals with leading zero would be treated as octal,
                # COMET source files do not expect this
                    decimal = testsymbol
                    while decimal[0] == '0' and len(decimal)>1:
                        decimal = decimal[1:]
                    testsymbol = decimal
                
                argcopy += testsymbol
                testsymbol = ''
            argcopy += c
    
    if NOBODMAS:
        # add bracket pairs at interesting locations to simulate left-to-right evaluation
        
        aslist = list(argcopy) # turn it into a list so that we can add characters without affecting indexes
        bracketstack=[0]
        symvalid = False
    
        for c in range (len(aslist)):
            if aslist[c] == "(":
                bracketstack = [c]+bracketstack
            elif aslist[c] == ")":
                bracketstack = bracketstack[1:]
            elif (not aslist[c].isalnum()) and (not aslist[c]=='.') and (not aslist[c].isspace()) and symvalid:
                aslist[c] = ")"+aslist[c]
                aslist[bracketstack[0]] = '('+aslist[bracketstack[0]]
                symvalid = False
            elif aslist[c].isalnum():
                symvalid = True
    
        argcopy2=""
        for entry in aslist:
            argcopy2 += entry
    #    print(argcopy,"->",argcopy2)
        argcopy = argcopy2
    
    farg = eval(argcopy)
    if farg >= -.5:
        farg += 0.5
    else:
        farg -= 0.5
    narg = int(farg)
#    print(arg, " -> ",argcopy," == ",narg)
    
    if signed:
        if byte:
            if (  -128 < narg > 127):
                warning ("Signed byte value truncated from "+str(narg));
            narg = (narg + 128)%256 -128
        elif word:
            if (  -32768 < narg > 32767):
                warning ("Signed word value truncated from "+str(narg));
            narg = (narg + 32768)%65536 - 32768
    else:
        if byte:
            if (  0 < narg > 255):
                warning ("Unsigned byte value truncated from "+str(narg));
            narg %= 256
        elif word:
            if (  0 < narg > 65535):
                warning ("Unsigned byte value truncated from "+str(narg));
            narg %= 65536
    return narg

def double(arg, allow_af_instead_of_sp=0, allow_af_alt=0, allow_index=1):
# decode double register [bc, de, hl, sp][ix,iy] --special:  af af'
    double_mapping = {'BC':([],0), 'DE':([],1), 'HL':([],2), 'SP':([],3), 'IX':([0xdd],2), 'IY':([0xfd],2), 'AF':([],5), "AF'":([],4) }
    rr = double_mapping.get(arg.strip().upper(),([],-1))
    if (rr[1]==3) and allow_af_instead_of_sp:
        rr = ([],-1)
    if rr[1]==5:
        if allow_af_instead_of_sp:
            rr = ([],3)
        else:
            rr = ([],-1)
    if (rr[1]==4) and not allow_af_alt:
        rr = ([],-1)
    
    if (rr[0] != []) and not allow_index:
        rr = ([],-1)
    
    return rr

def single(arg, allow_i=0, allow_r=0, allow_index=1, allow_offset=1, allow_half=1):
#decode single register [b,c,d,e,h,l,(hl),a][(ix {+c}),(iy {+c})]
    single_mapping = {'B':0, 'C':1, 'D':2, 'E':3, 'H':4, 'L':5, 'A':7, 'I':8, 'R':9, 'IXH':10, 'IXL':11, 'IYH':12, 'IYL':13 }
    m = single_mapping.get(arg.strip().upper(),-1)
    prefix=[]
    postfix=[]
    
    if m==8 and not allow_i:
        m = -1
    if m==9 and not allow_r:
        m = -1
 
    if allow_half:
        if m==10:
            prefix = [0xdd]
            m = 4
        if m==11:
            prefix = [0xdd]
            m = 5
        if m==12:
            prefix = [0xfd]
            m = 4
        if m==13:
            prefix = [0xfd]
            m = 5
    else:
        if m >= 10 and m <= 13:
            m = -1
    
    if m==-1 and re.search("\A\s*\(\s*HL\s*\)\s*\Z", arg, re.IGNORECASE):
        m = 6
    
    if m==-1 and allow_index:
        match = re.search("\A\s*\(\s*IX\s*\)\s*\Z", arg, re.IGNORECASE)
        if match:
            m = 6
            prefix = [0xdd]
            postfix = [0]
            
        elif allow_offset:
            match = re.search("\A\s*\(\s*IX\s*([+-].*)\s*\)\s*\Z", arg, re.IGNORECASE)
            if match:
                m = 6
                prefix = [0xdd]
                if p==2:
                    postfix = [parse_expression(match.group(1), byte=1)]
                else:
                    postfix = [0]

    if m==-1 and allow_index:
        match = re.search("\A\s*\(\s*IY\s*\)\s*\Z", arg, re.IGNORECASE)
        if match:
            m = 6
            prefix = [0xfd]
            postfix = [0]

        elif allow_offset:
            match = re.search("\A\s*\(\s*IY\s*([+-].*)\s*\)\s*\Z", arg, re.IGNORECASE)
            if match:
                m = 6
                prefix = [0xfd]
                if p==2:
                    postfix = [parse_expression(match.group(1), byte=1)]
                else:
                    postfix = [0]
    
    return prefix,m,postfix

def condition(arg):
# decode condition [nz, z, nc, c, po, pe, p, m]
    condition_mapping = {'NZ':0, 'Z':1, 'NC':2, 'C':3, 'PO':4, 'PE':5, 'P':6, 'M':7 }
    return condition_mapping.get(arg.upper(),-1)


def dump(bytes):
    def initpage(page):
        memory[page] = array.array('B')
        
        memory[page].append(0)
        while len(memory[page]) < 16384:
            memory[page].extend(memory[page])
    
    global dumppage, dumporigin, dumpspace_pending, lstcode, listingfile
    
    if (p==2):
        if dumpspace_pending > 0:
            dumporigin += dumpspace_pending
            dumppage += dumporigin // 16384
            dumporigin %= 16384
            dumpspace_pending = 0
        
        if memory[dumppage]=='':
            initpage(dumppage)
        lstcode = ""
        for b in bytes:
           # if b<0 or b>255:
           #     warning("Dump byte out of range")
            memory[dumppage][dumporigin] = b
            if listingfile != None:
              lstcode=lstcode+"%02X "%(b)
            dumporigin += 1
            if dumporigin == 16384:
                dumporigin = 0
                dumppage += 1
                
                if memory[dumppage]=='':
                    initpage(dumppage)

def check_args(args,expected):
    if args=='':
        received = 0
    else:
        received = len(args.split(','))
    if expected!=received:
        fatal("Opcode wrong number of arguments, expected "+str(expected)+" received "+str(args))

def op_ORG(p,opargs):
    global origin
    check_args(opargs,1)
    origin = parse_expression(opargs, word=1)
    return 0

def op_DUMP(p,opargs):
    global dumppage, dumporigin, dumpused, firstpage, firstpageoffset, dumpspace_pending

    if dumpused:
        check_lastpage()
    dumpused = True

    dumpspace_pending = 0
    if ',' in opargs:
        page,offset = opargs.split(',',1)
        offset = parse_expression(offset, word=1)
        dumppage = parse_expression(page) + (offset//16384)
        dumporigin = offset % 16384
    else:
        offset = parse_expression(opargs)
        if (offset<16384):
            error ("DUMP value out of range")
        dumppage = (offset//16384) - 1
        dumporigin = offset % 16384
    
    if ((dumppage*16384 + dumporigin) < (firstpage*16384 + firstpageoffset)):
        firstpage = dumppage
        firstpageoffset = dumporigin
    
    return 0

def op_PRINT(p, opargs):
    text = []
    for expr in opargs.split(","):
        if expr.strip().startswith('"'):
            text.append(expr.strip().rstrip()[1:-1])
        else:
            a = parse_expression(expr, silenterror=True)
            if a:
                text.append(str(a))
            else:
                text.append("?")
    print(global_currentfile, "PRINT: ", ",".join(text))
    return 0

def check_lastpage():
    global lastpage, lastpageoffset
    if dumppage > lastpage:
        lastpage = dumppage
        lastpageoffset = dumporigin
    elif (dumppage == lastpage) and (dumporigin > lastpageoffset):
        lastpageoffset = dumporigin

def op_AUTOEXEC(p,opargs):
    global autoexecpage, autoexecorigin
    check_args(opargs,0)
    if (p==2):
        if (autoexecpage>0 or autoexecorigin>0):
            fatal("AUTOEXEC may only be used once.");
        autoexecpage = dumppage + 1; # basic type page numbering
        autoexecorigin = dumporigin;

    
    return 0

def op_EQU(p,opargs):
    global symboltable
    check_args(opargs,1)
    if (symbol):
        if opargs.upper().startswith("FOR") and (opargs[3].isspace() or opargs[3]=='('):
            set_symbol(symbol, 0)
            
            limit = parse_expression(opargs[4:].strip())
            if limit < 1:
                error("FOR range < 1 not allowed")
            forstack.append( [symbol,global_currentfile,0,limit] )

        else:
            if p==1:
                set_symbol(symbol, parse_expression(opargs, signed=1, silenterror=1))
            else:
                expr_result = parse_expression(opargs, signed=1)

                existing = get_symbol(symbol)

                if existing == '':
                    set_symbol(symbol, expr_result)
                elif existing != expr_result:
                    fatal("Symbol "+expand_symbol(symbol)+": expected "+str(existing)+" but calculated "+str(expr_result)+", has this symbol been used twice?")
    else:
        warning("EQU without symbol name")
    return 0

def op_NEXT(p,opargs):
    global global_currentfile

    
    check_args(opargs,1)
    foritem = forstack.pop()
    if opargs != foritem[0]:
        fatal("NEXT symbol "+opargs+" doesn't match FOR: expected "+foritem[0])
    foritem[2] += 1

    set_symbol(foritem[0], foritem[2], explicit_currentfile=foritem[1])

    if foritem[2] < foritem[3]:
        global_currentfile = foritem[1]
        forstack.append(foritem)
    
    return 0

def op_ALIGN(p,opargs):
    global dumpspace_pending
    check_args(opargs,1)

    align = parse_expression(opargs)
    if align<1:
        fatal("Invalid alignment")
    elif (align & (-align)) != align:
        fatal("Alignment is not a power of 2")

    s = (align - origin%align)%align
    dumpspace_pending += s
    return s;

def op_DS(p,opargs):
    return op_DEFS(p,opargs)
def op_DEFS(p,opargs):
    global dumppage, dumporigin, dumpspace_pending
    check_args(opargs,1)
    
    if opargs.upper().startswith("ALIGN") and (opargs[5].isspace() or opargs[5]=='('):
        return op_ALIGN(p,opargs[5:].strip())

    s = parse_expression(opargs)
    if s<0:
        fatal("Allocated invalid space < 0 bytes ("+str(s)+")")
    dumpspace_pending += s
    return s

def op_DB(p,opargs):
    return op_DEFB(p,opargs)
def op_DEFB(p,opargs):
    s = opargs.split(',')
    if (p==2):
        for b in s:
            byte=(parse_expression(b, byte=1, silenterror=1))
            if byte=='':
                fatal("Didn't understand DB or character constant "+b)
            else:
                dump([byte])
    return len(s)

def op_DW(p,opargs):
    return op_DEFW(p,opargs)
def op_DEFW(p,opargs):
    s = opargs.split(',')
    if (p==2):
        for b in s:
            b=(parse_expression(b, word=1))
            dump([b%256, b//256])
    return 2*len(s)

def op_DM(p,opargs):
    return op_DEFM(p,opargs)
def op_DEFM(p,opargs):
    messagelen = 0
    if opargs.strip()=="44" or opargs=="(44)":
        dump ([44])
        messagelen = 1
    else:
        matchstr = opargs
        while matchstr.strip():
            match = re.match('\s*\"(.*)\"(\s*,)?(.*)', matchstr)
            if not match:
                match = re.match('\s*([^,]*)(\s*,)?(.*)', matchstr)
                byte=(parse_expression(match.group(1), byte=1, silenterror=1))
                if byte=='':
                    fatal("Didn't understand DM character constant "+b)
                elif p==2:
                    dump([byte])

                messagelen += 1            
            else:
                message = list(match.group(1))

                if p==2:
                    for i in message:
                        dump ([ord(i)])
                messagelen += len(message)

            matchstr = match.group(3)

            if match.group(3) and not match.group(2):
                matchstr = '""' + matchstr
                # For cases such as  DEFM "message with a "" in it"
                # I can only apologise for this, this is an artefact of my parsing quotes
                # badly at the top level but it's too much for me to go back and refactor it all.
                # Of course, it would have helped if Comet had had sane quoting rules in the first place.
    return messagelen

def op_MDAT(p,opargs):
    global dumppage, dumporigin
    match = re.search('\A\s*\"(.*)\"\s*\Z', opargs)
    filename = os.path.join(global_path, match.group(1))
    
    try:
        mdatfile = open(filename,'rb')
    except:
        fatal("Unable to open file for reading: "+filename)
    
    mdatfile.seek(0,2)
    filelength = mdatfile.tell()
    if p==1:
        dumporigin += filelength
        dumppage += dumporigin // 16384
        dumporigin %= 16384
    elif p==2:
        mdatfile.seek(0)
        mdatafilearray = array.array('B')
        mdatafilearray.fromfile(mdatfile, filelength)
        dump(mdatafilearray)
    
    mdatfile.close()
    return filelength

def op_INCLUDE(p,opargs):
    global global_path, global_currentfile
    global include_stack
    
    savedorigin = origin
    
    match = re.search('\A\s*\"(.*)\"\s*\Z', opargs)
    filename = match.group(1)
    
    include_stack.append((global_path, global_currentfile))
    assembler_pass(p, filename)
    global_path, global_currentfile = include_stack.pop()

    return 0
    # global origin has already been updated


def op_FOR(p,opargs):
    args = opargs.split(',',1)
    limit = parse_expression(args[0])
    bytes = 0
    for iterate in range(limit):
        symboltable['FOR'] = iterate
        if CASE:
            symboltable['for'] = iterate
        bytes += assemble_instruction(p,args[1].strip())
    
    if limit != 0:
        del symboltable['FOR']
        if CASE:
            del symboltable['for']

    return bytes

def op_noargs_type(p,opargs,instr):
    check_args(opargs,0)
    if (p==2):
        dump(instr)
    return len(instr)
   
def op_ASSERT(p,opargs):
    check_args(opargs,1)
    if (p==2):
        value = parse_expression(opargs)
        if value == 0:
            fatal("Assertion failed ("+opargs+")")
    return 0


def op_NOP(p,opargs):
    return op_noargs_type(p,opargs,[0x00])
def op_RLCA(p,opargs):
    return op_noargs_type(p,opargs,[0x07])
def op_RRCA(p,opargs):
    return op_noargs_type(p,opargs,[0x0F])
def op_RLA(p,opargs):
    return op_noargs_type(p,opargs,[0x17])
def op_RRA(p,opargs):
    return op_noargs_type(p,opargs,[0x1F])
def op_DAA(p,opargs):
    return op_noargs_type(p,opargs,[0x27])
def op_CPL(p,opargs):
    return op_noargs_type(p,opargs,[0x2F])
def op_SCF(p,opargs):
    return op_noargs_type(p,opargs,[0x37])
def op_CCF(p,opargs):
    return op_noargs_type(p,opargs,[0x3F])
def op_HALT(p,opargs):
    return op_noargs_type(p,opargs,[0x76])
def op_DI(p,opargs):
    return op_noargs_type(p,opargs,[0xf3])
def op_EI(p,opargs):
    return op_noargs_type(p,opargs,[0xfb])
def op_EXX(p,opargs):
    return op_noargs_type(p,opargs,[0xd9])
def op_NEG(p,opargs):
    return op_noargs_type(p,opargs,[0xed,0x44])
def op_RETN(p,opargs):
    return op_noargs_type(p,opargs,[0xed,0x45])
def op_RETI(p,opargs):
    return op_noargs_type(p,opargs,[0xed,0x4d])
def op_RRD(p,opargs):
    return op_noargs_type(p,opargs,[0xed,0x67])
def op_RLD(p,opargs):
    return op_noargs_type(p,opargs,[0xed,0x6F])
def op_LDI(p,opargs):
    return op_noargs_type(p,opargs,[0xed,0xa0])
def op_CPI(p,opargs):
    return op_noargs_type(p,opargs,[0xed,0xa1])
def op_INI(p,opargs):
    return op_noargs_type(p,opargs,[0xed,0xa2])
def op_OUTI(p,opargs):
    return op_noargs_type(p,opargs,[0xed,0xa3])
def op_LDD(p,opargs):
    return op_noargs_type(p,opargs,[0xed,0xa8])
def op_CPD(p,opargs):
    return op_noargs_type(p,opargs,[0xed,0xa9])
def op_IND(p,opargs):
    return op_noargs_type(p,opargs,[0xed,0xaa])
def op_OUTD(p,opargs):
    return op_noargs_type(p,opargs,[0xed,0xab])
def op_LDIR(p,opargs):
    return op_noargs_type(p,opargs,[0xed,0xb0])
def op_CPIR(p,opargs):
    return op_noargs_type(p,opargs,[0xed,0xb1])
def op_INIR(p,opargs):
    return op_noargs_type(p,opargs,[0xed,0xb2])
def op_OTIR(p,opargs):
    return op_noargs_type(p,opargs,[0xed,0xb3])
def op_LDDR(p,opargs):
    return op_noargs_type(p,opargs,[0xed,0xb8])
def op_CPDR(p,opargs):
    return op_noargs_type(p,opargs,[0xed,0xb9])
def op_INDR(p,opargs):
    return op_noargs_type(p,opargs,[0xed,0xba])
def op_OTDR(p,opargs):
    return op_noargs_type(p,opargs,[0xed,0xbb])

def op_cbshifts_type(p,opargs,offset,step_per_register=1):
    args = opargs.split(',',1)
    if len(args) == 2:
        # compound instruction of the form RLC B,(IX+c)
        pre1,r1,post1 = single(args[0], allow_half=0, allow_index=0)
        pre2,r2,post2 = single(args[1], allow_half=0, allow_index=1)
        if r1==-1 or r2==-1:
            fatal("Registers not recognized for compound instruction")
        if r1==6:
            fatal("(HL) not allowed as target of compound instruction")
        if len(pre2)==0:
            fatal("Must use index register as operand of compound instruction")
                
        instr=pre2
        instr.extend([0xcb])
        instr.extend(post2)
        instr.append(offset + step_per_register*r1)
        
    else:
        check_args(opargs,1)
        pre,r,post = single(opargs,allow_half=0)
        instr = pre
        instr.extend([0xcb])
        instr.extend(post)
        if r==-1:
            fatal ("Invalid argument")
        else:
            instr.append(offset + step_per_register*r)

    if (p==2):
        dump(instr)
    return len(instr)

def op_RLC(p,opargs):
    return op_cbshifts_type(p,opargs,0x00)
def op_RRC(p,opargs):
    return op_cbshifts_type(p,opargs,0x08)
def op_RL(p,opargs):
    return op_cbshifts_type(p,opargs,0x10)
def op_RR(p,opargs):
    return op_cbshifts_type(p,opargs,0x18)
def op_SLA(p,opargs):
    return op_cbshifts_type(p,opargs,0x20)
def op_SRA(p,opargs):
    return op_cbshifts_type(p,opargs,0x28)
def op_SLL(p,opargs):
    if (p==1):
        warning("SLL doesn't do what you probably expect on z80b! Use SL1 if you know what you're doing.")
    return op_cbshifts_type(p,opargs,0x30)
def op_SL1(p,opargs):
    return op_cbshifts_type(p,opargs,0x30)
def op_SRL(p,opargs):
    return op_cbshifts_type(p,opargs,0x38)



def op_register_arg_type(p,opargs,offset,ninstr,step_per_register=1):
    check_args(opargs,1)
    pre,r,post = single(opargs,allow_half=1)
    instr = pre
    if r==-1:
        match = re.search("\A\s*\(\s*(.*)\s*\)\s*\Z", opargs)
        if match:
            fatal ("Illegal indirection")

        instr.extend(ninstr)
        if (p==2):
            n = parse_expression(opargs, byte=1)
        else:
            n = 0
        instr.append(n)
    else:
        instr.append(offset + step_per_register*r)
    instr.extend(post)
    if (p==2):
        dump(instr)
    return len(instr)

def op_SUB(p,opargs):
    return op_register_arg_type(p,opargs, 0x90, [0xd6])
def op_AND(p,opargs):
    return op_register_arg_type(p,opargs, 0xa0, [0xe6])
def op_XOR(p,opargs):
    return op_register_arg_type(p,opargs, 0xa8, [0xee])
def op_OR(p,opargs):
    return op_register_arg_type(p,opargs, 0xb0, [0xf6])
def op_CP(p,opargs):
    return op_register_arg_type(p,opargs, 0xb8, [0xfe])

def op_registerorpair_arg_type(p,opargs,rinstr,rrinstr,step_per_register=8,step_per_pair=16):
    check_args(opargs,1)
    pre,r,post = single(opargs)
    
    if r==-1:
        pre,rr = double(opargs)
        if rr==-1:
            fatal ("Invalid argument")
        
        instr = pre
        instr.append(rrinstr + step_per_pair*rr)
    else:
        instr = pre
        instr.append(rinstr + step_per_register*r)
        instr.extend(post)
    if (p==2):
        dump(instr)
    return len(instr)

def op_INC(p,opargs):
# Oh dear - COMET also used "INC" for INClude source file
    if '"' in opargs:
        return op_INCLUDE(p,opargs)
    
    return op_registerorpair_arg_type(p,opargs, 0x04, 0x03)
def op_DEC(p,opargs):
    return op_registerorpair_arg_type(p,opargs, 0x05, 0x0b)

def op_add_type(p,opargs,rinstr,ninstr,rrinstr,step_per_register=1,step_per_pair=16):
    args = opargs.split(',',1)
    r=-1
    
    if len(args) == 2:
        pre,r,post = single(args[0])
    
    if (len(args) == 1) or r==7:
        pre,r,post = single(args[-1])
        instr = pre
        if r==-1:
            match = re.search("\A\s*\(\s*(.*)\s*\)\s*\Z", args[-1])
            if match:
                fatal ("Illegal indirection")

            instr.extend(ninstr)
            if (p==2):
                n = parse_expression(args[-1], byte=1)
            else:
                n = 0
            instr.append(n)
        else:
            instr.extend(rinstr)
            instr[-1] += step_per_register*r
        
        instr.extend(post)
    else:
        pre,rr1 = double(args[0])
        dummy,rr2 = double(args[1])
        
        if (rr1 == rr2) and (pre != dummy):
            fatal ("Can't mix index registers and HL")
        if (len(rrinstr) > 1) and pre:
            fatal ("Can't use index registers in this instruction")
        
        if (len(args) != 2) or (rr1 != 2):
            fatal("Invalid argument")
        
        instr = pre
        instr.extend(rrinstr)
        instr[-1] += step_per_pair*rr2
    
    if (p==2):
        dump(instr)
    return len(instr)

def op_ADD(p,opargs):
    return op_add_type(p,opargs,[0x80], [0xc6],[0x09])
def op_ADC(p,opargs):
    return op_add_type(p,opargs,[0x88], [0xce],[0xed,0x4a])
def op_SBC(p,opargs):
    return op_add_type(p,opargs,[0x98], [0xde],[0xed,0x42])

def op_bit_type(p,opargs,offset):
    check_args(opargs,2)
    arg1,arg2 = opargs.split(',',1)
    b = parse_expression(arg1)
    if b>7 or b<0:
        fatal ("argument out of range")
    pre,r,post = single(arg2,allow_half=0)
    instr = pre
    instr.append(0xcb)
    instr.extend(post)
    instr.append(offset + r + 8*b)
    if (p==2):
        dump(instr)
    return len(instr)

def op_BIT(p,opargs):
    return op_bit_type(p,opargs, 0x40)
def op_RES(p,opargs):
    return op_bit_type(p,opargs, 0x80)
def op_SET(p,opargs):
    return op_bit_type(p,opargs, 0xc0)

def op_pushpop_type(p,opargs,offset):
    
    check_args(opargs,1)
    prefix, rr = double(opargs, allow_af_instead_of_sp=1)
    instr = prefix
    if rr==-1:
        fatal ("Invalid argument")
    else:
        instr.append(offset + 16 * rr)
    if (p==2):
        dump(instr)
    return len(instr)

def op_POP(p,opargs):
    return op_pushpop_type(p,opargs, 0xc1)
def op_PUSH(p,opargs):
    return op_pushpop_type(p,opargs, 0xc5)

def op_jumpcall_type(p,opargs,offset, condoffset):
    args = opargs.split(',',1)
    if len(args) == 1:
        instr = [offset]
    else:
        cond = condition(args[0])
        if cond == -1:
            fatal ("Expected condition, received "+opargs)
        instr = [condoffset + 8*cond]

    match = re.search("\A\s*\(\s*(.*)\s*\)\s*\Z", args[-1])
    if match:
        fatal ("Illegal indirection")
    
    if (p==2):
        nn = parse_expression(args[-1],word=1)
        instr.extend([nn%256, nn//256])
        dump(instr)
    
    return 3

def op_JP(p,opargs):
    if (len(opargs.split(',',1)) == 1):
        prefix, r, postfix = single(opargs, allow_offset=0,allow_half=0)
        if r==6:
            instr = prefix
            instr.append(0xe9)
            if (p==2):
                dump(instr)
            return len(instr)
    return op_jumpcall_type(p,opargs, 0xc3, 0xc2)

def op_CALL(p,opargs):
    return op_jumpcall_type(p,opargs, 0xcd, 0xc4)

def op_DJNZ(p,opargs):
    check_args(opargs,1)
    if (p==2):
        target = parse_expression(opargs,word=1)
        displacement = target - (origin + 2)
        if displacement > 127 or displacement < -128:
            fatal ("Displacement from "+str(origin)+" to "+str(target)+" is out of range")
        dump([0x10,(displacement+256)%256])
    
    return 2

def op_JR(p,opargs):
    args = opargs.split(',',1)
    if len(args) == 1:
        instr = 0x18
    else:
        cond = condition(args[0])
        if cond == -1:
            fatal ("Expected condition, received "+opargs)
        elif cond >= 4:
            fatal ("Invalid condition for JR")
        instr = 0x20 + 8*cond
    if (p==2):
        target = parse_expression(args[-1],word=1)
        displacement = target - (origin + 2)
        if displacement > 127 or displacement < -128:
            fatal ("Displacement from "+str(origin)+" to "+str(target)+" is out of range")
        dump([instr,(displacement+256)%256])
    
    return 2

def op_RET(p,opargs):
    if opargs=='':
        if (p==2):
            dump([0xc9])
    else:
        check_args(opargs,1)
        cond = condition(opargs)
        if cond == -1:
            fatal ("Expected condition, received "+opargs)
        if (p==2):
            dump([0xc0 + 8*cond])
    return 1

def op_IM(p,opargs):
    check_args(opargs,1)
    if (p==2):
        mode = parse_expression(opargs)
        if (mode>2) or (mode<0):
            fatal ("argument out of range")
        if mode > 0:
            mode += 1
        
        dump([0xed, 0x46 + 8*mode])
    return 2

def op_RST(p,opargs):
    check_args(opargs,1)
    if (p==2):
        vector = parse_expression(opargs)
        if (vector>0x38) or (vector<0) or ((vector%8) != 0):
            fatal ("argument out of range or doesn't divide by 8")
        
        dump([0xc7 + vector])
    return 1

def op_EX(p,opargs):
    check_args(opargs,2)
    args = opargs.split(',',1)
    
    if re.search("\A\s*\(\s*SP\s*\)\s*\Z", args[0], re.IGNORECASE):
        pre2,rr2 = double(args[1],allow_af_instead_of_sp=1, allow_af_alt=1, allow_index=1)
    
        if rr2==2:
            instr = pre2
            instr.append(0xe3)
        else:
            fatal("Can't exchange "+args[0]+" with "+args[1])
    else:
        pre1,rr1 = double(args[0],allow_af_instead_of_sp=1, allow_index=0)
        pre2,rr2 = double(args[1],allow_af_instead_of_sp=1, allow_af_alt=1, allow_index=0)
    
        if rr1==1 and rr2==2:
            # EX DE,HL
            instr = pre1
            instr.extend(pre2)
            instr.append(0xeb)
        elif (rr1==3 and rr2==4):
            instr=[0x08]
        else:
            fatal("Can't exchange "+args[0]+" with "+args[1])
    
    if (p==2):
        dump(instr)
    
    return len(instr)

def op_IN(p,opargs):
    check_args(opargs,2)
    args = opargs.split(',',1)
    if (p==2):
        pre,r,post = single(args[0],allow_index=0,allow_half=0)
        if r!=-1 and r!=6 and re.search("\A\s*\(\s*C\s*\)\s*\Z", args[1], re.IGNORECASE):
            dump([0xed, 0x40+8*r])
        elif r==7:
            match = re.search("\A\s*\(\s*(.*)\s*\)\s*\Z", args[1])
            if match==None:
                fatal("No expression in "+args[1])
            
            n = parse_expression(match.group(1))
            dump([0xdb, n])
        else:
            fatal("Invalid argument")
    return 2

def op_OUT(p,opargs):
    check_args(opargs,2)
    args = opargs.split(',',1)
    if (p==2):
        pre,r,post = single(args[1],allow_index=0,allow_half=0)
        if r!=-1 and r!=6 and re.search("\A\s*\(\s*C\s*\)\s*\Z", args[0], re.IGNORECASE):
            dump([0xed, 0x41+8*r])
        elif r==7:
            match = re.search("\A\s*\(\s*(.*)\s*\)\s*\Z", args[0])
            n = parse_expression(match.group(1))
            dump([0xd3, n])
        else:
            fatal("Invalid argument")
    return 2

def op_LD(p,opargs):
    check_args(opargs,2)
    arg1,arg2 = opargs.split(',',1)
    
    prefix, rr1 = double(arg1)
    if rr1 != -1:
        prefix2, rr2 = double(arg2)
        if rr1==3 and rr2==2:
            instr = prefix2
            instr.append(0xf9)
            dump(instr)
            return len(instr)
        
        match = re.search("\A\s*\(\s*(.*)\s*\)\s*\Z", arg2)
        if match:
            # ld rr, (nn)
            if p==2:
                nn = parse_expression(match.group(1),word=1)
            else:
                nn = 0
            instr = prefix
            if rr1==2:
                instr.extend([0x2a, nn%256, nn//256])
            else:
                instr.extend([0xed, 0x4b + 16*rr1, nn%256, nn//256])
            dump(instr)
            return len (instr)
        else:
            #ld rr, nn
            if p==2:
                nn = parse_expression(arg2,word=1)
            else:
                nn = 0
            instr = prefix
            instr.extend([0x01 + 16*rr1, nn%256, nn//256])
            dump(instr)
            return len (instr)
    
    prefix, rr2 = double(arg2)
    if rr2 != -1:
        match = re.search("\A\s*\(\s*(.*)\s*\)\s*\Z", arg1)
        if match:
            # ld (nn), rr
            if p==2:
                nn = parse_expression(match.group(1))
            else:
                nn = 0
            instr = prefix
            if rr2==2:
                instr.extend([0x22, nn%256, nn//256])
            else:
                instr.extend([0xed, 0x43 + 16*rr2, nn%256, nn//256])
            dump(instr)
            return len (instr)
    
    prefix1,r1,postfix1 = single(arg1, allow_i=1, allow_r=1)
    prefix2,r2,postfix2 = single(arg2, allow_i=1, allow_r=1)
    if r1 != -1 :
        if r2 != -1:
            if (r1 > 7) or (r2 > 7):
                if r1==7:
                    if r2==8:
                        dump([0xed,0x57])
                        return 2
                    elif r2==9:
                        dump([0xed,0x5f])
                        return 2
                if r2==7:
                    if r1==8:
                        dump([0xed,0x47])
                        return 2
                    elif r1==9:
                        dump([0xed,0x4f])
                        return 2
                fatal("Invalid argument")
            
            if r1==6 and r2==6:
                fatal("Ha - nice try. That's a HALT.")
            
            if (r1==4 or r1==5) and (r2==4 or r2==5) and prefix1 != prefix2:
                fatal("Illegal combination of operands")
            
            if r1==6 and (r2==4 or r2==5) and len(prefix2) != 0:
                fatal("Illegal combination of operands")

            if r2==6 and (r1==4 or r1==5) and len(prefix1) != 0:
                fatal("Illegal combination of operands")

            instr = prefix1
            if len(prefix1) == 0:
                instr.extend(prefix2)
            instr.append(0x40 + 8*r1 + r2)
            instr.extend(postfix1)
            instr.extend(postfix2)
            dump(instr)
            return len(instr)
        
        else:
            if r1 > 7:
                fatal("Invalid argument")

            if r1==7 and re.search("\A\s*\(\s*BC\s*\)\s*\Z", arg2, re.IGNORECASE):
                dump([0x0a])
                return 1
            if r1==7 and re.search("\A\s*\(\s*DE\s*\)\s*\Z", arg2, re.IGNORECASE):
                dump([0x1a])
                return 1
            match = re.search("\A\s*\(\s*(.*)\s*\)\s*\Z", arg2)
            if match:
                if r1 != 7:
                    fatal("Illegal indirection")
                if p==2:
                    nn = parse_expression(match.group(1), word=1)
                    dump([0x3a, nn%256, nn//256])
                return 3
            
            instr = prefix1
            instr.append(0x06 + 8*r1)
            instr.extend(postfix1)
            if (p==2):
                n = parse_expression(arg2, byte=1)
            else:
                n = 0
            instr.append(n)
            dump(instr)
            return len(instr)
    
    elif r2==7:
        # ld (bc/de/nn),a
        if re.search("\A\s*\(\s*BC\s*\)\s*\Z", arg1, re.IGNORECASE):
            dump([0x02])
            return 1
        if re.search("\A\s*\(\s*DE\s*\)\s*\Z", arg1, re.IGNORECASE):
            dump([0x12])
            return 1
        match = re.search("\A\s*\(\s*(.*)\s*\)\s*\Z", arg1)
        if match:
            if p==2:
                nn = parse_expression(match.group(1), word=1)
                dump([0x32, nn%256, nn//256])
            return 3
    fatal("LD args not understood - "+arg1+", "+arg2)
    
    return 1

#ifstate=0: parse all code
#ifstate=1: parse this code, but stop at ELSE
#ifstate=2: do not parse this code, but might start at ELSE
#ifstate=3: do not parse any code until ENDIF

def op_IF(p,opargs):
    global ifstate, ifstack
    check_args(opargs,1)
    
    ifstack.append( (global_currentfile,ifstate) )
    if ifstate < 2:
        cond = parse_expression(opargs)
        if cond:
            ifstate = 1
        else:
            ifstate = 2
    else:
        ifstate = 3
    return 0

def op_ELSE(p,opargs):
    global ifstate, ifstack
    if ifstate==1 or ifstate==3:
        ifstate = 3
    elif ifstate==2:
        if opargs.upper().startswith("IF"):
            cond = parse_expression(opargs[2:].strip())
            if cond:
                ifstate = 1
            else:
                ifstate = 2
        else:
            ifstate = 1
    else:
        fatal("Mismatched ELSE")

    return 0

def op_ENDIF(p,opargs):
    global ifstate, ifstack
    check_args(opargs,0)

    if len(ifstack) == 0:
        fatal("Mismatched ENDIF")

    ifline,state = ifstack.pop()
    ifstate = state
    
    return 0

def assemble_instruction(p, line):
    opcodeargs = line.split(None,1)
    if '(' in opcodeargs[0]:
        opcodeargs = [line[:line.index('(')] , line[line.index('('):] ]

    if len(opcodeargs)>1:
        args = opcodeargs[1].strip()
    else:
        args=''
        
    inst = opcodeargs[0].upper()
    if (ifstate < 2) or inst=='IF' or inst=='ELSE' or inst=='ENDIF':
        functioncall = 'op_'+inst+'(p,args)'
        if PYTHONERRORS:
            return eval(functioncall)
        else:
            try:
                return eval(functioncall)
            except SystemExit as e:
                sys.exit(e)
            except:
                fatal("OpCode not recognised")
    else:
        return 0

def assembler_pass(p, inputfile):
    global memory, symboltable, symusetable, labeltable, origin, dumppage, dumporigin, symbol
    global global_currentfile, global_currentline, lstcode, listingfile
# file references are local, so assembler_pass can be called recursively (for op_INC)
# but copied to a global identifier for warning printouts
    global global_path
    
    global_currentfile="command line"
    global_currentline="0"
    
    # just read the whole file into memory, it's not going to be huge (probably)
    # I'd prefer not to, but assembler_pass can be called recursively
    # (by op_INCLUDE for example) and fileinput does not support two files simultaneously
    
    this_currentfilename = os.path.join(global_path, inputfile)
    if os.sep in this_currentfilename:
        global_path = os.path.dirname(this_currentfilename)
    
    try:
        currentfile = open(this_currentfilename,'r')
        wholefile=currentfile.readlines()
        wholefile.insert(0, '') # prepend blank so line numbers are 1-based
        currentfile.close()
    except:
        fatal("Couldn't open file "+this_currentfilename+" for reading")
    
    
    consider_linenumber=0
    while consider_linenumber < len(wholefile):

        currentline = wholefile[consider_linenumber]

        global_currentline = currentline
        global_currentfile = this_currentfilename+":"+str(consider_linenumber)
            # write these every instruction because an INCLUDE may have overwritten them
        
        symbol = ''
        opcode = ''
        inquotes = ''
        inquoteliteral = False
        i = ""
        for nexti in currentline+" ":
            if (i==';' or i=='#') and not inquotes:
                break
            if i==':' and not inquotes:
                symbol = opcode
                opcode=''
                i = ''
            
            if i == '"':
                if not inquotes:
                    inquotes = i
                else:
                    if (not inquoteliteral) and nexti=='"':
                        inquoteliteral = True
                        
                    elif inquoteliteral:
                        inquoteliteral = False
                        inquotes += i
                        
                    else:
                        inquotes += i
                    
                        if inquotes == '""':
                            inquotes = '"""'
                        elif inquotes == '","':
                            inquotes = " 44 "
                            i = ""
                    
                        opcode += inquotes
                        inquotes = ""
            elif inquotes:
                inquotes += i
            else:
                opcode += i
            
            lasti = i
            i = nexti
        
        symbol = symbol.strip()
        opcode = opcode.strip()
        
        if inquotes:
            fatal("Mismatched quotes")
        
        if len( symbol.split()) > 1:
            fatal("Whitespace not allowed in symbol name")
        
        if (symbol and (opcode[0:3].upper() !="EQU") and (ifstate < 2)):
            if p==1:
                set_symbol(symbol, origin, is_label=True)
            elif get_symbol(symbol) != origin:
                fatal("Symbol "+symbol+": expected "+str(get_symbol(symbol))+" but calculated "+str(origin)+", has this symbol been used twice?")
        
        if (opcode):
            bytes = assemble_instruction(p,opcode)
            if p>1 and listingfile != None:
              lstout="%04X %-13s\t%s"%(origin,lstcode,wholefile[consider_linenumber].rstrip())
              lstcode=""
              writelisting(lstout)
            origin = (origin + bytes) % 65536
        else:
          if p>1 and listingfile != None:
            lstout="    %-13s\t%s"%("",wholefile[consider_linenumber].rstrip())
            lstcode=""
            writelisting(lstout)

        if global_currentfile.startswith(this_currentfilename+":") and int(global_currentfile.rsplit(':',1)[1]) != consider_linenumber:
            consider_linenumber = int(global_currentfile.rsplit(':')[1])

        consider_linenumber += 1

###########################################################################

try:
    option_args, file_args = getopt.getopt(sys.argv[1:], 'ho:s:eD:I:', ['version','help','nozip','obj=','case','nobodmas','exportfile=','importfile=','mapfile=','lstfile='])
    file_args = [os.path.normpath(x) for x in file_args]
except getopt.GetoptError:
    printusage()
    sys.exit(2)

inputfile = ''
outputfile = ''
objectfile = ''

PYTHONERRORS = False
ZIP = True
CASE = False
NOBODMAS = False

lstcode=""
listsymbols=[]
predefsymbols=[]
includefiles=[]
importfiles=[]
exportfile = None
mapfile = None
listingfile = None

def writelisting(line):
  if listingfile != None:
    listingfile.write(line+"\n")

for option,value in option_args:
    if option in ['--version']:
        printusage()
        print("")
        printlicense()
        sys.exit(0)
    if option in ['--help','-h']:
        printusage()
        sys.exit(0)
    
    if option in ['-o']:
        outputfile=value

    if option in ['--obj']:
        objectfile=value
    
    if option in ['-s']:
        listsymbols.append(value)
    
    if option in ['-e']:
        PYTHONERRORS = True # let python do its own error handling
    
    if option in ['--nozip']:
        ZIP = False # save the disk image without compression

    if option in ['--nobodmas']:
        NOBODMAS = True # use no operator precedence
    
    if option in ['--case']:
        CASE = True

    if option in ['--exportfile']:
        if exportfile == None:
            exportfile = value
        else:
            print("Export file specified twice")
            printusage()
            sys.exit(2)

    if option in  ['--importfile']:
        importfiles.append(value)

    if option in ['--mapfile']:
        if mapfile == None:
            mapfile = value
        else:
            print("Map file specified twice")
            printusage()
            sys.exit(2)

    if option in ['--lstfile']:
        if listingfile == None:
            listingfile=open(value,"wt")
        else:
            print("List file specified twice")
            printusage()
            sys.exit(2)

    if option in ['-D']:
        predefsymbols.append(value)

    if option in ['-I']:
        includefiles.append(value)

if len(file_args) == 0 and len(includefiles) == 0:
    print("No input file specified")
    printusage()
    sys.exit(2)

if (objectfile != '') and (len(file_args) != 1):
    print("Object file output supports only a single source file")
    printusage()
    sys.exit(2)

if (outputfile == '') and (objectfile == ''):
    outputfile = os.path.splitext(file_args[0])[0] + ".dsk";

image = new_disk_image()

for pathname in includefiles:
    save_file_to_image(image,pathname)
    
for inputfile in file_args:

    if (inputfile == outputfile) or (inputfile == objectfile):
        print("Output file and input file are the same!")
        printusage()
        sys.exit(2)

    symboltable = {}
    symbolcase = {}
    symusetable = {}
    labeltable = {}
    memory = []
    forstack=[]
    ifstack = []
    ifstate = 0

    for value in predefsymbols:
        sym=value.split('=',1)
        if len(sym)==1:
            sym.append("1")
        if not CASE:
            sym[0]=sym[0].upper()
        if PYTHONERRORS:
            val = int(sym[1])
        else:
            try:
                val = int(sym[1])
            except:
                print("Error: Invalid value for symbol predefined on command line, "+value)
                sys.exit(1)
        set_symbol(sym[0], int(sym[1]))

    for picklefilename in importfiles:
        picklefile = open(picklefilename)
        u = pickle.Unpickler(picklefile)
        ImportSymbols = u.load()
        for sym,val in list(ImportSymbols.items()):
            if not CASE:
                symboltable[sym.upper()]=val
            else:
                symboltable[sym]=val
        picklefile.close()

    firstpage=32
    firstpageoffset=16384
    lastpage=-1
    lastpageoffset=0

    # always 32 memory pages, each a 16k array allocate-on-write
    for initmemorypage in range(32):
        memory.append('')

    for p in 1,2:
        print("pass ",p,"...")
    
        global_path=''
        include_stack=[]
    
        origin = 32768
        dumppage = 1
        dumporigin = 0
        dumpspace_pending = 0
        dumpused = False
        autoexecpage = 0
        autoexecorigin = 0
    
        assembler_pass(p, inputfile)

    check_lastpage()

    if len(ifstack) > 0:
        print("Error: Mismatched IF and ENDIF statements, too many IF")
        for item in ifstack:
            print(item[0])
        sys.exit(1)
    if len(forstack) > 0:
        print("Error: Mismatched EQU FOR and NEXT statements, too many EQU FOR")
        for item in forstackstack:
            print(item[1])
        sys.exit(1)

    printsymbols = {}
    for symreg in listsymbols:
        # add to printsymbols any pair from symboltable whose key matches symreg
        for sym in symboltable:
            if re.search(symreg, sym, re.IGNORECASE):
                printsymbols[symbolcase[sym]] = symboltable[sym]

    if printsymbols != {}:
        print(printsymbols)

    if exportfile:
        f = open(exportfile,'w')
        p = pickle.Pickler(f)
        p.dump(symboltable)

    if mapfile:
        addrmap = {}
        for sym,count in sorted(list(symusetable.items()), key=lambda x: x[1]):
            if sym in labeltable:
                addrmap[labeltable[sym]] = symbolcase[sym]

        with open(mapfile,'w') as f:
            for addr,sym in sorted(addrmap.items()):
                f.write("%04X=%s\n" % (addr,sym))

    save_memory(memory, image=image, filename=os.path.splitext(os.path.basename(inputfile))[0])
    if objectfile != "":
        save_memory(memory, filename=objectfile)


if outputfile != '':
    save_disk_image(image, outputfile)

print("Finished")
