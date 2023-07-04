

# BUILD INSTRUCTIONS

## Build the default target for a given system

In order to build a game or a test, you need to be in the `src` directory.
You  build a project (either a game or a test) for a specific system by either using the `xl build` script or an equivalent `make` command. 
A convenient shortcut is 
### Using `xl build`

I recommend that you use `xl build` as follows:

`xl build [project_name] [optional system_name] [optional number of threads]`

where `[project name]` is either an example, a built-in game or user-defined project.

A convenient shortcut is using just `xl` as follows:

`xl [project_name] [optional system_name] [optional number of threads]`

The `[optional system_name]` parameter is the name of the target. If no target is specified then the native target (the host terminal) is implied.

The `[optional number of threads]` is used to specify the number of threads to use for multiple targets that can be built in parallel.


Built-in games are 

`chase`, `shoot`, `bomber`, `snake`, `horde`. 

Current examples are:

`helloworld`, `text`, `pause`, `background`, `tiles`, `input`, `sounds`, `matrix`, `zombies`, `invaders`.

For the list of supported systems look at 
https://github.com/Fabrizio-Caruso/CROSS-LIB/blob/master/docs/STATUS.md

If no system name is given, a native version of the game or test is built by using `gcc` and `ncurses`.

*Only when using the script*: If you use the script you can pass `games`, `tests` or `all` as `[game_or_test_name]` to build all games, tests or all projects (games and tests) for a specific system or list of systems.

As system you can also pass a group of systems such as: 
- `[compiler name]_targets` to build a given project for all targets compiled by a given compiler (e.g., `cc65_targets`, `z88dk_targets`, `cmoc_targets`, etc.)
- `all` to build a given project on all possible targets (avoid this if you are not sure as it fails if you have not installed *all* compilers).

Examples (`xl build` is shortened to `xl`):
- `xl snake` -> It builds Cross Snake for the native console by using `gcc` and `ncurses`.
- `xl chase` -> It builds Cross Chase for the native console by using `gcc` and `ncurses`.
- `xl bomber atari` -> It builds Cross Bomber for the Atari 8-bit target (by using the appropriate cross-compiler, i.e., CC65)
- `xl snake vic20` -> It builds Cross Snake for the Commodore Vic 20.
- `xl games msx` -> It builds all game projects for the MSX target (by using the appropriate cross-compiler, i.e., the ones in Z88DK).
- `xl bomber cc65_targets` -> It builds Cross Bomber for all targets that use CC65.
- `xl tests c64` -> It builds all tests for the Commodore 64 target
- `xl all coco` -> It builds all projects (games and tests) for the TRS-80 Color Computer (by using the appropriate cross-compiler, i.e., CMOC)


### Using `make`
Using a standard `make` command is possible but you will get fewer options.

For game projects you can use:

`make [system_name] -f ./games/[game_name]/Makefile.[game_name]`

For test projects you can use:
`make [system_name] -f ./tests/[test_name]/Makefile.[test_name]`

