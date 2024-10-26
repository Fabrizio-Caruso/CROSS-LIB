

# CROSS LIB
![LOGO](snapshots/CrossLibLogoByHamletShorter.jpg)

*Cross-Lib* and games that use it (e.g., Chase, Shoot, Bomber, Snake, Horde, Verbix, Shuriken, Stinger, Trex)

by Fabrizio Caruso (fabrizio_caruso@hotmail.com)

The universal *retro-hardware abstraction layer*: Use the **very same code** for about 200 vintage computers, consoles, calculators and other devices.

Read the <a href="https://github.com/Fabrizio-Caruso/CROSS-LIB/blob/master/docs/DISCLAIMER.md"><b>Disclaimer.</b></a>

<br />

The logo was designed by *Hamlet*.




![MSX](snapshots/XSnake_MSX2.png)
![Atari](snapshots/XChase_A8.png)

![CPC](snapshots/XHorde_CPC.png)
![Atari](snapshots/XBomber_A8.png)

![SMS](snapshots/XShoot_sms.png)
![GB](snapshots/XSnake_GB.png)

![Atari](snapshots/XHorde_A8.png)
![SMS](snapshots/XHorde_SMS.png)

![Vic20](snapshots/XVerbix_Vic20_gameplay.jpg)
![Vic20](snapshots/XVerbix_Vic20_title.jpg)
-------------------------------------------

## THE GOAL

*Cross-Lib* is a *retro-hardware abstraction layer* for coding "universal" games with **the very same code** for hundreds of mostly 8-bit systems (consoles, computers, scientific calculators, hand-held consoles, arcade boards, hacked toy computers, etc.).

*Cross-Lib* is a *WORA* (Write Once Run Anywhere) framework for vintage systems, i.e., you code a game **once** in an abstract hardware-agnostic way and *Cross-Lib* produces the very same game for about 200 vintage systems.

-------------------------------------------

