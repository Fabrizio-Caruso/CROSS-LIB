from __future__ import print_function


import sys
import os

from print_functions import printc, bcolors
from LoggerSingleton import LoggerSingleton
from init import *

logger = LoggerSingleton.initLogger('xl', '../logs')


class TerminalConfig():
    def __init__(
        self,
        verbose = "0",
        color_terminal = "0",
        test = "0",
        native_console = "",
        fast_test = "1",
        interactive_test = "0"
        ):
        # terminal
        self.verbose = verbose
        self.color_terminal = color_terminal
        self.test = test
        self.native_console = native_console
        self.fast_test = fast_test
        self.interactive_test = interactive_test


class RomConfig():
    def __init__(
        self,
        vice_path = "",
        vice_rom_path = "",
        mame_path = "",
        mame_rom_path = ""
        ):
        self.vice_path = vice_path
        self.vice_rom_path = vice_rom_path
        self.mame_path = mame_path
        self.mame_rom_path = mame_rom_path

class ExtendConfig():
    def __init__(
        self,
        extend_algorithm = "",
        replace_shapes = ""
        ):
        self.extend_algorithm = extend_algorithm
        self.replace_shapes = replace_shapes


class BuildConfig():
    def __init__(
        self,
        gnu_make = "make",
        compilation_threads = "4",
        compilation_threads_string = "",
        parallelize_multi_build = "0",
        z88dk_compiler = "sccz80",
        z88dk_compiler_opts = "",
        sccz80_compiler_opts = "",
        zsdcc_compiler_opts = "zsdcc",
        cmoc_compiler_opts = "",
        cc65_compiler_opts = "",
        lcc1802_compiler_opts = "",
        gcc4ti99_compiler_opts = "",
        vbcc_compiler_opts = "",
        native_compiler_opts = "",
        native_compiler = "gcc",
        tool_compiler = "gcc",
        use_tools = "1"
        ):

        # build
        self.gnu_make                   = gnu_make
        self.compilation_threads        = compilation_threads
        self.compilation_threads_string = compilation_threads_string
        self.parallelize_multi_build    = parallelize_multi_build
        self.z88dk_compiler             = z88dk_compiler
        self.z88dk_compiler_opts        = z88dk_compiler_opts
        self.sccz80_compiler_opts       = sccz80_compiler_opts
        self.zsdcc_compiler_opts        = zsdcc_compiler_opts
        self.cmoc_compiler_opts         = cmoc_compiler_opts
        self.cc65_compiler_opts         = cc65_compiler_opts
        self.lcc1802_compiler_opts      = lcc1802_compiler_opts
        self.gcc4ti99_compiler_opts     = gcc4ti99_compiler_opts
        self.vbcc_compiler_opts         = vbcc_compiler_opts
        self.native_compiler_opts       = native_compiler_opts
        self.native_compiler            = native_compiler
        self.tool_compiler              = tool_compiler
        self.use_tools                  = use_tools

    def get_opts(self):
        return \
            self.gnu_make, \
            self.compilation_threads, \
            self.compilation_threads_string, \
            self.parallelize_multi_build, \
            self.z88dk_compiler, \
            self.z88dk_compiler_opts, \
            self.sccz80_compiler_opts, \
            self.zsdcc_compiler_opts, \
            self.cmoc_compiler_opts, \
            self.cc65_compiler_opts, \
            self.lcc1802_compiler_opts, \
            self.gcc4ti99_compiler_opts, \
            self.vbcc_compiler_opts, \
            self.native_compiler_opts, \
            self.native_compiler, \
            self.tool_compiler, \
            self.use_tools


class OptionConfig:
    def __init__(
        self,
        
        terminal_config = TerminalConfig(),
        build_config    = BuildConfig(), 
        rom_config      = RomConfig(),
        extend_config   = ExtendConfig(),  

    ):
        self.terminal_config = terminal_config
               
        self.build_config = build_config
               
        # rom
        self.rom_config = rom_config

        # extend
        self.extend_config = extend_config


