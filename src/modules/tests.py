import sys
import os


from init import *
from import_from_source import printc
from file_functions import files_in_path
from run import run_command
from print_functions import printc, bcolors
from test_self_defs import *


from build_functions import *

from execute import execute_string

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

def check_programs(option_config, title, command_list, expected_list, silent=False):
    total_result = {}

    max_len = 0
    for compiler in command_list.keys():
        if len(compiler)>max_len:
            max_len = len(compiler)
    max_len = 28

    for compiler in command_list.keys():
        result = os.system(command_list[compiler] + " > /dev/null 2>&1")
        spaces = " " * (max_len+1-len(compiler))
        if result==expected_list[compiler]:
            # print("[" + compiler + "] found")
            res = "found\n"
            res_color = bcolors.OKGREEN
            res_color2 = bcolors.OKGREEN
            total_result[compiler]=True
        else:

            total_result[compiler]=False
            if compiler in BUILDABLE_TOOLS:
                res = "NOT built\n"
                res_color = bcolors.OKBLUE
                res_color2 = bcolors.OKCYAN
            else:
                res = "NOT found\n"
                res_color = bcolors.WARNING
                res_color2 = bcolors.WARNING
        if not silent:
            printc(option_config, res_color,"[" + compiler + "]")
            printc(option_config, res_color2, spaces + res)
    return total_result



def test_cross_compilers(option_config):
    return check_programs(option_config, "CROSS_COMPILERS", CROSS_COMPILER_COMMAND, \
                          CROSS_COMPILER_COMMAND_EXPECTED)

def test_native_compilers(option_config):
    return check_programs(option_config, "NATIVE_COMPILERS", NATIVE_COMPILER_COMMAND, \
                          NATIVE_COMPILER_COMMAND_EXPECTED)

def test_make(option_config, silent):
    return check_programs(option_config, "MAKE", MAKE_COMMAND,MAKE_COMMAND_EXPECTED, silent)

def test_tools(option_config, silent=False):
    return check_programs(option_config, "TOOLS", TOOL_COMMAND,TOOL_COMMAND_EXPECTED, silent)

def test_emulators(option_config):
    return check_programs(option_config, "EMULATORS", EMULATOR_COMMAND,EMULATOR_COMMAND_EXPECTED)

def test_interpreters(option_config):
    return check_programs(option_config, "INTERPRETERS", INTERPRETER_COMMAND,INTERPRETER_COMMAND_EXPECTED)

def test_libraries(option_config):
    return check_programs(option_config, "LIBRARIES", LIBRARY_COMMAND, LIBRARY_COMMAND_EXPECTED)

def test_roms(option_config):
    return check_programs(option_config, "ROMS", ROM_COMMAND, ROM_COMMAND_EXPECTED)


def test_compilers(option_config):
    test_native_compilers(option_config)
    test_cross_compilers(option_config)


def run_single_unit_test(option_config, test_file_name, path="unit_tests"):
    command_string = "python -t " + path + "/" + test_file_name
    printc(option_config, bcolors.OKBLUE,"----------------------------------------\n")
    printc(option_config, bcolors.OKCYAN,test_file_name + "\n")
    printc(option_config, bcolors.OKBLUE,"--------------------------------\n")
    verbosity = option_config.terminal_config.verbose
    option_config.terminal_config.verbose = 1
    run_command(option_config, command_string)
    option_config.terminal_config.verbose = verbosity


def _unit_tests(option_config, path="./"):
    printc(option_config, bcolors.OKBLUE,"----------------------------------------\n")
    printc(option_config, bcolors.OKCYAN, "UNIT-TESTS")
    printc(option_config, bcolors.OKBLUE,"\n----------------------------------------\n")

    assets_tests = files_in_path("assets/unit_tests")
    
    for test in assets_tests:
        run_single_unit_test(option_config, test, "assets/unit_tests")
    
    main_tests = files_in_path("./unit_tests")

    for test in main_tests:
        run_single_unit_test(option_config, test)




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


