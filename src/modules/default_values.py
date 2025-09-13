DEFAULT_TERMINAL_X_SIZE = 20
DEFAULT_TERMINAL_Y_SIZE = 17

def insert_default_sizes(option_config, params):

    if len(params)==4 and params[2].startswith('terminal'):
        target = params[2]
        size_target = params[3]

        if size_target == "atari_lynx" or "8x6" in size_target:
            target = "terminal8x6"
        elif size_target in ["apple2", "apple2enh"]:
            target = "terminal7x8"
        elif size_target in ["oric1", "atmos", "oric", "comx_ntsc"]:
            target = "terminal6x8"
        elif size_target in ['comx', 'pecom', 'micro']:
            target = "terminal6x9"
        else:
            target = "terminal8x8"

        if size_target == "atari_lynx":
            xsize = '20'
            ysize = '17'
        elif size_target == "pc6001":
            xsize = '24'
            ysize = '16'
        elif size_target in ['ti82', 'ti83', 'ti8x']:
            xsize = '12'
            ysize = '8'
        elif size_target in ['ti85', 'ti86','z88']:
            xsize = '16'
            ysize = '8'
        elif size_target in ("oric1", "oric", "atmos"):
            xsize = '38'
            ysize = '28'
        elif size_target == "supervision":
            xsize = '20'
            ysize = '20'
        elif size_target == "gamate":
            xsize = '20'
            ysize = '19'
        elif size_target in ["apple2", "apple2enh"]:
            xsize = '20'
            ysize = '24'
        elif size_target in ['comx', 'pecom', 'micro']:
            xsize = '40'
            ysize = '24'
        elif size_target == "comx_ntsc":
            xsize = '40'
            ysize = '24'
        elif size_target in ["atari", "atari5200"]:
            xsize = '20'
            ysize = '24'
        elif size_target in ["c64", "c16", "c128"]:
            xsize = '40'
            ysize = '25'
        elif size_target == "vic20":
            xsize = '22'
            ysize = '23'
        elif size_target in ["mo5","to7"]:
            xsize = '39'
            ysize = '25'
        elif size_target in ["coco","dragon"]:
            xsize = '16'
            ysize = '24'
        elif size_target in ["sms", "spectrum", "creativision", "msx", "mtx", \
                             "mtx512", "mtx500", "svi", "sg1000", "sc3000", "einstein"]:
            xsize = '32'
            ysize = '24'
        elif size_target == "cpc":
            xsize = '20'
            ysize = '25'
        elif size_target in ["gb", "gg", "gamegear"]:
            xsize = '20'
            ysize = '18'
        elif size_target == "atari7800":
            xsize = '20'
            ysize = '28'
        else:
            if option_config.terminal_config.verbose:
                print("Default geometry for this target is not supported. Using default sizes.")
            xsize = str(DEFAULT_TERMINAL_X_SIZE)
            ysize = str(DEFAULT_TERMINAL_Y_SIZE)
        return [params[0], params[1], target, xsize, ysize]

    if len(params)==3 and params[2].startswith('terminal'):
        target = params[2]
        if target in ("terminal", "terminal8x8", "terminal8x6"):
            xsize = '20'
            ysize = '17'
        elif target == "terminal7x8":
            xsize = '20'
            ysize = '24'
        elif target in ("terminal6x9", "terminal6x8"):
            xsize = '40'
            ysize = '24'
        return [params[0], params[1], target, xsize, ysize]
    return params