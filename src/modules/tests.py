import sys
import os


from init import *
from import_from_source import printc
from file_functions import files_in_path
from run import run_command
from print_functions import printc, bcolors
from test_self_defs import *
from clean_functions import clean_output


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
    'abcwrite' : '../tools/z88dk/abc80/abcdisk-2.7/abcwrite -h',
    'bbcim'    : '../tools/bbc/bbcim.' + NATIVE_EXTENSION + ' -h',
    'bin2abc'  : '../tools/z88dk/abc80/abcdisk-2.7/bin2abc -h',
    'cc1541'   : '../tools/generic/CC1541/cc1541 -h',
    'f2k5'     : '../tools/cmoc/mo5/f2k5.' + NATIVE_EXTENSION + '',
    'file2dsk' : '../tools/cmoc/coco/file2dsk/file2dsk.' + NATIVE_EXTENSION + ' -h',
    'fixcart'  : '../tools/cc65/gamate/gamate-fixcart.' + NATIVE_EXTENSION + '',
    'm20'      : '../tools/olivetti_m20/m20.' + NATIVE_EXTENSION + ' -h',
    'mkatr'    : '../tools/cc65/atari/mkatr-master/mkatr -h',
    'makewzd'  : '../tools/z88dk/oz/makewzd.' + NATIVE_EXTENSION + ' -h',
    'nocart'   : '../tools/z88dk/cpc/nocart/nocart.' + NATIVE_EXTENSION + ' -h',
    'old2mfm'  : '../tools/cc65/telestrat/old2mfm',
    'ea5split' : '../tools/ti99/ea5split.' + NATIVE_EXTENSION + ' -h',
    'elf2ea5'  : '../tools/ti99/elf2ea5.' + NATIVE_EXTENSION + ' -h',
    'exomizer' : '../tools/generic/exomizer/exomizer -h',
    'sapfs'    : '../tools/cmoc/mo5/sapfs.' + NATIVE_EXTENSION + '',
    'tap2dsk'  : '../tools/cc65/telestrat/tap2dsk',
    }


LIGHT_TOOL_COMMAND = \
    {
    'bbcim'    : '../tools/bbc/bbcim.' + NATIVE_EXTENSION + ' -h',
    'f2k5'     : '../tools/cmoc/mo5/f2k5.' + NATIVE_EXTENSION + '',
    'file2dsk' : '../tools/cmoc/coco/file2dsk/file2dsk.' + NATIVE_EXTENSION + ' -h',
    'fixcart'  : '../tools/cc65/gamate/gamate-fixcart.' + NATIVE_EXTENSION + '',
    'mkatr'    : '../tools/cc65/atari/mkatr-master/mkatr -h',
    'makewzd'  : '../tools/z88dk/oz/makewzd.' + NATIVE_EXTENSION + ' -h',
    'ea5split' : '../tools/ti99/ea5split.' + NATIVE_EXTENSION + ' -h',
    'elf2ea5'  : '../tools/ti99/elf2ea5.' + NATIVE_EXTENSION + ' -h',
    'sapfs'    : '../tools/cmoc/mo5/sapfs.' + NATIVE_EXTENSION + '',
    }
    

