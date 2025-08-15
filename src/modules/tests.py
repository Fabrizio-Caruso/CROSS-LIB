import sys
import os


from init import *


CROSS_COMPILER_COMMAND = \
    {
    'cc65'        : 'cl65',
    'z88dk'       : 'zcc',
    'cmoc'        : 'cmoc',
    'cc6303'      : 'cc68 -h',
    'lcc1802'     : 'lcc',
    'vbcc'        : 'vc -h',
    'ack'         : 'ack -h',
    'tms9900-gcc' : 'tms9900-gcc -h'
    }

CROSS_COMPILER_COMMAND_EXPECTED = \
    {
    'cc65'   : 0,
    'z88dk'  : 0,
    'cmoc'   : 256,
    'cc6303' : 256,
    'lcc1802': 0,
    'vbcc'   : 256,
    'ack'    : 0,
    'tms9900-gcc': 256
    }


NATIVE_COMPILER_COMMAND = \
    {
    'gcc'         : 'gcc',
    'g++'         : 'g++',
    }

NATIVE_COMPILER_COMMAND_EXPECTED = \
    {
    'gcc'    : 256,
    'g++'    : 256,
    }


TOOL_COMMAND = \
    {
    'f2k5'     : '../tools/cmoc/mo5/f2k5.' + NATIVE_EXTENSION + '',
    'sapfs'    : '../tools/cmoc/mo5/sapfs.' + NATIVE_EXTENSION + '',
    'file2dsk' : '../tools/cmoc/coco/file2dsk/file2dsk.' + NATIVE_EXTENSION + ' -h',
    'makewzd'  : '../tools/z88dk/oz/makewzd.' + NATIVE_EXTENSION + ' -h',
    'fixcart'  : '../tools/cc65/gamate/gamate-fixcart.' + NATIVE_EXTENSION + '',
    'bbcim'    : '../tools/bbc/bbcim.' + NATIVE_EXTENSION + ' -h',
    'nocart'   : '../tools/z88dk/cpc/nocart/nocart.' + NATIVE_EXTENSION + ' -h',
    'm20'      : '../tools/olivetti_m20/m20.' + NATIVE_EXTENSION + ' -h',
    'elf2ea5'  : '../tools/ti99/elf2ea5.' + NATIVE_EXTENSION + ' -h',
    'ea5split' : '../tools/ti99/ea5split.' + NATIVE_EXTENSION + ' -h',
    }

TOOL_COMMAND_EXPECTED = \
    {
    'f2k5'     : 0,
    'sapfs'    : 256,
    'file2dsk' : 256,
    'makewzd'  : 256,
    'fixcart'  : 65280,
    'bbcim'    : 256,
    'nocart'   : 256,
    'm20'      : 256,
    'elf2ea5'  : 256,
    'ea5split' : 256,
    }


EMULATOR_COMMAND = \
    {
    'c1541'    : 'c1541 -h',
    'x64'      : 'x64 -h',
    'xvic'     : 'xvic -h',
    'xplus4'   : 'xplus4 -h',
    'xpet'     : 'xpet -h',
    'mame'     : 'mame -h',
    }

EMULATOR_COMMAND_EXPECTED = \
    {
    'java'     : 0,
    'perl'     : 0,
    'c1541'    : 0,
    'x64'      : 0,
    'xvic'     : 0,
    'xplus4'   : 0,
    'xpet'     : 0,
    'mame'     : 0,
    }


BUILDABLE_TOOLS = \
{
    'f2k5',
    'sapfs',
    'file2dsk',
    'makewzd',
    'fixcart',
    'bbcim',
    'nocart',
    'm20',
    'elf2ea5',
    'ea5split',
}


INTERPRETER_COMMAND = \
    {
    'java'     : 'java -h',
    'perl'     : 'perl -h',
    }

INTERPRETER_COMMAND_EXPECTED = \
    {
    'java'     : 0,
    'perl'     : 0,
    }

LIBRARY_COMMAND = \
    {
    'ncurses'  : "ls /usr/include/ncurses.h",
    }

LIBRARY_COMMAND_EXPECTED = \
    {
    'ncurses'  : 0,
    }

ROM_COMMAND = \
    {
    'msx/msx.zip'      : 'ls ../roms/mame/msx.zip',
    'msx/canon20.zip'  : 'ls ../roms/mame/canonv20.zip',

    'c16/kernal'     : 'ls ../roms/vice/plus4/kernal',
    'c16/basic'     : 'ls ../roms/vice/plus4/basic',
    'c16/3plus1hi'     : 'ls ../roms/vice/plus4/3plus1hi',
    'c16/3plus1lo'     : 'ls ../roms/vice/plus4/3plus1lo',

    'c64/basic'     : 'ls ../roms/vice/c64/basic',
    'c64/chargen'     : 'ls ../roms/vice/c64/chargen',
    'c64/kernal'     : 'ls ../roms/vice/c64/kernal',

    'pet/basic1'     : 'ls ../roms/vice/pet/basic1',
    'pet/kernal1'     : 'ls ../roms/vice/pet/kernal1',

    'pet/basic2'     : 'ls ../roms/vice/pet/basic2',
    'pet/kernal2'     : 'ls ../roms/vice/pet/kernal2',

    'pet/basic4'     : 'ls ../roms/vice/pet/basic4',
    'pet/kernal4'     : 'ls ../roms/vice/pet/kernal4',

    'pet/chargen'     : 'ls ../roms/vice/pet/chargen',
    'pet/characters.901640-01.bin'     : 'ls ../roms/vice/pet/characters.901640-01.bin',

    'pet/edit1g'     :'ls ../roms/vice/pet/edit1g',
    'pet/edit2b'     :'ls ../roms/vice/pet/edit2b',
    'pet/edit2g'     :'ls ../roms/vice/pet/edit2g',

    'pet/edit4b40'     :'ls ../roms/vice/pet/edit4b40',
    'pet/edit4b80'     :'ls ../roms/vice/pet/edit4b80',
    'pet/edit4g40'     :'ls ../roms/vice/pet/edit4g40',

    'vic20/basic'     : 'ls ../roms/vice/vic20/basic',
    'vic20/chargen'     : 'ls ../roms/vice/vic20/chargen',
    'vic20/kernal'     : 'ls ../roms/vice/vic20/kernal',
    }


