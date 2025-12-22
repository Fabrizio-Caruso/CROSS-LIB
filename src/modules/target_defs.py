from __future__ import print_function


import os
import sys

from default_values import default_tile_size, default_screen_size, default_terminal_size_string, get_terminal_target
from file_functions import list_of_projects
from project_functions import project_category
from print_functions import bcolors, printc

DEBUG_TARGET  = "ncurses_debug"
NATIVE_TARGET = "ncurses"


def binary_factor(target):
    platform = sys.platform
    if platform in ["cygwin", "msys"]:
        TARGETS_WITH_2_BINARIES = ['atari', 'aquarius', 'cpc', 'coco3', 'mo5', 'ace', 'mtx500', \
                                   'mtx512', 'laser500']
        TARGETS_WITH_3_BINARIES = ['bbc', 'bbcmaster', 'coco']
    else:
        TARGETS_WITH_2_BINARIES = ['atari', 'aquarius', 'cpc', 'coco3', 'mo5', 'ace', 'mtx500', \
                                   'mtx512', 'laser500', 'coco']
        TARGETS_WITH_3_BINARIES = ['bbc', 'bbcmaster']
    if target in TARGETS_WITH_2_BINARIES:
        return 2
    elif target in TARGETS_WITH_3_BINARIES:
        return 3
    else:
        return 1


def retrieve_targets():
    with open("../docs/STATUS.md", "r") as f:
        raw_lines = [line for line in f if line.lstrip().startswith("*")]
        lines = []
        for raw_line in raw_lines:
            line = raw_line.replace('\n','').split('|')
            lines.append(line)
    return lines


def display_targets(lines, compiler="", cpu="", working=""):
    
    if compiler!='':
        lines = [line for line in lines if line[4].lower().strip()==compiler]

    if cpu!='':
        lines = [line for line in lines if cpu.lower() in line[3].lower().strip()]
    
    if working!='':
        lines = [line for line in lines if working.lower() in line[2].lower().strip()]

    
    print("target name         | long target name                            | working  |   CPU family  | dev-kit | real-time input |    graphics    |  sound  | notes      ")
    for line in lines:
        print(line[0], line[1], line[2], line[3], line[4], line[5], line[6], line[7], line[8])

    print("Targets found: " + str(len(lines)))


def get_target_map(lines):
    target_map = {}
    for line in lines:
        target_map[line[0].replace('*','').strip()]=[line[1].strip(), line[2].strip(), line[3].strip(), line[4].strip(), line[5].strip(), line[6].strip(), line[7].strip(), line[8].strip()]
    return target_map


def info(option_config, params):
    
    lines = retrieve_targets()
    target_map = get_target_map(lines)
    
    # target= params[1].lstrip()
    # print(target_map[target][0].lstrip())

    # print(str(target_map))
    if len(params)==1 or params[1]=="targets":
        display_targets(lines)
        return
    
    if params[1] in ("cc65", "z88dk", "cmoc", "lcc1802", "cc6303", "ack", "tms9900-gcc", "vbcc", "xtc68"):
        display_targets(lines, params[1])
        return
    
    if params[1] in ("6502", "z80", "6809", "1802", "6803", "8080", "8085", "8086", "8088", "386", "tms9900", "68000", "68020", "gbz80"):
        to_check = params[1].replace("z80", "Zilog 80").replace("8088","8086").replace("tms9900", "TMS 9900").replace("6502","MOS 6502")
        display_targets(lines, "", cpu=to_check)
        return

    if params[1] in("yes","ok","no","ko","?"):
        to_check = params[1].replace("ok", "yes").replace("ko","no")
        display_targets(lines, "", cpu="", working=to_check)
        return
    

    if params[1] in list_of_projects("all"):
        project = params[1]
        category = project_category(project)

        project_dir = category+"s/"+project
        split = os.path.exists(project_dir+"/split_files")
        shapes = os.path.exists(project_dir+"/shapes")
        if shapes:
            shape_dirs = str(os.listdir(project_dir+"/shapes"))[:-1][1:]
        else:
            shape_dirs = None
        
        game_config = os.path.exists(project_dir+"/config/game_config.mk")
        
        tiles = os.listdir(project_dir+"/tiles/8x8")
        
        override = os.path.exists(project_dir+"/makefiles") and os.path.exists(project_dir+"/makefiles/Makefile.override")
        
        memory = os.path.exists(project_dir+"/memory")
        
        print("project            :      ", end=""); printc(option_config, bcolors.BOLD, project); print("")

        print("")
        print("category           :      ", end=""); printc(option_config, bcolors.OKBLUE, category); print("")

        print("")
        print("split project      :      " + str(split))
        print("")
        print("no. of tiles       :      " + str(len(tiles)))
        print("shapes             :      " + str(shape_dirs))
        print("")
        print("code configuration :      " + str(game_config))
        print("override           :      " + str(override))
        print("memory optimizer   :      " + str(memory))
    elif params[1] in target_map.keys():
        target = params[1]
        xsize, ysize = default_screen_size(target)
        xtile, ytile = default_tile_size(target)
        
        if xsize==None:
            xsize = "not available"
            ysize = "not available"
        else:
            xsize = str(xsize)
            ysize = str(ysize)
            
        xtile = str(xtile)
        ytile = str(ytile)
        
        if target in PARALLEL_TARGETS:
            parallel = "supported"
        else:
            parallel  = "not supported"
        
        print("target          :         ", end=""); printc(option_config, bcolors.BOLD,target); print("")
        print("name            :         " + target_map[target][0])
        print("working         :         " + target_map[target][1])
        print("cpu             :         " + target_map[target][2])
        print("compiler        :         " + target_map[target][3])
        print("real-time       :         " + target_map[target][4])
        print("graphics        :         " + target_map[target][5])
        print("sound           :         " + target_map[target][6])

        print("")
        print("x size          :         " + xsize)
        print("y size          :         " + ysize)
        print("")
        print("x tile          :         " + xtile)
        print("y tile          :         " + ytile)
        print("")
        print("parallelism     :         " + parallel)
        print("")
        print("no. of binaries :         " + str(binary_factor(target)))
    else:
        print("No information found")