## ARTICLE ABOUT CROSS-LIB
Article in English (updated and modified version of an article published in C.A.L.L. Apple Volume 3 Number 1 (January 2023):
https://github.com/Fabrizio-Caruso/CROSS-LIB/blob/master/docs/EnglishArticle.md

Article in French (first published in the French magazine "Programmez! Hors série #6") that describes Cross-Lib in details.
https://github.com/Fabrizio-Caruso/CROSS-LIB/blob/master/docs/articles/CROSS_LIB_PUBLIC.pdf

-------------------------------------------


## THE GAMES

These games are the proof of the flexibility of *Cross-Lib*.

Play the games online:

https://github.com/Fabrizio-Caruso/CROSS-LIB/blob/master/docs/GAMES.md

1. <a href="https://github.com/Fabrizio-Caruso/CROSS-LIB/blob/master/docs/GAMES.md#trex"><b>Trex</b></a> is a mini-game and an obvious clone of off-line Chrome T-Rex game.
2. <a href="https://github.com/Fabrizio-Caruso/CROSS-LIB/blob/master/docs/GAMES.md#stinger"><b>Stinger</b></a> is a tank-shooter with several enemies, levels, power-ups and items to pick.
3. <a href="https://github.com/Fabrizio-Caruso/CROSS-LIB/blob/master/docs/GAMES.md#shuriken"><b>Shuriken</b></a> is somehow inspired by Pac-man and Pengo.
4. <a href="https://github.com/Fabrizio-Caruso/CROSS-LIB/blob/master/docs/GAMES.md#verbix"><b>Verbix</b></a> is a Scrabble-like game that looks like Tetrix with letters.
5. <a href="https://github.com/Fabrizio-Caruso/CROSS-LIB/blob/master/docs/GAMES.md#horde"><b>Horde</b></a> is a zombie shooter with several different enemies, levels, power-ups and special items to pick.
6. <a href="https://github.com/Fabrizio-Caruso/CROSS-LIB/blob/master/docs/GAMES.md#snake"><b>Snake</b></a> is my personal re-interpretation of Nibbler (arcade game) to which I have added more game elements
7. <a href="https://github.com/Fabrizio-Caruso/CROSS-LIB/blob/master/docs/GAMES.md#bomber"><b>Bomber</b></a> is a mini-game and clone of Air Attack (aka Blitz).
8. <a href="https://github.com/Fabrizio-Caruso/CROSS-LIB/blob/master/docs/GAMES.md#shoot"><b>Shoot</b></a> is a shooter somehow similar to Robotron. It has many items and secrets to discover.
9. <a href="https://github.com/Fabrizio-Caruso/CROSS-LIB/blob/master/docs/GAMES.md#chase"><b>Chase</b></a> is somehow similar to *Gnome Robots* but in real-time game and with several items and power-ups.

-------------------------------------------


## CURRENTLY SUPPORTED SYSTEMS AND ARCHITECTURES

Cross-Lib can be used to build games for about 200 different vintage systems (computers, consoles, hand-helds, arcade boards, scientific pocket calculators, etc.). 
A partial list of the supported systems with their status is in: 

https://github.com/Fabrizio-Caruso/CROSS-LIB/blob/master/docs/STATUS.md

The most significant supported vintage CPU architectures are:

- COSMAC RCA 1802 
- Intel 8080 
- MOS 6502 
- Motorola 6803 
- Motorola 6809
- TMS 9900
- Zilog 80 

Cross-Lib has also some experimental support for vintage 16-bit and 32-bit systems and for the native PC console.

For a more complete list of architectures you can look at:

https://github.com/Fabrizio-Caruso/CROSS-LIB/blob/master/docs/ARCHITECTURES.md

-------------------------------------------

## THE TOOL-CHAIN

Cross-Lib provides a full tool-chain to build universal vintage 8-bit games. 

### Cross-Lib components

Cross-Lib provides:
- a library with hardware-agnostic APIs for game development;
- scripts that convert automatically compiler-agnostic graphics assets to target-specific graphics assets formats;
- makefiles that handle the build process;
- the "user-friendly" script `xl` to manage game projects (see the subsequent sections below).



### The build process

When a game is built for a specific target, the Cross-Lib tool-chain will automatically:
1. if needed, convert the abstract graphics assets in graphics assets for the given target on the fly;
2. compile the game code (and the target-specific game assets) by using the appropriate compiler;
3. if needed, whenever possible, create a disk/cassette/cartridge image.

In most cases the user will just have to
- code in C a game that uses Cross-Lib APIs;
- draw the graphics assets;
- run the `xl` script to create, build and test the game.

![User](snapshots/User2.bmp)

The `xl` script will trigger the full build process which will produce the target specific version of the project 
or massively build the same project for several targets (or even build several projects for one or more targets).
For example for the build of a project for the sole GameBoy target we may represent 
the interactions of various Cross-Lib components with this diagram:

![User](snapshots/CrossLibFlow8.png)


### The compilers

Code written in C with Cross-Lib can be compiled by several different cross-compilers and natively on the PC.

The tool-chain currently supports: `CC65`, `Z88DK` (`SCCZ80` and `ZSDCC`), `CMOC`, `LCC1802`, `ACK`, `XTC68`, `GCC`, `GCC-Z8K`, `GCC-ATARI-ST`, `GCC-AMIGA`, `GCC-TI`, `VBCC`, `CC6303`.
For more details on the supported compilers we refer to:

https://github.com/Fabrizio-Caruso/CROSS-LIB/blob/master/docs/COMPILERS.md

Thanks to Alex Thissen, a version of Cross-Lib with (Z88DK, CC65, CMOC and native compilers) is available as a Docker image (to be built by using the provided Dockerfile) with a devcontainer configuration.

-------------------------------------------

## INSTALLATION

The installation procedure is not strictly necessary because Cross-Lib can be used through GitHub Codespaces on any browser.
People can also use a pre-build docker image, a dockerfile to build an image or just clone this repository and install all the dependencies (compilers and a few tools).

For the details go here:

https://github.com/Fabrizio-Caruso/CROSS-LIB/blob/master/docs/INSTALL.md

-------------------------------------------

## HOW TO START

Inside the project main directory, you find the `src` and the `build` directories:
- `src` contains the source code and all other files that are necessary to build the games and tests
- `build` is supposed to be empty at first and it is the target directory for the build process.

First of all you need to be in the `src` directory.
From `src` you can use the `xl` script (or the `make` command) to do different operations on projects.

For a description of the `xl` script and its parameters, take a look at the next sections.

-------------------------------------------

## THE `xl` SCRIPT

The script `xl` is found inside the `src` directory and it is meant to be used from `src`.
Make sure that it has execution rights (`chmod +x ./xl`) and, for your convenience, that the current directory `.` is included in `$PATH`.
Its name stands obviously for `Cross-Lib` as it can execute most of the operations people may want to do with Cross-Lib.
It can be used to create, build, delete and perform other operations on all Cross-Lib projects (tests, built-in games and custom games). 

`xl` is used as follows:
```
xl <command> <[optional] parameters>
```

You can display its instructions and some examples by using:
```
xl help <[optional] command>
```


You can build games and examples with :

`xl [game_or_test_name] [optional system_name]`

For example `xl snake vic20` builds the game snake for the Commodore Vic 20 +16K.

-------------------------------------------

## HOW TO USE THE `xl` SCRIPT 

https://github.com/Fabrizio-Caruso/CROSS-LIB/blob/master/docs/XL.md

-------------------------------------------

## HOW TO CODE HARDWARE-AGNOSTIC CODE WITH CROSS-LIB

https://github.com/Fabrizio-Caruso/CROSS-LIB/blob/master/docs/HARDWARE_AGNOSTIC_CODE.md

-------------------------------------------
## CROSS-LIB APIS

Cross-Lib provides several APIs that allow to code games in a hardware-agnostic way for all supported targets.
The currently available APIs are described at

https://github.com/Fabrizio-Caruso/CROSS-LIB/blob/master/docs/CROSS_LIB_APIS.md


-------------------------------------------

## LOADING THE GAMES
In order to run the game you will have to follow a different procedure depending on whether you want to load it into an emulated system or a real system.

### Loading the game into an emulated vintage system
In most cases loading an executable into an emulator is straightforward. 
For a detailed guide on how to load the game on several emulated systems for which the procedure is not obvious, we refer to: 

https://github.com/Fabrizio-Caruso/CROSS-LIB/blob/master/docs/HOW_TO_LOAD_THE_GAME.md

### Loading the game into a real vintage system
This depends on the systems and the format used to store the game. For some hints on this take a look at:

https://github.com/Fabrizio-Caruso/CROSS-LIB/blob/master/docs/HOW_TO_LOAD_THE_GAME_ON_REAL_HARDWARE.md



-------------------------------------------

## ACKNOWLEDGEMENTS

Cross-Lib would not exist without the cross-compilers and the support of their developers and the support of some people who have helped me handle the input/output of some targets.

A partial list of the people who have helped in found in: https://github.com/Fabrizio-Caruso/CROSS-LIB/blob/master/docs/CREDITS.txt

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

For example I do not authorize selling hardware, software, firmware or any other product that contains any part of Cross-Lib and/or its games (e.g., pre-loaded disks, tapes, cartridges, etc.).
On the other hand I authorize people to load the games in the most convenient way for their personal use.

