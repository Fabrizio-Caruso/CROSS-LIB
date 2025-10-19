from __future__ import print_function


import os

from default_values import default_tile_size, default_screen_size, default_terminal_size_string, get_terminal_target
from file_functions import list_of_projects
from project_functions import project_category
from print_functions import bcolors, printc

DEBUG_TARGET  = "ncurses_debug"
NATIVE_TARGET = "ncurses"


def info(option_config, params):
    
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
    else:
        target = params[1]
        xsize, ysize = default_screen_size(target)
        xtile, ytile = default_tile_size(target)
        
        if xsize==None:
            print("Information not available")
            return
        else:
            xsize = str(xsize)
            ysize = str(ysize)
            
        xtile = str(xtile)
        ytile = str(ytile)
        
        if target in PARALLEL_TARGETS:
            parallel = "supported"
        else:
            parallel  = "not supported"
        
        print("target      :         ", end=""); printc(option_config, bcolors.BOLD,target); print("")
        print("")
        print("x size      :         " + xsize)
        print("y size      :         " + ysize)
        print("")
        print("x tile      :         " + xtile)
        print("y tile      :         " + ytile)
        print("")
        print("parallelism :         " + parallel)
        

#TODO: cc65 targets
#TODO: cmoc targets
#TODO: lcc1802 targets
#TODO: mc10 (6303)
#TODO: c128 (complicated because of c1541)
#TODO: cpc (complicated because of nocart)
PARALLEL_TARGETS = \
    [
    'stdio',

    'apple2',
    'apple2enh',
    'atari',
    'atari_lynx',
    'atari5200',
    'atari7800',
    'atmos',
    'c16',
    'c64',
    'cbm610',
    'cbm510',
    'creativision',
    'gamate',
    'oric',
    'oric1',
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



    