#TODO: cc65 targets
#TODO: cmoc targets
#TODO: lcc1802 targets
#TODO: mc10 (6303)
#TODO: c128 (complicated because of c1541)
#TODO: cpc (complicated because of nocart)
PARALLEL_TARGETS = \
    [
    'stdio',

    'atari',
    'atari_lynx',
    'atari5200',
    'atari7800',
    'atmos',
    'c16',
    'c64',
    'c65',
    'cbm610',
    'cbm510',
    'creativision',
    'cx16',
    'gamate',
    'oric',
    'oric1',
    'mega65',
    'nes',
    'pet',
    'pce',
    'supervision',
    'vic20',

    'ncurses',
    'terminal',
    'terminal8x8',
    'terminal8x6',
    'terminal7x8',
    'terminal6x9',
    'terminal6x8',

    'ace',
    'agon',
    'aquarius',
    'aussie',
    'bit90',
    'camputers_lynx',
    'cpm',
    'coleco',
    'eg2k',
    'einstein',
    'fp1100',
    'g850',
    
    'g815',
    'g800',
    'e200',
    'g815',
    
    'gb',
    'gal',
    'gl6000sl',
    'gamegear',
    'hector',
    'hector1',
    'hectorhr',
    'interact',
    'kaypro',
    'kaypro83',
    'kc85',
    'lambda',
    'laser500',
    'm5',
    'm5_rom',
    'm5_keyboard',
    'm5_keyboard_rom',
    'm100',
    'map1010',
    'map1010_mono',
    'map1010_no_gfx',
    'mc1000',
    'mc1000_mono',
    'microbee',
    'msx',
    'mtx500',
    'mtx512',
    'mz',
    'mz2500',
    'newbrain',
    'nc100',
    'nc200',
    'pc8201',
    'pc88',
    'phc25',
    'phc25_mono',
    'phc25_no_gfx',
    'pv1000',
    'pv1000_no_gfx',
    'px4',
    'px8',
    'samcoupe',
    'sc3000',
    'sc3000_rom',
    'sg1000',
    'smc777',
    'sms',
    'spectrum',
    'svi',
    't200',
    'trs80',
    'vg5k',
    'vz',
    'vz200',
    'x1',
    'zx81',
    'zx81_wrx',
    'zx81_8x6',
    'zx81_wrx64',
    'zx81_8x6_wrx64',
    'zx81_wrx128',
    'zx81_8x6_wrx128',
    'z9001',
    'kc',
    'z1013',
    'zx80',
    'zxn'
    ]


DEFAULT_TARGET_MAP = {\
                     "vic20":"vic20_exp_16k", \
                     "c16":"c16_16k", \
                     "plus4":"c16_32k", \
                     "pet":"pet_16k", \
                     "msx": "msx_16k" \
                     }

VICE_TARGETS = ["c64",
                "vic20", 
                "vic20_unexpanded", 
                "vic20_exp_3k", 
                "vic20_exp_8k", 
                "vic20_exp_16k",
                "c16", 
                "plus4", 
                "c16_16k", 
                "c16_32k",
                "pet", 
                "pet_8k", 
                "pet_16k"
               ]

MAME_TARGETS = ["msx", "msx_16k"]



    