def all_compilers_opts(option_config, zsdcc_extra_optimization, compiler_opts):

    gnu_make, \
    compilation_threads, \
    compilation_threads_string, \
    parallelize_multi_build, \
    z88dk_compiler, \
    z88dk_compiler_opts, \
    sccz80_compiler_opts, \
    zsdcc_compiler_opts, \
    cmoc_compiler_opts, \
    cc65_compiler_opts, \
    lcc1802_compiler_opts, \
    gcc4ti99_compiler_opts, \
    vbcc_compiler_opts, \
    native_compiler_opts, \
    native_compiler, \
    tool_compiler, \
    use_tools \
    = option_config.build_config.get_opts()
    
    if z88dk_compiler in ('zsdcc','sdcc'):
        compiler_selection = " -compiler=sdcc "
        compiler_opts = compiler_opts + " " + zsdcc_compiler_opts
    elif z88dk_compiler in ('ez80clang', 'clang'):
        compiler_selection = " -compiler=ez80clang "
    else:
        compiler_selection = ""
        compiler_opts = compiler_opts + " " + sccz80_compiler_opts


    return " ZSDCC_MAKEFILE_COMPILATION_OPTS='" + zsdcc_compiler_opts + " " + \
                                                  zsdcc_extra_optimization + "'" + \
           " Z88DK_MAKEFILE_COMPILATION_OPTS='" + z88dk_compiler_opts + compiler_opts + \
                                                  compiler_selection + "'" + \
           " GCC4TI99_MAKEFILE_COMPILATION_OPTS='" + gcc4ti99_compiler_opts + "'" + \
           " VBCC_MAKEFILE_COMPILATION_OPTS='" + vbcc_compiler_opts + "'" + \
           " NATIVE_MAKEFILE_COMPILATION_OPTS='" + native_compiler_opts + "'" + \
           " CMOC_MAKEFILE_COMPILATION_OPTS='" + cmoc_compiler_opts + "'" + \
           " CC65_MAKEFILE_COMPILATION_OPTS='" + cc65_compiler_opts + "'" + \
           " LCC1802_MAKEFILE_COMPILATION_OPTS='" + lcc1802_compiler_opts + "'"


def config(option_config):

    printc(option_config, bcolors.BOLD,"[terminal]\n")

    verbose = option_config.terminal_config.verbose
    color_terminal = option_config.terminal_config.color_terminal
    fast_test = option_config.terminal_config.fast_test
    interactive_test = option_config.terminal_config.interactive_test
    
    print("verbose:                 " + str(verbose))
    print("color_terminal:          " + str(color_terminal))
    print("fast_test:               " + str(fast_test))
    print("interactive_test:        " + str(interactive_test))

    print("")

    gnu_make, \
    compilation_threads, \
    compilation_threads_string, \
    parallelize_multi_build, \
    z88dk_compiler, \
    z88dk_compiler_opts, \
    sccz80_compiler_opts, \
    zsdcc_compiler_opts, \
    cmoc_compiler_opts, \
    cc65_compiler_opts, \
    lcc1802_compiler_opts, \
    gcc4ti99_compiler_opts, \
    vbcc_compiler_opts, \
    native_compiler_opts, \
    native_compiler, \
    tool_compiler, \
    use_tools \
    = option_config.build_config.get_opts()

    compilation_threads = option_config.build_config.compilation_threads

    printc(option_config, bcolors.BOLD,"[build]\n")
    if not compilation_threads_string.isnumeric():
        print("compilation_threads:     " + str(compilation_threads_string) + " (" + compilation_threads + ")" )

    else:
        print("compilation_threads:     " + str(compilation_threads))
    print("parallelize_multi_build: " + str(parallelize_multi_build))

    print("z88dk_compiler:          " + z88dk_compiler)
    print("z88dk_compiler_opts:     " +  z88dk_compiler_opts)
    print("sccz80_compiler_opts:    " + sccz80_compiler_opts)
    print("zsdcc_compiler_opts:     " +  zsdcc_compiler_opts)
    print("cmoc_compiler_opts:      " +  cmoc_compiler_opts)
    print("cc65_compiler_opts:      " +  cc65_compiler_opts)
    print("lcc1802_compiler_opts:   " +  lcc1802_compiler_opts)
    print("gcc4ti99_compiler_opts:  " +  gcc4ti99_compiler_opts)
    print("vbcc_compiler_opts:      " +  vbcc_compiler_opts)
    print("native_compiler_opts:    " +  native_compiler_opts)

    print("native_compiler:         " +  native_compiler)
    print("use_tools:               " + str(use_tools))

    print("tool_compiler:           " +  tool_compiler)

    print("")
    printc(option_config, bcolors.BOLD,"[run]\n")
    print("vice_path:               " +  option_config.rom_config.vice_path)
    print("vice_rom_path:           " +  option_config.rom_config.vice_rom_path)
    print("mame_path:               " +  option_config.rom_config.mame_path)
    print("mame_rom_path:           " +  option_config.rom_config.mame_rom_path)

    print("")
    printc(option_config, bcolors.BOLD,"[extend]\n")
    print("extend_algorithm:        " +  option_config.extend_config.extend_algorithm)
    print("replace_shapes:          " +  str(option_config.extend_config.replace_shapes))
    
    
