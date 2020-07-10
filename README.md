# CROSS CHASE
Cross Lib and Cross Chase

by Fabrizio Caruso (fabrizio_caruso@hotmail.com)

## THE GOAL

This is a personal project whose goal is the creation of:
1. *CrossLib*, a *retro-hardware abstraction layer* for coding "universal" (mostly 8-bit) games. These games are the proof of the flexibility of *CrossLib*.
3. *Cross Chase* is a "universal" 8-bit game that can be compiled for and run
on (nearly) ALL 8-bit computers, game consoles, hand-held game consoles and scientific calculators. 
It can be parametrized in a way to make it run on systems with as little as 3k or 4k of available memory for the code.
4. *Cross Shoot* is a second "universal" 8-bit game that can also be compiled on a multitude of 8-bit computers and consoles. 
It requires more memory than "Cross Chase" but it should be equally universal as long as the required memory is available.


-------------------------------------------

## CROSS CHASE - THE GAME 
The game's goal is to lure the "ghosts" into the mines and possibly kill the "skull" by repeatedly using the gun.
You have to avoid all enemies, the mines, all walls, rockets and chasing missiles.
All flashing items have positive effects and can grant you some special powers. Discover the 10 items.
If you use the gun against the skull and/or the missiles and rockets, you can unlock some special secret items.
There are 20 different levels including 4 boss levels.

You can play the game on-line for different systems:

1. MSX version: https://homebrew.file-hunter.com/index.php?id=crosschase
2. Commodore 264 series (C16/C116/C+4): http://plus4world.powweb.com/play/cross_chase_32k_v0_9
3. Sinclair ZX Spectrum 48K: https://spectrumcomputing.co.uk/playonline.php?eml=1&downid=112056
4. Atari LYNX: https://atarigamer.com/lynx/play/CrossChase/675300242
5. Video Technology Laser 500: https://nippur72.github.io/laser500emu/?restore=false&load=cross_chase


-------------------------------------------

## CROSS SHOOT - THE GAME 
The game's goal is to kill all common enemies or kill all 4 "skulls". 
Common enemies can be killed quickly if pushed against the walls by shooting at them.
The "skulls" and the boss cannot be pushed and are harder to kill.
You have to avoid all enemies, all walls, rockets and chasing missiles.
All flashing items have positive effects and can grant you special powers. 
Your bullets have a limited power that can be improved by catching the multiple times special fire power item. 
You can also fetch the mine item that grants you 3 powerful mines to use against all your enemies.
Discover the 11 items. Some items are secret. In total the game has 12 secrets.
You can unlock the 12 secrets by shooting at the skulls and at the missile and rocket bases in different ways.
The game has 16 different levels including 2 boss levels.


-------------------------------------------


## CURRENTLY SUPPORTED ARCHITECTURES
The main goal is to get the library and game to work on most vintage systems with possibly all 8-bit architectures as well as some 16 and 32-bit architectures.
A partial list of the supported systems is found in: https://github.com/Fabrizio-Caruso/CROSS-CHASE/blob/master/docs/SYSTEMS.md

The most significant supported CPU architecture are described below.

### 8-BIT ARCHITECTURES
- COSMAC RCA 1802 and derivates
- Intel 8080 and derivatives
- MOS 6502 and derivatives
- Motorola 6809 and derivatives
- Zilog 80 and derivatives


### 16-BIT ARCHITECTURES
- Motorola 68000/68008
- Zilog Z8000
- Intel 8088/8086 
- PDP11
- MOS 65816

### 32-BIT ARCHITECTURES
- PowerPC
- Intel 386
- Motorola 68020
- MIPS32r2 (little-endian)

### 32/64-BIT ARCHITECTURE
We also support any current architecture for which GCC can produce a working binary.

-------------------------------------------

## THE TOOL-CHAIN

How is it this done?
This is achieved  by having "Cross Lib" provide APIs for the game code.
The game and library code are  separated: 
- the game code only depends on the APIs interface and 
- the APIs do not depend on the game code. Therefore it must be possible to re-use the same library for other games/programs.