# LoggerSingleton.initLogger(__name__)
logger = LoggerSingleton.initLogger('xl', '../logs')

# console_logger = LoggerSingleton.initConsoleLogger('console')

# console_logger.info('Console logger started')

logger.info('Started')


def test_projects(option_config, projects, target="stdio"):

    compilation_threads = option_config.build_config.compilation_threads
    verbose = option_config.terminal_config.verbose
    clean_test(option_config)

    if projects in ["examples", "projects", "games"]:
        number_of_projects = len(dirs_in_path("./"+projects))
    elif projects == "all":
        number_of_projects = \
        len(dirs_in_path("./examples")) + \
        len(dirs_in_path("./games")) + \
        len(dirs_in_path("./projects"))
    else:
        number_of_projects = 1
    if verbose:
        print("number of projects: " + str(number_of_projects))
    #multiple_build
    build(option_config, ["build",projects,target,compilation_threads,""])
    built_files = built_files_in_path("../build")

    # TODO: Handle target=*_targets and all
    if target.endswith("_targets") or target=="all":
        if verbose:
            print("Test on the number of binaries for all these targets is not supported")

    if target in TARGETS_WITH_3_BINARIES:
        files_per_project = 3
    elif target in TARGETS_WITH_2_BINARIES:
        files_per_project = 2
    else:
        files_per_project = 1

    if verbose:
        print("Expected binaries per project: " + str(files_per_project))

    if len(built_files)<files_per_project*number_of_projects:
        printc(option_config, bcolors.FAIL, "[xl build]  KO\n")
        print("Built files: " +str(built_files))
        print("Expected number : " + str(number_of_projects))
        print("No. Built files : " + str(len(built_files)))

        # success=0
        return False
    printc(option_config, bcolors.OKGREEN, "[xl build]        OK\n")
    return True


def test_clean_tools(option_config):
    pass

def no_check(option_config, target):
    return 1

def execute_commands(option_config, command_strings, target, silent = True):
    for command_string in command_strings:
        execute_string(option_config, command_string.replace("__target__", target), silent)


def display_ok_ko(option_config, result):
    if result:
        printc(option_config, bcolors.OKGREEN,"OK"+"\n")
    else:
        printc(option_config, bcolors.FAIL,   "KO"+"\n")



def check_clean(option_config, target):    
    files = files_in_path("../build")
    return not(len(files)-1)

def check_tools(option_config, target):
    tools_result_map=test_tools(option_config, silent=True)
    number_of_tools = len(tools_result_map.keys())

    built_tools = 0
    for _, result in tools_result_map.items():
        if result:
            built_tools+=1
    return built_tools

def check_complex(option_config, target):
    number_files = len(files_in_path("../build"))-1
    # print(str(files_in_path("../build")))
    return number_files==2


def check_examples(option_config, target):
    number_files = len(files_in_path("../build"))-1
    number_of_examples = len(dirs_in_path("./examples"))

    return number_files == number_of_examples*binary_factor(target)


def check_games(option_config, target):
    number_files = len(files_in_path("../build"))-1
    number_of_games = len(dirs_in_path("./games"))

    return number_files == number_of_games*binary_factor(target)


def check_games_terminal(option_config, target):
    number_files = len(files_in_path("../build"))-1
    number_of_games = len(dirs_in_path("./games"))
    return number_files == number_of_games*binary_factor("terminal")


def check_rename(option_config, target):
    number_files = len(files_in_path("../build"))-1
    return number_files==2*binary_factor(target)


def check_create(option_config, target):
    number_files = len(files_in_path("../build"))-1
    return number_files==4*binary_factor(target)

