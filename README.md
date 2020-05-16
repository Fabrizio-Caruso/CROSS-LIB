# CROSS CHASE
Cross Lib and Cross Chase

by Fabrizio Caruso (fabrizio_caruso@hotmail.com)

## THE GOAL

This is a personal project whose goal is the creation of:
1. the universal 8-bit abstraction layer "Cross Lib" for coding universal 8-bit games
2. the "universal" 8-bit game "Cross Chase" that has to be a fun and that should run
on (nearly) ALL 8 bit computers, consoles, handhelds and scientific calculators. 
The game will be the proof of concept of how flexible the library is.

## THE GAME 
The game's goal is to lure the "ghosts" into the mines and possibly kill the "skull" by repeatedly using the gun.
You have to avoid all enemies, the mines, all walls, rockets and chasing missiles.
All flashing items have positive effects and can grant you some special powers. Discover the 10 items.
If you use the gun against the skull and/or the missiles and rockets, you can unlock some special secret items.

You can play the game on-line for different targets:

1. MSX version: https://homebrew.file-hunter.com/index.php?id=crosschase
2. Commodore 264 series (C16/C116/C+4): http://plus4world.powweb.com/play/cross_chase_32k_v0_9
3. Sinclair ZX Spectrum 48K: https://spectrumcomputing.co.uk/playonline.php?eml=1&downid=112056
4. Atari LYNX: https://atarigamer.com/lynx/play/CrossChase/675300242

## CURRENTLY SUPPORTED ARCHITECTURES
The main goal is to get the library and game to work on most 8-bit architectures but other systems are also considered.

### 8-BIT ARCHITECTURES
- COSMAC RCA 1802
- Intel 8080 and its derivatives
- MOS 6502 and its derivatives
- Motorola 6809 and its derivatives
- Zilog 80 and its derivatives


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


## THE TOOL-CHAIN

How is it this done?
This is achieved  by having "Cross Lib" provide APIs for the game code.
The game and library code are  separated: 
- the game code only depends on the APIs interface and 
- the APIs do not depend on the game code. Therefore it must be possible to re-use the same library for other games/programs.