# ---------------------------------------------


# ---------------------------------------------
#  System               Value
# ---------------------------------------------
#  Linux                linux or linux2 (*)
#  Windows              win32
#  Windows/Cygwin       cygwin
#  Windows/MSYS2        msys
#  Mac OS X             darwin
#  OS/2                 os2
#  OS/2 EMX             os2emx
#  RiscOS               riscos
#  AtheOS               atheos
#  FreeBSD 7            freebsd7
#  FreeBSD 8            freebsd8
#  FreeBSD N            freebsdN
#  OpenBSD 6            openbsd6
# ---------------------------------------------

def read_config_option(config, section, option):
    if config.has_option(section,option):
        return config.get(section,option)
    return ""

def read_config(config_file="./config.ini"):
    if not os.path.exists(config_file):
        raise Exception
    try:
        import configparser
    except ImportError:
        # Python 2.x fallback
        print("Python 2.x configuration")
        import ConfigParser as configparser

    config = configparser.ConfigParser()

    try:

        config.read(config_file)

        verbose = read_config_option(config,"terminal","verbose")

        if verbose!="":
            verbose=int(verbose)
        else:
            verbose=0
        if verbose:
            print("----------------------------")
            print("Verbose Mode ON")

        color_terminal = read_config_option(config,"terminal","color_terminal")
        if color_terminal!="":
            color_terminal=int(color_terminal)
        else:
            color_terminal=0

        logger.info("Color terminal: %s", str(color_terminal))

        fast_test = read_config_option(config,"terminal","fast_test")
        if fast_test!="":
            fast_test=int(fast_test)
        else:
            fast_test=1
            
        interactive_test = read_config_option(config,"terminal","interactive_test")
        if interactive_test!="":
            interactive_test=int(interactive_test)
        else:
            interactive_test=0

        terminal_config = TerminalConfig(verbose=verbose, color_terminal=color_terminal, test=0, native_console=0, fast_test=fast_test, interactive_test=interactive_test)

        if verbose:
            print("Config file found with: " + str(config.sections()))

        gnu_make = read_config_option(config,"build","gnu_make")


        compilation_threads_string = read_config_option(config,"build","compilation_threads")


        if compilation_threads_string in ("auto", "automatic"):
            if verbose:
                print("----------------------------")
            import multiprocessing
            compilation_threads = str(multiprocessing.cpu_count())
            if verbose:
                print("Detected " + compilation_threads + " threads...")
                print("----------------------------")
        elif compilation_threads_string == "default":
            compilation_threads = str(DEFAULT_COMPILATION_THREADS)
        else:
            compilation_threads = compilation_threads_string
        logger.info("Compilation threads: %s", str(compilation_threads))

        parallelize_multi_build = read_config_option(config,"build","parallelize_multi_build")

        if parallelize_multi_build!="":
            parallelize_multi_build=int(parallelize_multi_build)
        else:
            parallelize_multi_build=0

        z88dk_compiler_opts = read_config_option(config,"build","z88dk_compiler_opts")
        z88dk_compiler = read_config_option(config,"build","z88dk_compiler")
        sccz80_compiler_opts = read_config_option(config,"build","sccz80_compiler_opts")
        zsdcc_compiler_opts = read_config_option(config,"build","zsdcc_compiler_opts")
        cmoc_compiler_opts = read_config_option(config,"build","cmoc_compiler_opts")
        cc65_compiler_opts = read_config_option(config,"build","cc65_compiler_opts")
        lcc1802_compiler_opts = read_config_option(config,"build","lcc1802_compiler_opts")
        gcc4ti99_compiler_opts = read_config_option(config,"build","gcc4ti99_compiler_opts")
        vbcc_compiler_opts = read_config_option(config,"build","vbcc_compiler_opts")
        native_compiler_opts = read_config_option(config,"build","native_compiler_opts")

        native_compiler = read_config_option(config,"build", "native_compiler")

        tool_compiler = read_config_option(config,"build", "tool_compiler")

        vice_path = read_config_option(config,"run", "vice_path")
        vice_rom_path = read_config_option(config,"run", "vice_rom_path")

        mame_path = read_config_option(config,"run", "mame_path")
        mame_rom_path = read_config_option(config,"run", "mame_rom_path")


        rom_config = RomConfig(vice_path=vice_path, vice_rom_path=vice_rom_path, mame_path=mame_path, mame_rom_path=mame_rom_path)
        
        extend_algorithm = read_config_option(config,"extend", "extend_algorithm")
        replace_shapes = read_config_option(config,"extend", "replace_shapes")
        

        use_tools = read_config_option(config,"build", "use_tools")

        if replace_shapes!="":
            replace_shapes = int(replace_shapes)

        extend_config = ExtendConfig(extend_algorithm=extend_algorithm, replace_shapes=replace_shapes)


        if use_tools!="":
            use_tools = int(use_tools)

        build_config = BuildConfig( \
            gnu_make,
            compilation_threads,
            compilation_threads_string,
            parallelize_multi_build,
            z88dk_compiler,
            z88dk_compiler_opts,
            sccz80_compiler_opts,
            zsdcc_compiler_opts,
            cmoc_compiler_opts,
            cc65_compiler_opts,
            lcc1802_compiler_opts,
            gcc4ti99_compiler_opts,
            vbcc_compiler_opts,
            native_compiler_opts,
            native_compiler,
            tool_compiler,
            use_tools
            )

        option_config = OptionConfig(terminal_config, build_config, rom_config, extend_config)

        if verbose:
            if color_terminal:
                print("Color terminal ON\n")
                # printc(option_config, bcolors.OKCYAN, "Color terminal ON\n")
            else:
                print("Color terminal OFF")

        return option_config
    except Exception as e:
        print("An exception occurred while reading the configuration: " + str(e))
        logger.debug("Error while reading configuration file")
        exc_type, exc_obj, exc_tb = sys.exc_info()
        fname = os.path.split(exc_tb.tb_frame.f_code.co_filename)[1]
        print("exc_type: %s, fname: %s, exc_tb.tb_lineno: %s", exc_type, fname, exc_tb.tb_lineno)

        raise e