def check_make(option_config, target):
    success = 1
    dirs_in_proj_after = len(dirs_in_path("./projects/_foo_test"))

    if dirs_in_proj_after != 4:
        success=0

    dirs_in_shapes_after = len(dirs_in_path("./projects/_foo_test/shapes"))

    if dirs_in_shapes_after != 5:
        success=0
    return success



def test_execute(option_config, target, test_name, commands, check = no_check, cleanup_commands = DEFAULT_CLEANUP_COMMANDS):
    print("-----------------------------------")
    print("TEST: " + bcolors.BOLD + test_name + bcolors.ENDC)
    print("-----------------------------------")

    # Execute list of commands
    execute_commands(option_config, commands, target, silent = False)
    
    # Check result and display OK/KO
    result = check(option_config, target)
    display_ok_ko(option_config, result)
    print("")

    # Clean-up commands
    execute_commands(option_config, cleanup_commands, target, silent = True)
    return result


def binary_factor(target):
    if target in TARGETS_WITH_2_BINARIES:
        return 2
    elif target in TARGETS_WITH_3_BINARIES:
        return 3
    else:
        return 1

# ---------------------------------------------------------
# DEFAULT SELF TESTS
# ---------------------------------------------------------

STANDARD_SELF_TESTS = \
    [ \
        ("test xl clean",            CLEAN_TEST,          check_clean), \
        ("test xl tools",            TOOLS_TEST,          check_tools,   CLEANUP_TOOLS_TEST), \
        ("test several xl commands", COMPLEX_TEST,        check_complex, CLEANUP_COMPLEX_TEST), \
        ("test xl dev tools",        DEV_TOOLS_TEST), \
        ("test xl create",           CREATE_TEST,         check_create,  CLEANUP_CREATE_TEST), \
        ("test xl make",             MAKE_TEST,           check_make), \
        ("test xl rename",           RENAME_TEST,         check_rename,  CLEANUP_RENAME_TEST) \
    ]


PARALLEL_BUILD_TESTS = \
    [ \
        ("test xl examples",         EXAMPLES_TEST,       check_examples), \
        ("test xl games",            GAMES_TEST,          check_games), \
        ("test xl games terminal",   GAMES_TERMINAL_TEST, check_games_terminal), \
    ]
    
INTERACTIVE_TESTS = \
    [ \
        ("test xl run",              RUN_TEST,       no_check,      CLEANUP_RUN_TEST), \
    ]
    

def test_self(option_config, target = "stdio"):
    option_config.terminal_config.test = 1

    printc(option_config, bcolors.OKCYAN,"----------------------------------------\n")
    printc(option_config, bcolors.OKCYAN, "XL SCRIPT TEST")
    printc(option_config, bcolors.OKCYAN,"\n----------------------------------------\n")
    success = 1
    printc(option_config, bcolors.BOLD,  "target: ")
    printc(option_config, bcolors.OKBLUE,target+"\n")
    printc(option_config, bcolors.OKCYAN,"----------------------------------------\n")

    self_tests = STANDARD_SELF_TESTS
    if option_config.terminal_config.interactive_test:
        self_tests += INTERACTIVE_TESTS
    if not option_config.terminal_config.fast_test:
        self_tests += PARALLEL_BUILD_TESTS

    for test in self_tests:
        execute_string(option_config, "xl clean", silent = True)
        success *= test_execute(option_config, target, *test)

    execute_string(option_config, "xl clean", silent = True)

    option_config.terminal_config.test = 0
    return success


def test_all(option_config, params):

    test_compilers(option_config)
    test_tools(option_config)
    test_libraries(option_config)
    test_interpreters(option_config)
    _unit_tests(option_config)
    return test_self(option_config, params)

expected_files = {
    "cc65": 8,
    "z88dk": 5,
    "cmoc": 3,
    "lcc1802": 1,
    "z88dk_alt": 38,
    "stdio_alt": 1
    }


