DEFAULT_TERMINAL_X_SIZE = 20
DEFAULT_TERMINAL_Y_SIZE = 17


def default_tile_size(target):
        if target in ["atari_lynx", "terminal8x6"]:
            xtile = 8
            ytile = 6
        elif target in ["apple2", "apple2enh", "terminal7x8"]:
            xtile = 7
            ytile = 8
        elif target in ["oric1", "atmos", "oric", "comx_ntsc", "terminal6x8"]:
            xtile = 6
            ytile = 8
        elif target in ['comx', 'pecom', 'micro', "terminal6x9"]:
            xtile = 6
            ytile = 9
        else:
            xtile = 8
            ytile = 8

        return xtile, ytile


def default_screen_size(target):
        if target in ("terminal", "terminal8x8", "terminal8x6", "atari_lynx"):
            xsize = 20
            ysize = 17
        elif target == "pc6001":
            xsize = 24
            ysize = 16
        elif target in ['ti82', 'ti83', 'ti8x']:
            xsize = 12
            ysize = 8
        elif target in ['ti85', 'ti86','z88']:
            xsize = 16
            ysize = 8
        elif target in ("oric1", "oric", "atmos"):
            xsize = 38
            ysize = 28
        elif target == "supervision":
            xsize = 20
            ysize = 20
        elif target == "gamate":
            xsize = 20
            ysize = 19
        elif target in ["apple2", "apple2enh", "atari", "atari5200", "terminal7x8"]:
            xsize = 20
            ysize = 24
        elif target in ['comx', 'pecom', 'micro', 'comc_ntsc', 'laser500', "terminal6x9", "terminal6x8", "aquarius"]:
            xsize = 40
            ysize = 24
        elif target in ["c64", "c16", "plus4", "c128", "vg5k", "cbm510"]:
            xsize = 40
            ysize = 25
        elif target in ["sam"]:
            xsize = 32
            ysize = 22
        elif target in ["camputers"]:
            xsize = 32
            ysize = 32
        elif target == "vic20":
            xsize = 22
            ysize = 23
        elif target in ["mo5","to7"]:
            xsize = 39
            ysize = 25
        elif target in ["coco","dragon", "mc1000"]:
            xsize = 16
            ysize = 24
        elif target in ["sms", "spectrum", "creativision", "msx", "mtx", \
                        "zx81", "zx80", "mtx512", "mtx500", "svi", "sg1000", \
                        "sc3000", "einstein", "m5", "ace", "coleco", "vz", "vz200"]:
            xsize = 32
            ysize = 24
        elif target in ["nes"]:
            xsize = 32
            ysize = 28
        elif target in ["pet", "cbm610"]:
            xsize = 80
            ysize = 25
        elif target in ["ncurses"]:
            xsize = 80
            ysize = 24
        elif target in ["pce"]:
            xsize = 60
            ysize = 25
        elif target in ["ti99"]:
            xsize = 32
            ysize = 23
        elif target == "cpc":
            xsize = 20
            ysize = 25
        elif target in ["gb", "gg", "gamegear"]:
            xsize = 20
            ysize = 18
        elif target == "atari7800":
            xsize = 20
            ysize = 28
        else:
            xsize = None
            ysize = None
        return xsize, ysize


def default_terminal_size_string(target):
    xsize, ysize = default_screen_size(target)
    
    if xsize == None:
        xsize = DEFAULT_TERMINAL_X_SIZE
        ysize = DEFAULT_TERMINAL_Y_SIZE
    
    xsize = str(xsize)
    ysize = str(ysize)
    return xsize, ysize


def get_terminal_target(target):
    xtile, ytile = default_tile_size(target)
    
    return "terminal"+str(xtile)+'x'+str(ytile)


def insert_default_sizes(option_config, params):

    if len(params)==4 and params[2].startswith('terminal'):
        target = params[3]

        terminal_target = get_terminal_target(target)

        xsize, ysize = default_terminal_size_string(target)

        return [params[0], params[1], terminal_target, xsize, ysize]

    if len(params)==3 and params[2].startswith('terminal'):
        terminal_target = params[2]
        xsize, ysize = default_terminal_size_string(terminal_target)

        return [params[0], params[1], terminal_target, xsize, ysize]
    return params