def default_config():
    try:
        terminal_config = TerminalConfig( \
            verbose=0, 
            color_terminal=0, 
            test=0, 
            native_console=0, 
            fast_test=1, 
            interactive_test=0
        )

        rom_config = RomConfig( \
            vice_path="", 
            vice_rom_path="../roms/vice/", 
            mame_path="", 
            mame_rom_path="../roms/mame/"
        )

        extend_config = ExtendConfig( \
            extend_algorithm="duplicate", 
            replace_shapes=1
        )

        build_config = BuildConfig( \
            gnu_make = "make",
            compilation_threads = str(DEFAULT_COMPILATION_THREADS),
            compilation_threads_string = "default",
            parallelize_multi_build = "0",
            z88dk_compiler = "sccz80",
            z88dk_compiler_opts = "",
            sccz80_compiler_opts = "",
            zsdcc_compiler_opts = "",
            cmoc_compiler_opts = "",
            cc65_compiler_opts = "-Or -Cl",
            lcc1802_compiler_opts = '"-Wf-volatile" -O "-Wp-D nofloats" "-Wa-D LCCNOLONG" "-Wf-mulcall"',
            gcc4ti99_compiler_opts = "-O2 -fno-peephole2 -fno-function-cse",
            vbcc_compiler_opts = "",
            native_compiler_opts = "",
            native_compiler = "gcc",
            tool_compiler = "gcc",
            use_tools = "0"
        )
        
        option_config = OptionConfig(terminal_config, build_config, rom_config, extend_config)
        
    except:
        logger.warning("Error in default config")
        print("Error in default config")
        sys.exit(-1)
    return option_config


def get_config():
    if python_version<2 or (python_version<=2 and python_subversion<7):
        print("Python version not supported")
        logger.error("Python version not supported")
        sys.exit(-1)
    try:
        option_config = read_config()
        logger.info("(read_config) Configuration file read")
    except:
        if option_config.terminal_config.verbose:
            print("Failed to read config.ini")
        logger.warning("Reverting to default configuration")
        option_config = default_config()
        print("WARNING: Using default config")
    return option_config


def handle_make(option_config, make_test):
    if option_config.build_config.gnu_make == "auto":
        if option_config.terminal_config.verbose:
            print("automatic detection of GNU make")
        if make_test['gmake']:
            option_config.build_config.gnu_make = "gmake"
        else:
            option_config.build_config.gnu_make = "make"
    return option_config.build_config.gnu_make