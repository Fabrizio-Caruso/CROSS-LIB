DEFAULT_CLEANUP_COMMANDS = \
    [ \
        "xl clean tools", \
        "xl clean", \
    ]


NO_CLEANUP = []


# ---------------------------------------------------------

CLEAN_TEST = \
    [ \
        "xl delete _cloned_test_project -y", \
        "xl delete _foo_test -y", \
        "xl clean", \
    ]


# ---------------------------------------------------------

TOOLS_TEST = \
    [ \
        "xl clean tools", \
        "xl tools", \
    ]


CLEANUP_TOOLS_TEST = \
    [ \
        "xl clean tools",
    ]

# ---------------------------------------------------------
CREATE_TEST = \
    [ \
        "xl create _test_project1 game", \
        "xl create _test_project2 demo", \
        "xl create _test_project3 text", \
        "xl create _test_project4 helloworld", \
        "xl build _test_project1 __target__ ", \
        "xl build _test_project2 __target__ ", \
        "xl build _test_project3 __target__ ", \
        "xl build _test_project4 __target__ ", \
    ]

CLEANUP_CREATE_TEST = \
    [ \
        "xl delete _test_project1 -y", \
        "xl delete _test_project2 -y", \
        "xl delete _test_project3 -y", \
        "xl delete _test_project4 -y", \
    ]

# ---------------------------------------------------------

COMPLEX_TEST = \
    [ \
        "xl delete _foo_test -y", \
        "xl clean tools", \
        "xl tools", \
        "xl clean tools", \
        "xl delete _test_project -y", \
        "xl clean", \
        "xl create _foo_test", \
        "xl slow _foo_test __target__ 42", \
        "xl clean" , \
        "xl tiles _foo_test", \
        "xl size _foo_test stdio_sized 12 12", \
        "xl reset", \
        "xl size _foo_test stdio_sized 32 24", \
        "xl reset", \
        "xl tiles _foo_test 8 8", \
        "xl tiles _foo_test 6 8", \
        "xl tiles _foo_test 6 9", \
        "xl tiles _foo_test 7 8", \
        "xl tiles _foo_test 8 8", \
        ]

CLEANUP_COMPLEX_TEST = \
    [ \
        "xl delete _foo_test -y"
    ]

# ---------------------------------------------------------

RENAME_TEST = \
    [ \
        "xl delete _test_project -y", \
        "xl delete _foo_test -y", \
        "xl create _test_project test", \
        "xl list", \
        "xl clean", \
        "xl build _test_project __target__", \
        "xl rename _test_project _foo_test", \
        "xl build _foo_test __target__", \
        "xl rename _foo_test _test_project", \
    ]


CLEANUP_RENAME_TEST = \
    [ \
        "xl delete _test_project -y", \
    ]

# ---------------------------------------------------------

MAKE_TEST = \
    [ \
        "xl create _foo_test", \
        "xl make _foo_test", \
        "xl reset _foo_test", \
        "xl rebuild _foo_test __target__", \
        "xl show _foo_test", \
        "xl extend _foo_test", \
    ]


# ---------------------------------------------------------

EXAMPLES_TEST = \
    [ \
        "xl examples __target__", \
    ]


# ---------------------------------------------------------

GAMES_TEST = \
    [ \
        "xl games __target__", \
    ]


# ---------------------------------------------------------

RUN_TEST = \
    [ \
        "xl create _test_project", \
        "xl build _test_project __target__", \
        "xl run _test_project __target__", \
        "xl clone _test_project _cloned_test_project", \
        "xl build _cloned_test_project __target__", \
        "xl run _cloned_test_project __target__", \
    ]

CLEANUP_RUN_TEST = \
    [ \
            "xl delete _test_project -y", \
            "xl delete _cloned_test_project -y", \
    ]


# ---------------------------------------------------------
# No clean-up necessary
DEV_TOOLS_TEST = \
    [ \
        "xl tile ./assets/examples/single_tiles/tile_shape0.txt", \
        "xl string abcdefghijklmnopqrstuvwxyz1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ", \
        "xl split games/shuriken/docs/16x12_multi_tile.txt", \
        "xl import ./assets/examples/tile_sets/asm/tile_8x6_shapeA.txt", \
        "xl rip ./assets/examples/tile_sets/asm/tile_8x6_shapeA.txt", \
        "xl rotate ./assets/examples/tile_sets/asm/tile_8x6_shapeA.txt", \
    ]


# ---------------------------------------------------------
GAMES_TERMINAL_TEST = \
    [ \
        "xl games terminal __target__", \
    ]