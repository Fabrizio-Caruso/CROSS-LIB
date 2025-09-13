import os
from import_from_source import printc, bcolors
from LoggerSingleton import LoggerSingleton
from default_values import insert_default_sizes
from target_defs import *
from init import NATIVE_EXTENSION

C64_EMULATOR = "x64"
VIC20_EMULATOR = "xvic"
PLUS4_EMULATOR = "xplus4"
PET_EMULATOR = "xpet"

MAME_EMULATOR = "mame"

logger = LoggerSingleton.initLogger('xl', '../logs')

def run_command(option_config, command_string):
    logger.info("Running command: %s", command_string)
    test = option_config.terminal_config.test           # run instde test_self makes test      = False
    verbose = option_config.terminal_config.verbose
    native_console = option_config.terminal_config.native_console # run inside test_self makes native_console = False
    
    if verbose:
        printc(option_config, bcolors.BOLD, command_string+"\n")
    if not native_console and (test or not verbose):
        command_string+= " > /dev/null 2>&1"

    res = os.system(command_string)

    if not native_console and res:
        printc(option_config, bcolors.FAIL, "return code: " + str(res)+"\n")
        

def run_native(option_config, params, target):
    command_prefix = "../build/X" + params[1] + "_" + target

    if len(params)>=4:
        xsize = params[2]
        ysize = params[3]
        command_string = command_prefix + "_" + xsize + "X" + ysize + "." + NATIVE_EXTENSION
    else:
        command_string = command_prefix + "." + NATIVE_EXTENSION
    option_config.terminal_config.native_console = 1
    run_command(option_config, command_string)
    option_config.terminal_config.native_console = 0


def run_stdio(option_config, params):
    run_native(option_config, params, "stdio")


# Run a project by using an emulator if necessary. Only few emulators are supported.
def run(option_config, params):

    if len(params)==2:
        run_native(option_config, params, "ncurses")

    if len(params)>=3:
        params = insert_default_sizes(option_config, params)

        target = params[2]
        if target in DEFAULT_TARGET_MAP:
            target = DEFAULT_TARGET_MAP[target]
            if option_config.terminal_config.verbose:
                print("defaulting to target: " + target)

        if target=="stdio":
            run_stdio(option_config, params)
        elif target=="ncurses":
            run_native(option_config, params[0:2]+params[3:], "ncurses")
        elif target=="terminal":
            run_native(option_config, params[0:2]+params[3:], "terminal8x8")
        elif target.startswith("terminal"):
            run_native(option_config, params[0:2]+params[3:], target)

        # e.g. xl run bomber 20 30
        elif(len(params)>=3 and params[2].isnumeric() and params[3].isnumeric()):
            run_native(option_config, params, "ncurses")

        # TODO: replace this with a check on prefixes
        elif target in VICE_TARGETS:
            
            extension = "prg"
            rom_directory = option_config.rom_config.vice_rom_path
            if target=="c64":
                rom_directory += "c64"

                emulator = C64_EMULATOR
            elif target.startswith("vic20"):
                rom_directory += "vic20"

                if target=="vic20_exp_3k":
                    emulator = VIC20_EMULATOR + " -memory 3k"
                elif target=="vic20_unexpanded":
                    emulator = VIC20_EMULATOR + " -memory none"
                else:
                    emulator = VIC20_EMULATOR + " -memory 24k"
            elif target.startswith("c16") or target=="plus4":
                rom_directory += "plus4"

                emulator = PLUS4_EMULATOR
            elif target.startswith("pet"):
                rom_directory += "pet"
                emulator = PET_EMULATOR
            else:
                print("'xl run' does not support this target")
                return
            command_string = option_config.rom_config.vice_path + emulator + " -directory " + \
                             rom_directory + " -autostartprgmode 1  ../build/X" + \
                             params[1] + "_" + target + "." + extension

            run_command(option_config, command_string)

        elif target in MAME_TARGETS:
            emulator = MAME_EMULATOR

            if target.startswith("msx"):
                extension = "rom"
                mame_sub_target = "canonv20"
                command_string = option_config.rom_config.mame_path + emulator + " " + mame_sub_target + \
                " -rompath " + option_config.rom_config.mame_rom_path + \
                " -window -uimodekey DEL  -skip_gameinfo  -cart1 ../build/X" + \
                params[1] + "_" + target + "." + extension
                run_command(option_config, command_string)
        else:
            print("'xl run' does not support this target")
            return