The tool-chain currently supports: *CC65*, *Z88DK* (*SCCZ80* and *ZSDCC*), *CMOC*, *LCC1802*, *ACK*, *XTC68*, *GCC*, *GCC-Z8K*, *GCC-ATARI-ST*, *GCC-AMIGA*, *GCC-TI*.

For more details look at: 
https://github.com/Fabrizio-Caruso/CROSS-CHASE/blob/master/docs/COMPILERS.md

-------------------------------------------

## HOW TO COMPILE CROSS CHASE
In order to compile the game you need to be in a POSIX environment such as Windows+Cygwin, Linux, FreeBSD, MacOS or any Unix-like environment that supports the "make" command.

For more details we refer to: 
https://github.com/Fabrizio-Caruso/CROSS-CHASE/blob/master/docs/PREREQUISITES.md


### Compilation tests

You can test many targets if you have installed `GCC` by simply running `make simple_test`, which will build targets that only depend on `GCC`.

If you want to run multiple tests that depend on several cross-compilers, not including Z88DK, you can run `make no_z88dk_test`.

If you want to run multiple tests that depend on several cross-compilers, including Z88DK, you can run`make test` (very long execution time).

More details on tests are in:  
https://github.com/Fabrizio-Caruso/CROSS-CHASE/blob/master/docs/TESTS.md


### Build the default target for a given system

If you just want to build the game only for a specific system without choosing a specific version, 
you can build the default target for a given system with

`make [system_name]`

For the list of systems either run `make list` or look at 
https://github.com/Fabrizio-Caruso/CROSS-CHASE/blob/master/docs/SYSTEMS.md

Examples: 
- `make c64` builds the default Commodore 64 binary.
- `make nes` builds the default binary for the Nintendo NES videogame console.
- `make ti83` builds the default binary for the Texas Instrument TI 83 scientific calculator.
- `make spectrum` builds the default binary for the Sinclair ZX Spectrum.


### Build all targets for a given system

You can build all targets for a given system with

`make [system_name]_targets`

See the list of systems above or run `make list`.

Examples:
- `make vic20_targets` builds the binaries for the Commodore Vic 20.
- `make spectrum_targets` builds the binaries for the Sinclair ZX Spectrum.


### Build all targets that are built by a given compiler or devkit:

`make [compiler_name]_targets`

You can find the list of compilers and dev-kit if you either run `make help` or look at 
https://github.com/Fabrizio-Caruso/CROSS-CHASE/blob/master/docs/COMPILER_NAMES.md


Exampeles:
- `make gcc_targets` builds all targets by using GCC for the native host console (e.g., CYGWIN, Linux, etc. console).
- `make cc65_targets` builds all targets that are built with the CC65 cross-compiler for the MOS 6502 systems.
- `make z88dk_targets` [EXTREMELY SLOW] builds all targets that are built with the SCCZ80 and ZSDCC cross-compilers of the Z88DK dev-kit for Zilog 80 and Intel 8080 systems.

### Detailed build instructions 

For more details on how to build either run `make help` or look at 
https://github.com/Fabrizio-Caruso/CROSS-CHASE/blob/master/docs/BUILD.md


-------------------------------------------

## HOW TO COMPILE CROSS SHOOT

*Cross Shoot* can be compiled similarly as *Cross Chase* but you need to use the specific Makefile `Makefile.Shoot`.

Exmaples:
- `make c64 -f Makefile.Shoot` builds the default Commodore 64 binary.
- `make vic20 -f Makefile.Shoot` builds the default Commodore Vic 20 binary.
- `make c16 -f Makefile.Shoot` builds the default Commodore 16 binary.
- `make all -f Makefile.Shoot` builds all currently available targets for *Cross Shoot*.


-------------------------------------------

## LOADING THE GAMES
In order to run the game you will have to follow a different procedure depending on whether you want to load it into an emulated system or a real system.