ROM_COMMAND_EXPECTED = \
    {
    'msx/msx.zip'        : 0,
    'msx/canon20.zip'    : 0,

    'c16/kernal'     : 0,
    'c16/basic'      : 0,
    'c16/3plus1hi'   : 0,
    'c16/3plus1lo'   : 0,

    'c64/kernal'     : 0,
    'c64/chargen'     :0,
    'c64/basic'     : 0,

    'pet/kernal1'     : 0,
    'pet/basic1'     : 0,

    'pet/kernal2'     : 0,
    'pet/basic2'     : 0,

    'pet/kernal4'     : 0,
    'pet/basic4'     : 0,

    'pet/chargen'     :0,
    'pet/characters.901640-01.bin'     :0,
    'pet/chargen'     :0,

    'pet/edit1g'     :0,
    'pet/edit2b'     :0,
    'pet/edit2g'     :0,

    'pet/edit4b40'     :0,
    'pet/edit4b80'     :0,
    'pet/edit4g40'     :0,



    'vic20/kernal'     : 0,
    'vic20/chargen'     :0,
    'vic20/basic'     : 0,
    }

MAKE_COMMAND = \
    {
    'make'     : 'make -h',
    'gmake'     : 'gmake -h',
    }

MAKE_COMMAND_EXPECTED = \
    {
    'make'  : 0,
    'gmake' : 0,
    }

def check_programs(title, command_list, expected_list, verbose=True):
    if verbose:
        print("----------------------------------------\n")
        print(title)
        print("----------------------------------------\n")

    total_result = {}

    # max_len = 0
    # for compiler in command_list.keys():
        # if len(compiler)>max_len:
            # max_len = len(compiler)
    max_len = 28

    for compiler in command_list.keys():
        result = os.system(command_list[compiler] + " > /dev/null 2>&1")
        spaces = " " * (max_len+1-len(compiler))
        if result==expected_list[compiler]:
            if verbose:
                print("[" + compiler + "] found")
            # res = "found\n"
            # res_color = bcolors.OKGREEN
            # res_color2 = bcolors.OKGREEN
            total_result[compiler]=True
            # print(res)
        else:

            total_result[compiler]=False
            if compiler in BUILDABLE_TOOLS:
                res = "NOT built\n"
                # res_color = bcolors.OKBLUE
                # res_color2 = bcolors.OKCYAN
            else:
                res = "NOT found\n"
                # res_color = bcolors.WARNING
                # res_color2 = bcolors.WARNING
            if verbose:
                print("[" + compiler + "] " + res)
            # if verbose:
                # printc(option_config, res_color,"[" + compiler + "]")
        # if verbose:
            # printc(option_config, res_color2, spaces + res)
        # print(res)
    if verbose:
        print("----------------------------------------\n\n")
    # print(total_result)
    return total_result



def test_cross_compilers():
    return check_programs("CROSS_COMPILERS", CROSS_COMPILER_COMMAND, \
                          CROSS_COMPILER_COMMAND_EXPECTED)

def test_native_compilers():
    return check_programs("NATIVE_COMPILERS", NATIVE_COMPILER_COMMAND, \
                          NATIVE_COMPILER_COMMAND_EXPECTED)

def test_make(verbose=True):
    return check_programs("MAKE", MAKE_COMMAND,MAKE_COMMAND_EXPECTED, verbose)

def test_tools(verbose=True):
    return check_programs("TOOLS", TOOL_COMMAND,TOOL_COMMAND_EXPECTED, verbose)

def test_emulators():
    return check_programs("EMULATORS", EMULATOR_COMMAND,EMULATOR_COMMAND_EXPECTED)

def test_interpreters():
    return check_programs("INTERPRETERS", INTERPRETER_COMMAND,INTERPRETER_COMMAND_EXPECTED)

def test_libraries():
    return check_programs("LIBRARIES", LIBRARY_COMMAND, LIBRARY_COMMAND_EXPECTED)

def test_roms():
    return check_programs("ROMS", ROM_COMMAND, ROM_COMMAND_EXPECTED)


def test_compilers():
    test_native_compilers()
    test_cross_compilers()


# TODO: Handle aliases such as dragon->coco

# TODO: Handle Windows vs Linux and targets with 3 binaries
if platform in ["cygwin", "msys"]:
    TARGETS_WITH_2_BINARIES = ['aquarius', 'cpc', 'coco3', 'to5', 'ace', 'mtx500', \
                               'mtx512', 'laser500']
    TARGETS_WITH_3_BINARIES = ['coco','abc80']
else:
    TARGETS_WITH_2_BINARIES = ['aquarius', 'cpc', 'coco3', 'to5', 'ace', 'mtx500', \
                               'mtx512', 'laser500','coco']
    TARGETS_WITH_3_BINARIES = ['abc80']

