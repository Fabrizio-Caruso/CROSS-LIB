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
    'c16',
    'c64',
    'cbm610',
    'cbm510',
    'creativision',
    'gamate',
    'oric',
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
    'gb',
    'gal',
    'gl6000sl',
    'gamegear',
    'hector',
    'hectorhr',
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
    'pv1000',
    'pv1000_no_gfx',
    'px4',
    'px8',
    'samcoupe',
    'sc3000',
    'sc3000_rom',
    'sg1000',
    'sms',
    'spectrum',
    'svi',
    't200',
    'trs80',
    'vg5k',
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