The program is written in ANSI C and currently compiled with 
- CC65 for all MOS 6502 targets (http://www.cc65.org/), 
- SCCZ80 (from Z88DK) for most Zilog 80 and Intel 8080 targets (https://www.z88dk.org/), 
- ZSDCC (from Z88DK) for some Zilog 80 and Intel 8080 targets (https://www.z88dk.org/),
- CMOC for Motorola 6809 targets (https://perso.b2b2c.ca/~sarrazip/dev/cmoc.html),
- LCC1802 for COSMAC RCA 1802 targets (https://sites.google.com/site/lcc1802/downloads),
- ACK for PC 8088/8086, CP/M Intel 8080, Linux 386/68K/PPC/MIPS, PDP11 (https://github.com/davidgiven/ack),
- XTC68 (C68) for the Motorola 68008-based Sinclair QL (https://github.com/stronnag/xtc68),
- Z8KGCC for the Zilog Z8000-based Olivetti M20 series (http://www.z80ne.com/m20/sections/download/z8kgcc/z8kgcc.html),
- "GCC for Atari ST" (http://vincent.riviere.free.fr/soft/m68k-atari-mint/),
- "GCC for Amiga" (https://github.com/bebbo/amiga-gcc),
- "GCC for TI" for the Texas TI99/4a (http://atariage.com/forums/topic/164295-gcc-for-the-ti/page-6),
- GCC for modern 32/64-bit PC under any operating systems 

Previous versions used 
- WinCMOC for 6809 targets  (https://sourceforge.net/projects/cmoc-win32/).
WinCMOC has been abandoned because it uses an old CMOC version that has broken pointer to functions and because it is no longer maintained by its author. 

For other targets, the following compilers and dev-kits may be included in future versions of the tool-chain:
- QDOS-GCC for the Sinclair QL (http://morloch.hd.free.fr/qdos/qdosgcc.html)
- SDCC for Z80 and other targets (http://sdcc.sourceforge.net/),
- PVSNESLIB for the Super Nintendo Entertainment System (https://github.com/alekmaul/pvsneslib),
- amiga-os-toolchain for all Amigas (https://github.com/cahirwpz/amigaos-cross-toolchain),
- GCC6809 for other 6809 targets (https://sourceforge.net/projects/freewpc/files/GCC-6809/),
- TIGCC for 68k-based TI calculators (http://tigcc.ticalc.org/).

The tool-chain also uses some third-party target-specific tools to create usable cassette/disk/rom images.
This is documented in https://github.com/Fabrizio-Caruso/CROSS-CHASE/blob/master/tools/README.txt

## HOW TO COMPILE THE GAME
In order to compile the game you need to be in a POSIX environment such as Windows+Cygwin, Linux, FreeBSD, MacOS or any Unix-like environment that supports the "make" command.

### Prerequisites
- [Non-UNIX only] Install a POSIX environment if necessary, e.g., Cygwin.
- Install `make` in your environment.
- Install the required cross-compilers (see list of supported cross-compilers above).
- [Only some compilers] Install the additional requirements for a given cross-compiler, e.g., CMOC and LCC1802 require a separate linker, some GCC targets require "ncurses".
- [Only some compilers] Make sure they are either installed as described in the Makefile variables: `Z88DK_PATH`, `CC65_PATH`, `LCC1802_PATH`, etc. or set these variables accordingly.
- [Z88DK-only] Select the optimization level in Makefile by setting the variables: `SCZZ80_OPTS` (`-O3` for highest optimizations), `ZSDCC_OPTS` (`-SO3` for high optimizations; `-SO3 --max-allocs-per-node200000` for highest and extremely slow optimizations).

# BUILD INSTRUCTIONS

## Build all targets for a given system

You can build all targets for a given system with

`make [system_name]_targets`

For the list of systems `make list`

## Build all targets that are built by a given compiler or devkit:

`make [compiler_name]_targets`

For the list of compilers and dev-kits look at the README.md file.

## Detailed build instructions with examples

https://github.com/Fabrizio-Caruso/CROSS-CHASE/blob/master/docs/BUILD.md

## THE GAME CODE

The game should be as portable as possible.
Therefore the following coding choices and design rules are applied:
1. ANSI C (for the game logic);
2. strictly separated input/output and hardware-dependent code from the game logic;
3. input for keyboard/joystick and output for sound and display are provided by CrossLib

Some target(s) may get specific graphic code with re-defined characters, software/hardware sprites and music/sound effects.

## SUPPORTED TARGETS 

(as of 2020/05/04)

In principle you can compile the game with minimal input and output for any recent or old/ancient architecture for which there is an ANSI C capable compiler.
For most vintage targets, you can build a version of the game with some graphics, sounds and real time input.

For a complete list look at:
https://github.com/Fabrizio-Caruso/CROSS-CHASE/blob/master/docs/TARGETS.md


-------------------------------------------
STATUS - ISSUES

Z80-based targets:
https://docs.google.com/spreadsheets/d/1qo2skBUtCUCAac3knEE2x-bUBdvxEA_6qvElacJ3xNY/edit?usp=sharing

-------------------------------------------


## THE FUTURE

It should be possible to write more universal games using "Cross Lib".

The future main features of "Cross Lib":
- better separation and dependences with the game 
- better support multi-target input/output


## ADAPTIVE GRAPHICS

The tool-chain and CrossLib will produce a game with simple black and white ASCII graphics and no sound if none of these is available. 
If colors, graphics and sounds are available the tool-chain and CrossLib will produce a game with some simple sound effects and with some possibly colored graphics.

![Atari 800](snapshots/atari800.jpg)
![Spectrum 48k](snapshots/spectrum.jpg)
![Vic 20](snapshots/vic20_1.jpg)
![MSX](snapshots/msx.jpg)
![Oric 1/Atmos](snapshots/Oric.jpg)
![Amstrad CPC](snapshots/cpc.jpg)
![ZX81](snapshots/zx81_1.jpg)
![title](snapshots/title.jpg)
![zx81_title](snapshots/zx81_title.jpg)
![Vic 20 title](snapshots/vic20_title.jpg)
![C16 title](snapshots/c264_title.jpg)
![C16 hints](snapshots/c264_hints.jpg)
![C16 first level](snapshots/c264_level1.jpg)


## CROSSLIB DETAILS
CrossLib provides an abstraction layer for sound, input, display, etc.

### Sound
Sound abstraction is achieved by providing common APIs for the (few) sounds that CrossLib provides.

### Input
Input abstraction is also achived by common APIs that support either keyboard and/or joystick input for each possible target.

### Display
Display abstaction is provided by (at least) two levels of abstraction:
- Common APIs that, for each target, implement one of several graphics modes;
- Generic graphics modes that may be shared among a few targets.

So, for each target, at least one of the following modes has to be implemented:
1. memory-mapped graphics (display can be implemented by writing bytes into video memory)
2. conio-like (display is performed by either direct conio APIs or a wrapper that provides conio APIs, e.g., ADM3/A, VT52, VT100, etc.)
3. TGI (CC65-specific graphics APIs)
4. Z88DK Sprites (Z88DK-specific sprite APIs)
5. CPCRSLIB (Amstrad CPC-specific graphics APIs)
6. bit-mapped (display is implemented by writing into video memory and each bytes corresponds to one or more pixels)
7. buffered (stdlib-only "graphics" for targets with just stdlib support)

A graphic mode may implement the concrete graphics by either calling low level graphics instructions (e.g., writing into video-memory) or by calling higher level instructions (e.g., using control codes).

For example for a CPM target with ADM3/A control codes, we use
1. CrossLib APIs that call
2. conio.h APIs that call
3. ADM3/A control codes

For a target with direct or indirect video memory (e.g., Commodore 64, MSX, etc.), one may use:
1. CrossLib APIs that implement
2. Direct (or indirect) writes into video memory

Remark: For some targets multiple implementations are possible (C64 may use conio APIs as provided by C65, memory-mapped graphics, TGI APIs)


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