def targets_test(option_config, params):

    verbose = option_config.terminal_config.verbose
    success = clean_test(option_config)
    compilation_threads = option_config.build_config.compilation_threads
    native_compiler = option_config.build_config.native_compiler
    GNU_MAKE = option_config.build_config.gnu_make
    if params[1].startswith("z88dk") or params[1]=="cc65":
        parallel = " -j " + compilation_threads
    else:
        parallel = ""

    game_dir = "chase"
    project_type = "game"
    if is_project_split(game_dir):
        if verbose:
            printc(option_config, bcolors.OKBLUE,"Create main.c from split source files\n")
        create_main(game_dir, project_type)
    if params[1] in ("cc65", "z88dk", "cmoc", "lcc1802"):
        make_command = GNU_MAKE + parallel + " test_" + params[1] + "_extra " + \
                       " GNU_MAKE=" + GNU_MAKE + " _NATIVE_CC="+ native_compiler + " " + \
                       all_compilers_opts(option_config, "","") + \
                       " -f makefiles.other/chase/tests/Makefile.tests"
        run_command(option_config, make_command)
    elif params[1]=="z88dk_alt":
        make_command = GNU_MAKE + parallel + " GNU_MAKE=" + GNU_MAKE + \
                       " z88dk_quick_test -f makefiles.other/chase/tests/Makefile.z88dk_quick_tests"
        run_command(option_config, make_command)
    else:
        printc(option_config, bcolors.FAIL, "Parameter not recognized\n")
        if is_project_split(game_dir):
            if verbose:
                printc(option_config, bcolors.OKBLUE,"Delete main.c (because of split source files)\n")
            delete_main(option_config, game_dir, project_type)
        sys.exit(-1)
    if is_project_split("chase"):
        if verbose:
            printc(option_config, bcolors.OKBLUE,"Delete main.c (because of split source files)\n")
        delete_main(option_config, game_dir, project_type)


    built_files = len(files_in_path("../build"))-1
    if verbose:
        print("Number of built files: " + str(built_files))

    if params[1] in expected_files.keys():
        printc(option_config, bcolors.OKCYAN, "Built files: " + str(built_files)+"\n")
        printc(option_config, bcolors.OKBLUE, "Expected files: " + str(expected_files[params[1]])+"\n")
        if built_files != expected_files[params[1]]:
            printc(option_config, bcolors.FAIL, "binaries KO\n")
            success=0

        else:
            printc(option_config, bcolors.OKGREEN, "binaries OK\n")

    # if clean_test()==0:
        # success=0
    return success


# Self-test xl and native build
def test(option_config, params):
    if (len(params)<=1) or ((len(params)==2) and (params[1]=="check")):
        if test_all(option_config, "stdio"):
            printc(option_config, bcolors.OKGREEN, "TEST OK\n")
        else:
            printc(option_config, bcolors.FAIL, "TEST KO\n")
        return
    if params[1]=="self":
        if len(params)<3:
            test_self(option_config)
        else:
            test_self(option_config, params[2])
    elif params[1]=="compilers":
        test_compilers(option_config)
    elif params[1]=="tools":
        test_tools(option_config)
    elif params[1]=="emulators":
        test_emulators(option_config)
    elif params[1]=="cross-compilers" or params[1]=="cross_compilers":
        test_cross_compilers(option_config)
    elif params[1]=="native_compilers" or params[1]=="native-compilers":
        test_native_compilers(option_config)
    elif params[1]=="libraries":
        test_libraries(option_config)
    elif params[1]=="interpreters":
        test_interpreters(option_config)
    elif params[1]=="roms":
        test_roms(option_config)
    elif params[1]=="make":
        test_make(option_config, silent=False)
    elif params[1] in ("unit-tests", "unit_tests", "unit-test", "unit_test", "u"):
        _unit_tests(option_config)
    elif params[1] in ("cc65", "z88dk", "cmoc", "lcc1802") or params[1].endswith('_alt'):
        if targets_test(option_config, params):
            printc(option_config, bcolors.OKGREEN, "TEST OK\n")
        else:
            printc(option_config, bcolors.FAIL, "TEST KO\n")
        return
    else:
        projects = []
        for (_, dirnames, _) in walk("./projects"):
            projects.extend(dirnames)
            break

        games = []
        for (_, dirnames, _) in walk("./games"):
            games.extend(dirnames)
            break

        examples = []
        for (_, dirnames, _) in walk("./examples"):
            examples.extend(dirnames)
            break

        if params[1] in projects + games + examples or params[1] in \
           ['examples','games','projects','all']:
            if len(params)>=3:
                par1 = params[1]
                par2 = params[2]
            else:
                par1 = params[1]
                par2 = "stdio"
            if test_projects(option_config, par1,par2):
                printc(option_config, bcolors.OKGREEN, "TEST OK\n")
            else:
                printc(option_config, bcolors.FAIL, "TEST KO\n")
        else:
            test_all(option_config, params[1])
        return