### Loading the game into an emulated vintage system
In most cases loading an executable into an emulator is straightforward. 
For a detailed guide on how to load the game on several emulated systems for which the procedure is not obvious, we refer to: 
https://github.com/Fabrizio-Caruso/CROSS-CHASE/blob/master/docs/HOW_TO_LOAD_THE_GAME.md

### Loading the game into a real vintage system
This depends on the systems and the format used to store the game. For some hints on this take a look at:
https://github.com/Fabrizio-Caruso/CROSS-CHASE/blob/master/docs/HOW_TO_LOAD_THE_GAME_ON_REAL_HARDWARE.md

-------------------------------------------

## THE GAME CODE

The game code is *hardware-agnostic* and has to be as portable as possible.
Therefore the following coding choices and design decisions have been made:
1. ANSI C (for the game logic);
2. strictly separated input/output and hardware-dependent code (in CrossLib) from the game logic;
3. input for keyboard/joystick and output for sound and display are provided by CrossLib

Some target(s) may get specific graphic code with re-defined characters, software/hardware sprites and music/sound effects but the game code is hardware-agnostic.

The game code is in: 
https://github.com/Fabrizio-Caruso/CROSS-CHASE/tree/master/src/chase

-------------------------------------------

## SUPPORTED SYSTEMS 

In principle you can compile the game in turn-based mode with minimal input and output (*stdlib*) for any architecture for which there is an ANSI C capable compiler.
You can also compile the game in action mode with minimal input and output for any architecture for which there an ANCI C compiler with *ncurses* or *conio* libraries.
For most vintage systems, you can build a version of the game with some graphics, sounds and real time input.

For a complete list look at:
https://github.com/Fabrizio-Caruso/CROSS-CHASE/blob/master/docs/SYSTEMS.md


-------------------------------------------

## KNOWN ISSUES

Z80-based targets:
https://docs.google.com/spreadsheets/d/1qo2skBUtCUCAac3knEE2x-bUBdvxEA_6qvElacJ3xNY/edit?usp=sharing

-------------------------------------------


## THE FUTURE

It should be possible to write more universal games using "Cross Lib" such as: https://github.com/Fabrizio-Caruso/CROSS-SHOOT

The main future goals are
- better separation of CrossLib and the game code
- adding more systems and more compilers/dev-kits to support more architectures.

-------------------------------------------

## ADAPTIVE GRAPHICS

The tool-chain and CrossLib will produce a game with simple black and white ASCII graphics and no sound if none of these is available. 
If colors, graphics and sounds are available the tool-chain and CrossLib will produce a game with some simple sound effects and with some possibly colored graphics.

![Vic 20 title](snapshots/vic20_title.jpg)
![ZX81](snapshots/zx81_1.jpg)
![C16 first level](snapshots/c264_level1.jpg)

For more snapshots we refer to: 
https://github.com/Fabrizio-Caruso/CROSS-CHASE/blob/master/docs/SNAPSHOTS.md

-------------------------------------------

## CROSSLIB
CrossLib provides a *retro-hardware abstraction layer* for display, sound, input, etc.

CrossLib code is in: 
https://github.com/Fabrizio-Caruso/CROSS-CHASE/tree/master/src/cross_lib

### Sound
Sound abstraction is achieved by providing common APIs for the (few) sounds that CrossLib provides.

### Input
Input abstraction is also achieved by common APIs that support either keyboard and/or joystick input for each possible target.

### Display
Display abstraction is provided by (at least) two levels of abstraction:
- Common APIs that, for each target, implement one of several graphics modes;
- Generic graphics modes that may be shared among a few targets.

For more details on CrossLib we refer to: 
https://github.com/Fabrizio-Caruso/CROSS-CHASE/blob/master/docs/CROSSLIB.md


-------------------------------------------

## LICENCE

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from
the use of this software.

Permission is granted to anyone to use this software for non-commercial applications, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software in
a product, an acknowledgment in the product documentation would be
appreciated but is not required.

2. Altered source versions must be plainly marked as such, and must not
be misrepresented as being the original software.

3. This notice may not be removed or altered from any source distribution.


