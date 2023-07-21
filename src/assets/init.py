from os import walk

import sys

DEFAULT_THREADS = 8

python_version = sys.version_info[0]
python_subversion = sys.version_info[1]

def dirs_in_path(mypath):
    dirs = []
    for (dirpath, dirnames, filenames) in walk(mypath):
        dirs.extend(dirnames)
        break
    return dirs

game_projects = dirs_in_path("./games")
example_projects = dirs_in_path("./examples")

    

class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKCYAN = '\033[96m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'


def read_config_option(section, option):
    if config.has_option(section,option):
        return config.get(section,option)
    else:
        return ""

verbose = True 
compiler_opts = ""
compilation_threads = 1
zsdcc_compiler_opts = ""
sccz80_compiler_opts = ""
cmoc_compiler_opts = ""
native_compiler = "gcc"
tool_compiler = "gcc"  
cc65_compiler_opts = "-O -Cl"
lcc1802_compiler_opts = '"-Wf-volatile" -O "-Wp-D nofloats" "-Wa-D LCCNOLONG" "-Wf-mulcall"' 
use_tools = 0
color_terminal = 0


def printc(color,text):    
    if(color_terminal):
        print(color + text + bcolors.ENDC)
    else:
        print(text)


def read_config():

    try:
        import configparser
    except ImportError:
        # Python 2.x fallback
        print("Python 2.x configuration")
        import ConfigParser as configparser

    global verbose
    
    global compilation_threads
    global sccz80_compiler_opts 
    global zsdcc_compiler_opts
    global cmoc_compiler_opts
    global cc65_compiler_opts
    global lcc1802_compiler_opts

    global native_compiler
    
    global tool_compiler     
    
    global interactive_clean
    
    global vice_path
    global vice_rom_path
    
    global mame_path
    global mame_rom_path
    
    global extend_algorithm
    
    global replace_shapes
    
    global use_tools

    global color_terminal
    
    global config 

    config = configparser.ConfigParser()
    
    try:
        config.read('config.ini')
        
        verbose = read_config_option("terminal","verbose")
        if verbose!="":
            verbose=int(verbose)
        else:
            verbose=0
        if verbose:
            print("Verbose Mode ON")

        color_terminal = read_config_option("terminal","color")
        
        if verbose and color_terminal:
            printc(bcolors.OKBLUE, "Color terminal ON")
        else:
            print("Color terminal OFF")
        
        if verbose:
            print("Config file found with: " + str(config.sections()))
            print("")
        
        compilation_threads = read_config_option("build","threads") 
        if compilation_threads=="auto" or compilation_threads=="automaic":
            if verbose:
                print("Detecting number of threads...")
            import multiprocessing
            compilation_threads = str(multiprocessing.cpu_count())
            if verbose:
                print("Detected " + compilation_threads + " threads...")

        sccz80_compiler_opts = read_config_option("build","sccz80_compiler_opts") 
        zsdcc_compiler_opts = read_config_option("build","zsdcc_compiler_opts")
        cmoc_compiler_opts = read_config_option("build","cmoc_compiler_opts")
        cc65_compiler_opts = read_config_option("build","cc65_compiler_opts")
        lcc1802_compiler_opts = read_config_option("build","lcc1802_compiler_opts")

        
        native_compiler = read_config_option("build", "native_compiler")
        
        tool_compiler = read_config_option("tools", "tool_compiler")
        
        interactive_clean = read_config_option("clean", "interactive")
        
        vice_path = read_config_option("run", "vice_path")
        vice_rom_path = read_config_option("run", "vice_rom_path")
        
        mame_path = read_config_option("run", "mame_path")
        mame_rom_path = read_config_option("run", "mame_rom_path")
        
        extend_algorithm = read_config_option("extend", "extend_algorithm")
        
        replace_shapes = read_config_option("extend", "replace_shapes")
        
        use_tools = read_config_option("build", "use_tools")
        
        if interactive_clean!="":
            interactive_clean = int(interactive_clean)
        
        if replace_shapes!="":
            replace_shapes = int(replace_shapes)

        if use_tools!="":
            use_tools = int(use_tools)
        
    except Exception as e:
        print("An exception occurred while reading the configuration: " + str(e))
        print("WARNING: Setting default values")
        verbose = True 
        compiler_opts = ""
        compilation_threads = 1
        zsdcc_compiler_opts = ""
        sccz80_compiler_opts = ""
        cmoc_compiler_opts = ""
        native_compiler = "gcc"
        tool_compiler = "gcc"  
        cc65_compiler_opts = "-O -Cl"
        lcc1802_compiler_opts = '"-Wf-volatile" -O "-Wp-D nofloats" "-Wa-D LCCNOLONG" "-Wf-mulcall"' 
        use_tools = 0
        color_terminal = 0
        # TODO: Ask the user to continue or exit
    else:
        if verbose:
            printc(bcolors.OKGREEN, "config.ini OK")
            print("")