# Self-test xl and native build
def check(option_config, params, silent=False):
    if len(params)<=1:
        tools = test_tools(option_config, silent)
        native_compilers =test_native_compilers(option_config)
        cross_compilers = test_cross_compilers(option_config)
        libraries = test_libraries(option_config)
        make = test_make(option_config, silent=True)
        emulators = test_emulators(option_config)
        roms = test_roms(option_config)

        print("")

        printc(option_config, bcolors.BOLD, "SUMMARY\n")
        tools = True
        xl_run_cross_target = True
        native_target = True
        cross_target = True

        if not make['make'] and not make['gmake']:
            printc(option_config, bcolors.FAIL, "No make command detected!\n")

        if not native_compilers['gcc']:
            tools = False
            native_target = False

        if not native_compilers['g++']:
            xl_run_cross_target = True
            tools = False

        if not libraries['ncurses']:
            printc(option_config, bcolors.WARNING, \
                   "No ncurses library detected -> Native target impacted\n")
            native_target = False

        for _, present in cross_compilers.items():
            if not present:
                cross_target = False

        for _, present in emulators.items():
            if not present:
                xl_run_cross_target = False
                break

        for _, present in roms.items():
            if not present:
                xl_run_cross_target = False
                break

        if not tools:
            printc(option_config, bcolors.WARNING, "'xl tools' may be KO for some targets\n")

        if not native_target:
            printc(option_config, bcolors.WARNING, "'xl build <project>' may be KO (no native build)\n")

        if not cross_target:
            printc(option_config, bcolors.WARNING, "'xl build <project> <target>' may be KO for some targets\n")

        if not xl_run_cross_target:
            printc(option_config, bcolors.WARNING, "'xl run <project> <target>' may be KO for some targets\n")

        if tools and native_target and xl_run_cross_target and cross_target:
            printc(option_config, bcolors.OKCYAN, "No issue found\n")

    elif params[1]=="compilers":
        test_compilers(option_config)
    elif params[1]=="tools":
        test_tools(option_config)
    elif params[1]=="emulators":
        test_emulators(option_config)
    elif params[1]=="cross-compilers" or params[1]=="cross_compilers":
        test_cross_compilers(option_config)
    elif params[1]=="native_compilers" or params[1]=="native-compilers":
        test_native_compilers(option_config)
    elif params[1]=="libraries":
        test_libraries(option_config)
    elif params[1]=="interpreters":
        test_interpreters(option_config)
    elif params[1]=="make":
        test_make(option_config, silent=False)
    elif params[1]=="roms":
        test_roms(option_config)


def compilers(option_config):
    test(option_config, ["test","compilers"])
    
def unit_tests(option_config):
    # test(option_config, ["test","unit-tests"])
    _unit_tests(option_config)
    

def self(option_config, params):
    if len(params)<2:
        test_self(option_config, )
    else:
        test_self(option_config, params[1])