TOOL_COMMAND_EXPECTED = \
    {
    'abcwrite' : 256,
    'bin2abc'  : 0,
    'cc1541'   : 65280,
    'f2k5'     : 0,
    'sapfs'    : 256,
    'file2dsk' : 256,
    'makewzd'  : 256,
    'fixcart'  : 65280,
    'bbcim'    : 256,
    'nocart'   : 256,
    'm20'      : 256,
    'mkatr'    : 0,
    'elf2ea5'  : 256,
    'ea5split' : 256,
    'exomizer' : 256,
    'tap2dsk'  : 256,
    'old2mfm'  : 256,
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
    'abcwrite',
    'bin2abc',
    'cc1541',
    'f2k5',
    'sapfs',
    'file2dsk',
    'makewzd',
    'mkatr',
    'fixcart',
    'bbcim',
    'nocart',
    'm20',
    'elf2ea5',
    'ea5split',
    'exomizer',
    'tap2dsk',
    'old2mfm',
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
    for command in command_list.keys():
        if len(command)>max_len:
            max_len = len(command)
    max_len = 28

    for command in command_list.keys():
        result = os.system(command_list[command] + " > /dev/null 2>&1")
        spaces = " " * (max_len+1-len(command))
        if result==expected_list[command]:
            # print("[" + command + "] found")
            res = "found\n"
            res_color = bcolors.OKGREEN
            res_color2 = bcolors.OKGREEN
            total_result[command]=True
        else:

            total_result[command]=False
            if command in BUILDABLE_TOOLS:
                res = "NOT built\n"
                res_color = bcolors.OKBLUE
                res_color2 = bcolors.OKCYAN
            else:
                res = "NOT found\n"
                res_color = bcolors.WARNING
                res_color2 = bcolors.WARNING
        if not silent:
            printc(option_config, res_color,"[" + command + "]")
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

def test_light_tools(option_config, silent=False):
    return check_programs(option_config, "LIGHT_TOOLS", LIGHT_TOOL_COMMAND,TOOL_COMMAND_EXPECTED, silent)

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
    return test_cross_compilers(option_config)


def run_single_unit_test(option_config, test_file_name, path="unit_tests"):
    log_file_path  = "../logs/" + test_file_name + "_unit_test.log"
    command_string = "python -t " + path + "/" + test_file_name + " > " + log_file_path + " 2>&1"
    printc(option_config, bcolors.OKBLUE,"----------------------------------------\n")
    printc(option_config, bcolors.OKCYAN,test_file_name + "\n")
    printc(option_config, bcolors.OKBLUE,"--------------------------------\n")
    verbosity = option_config.terminal_config.verbose
    option_config.terminal_config.verbose = 1
    run_command(option_config, command_string)
    option_config.terminal_config.verbose = verbosity
    with open(log_file_path) as log_file:
        log_content = log_file.read()
        print("--start of log content--", flush=True)
        print(log_content, flush=True)
        print("--end of log content--", flush=True)
        
    return log_content


def _unit_tests(option_config, path="./"):
    printc(option_config, bcolors.OKBLUE,"----------------------------------------\n")
    printc(option_config, bcolors.OKCYAN, "UNIT-TESTS")
    printc(option_config, bcolors.OKBLUE,"\n----------------------------------------\n")

    result_map = {}

    assets_tests = files_in_path("assets/unit_tests")
    
    for test in assets_tests:
        result_map[test] = run_single_unit_test(option_config, test, "assets/unit_tests")
    
    main_tests = files_in_path("./unit_tests")

    for test in main_tests:
        result_map[test] = run_single_unit_test(option_config, test)
    return result_map

# TODO: Handle aliases such as dragon->coco

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

    files_per_project = binary_factor(target)

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
    files = built_files_in_path("../build")
    return not(len(files))

def check_tools(option_config, target):
    # tools_result_map=test_tools(option_config, silent=True)
    if option_config.terminal_config.fast_test:
        tools_result_map=test_light_tools(option_config, silent=True)
    else:
        tools_result_map=test_tools(option_config, silent=True)

    number_of_tools = len(tools_result_map.keys())

    built_tools = 0
    for _, result in tools_result_map.items():
        if result:
            built_tools+=1
    return built_tools

def check_complex(option_config, target):
    number_files = len(built_files_in_path("../build"))
    return number_files==2


def check_examples(option_config, target):
    number_files = len(built_files_in_path("../build"))
    number_of_examples = len(dirs_in_path("./examples"))
    return number_files == number_of_examples*binary_factor(target)


def check_games(option_config, target):
    number_files = len(built_files_in_path("../build"))
    number_of_games = len(dirs_in_path("./games"))

    return number_files == number_of_games*binary_factor(target)


def check_games_terminal(option_config, target):
    number_files = len(built_files_in_path("../build"))
    number_of_games = len(dirs_in_path("./games"))
    return number_files == number_of_games*binary_factor("terminal")


def check_rename(option_config, target):
    number_files = len(built_files_in_path("../build"))
    return number_files==2*binary_factor(target)


def check_create(option_config, target):
    number_files = len(built_files_in_path("../build"))
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


# ---------------------------------------------------------
# DEFAULT SELF TESTS
# ---------------------------------------------------------

STANDARD_SELF_TESTS = \
    [ \
        ("xl clean",            CLEAN_TEST,          check_clean), \
        ("xl dev tools",        DEV_TOOLS_TEST), \
        ("xl create",           CREATE_TEST,         check_create,  CLEANUP_CREATE_TEST), \
        ("xl rename",           RENAME_TEST,         check_rename,  CLEANUP_RENAME_TEST) \
    ]

MAKE_SELF_TESTS = \
    [ \
        ("xl make",             MAKE_TEST,           check_make), \
    ]

COMPLEX_SELF_TESTS = \
    [
        ("several xl commands", COMPLEX_TEST,        check_complex, CLEANUP_COMPLEX_TEST), \
    ]

TOOLS_SELF_TESTS = \
    [
        ("build some tools",    TOOLS_TEST,          check_tools,   CLEANUP_TOOLS_TEST),
    ]

PARALLEL_BUILD_TESTS = \
    [ \
        ("xl examples",         EXAMPLES_TEST,       check_examples), \
        ("xl games",            GAMES_TEST,          check_games), \
    ]

TERMINAL_BUILD_TESTS =  \
    [ \
        ("xl games terminal",   GAMES_TERMINAL_TEST, check_games_terminal), \
    ]

INTERACTIVE_TESTS = \
    [ \
        ("xl run",              RUN_TEST,       no_check), \
    ]
    

def _test_self(option_config, target = "stdio"):
    option_config.terminal_config.test = 1

    printc(option_config, bcolors.OKCYAN,"----------------------------------------\n")
    printc(option_config, bcolors.OKCYAN, "XL SCRIPT TEST")
    printc(option_config, bcolors.OKCYAN,"\n----------------------------------------\n")
    total_success = 1
    success_map = {}
    printc(option_config, bcolors.BOLD,  "target: ")
    printc(option_config, bcolors.OKBLUE,target+"\n")
    printc(option_config, bcolors.OKCYAN,"----------------------------------------\n")

    self_tests = STANDARD_SELF_TESTS 
    self_tests += MAKE_SELF_TESTS
    self_tests += TOOLS_SELF_TESTS

    if option_config.terminal_config.interactive_test:
        self_tests += INTERACTIVE_TESTS
    if option_config.terminal_config.terminal_test:
        self_tests += TERMINAL_BUILD_TESTS
    if not option_config.terminal_config.fast_test:
        self_tests += COMPLEX_SELF_TESTS
        self_tests += PARALLEL_BUILD_TESTS

    execute_commands(option_config, INITIAL_CLEANUP_COMMANDS, target, silent = True)
    for test in self_tests:
        success_map[test[0][0:]] = test_execute(option_config, target, *test)
        total_success*=success_map[test[0][0:]]
    execute_string(option_config, "xl clean", silent = True)
    option_config.terminal_config.test = 0
    return self_tests, success_map, total_success


def display_self_result(option_config, self_tests, success_map, total_success):
    print("")
    print("")
    print("-------------------------------")
    printc(option_config, bcolors.OKBLUE, "SELF TEST RESULTS\n")
    print("-------------------------------")

    max_len = 0
    for test in self_tests:
        if len(test[0][0:])>max_len:
            max_len = len(test[0][0:])

    for test in self_tests:
        success = success_map[test[0][0:]]
        (success_color, success_string) = (bcolors.OKGREEN, "OK") if success else  (bcolors.FAIL, "KO")
        
        
        spaces = " " * (max_len+5-len(test[0][0:])) 
        printc(option_config, bcolors.OKCYAN,test[0][0:] + " " + spaces)
        
        printc(option_config, success_color, success_string + "\n")
        print("-------------------------------")

    return self_tests, success_map, total_success


def test_self(option_config, target = "stdio"):
    self_tests, success_map, total_success = _test_self(option_config, target)
    return display_self_result(option_config, self_tests, success_map, total_success)


def _test_components(option_config):
    cross_compilers_result = test_cross_compilers(option_config)
    test_tools(option_config)
    libraries_result = test_libraries(option_config)
    interpreters_result = test_interpreters(option_config)

    test_emulators(option_config)
    native_compilers_result = test_native_compilers(option_config)
    test_roms(option_config)
    make_result = test_make(option_config, silent=False)
    return make_result, native_compilers_result, cross_compilers_result, libraries_result, interpreters_result


def test_components(option_config):
    
    make_result, native_compilers_result, cross_compilers_result, libraries_result, interpreters_result = _test_components(option_config)
    
    display_components_result(option_config, make_result, native_compilers_result, cross_compilers_result, libraries_result, interpreters_result)


def _display_component_and_result(option_config, component, result, color):
    printc(option_config, bcolors.OKCYAN, component)
    printc(option_config, color, result+"\n")
    print("-------------------------------")


def display_components_result(option_config, make_result, native_compilers_result, cross_compilers_result, libraries_result, interpreters_result):
    
    print("")
    print("")
    print("-------------------------------")
    printc(option_config, bcolors.OKBLUE, "COMPONENTS TEST RESULTS\n")
    print("-------------------------------")
    
    python_version = sys.version_info[0]
    python_subversion = sys.version_info[1]
    
    if python_version>=3:
        python_bool = "OK"
        python_color = bcolors.OKGREEN
    elif python_version>=2 and python_subversion>=7:
        python_bool = "WARNING"
        python_color = bcolors.WARNING
    else:
        python_bool = "KO"
        python_color = bcolors.FAIL
    
    make_bool, make_color =                       ("OK", bcolors.OKGREEN) if bool(len([value for value in make_result if make_result[value]==True] )) else            ("KO", bcolors.FAIL)
    cross_compilers_bool, cross_compilers_color = ("OK", bcolors.OKGREEN) if bool(len([value for value in cross_compilers_result if cross_compilers_result[value]==True] )) else ("WARNING", bcolors.WARNING)
    ncurses_bool, ncurses_color =                 ("OK", bcolors.OKGREEN) if libraries_result['ncurses'] else            ("WARNING", bcolors.WARNING)
    java_bool, java_color =                       ("OK", bcolors.OKGREEN) if interpreters_result['java'] else            ("WARNING", bcolors.WARNING)
    perl_bool, perl_color =                       ("OK", bcolors.OKGREEN) if interpreters_result['perl'] else            ("WARNING", bcolors.WARNING)
    gcc_bool, gcc_color =                         ("OK", bcolors.OKGREEN) if native_compilers_result['gcc'] else         ("WARNING", bcolors.WARNING)
    gpp_bool, gpp_color =                         ("OK", bcolors.OKGREEN) if native_compilers_result['g++'] else         ("WARNING", bcolors.WARNING)
    
    _display_component_and_result(option_config, "python version    ", python_bool, python_color)
    _display_component_and_result(option_config, "make              ", make_bool, make_color)
    _display_component_and_result(option_config, "gcc               ", gcc_bool, gcc_color)
    _display_component_and_result(option_config, "ncurses           ", ncurses_bool, ncurses_color)
    _display_component_and_result(option_config, "cross-compilers   ", cross_compilers_bool, cross_compilers_color)
    _display_component_and_result(option_config, "java              ", java_bool, java_color)
    _display_component_and_result(option_config, "perl              ", perl_bool, perl_color)
    _display_component_and_result(option_config, "g++               ", gpp_bool, gpp_color)


# These tests include:
# - Some dependencies (compilers, tools, libraries, interpreters)
# - Unit-tests for Python script 
# - Tests the output for the stdio target
# - Tests of the `xl` script commands 
def _test_standard_cases(option_config, params):

    make_result, native_compilers_result, cross_compilers_result, libraries_result, interpreters_result = _test_components(option_config)
    
    unit_tests_result = _unit_tests(option_config)
    
    if params=="stdio":
        output_result = _test_output(option_config)
    else:
        output_result = None
        
    self_tests, success_map, total_success = _test_self(option_config, params)
    
    return make_result, native_compilers_result, cross_compilers_result, libraries_result, interpreters_result, unit_tests_result, output_result, self_tests, success_map, total_success


def display_unit_test_result(option_config, unit_tests_result):

    print("")
    print("")
    print("-------------------------------")
    printc(option_config, bcolors.OKBLUE, "UNIT TESTS RESULTS\n")
    print("-------------------------------")

    max_len = 0
    for test in unit_tests_result.keys():
        if len(test)>max_len:
            max_len = len(test)

    for test in unit_tests_result.keys():
        (success_color, success_string) = (bcolors.OKGREEN, "OK") if unit_tests_result[test].endswith("OK\n") else  (bcolors.FAIL, "KO")
        
        spaces = " " * (max_len+5-len(test)) 
        printc(option_config, bcolors.OKCYAN,test + " " + spaces)
        
        printc(option_config, success_color, success_string + "\n")
        print("-------------------------------")


def test_standard_cases(option_config, params):
    make_result, native_compilers_result, cross_compilers_result, libraries_result, interpreters_result, unit_tests_result, output_result, self_tests, success_map, total_success = _test_standard_cases(option_config, params)

    display_components_result(option_config, make_result, native_compilers_result, cross_compilers_result, libraries_result, interpreters_result)
    display_unit_test_result(option_config, unit_tests_result)
    if output_result:
        display_output_result(option_config, output_result)
    display_self_result(option_config, self_tests, success_map, total_success)
    return total_success


# ["native", "cc65", "z88dk", "z88dk_alt", "cmoc", "lcc1802", "ack", "cc6303", "vbcc"]:\
def _test_compilation(option_config):
    compilers_check = test_compilers(option_config)
    result_map = {}
    success, built_binaries, expected_binaries = test_targets(option_config,["", "native"])
    result_map["native"] = (success, built_binaries, expected_binaries)
    for compiler in compilers_check.keys():
        if compilers_check[compiler]:
            success, built_binaries, expected_binaries = test_targets(option_config, ["", compiler])
            result_map[compiler] = success, built_binaries, expected_binaries
    if compilers_check["z88dk"]:
        success, built_binaries, expected_binaries = test_targets(option_config,["", "z88dk_alt"])
        result_map["z88dk_alt"] = success, built_binaries, expected_binaries
    clean(option_config, [])
    return result_map


def display_compilation_result(option_config, result_map):
    print("")
    print("")
    
    print("----------------------------------------")
    printc(option_config, bcolors.OKBLUE, "COMPILATION TEST RESULTS\n")
    print("----------------------------------------")
    max_len = 0
    for test in result_map.keys():
        if len(test)>max_len:
            max_len = len(test)
    for test in result_map.keys():
        success, built_binaries, expected_binaries = result_map[test]
        (success_color, success_string) = (bcolors.OKGREEN, "OK") if success else  (bcolors.FAIL, "KO")
        
        spaces = " " * (max_len+5-len(test)) 
        printc(option_config, bcolors.OKCYAN,test + " " + spaces)
        
        built_space = " " * (2-len(str(built_binaries))+1)
        expected_space = " " * (2-len(str(expected_binaries))+1)
        
        variable_binaries = " *" if test=="tms9900-gcc" else ""
        
        comparison = "       " + built_space + str(built_binaries) + "    " + expected_space + str(expected_binaries) + variable_binaries
        
        printc(option_config, success_color, success_string)
        printc(option_config, bcolors.OKCYAN, comparison + "\n")
        print("----------------------------------------")


def test_compilation(option_config):
    result_map = _test_compilation(option_config)
    display_compilation_result(option_config, result_map)


# These tests include
# - Some more dependencies (emulators, cross-compilers, native compilers, roms, make)
# - Tests in "test_standard_cases" with forced terminal target
# - Compilation tests for most installed compilers
def test_everything(option_config, params): # TODO: Do something with params    
    # Also check terminal target
    option_config.terminal_config.terminal_test = 1
    print("-------------------------------------------------")
    make_result, native_compilers_result, cross_compilers_result, libraries_result, interpreters_result, unit_tests_result, output_result, self_tests, success_map, total_success = _test_standard_cases(option_config, params)

    compilation_result = _test_compilation(option_config)
    
    display_components_result(option_config, make_result, native_compilers_result, cross_compilers_result, libraries_result, interpreters_result)
    
    display_unit_test_result(option_config, unit_tests_result)
    
    display_output_result(option_config, output_result)
    
    display_self_result(option_config, self_tests, success_map, total_success)
    
    display_compilation_result(option_config, compilation_result)


TEST_FILES = {
    "native"      : ["stdio", "ncurses", "terminal"],
    "cc65"        : ["vic20", "supervision", "atari", "atari_lynx", "creativision", "pet", "c64", "oric"],
    "z88dk"       : ["c128_z80_80col", "cpm_z80_adm3a", "spectrum_48k", "msx", "zx81_32k_wrx"],
    "cmoc"        : ["coco", "coco3", "mo5"],
    "lcc1802"     : ["comx", "tmc600"],
    "ack"         : ["msdos", "pc86"],
    "cc6303"      : ["mc10"],
    # "vbcc"        : ["bbc", "bbcmaster"], # Not enough memory for XL HD
    "vbcc"        : ["bbcmaster"],
    "tms9900-gcc"      : ["ti99"],
    }

# TODO: This should not be hard-coded.
Z88DK_ALT_EXPECTED_FILES = 45

def _test_targets(option_config, params):

    verbose = option_config.terminal_config.verbose
    success = clean_test(option_config)
    compilation_threads = option_config.build_config.compilation_threads
    native_compiler = option_config.build_config.native_compiler
    GNU_MAKE = option_config.build_config.gnu_make
    if params[1] == "z88dk_alt":
        parallel = " -j " + compilation_threads
    else:
        parallel = ""

    game_dir = "chase"
    project_type = "game"
    if is_project_split(game_dir):
        if verbose:
            printc(option_config, bcolors.OKBLUE,"Create main.c from split source files\n")
        create_main(game_dir, project_type)
    if params[1] in TEST_FILES.keys():
        devkit_test_files = TEST_FILES[params[1]]
        print("Testing: " + str(devkit_test_files)[1:][:-1])
        print("Targets: " + str(devkit_test_files))
        
        # expected_files = 0
        for test_file in devkit_test_files:
            # TODO: Parameters should be read from config.ini
            _use_tools = option_config.build_config.use_tools
            _tool_compiler = option_config.build_config.tool_compiler
            make_command = GNU_MAKE + parallel + " " + test_file + \
                           " GNU_MAKE=" + GNU_MAKE + " _NATIVE_CC="+ native_compiler + " " + \
                           " USE_TOOLS=" + str(_use_tools) + " TOOL_CC=" + _tool_compiler + " " + \
                           all_compilers_opts(option_config, "","") + \
                           " -f makefiles.other/chase/tests/Makefile.tests"
            # expected_files += binary_factor(test_file)
            run_command(option_config, make_command)
    elif params[1]=="z88dk_alt":
        make_command = GNU_MAKE + parallel + " GNU_MAKE=" + GNU_MAKE + \
                       " z88dk_quick_test -f makefiles.other/chase/tests/Makefile.z88dk_quick_tests"
        run_command(option_config, make_command)
        # expected_files = Z88DK_ALT_EXPECTED_FILES # TODO: Remove this hardcoded value
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
    return success


def test_targets(option_config, params):
    _test_targets(option_config, params)
    return process_test_targets_result(option_config, params)


def process_test_targets_result(option_config, params):
    built_files = len(built_files_in_path("../build"))
    verbose = option_config.terminal_config.verbose
    if params[1] in TEST_FILES.keys():
        devkit_test_files = TEST_FILES[params[1]]
        if verbose:
            print("Testing: " + str(devkit_test_files)[1:][:-1])
            print("Targets: " + str(devkit_test_files))
        
        expected_files = 0
        for test_file in devkit_test_files:
            expected_files += binary_factor(test_file)
    elif params[1]=="z88dk_alt":
        expected_files = Z88DK_ALT_EXPECTED_FILES # TODO: Remove this hardcoded value
    
    if verbose:
        print("Number of built files: " + str(built_files))

    if params[1] in TEST_FILES.keys() or params[1] in ("z88dk_alt"):
        printc(option_config, bcolors.OKCYAN, "Built files: " + str(built_files)+"\n")
        printc(option_config, bcolors.OKBLUE, "Expected files: " + str(expected_files)+"\n")
        if (params[1] != "tms9900-gcc" and built_files != expected_files) or (params[1] == "tms9900-gcc" and not built_files):
            printc(option_config, bcolors.FAIL, "Number of binaries KO\n")
            success=0
        else:
            if params[1] == "tms9900-gcc":
                printc(option_config, bcolors.OKGREEN, "Variable number of binaries OK\n")

            else:
                printc(option_config, bcolors.OKGREEN, "Number of binaries OK\n")
            success=1
    return success, built_files, expected_files


EXPECTED_HD_OUTPUT = \
{
    "clear":      ['\n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n'],
    "hello":      ['\n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '           HELLO WORLD          \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n'],
    "target":     ['\n', 'TARGET INFORMATION              \n', '                                \n', 'XSIZE 32  YSIZE 24              \n', '                                \n', 'TILES 91  8X8                   \n', '                                \n', 'GRAPHICS      ON                \n', '                                \n', 'COLOR         OFF               \n', '                                \n', 'TEXT COLOR    OFF               \n', '                                \n', 'JOYSTICK      OFF               \n', '                                \n', 'SOUND         OFF               \n', '                                \n', 'SMALL CHARS   ON                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n'],
    "display":    ['\n', '                                \n', '                                \n', '  0 1 2 3 4 5 6 7               \n', '  8 9 0 1 2 3 4 5               \n', '  6 7 8 9 0 1 2 3               \n', '  4 5 6 7 8 9 0 1               \n', '  2 3 4 5 6 7 8 9               \n', '  0 1 2 3 4 5 6 7               \n', '  8 9 0 1 2 3 4 5               \n', '  6 7 8 9 0 1 2 3               \n', '  4 5 6 7 8 9 0 1               \n', '  2 3 4 5 6 7 8 9               \n', '  0 1 2 3 4 5 6 7               \n', '  8 9 0                         \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '  END OF DEMO                   \n', '                                \n', 'ABCDEFGHIJKLMNOPQRSTUVWXYZ      \n', '0123456789                      \n', '                                \n'],
    "boundary":   ['\n', '00000000000000000000000000HI1234\n', '0                              0\n', '0                              0\n', '0                              0\n', '0                              0\n', '0                              0\n', '0                              0\n', '0                              0\n', '0                              0\n', '0                              0\n', '0                              0\n', '0                              0\n', '0                              0\n', '0                              0\n', '0                              0\n', '0                              0\n', '0                              0\n', '0                              0\n', '0                              0\n', '0                              0\n', '0                              0\n', '0                              0\n', '0         HELLO WORLD          0\n', '00000000000000000000000000000000\n'],
    "numbers":    ['\n', '                                \n', '                                \n', '             9                  \n', '                                \n', '             99                 \n', '                                \n', '             999                \n', '                                \n', '             5000               \n', '                                \n', '             20000              \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n'],
    "characters": ['\n', '                    01234567890 \n', '       PRINTD                   \n', '       0123456789               \n', '                                \n', '       BLUE                     \n', '                                \n', '                                \n', '        ABCDEFG HIJKLM          \n', '                                \n', '        NOPQRST UVWXYZ          \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', 'A B C D E F G H I J K L M N O P \n', '                                \n', 'Q R S T U V W X Y Z             \n', '                                \n', '                                \n', '                                \n', '       END OF DEMO              \n'],
}

EXPECTED_LD_OUTPUT = \
{
    "clear":      ['\n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n'],
    "hello":      ['\n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '           HELLO WORLD          \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n'],
    "target":     ['\n', 'TARGET INFORMATION              \n', '                                \n', 'XSIZE 32  YSIZE 24              \n', '                                \n', 'TILES 27  8X8                   \n', '                                \n', 'GRAPHICS      ON                \n', '                                \n', 'COLOR         OFF               \n', '                                \n', 'TEXT COLOR    OFF               \n', '                                \n', 'JOYSTICK      OFF               \n', '                                \n', 'SOUND         OFF               \n', '                                \n', 'SMALL CHARS   ON                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n'],
    "display":    ['\n', '                                \n', '                                \n', '  0 1 2 3 4 5 6 7               \n', '  8 9 0 1 2 3 4 5               \n', '  6 7 8 9 0 1 2 3               \n', '  4 5 6                         \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '  END OF DEMO                   \n', '                                \n', 'ABCDEFGHIJKLMNOPQRSTUVWXYZ      \n', '0123456789                      \n', '                                \n'],
    "boundary":   ['\n', '00000000000000000000000000HI1234\n', '0                              0\n', '0                              0\n', '0                              0\n', '0                              0\n', '0                              0\n', '0                              0\n', '0                              0\n', '0                              0\n', '0                              0\n', '0                              0\n', '0                              0\n', '0                              0\n', '0                              0\n', '0                              0\n', '0                              0\n', '0                              0\n', '0                              0\n', '0                              0\n', '0                              0\n', '0                              0\n', '0                              0\n', '0         HELLO WORLD          0\n', '00000000000000000000000000000000\n'],
    "numbers":    ['\n', '                                \n', '                                \n', '             9                  \n', '                                \n', '             99                 \n', '                                \n', '             999                \n', '                                \n', '             5000               \n', '                                \n', '             20000              \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n'],
    "characters": ['\n', '                    01234567890 \n', '       PRINTD                   \n', '       0123456789               \n', '                                \n', '       BLUE                     \n', '                                \n', '                                \n', '        ABCDEFG HIJKLM          \n', '                                \n', '        NOPQRST UVWXYZ          \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', '                                \n', 'A B C D E F G H I J K L M N O P \n', '                                \n', 'Q R S T U V W X Y Z             \n', '                                \n', '                                \n', '                                \n', '       END OF DEMO              \n'],
}


def test_project_output(option_config, project_name, target = "stdio"):
    clean(option_config, [])
    build(option_config, ["", project_name, target])
    run(option_config,["",project_name, target])
    if option_config.build_config.hd:
        expected_output = EXPECTED_HD_OUTPUT[project_name]
    else:
        expected_output = EXPECTED_LD_OUTPUT[project_name]
    
    if target == "stdio":
        with open("../logs/output_" + project_name + ".log","r") as output_file:
            text_content = output_file.readlines()
        if text_content==expected_output:
            print("OUTPUT OK for " + project_name)
            return True
        else:
            print("OUTPUT KO for " + project_name)
            print(str(text_content))
            return False
    return True


OUTPUT_TEST_PROJECTS = ["clear", "hello", "target", "display", "boundary", "numbers", "characters"]

def _test_output(option_config, target = "stdio"):
    prev_interactive_config = option_config.terminal_config.interactive_test
    option_config.terminal_config.interactive_test = 0
    clean_output(option_config)
    if not target in ["stdio", "ascii"] and not target.startswith("terminal"):
        target = "stdio"
    result = {}
    option_config.terminal_config.test = 1
    for project_name in OUTPUT_TEST_PROJECTS:
        result[project_name] = test_project_output(option_config, project_name, target)
    option_config.terminal_config.test = 0
    option_config.terminal_config.interactive_test = prev_interactive_config
    return result


def test_output(option_config, target = "stdio"):
    result = _test_output(option_config, target)
    display_output_result(option_config, result)
    return result


def display_output_result(option_config, result):
    
    print("")
    print("")
    
    print("-------------------------------")
    printc(option_config, bcolors.OKBLUE, "OUTPUT TEST RESULTS\n")
    print("-------------------------------")
    max_len = 0
    for project_name in OUTPUT_TEST_PROJECTS:
        if len(project_name)>max_len:
            max_len = len(project_name)
    for project_name in OUTPUT_TEST_PROJECTS:
        success = result[project_name]
        (success_color, success_string) = (bcolors.OKGREEN, "OK") if success else  (bcolors.FAIL, "KO")
        
        
        spaces = " " * (max_len+5-len(project_name)) 
        printc(option_config, bcolors.OKCYAN, project_name + " " + spaces)
        
        printc(option_config, success_color, success_string + "\n")
        print("-------------------------------")


# Self-test xl and native build
def test(option_config, params):
    if (len(params)<=1) or ((len(params)==2) and (params[1]=="standard")):
        if test_standard_cases(option_config, "stdio"):
            printc(option_config, bcolors.OKGREEN, "\nTEST OK\n")
        else:
            printc(option_config, bcolors.FAIL, "\nTEST KO\n")
        return
    if params[1] in ["self", "xl", "script", "scripts"]:
        if len(params)<3:
            test_self(option_config)
        else:
            test_self(option_config, params[2])
    elif params[1]=="components":
        test_components(option_config)
    elif params[1]=="output":
        if len(params)>2:
            target = params[2]
        else:
            target = "stdio"
        test_output(option_config, target)
    elif params[1] in ["targets", "compilation", "compile"]:
        test_compilation(option_config)
    elif params[1] in ["everything", "every", "complete", "e", "all", "a"]:
        test_everything(option_config, "stdio")
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
        unit_tests(option_config)
    elif params[1] in TEST_FILES.keys() or params[1].endswith('_alt'):
        if test_targets(option_config, params):
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
            test_standard_cases(option_config, params[1])
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
    unit_test_result = _unit_tests(option_config)
    display_unit_test_result(option_config, unit_test_result)
    

def self(option_config, params):
    if len(params)<2:
        test_self(option_config, )
    else:
        test_self(option_config, params[1])