Examples for games: 
- `make vic20 -f ./games/horde/Makefile.horde` builds *Cross Horde* for Commodore Vic 20.
- `make c64 -f ./games/chase/Makefile.chase` builds *Cross Chase* for Commodore 64.
- `make nes -f ./games/shoot/Makefile.shoot` builds *Cross Shoot* for the Nintendo NES videogame console.
- `make spectrum -f ./games/bomber/Makefile.bomber` builds *Cross Bomber* for the Sinclair ZX Spectrum.
- `make ti83 -f ./games/chase/Makefile.chase` builds *Cross Chase* for the Texas Instrument TI 83 scientific calculator.
- `make ncurses -f ./games/chase/Makefile.chase` builds *Cross Chase* for all targets by using GCC for the native host console (e.g., CYGWIN, Linux, etc. console).
- `make cc65_targets ./games/chase/Makefile.chase` builds *Cross Chase* for all targets that are built with the CC65 cross-compiler for the MOS 6502-based systems.
- `make cmoc_targets ./games/chase/Makefile.chase` builds *Cross Chase* for all targets that are built with the CMOC cross-compiler for the Motorola 6809-based systems.
- `make z88dk_targets ./games/chase/Makefile.chase` builds *Cross Chase* for all targets that are built with the SCCZ80 and ZSDCC cross-compilers of the Z88DK dev-kit for Zilog 80-based and Intel 8080-based systems.
- `make cc65_targets -f ./games/shoot/Makefile.shoot` builds *Cross Shoot* for all targets that are built with the CC65 cross-compiler for the MOS 6502-based systems.
- `make z88dk_targets -f ./games/bomber/Makefile.bomber` builds *Cross Bomber* for all targets that are built with the SCCZ80 and ZSDCC cross-compilers of the Z88DK dev-kit for Zilog 80-based and Intel 8080-based systems.
- `make lcc1802_targets -f ./games/bomber/Makefile.bomber` builds *Cross Bomber* for all targets that are built with the LCC1802 cross-compiler for the RCA COSMAC 1802-based systems.

Examples for tests:
- `make vic20 -f ./tests/tiles/Makefile.tiles` builds the test `tiles` for the Commodore Vic 20.
- `make cpc -f ./tests/sounds/Makefile.sounds` builds the test `sounds` for the Amstrad CPC computer.
- `make atari -f ./tests/invaders/Makefile.invaders` builds the tests `invaders` for the Atari 8-bit computer.


## Targets built by multiple dev-kits
Some targets, e.g., CP/M and Commodore 128, can be built by two different compilers because they can run different CPU architectures or because we may want to have multiple versions.
In such cases we can specify the CPU in the target name:

Examples using `xl build`
- `xl build chase c128_targets` builds Cross Chase for all Commodore 128 targets for both the MOS 8502 and for the more exotic Zilog 80 non-CP/M mode.
- `xl build chase c128_8502_targets` builds Cross Chase for all Commodore 128 targets for the MOS 8502 architecture in both 40 and 80 column mode.
- `xl build chase c128_z80_targets` builds Cross Chase for all Commodore 128 targets for the Zilog 80 non-CP/M mode in both 40 and 80 column mode.
- `xl build chase cpm_targets` builds Cross Chase for all "generic" CP/M targets for both the Intel 8080 and Zilog 80 architecture.
- `xl build chase cpm_8080_targets` builds Cross Chase for all "generic" CP/M targets for the Intel 8080 architecture (compatible with Zilog 80 systems).
- `xl build chase cpm_z80_targets` builds Cross Chase  for all "generic" CP/M targets for both the Zilog 80 architecture (not compatible with Intel 8080 systems).

Examples using `make`:
- `make c128_targets -f ./games/chase/Makefile.chase` builds Cross Chase for all Commodore 128 targets for both the MOS 8502 and for the more exotic Zilog 80 non-CP/M mode.
- `make c128_8502_targets -f ./games/chase/Makefile.chase` builds Cross Chase for all Commodore 128 targets for the MOS 8502 architecture in both 40 and 80 column mode.
- `make c128_z80_targets -f ./games/chase/Makefile.chase` builds Cross Chase for all Commodore 128 targets for the Zilog 80 non-CP/M mode in both 40 and 80 column mode.
- `make cpm_targets -f ./games/chase/Makefile.chase` builds Cross Chase for all "generic" CP/M targets for both the Intel 8080 and Zilog 80 architecture.
- `make cpm_8080_targets -f ./games/chase/Makefile.chase` builds Cross Chase for all "generic" CP/M targets for the Intel 8080 architecture (compatible with Zilog 80 systems).
- `make cpm_z80_targets -f ./games/chase/Makefile.chase` builds Cross Chase  for all "generic" CP/M targets for both the Zilog 80 architecture (not compatible with Intel 8080 systems).


