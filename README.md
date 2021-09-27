

# CROSS LIB
![LOGO](snapshots/logo_2.jpg)

*Cross-Lib* and games that use it (e.g., Cross Chase, Cross Shoot, Cross Bomber, Cross Snake, Cross Horde)

by Fabrizio Caruso (fabrizio_caruso@hotmail.com)

The universal *retro-hardware abstraction layer*.

![MSX](snapshots/XSnake_MSX2.png)
![Atari](snapshots/XChase_A8.png)

![CPC](snapshots/XHorde_CPC.png)
![Atari](snapshots/XBomber_A8.png)

![SMS](snapshots/XShoot_sms.png)
![GB](snapshots/XSnake_GB.png)

![Atari](snapshots/XHorde_A8.png)
![SMS](snapshots/XHorde_SMS.png)

-------------------------------------------

## THE GOAL

*Cross-Lib* is a *retro-hardware abstraction layer* for coding "universal" demos, games and programs for hundreds of mostly 8-bit systems (consoles, computers, scientific calculators, hand-held consoles, arcade boards, hacked toy computers, etc.). 

-------------------------------------------

## THE GAMES

These games are the proof of the flexibility of *Cross-Lib*.

Play the games online:

https://github.com/Fabrizio-Caruso/CROSS-LIB/blob/master/docs/GAMES.md

1. *Cross Chase* is somehow similar to *Gnome Robots*. The main difference is that it is a real-time game and that it has several items and power-ups to pick.
2. *Cross Shoot* is a shooter somehow similar to Robotron. It has many items and secrets to discover.
3. *Cross Bomber* is a mini-game and clone of Air Attack (aka Blitz).
4. *Cross Snake* is my personal re-interpretation of Nibbler (arcade game) to which I have added more game elements. It has 50 secrets and several items to discover.
5. *Cross Horde* is a zombie shooter with several different enemies, levels, power-ups and special items to pick.


-------------------------------------------


## CURRENTLY SUPPORTED SYSTEMS AND ARCHITECTURES

Cross-Lib can be used to build games for about 200 different vintage systems (computers, consoles, hand-helds, arcade boards, scientific pocket calculators, etc.). 
A partial list of the supported systems with their status is in: 

https://github.com/Fabrizio-Caruso/CROSS-LIB/blob/master/docs/STATUS.md

The most significant supported 8-bit CPU architectures are:

- COSMAC RCA 1802 and derivates
- Intel 8080 and derivatives
- MOS 6502 and derivatives
- Motorola 6809 and derivatives
- Motorola 6803
- Zilog 80 and derivatives

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
- user-friendly scripts to manage game projects (see the subsequent sections below).

### The compilers

Code written in C with Cross-Lib can be compiled by several different cross-compilers and natively on the PC.

The tool-chain currently supports: *CC65*, *Z88DK* (*SCCZ80* and *ZSDCC*), *CMOC*, *LCC1802*, *ACK*, *XTC68*, *GCC*, *GCC-Z8K*, *GCC-ATARI-ST*, *GCC-AMIGA*, *GCC-TI*.
For a more complete list of supported compilers we refer to:

https://github.com/Fabrizio-Caruso/CROSS-LIB/blob/master/docs/COMPILERS.md

### The build process

When a game is built for a specific target, Cross-Lib will:
1. convert the abstract graphics assets in graphics assets for the given target on the fly;
2. compile the game code (and the target-specific game assets) by using the appropriate compiler;
3. if needed, create a disk/cassette/cartridge image.

-------------------------------------------

## HOW TO START USING CROSS-LIB

Inside the project main directory, you find the `src` and the `build` directories:
- `src` contains the source code and all other files that are necessary to build the games and tests
- `build` is supposed to be empty at first and it is the target directory for the build process.


Remark: In order to use Cross-Lib scripts to build, create or delete game projects you need to be in the `src` directory.

You can use some scripts (and the `make` command) to do different operations on projects (such as create, build, delete, list).

For a description of the script commands and their parameters, take a look at the next sections.

-------------------------------------------

## THE `xl.py` SCRIPT

Inside the `src` directory, you will find the `xl.py` script. 
Make sure that it has execution rights (`chmod +x ./xl.py`) and, for your convenience, that the current directory `.` is included in `$PATH`.
This script can be used to create, build, delete and perform other operations on all Cross-Lib projects (tests, built-in games and custom games). 

`xl.py` is used as follows:
```
xl.py <command> <[optional] parameters>
```

You can display its instructions and some examples by using:
```
xl.py <help> <[optional] command>
```
where commands are `create`,`build`,`delete`,`clean`,`list`,`help`.

## HOW TO COMPILE GAMES AND TESTS

In order to compile the game you need:
- a POSIX environment (e.g., Windows+Cygwin, Linux)
- `make`
- `python` (2.x or 3.x)
- `gcc` and `ncurses` (necessary only for native builds)
- cross-compilers (necessary to build for vintage targets). 

For more details on the prerequisites we refer to: 

https://github.com/Fabrizio-Caruso/CROSS-LIB/blob/master/docs/PREREQUISITES.md

You  build a project (either a game or a test) for a specific system by either using the `build_xl.py` script or an equivalent `make` command. 


### Using `xl.py build` (recommended)

I recommend that you use `xl.py build` as follows:

`xl.py build [game_or_test_name] [optional system_name]`

Examples:
- `xl.py build snake` -> It builds Cross Snake for the native console by using `gcc` and `ncurses`.
- `xl.py build bomber atari` -> It builds Cross Bomber for the Atari 8-bit target (by using the appropriate cross-compiler, i.e., CC65)
- `xl.py build snake vic20` -> It builds Cross Snake for the Commodore Vic 20.
- `xl.py build games msx` -> It builds all game projects for the MSX target (by using the appropriate cross-compiler, i.e., the ones in Z88DK).
- `xl.py build bomber cc65_targets` -> It builds Cross Bomber for all targets that use CC65.
- `xl.py build tests c64` -> It builds all tests for the Commodore 64 target

### Using `make` (NOT recommended)

Using a standard `make` comamnd is possible but you will get fewer options.

For game projects you can use:

`make [system_name] -f ./games/[game_name]/Makefile.[game_name]`

For test projects you can use:

`make [system_name] -f ./tests/[test_name]/Makefile.[test_name]`

For more details and examples on build instructions for both `build_xl.py` and `make` we refer to:

https://github.com/Fabrizio-Caruso/CROSS-LIB/blob/master/docs/BUILD.md

-------------------------------------------
## BUILDING A NEW GAME PROJECT

In order to build a new game project we can use the `create_xl.py` scripts that will create the necessary initial source code files, graphic assets and Makefile inside a folder in the `games` folder.

The script is used as follows:

`xl.py create [game project name] [initial code type]`

where `[initial code type]` can be 
- empty for a helloworld initial code
- `game` for an initial code for a standard game with a main loop and a level loop
- `apis` for an initial code that uses most APIs.

Examples:
- `xl.py create foo` -> It builds a new game project `foo` with a trivial code that initializes sound, input and graphics and just displays `hello world` on the screen.
- `xl.py create bar game` -> It builds a new game project `bar` with code that initializes sound, input and graphics and contains the main loops that may be used in a standard game
- `xl.py create foobar apis` -> It builds a new game project `bar` with code that initializes sound, input and graphics and contains code that shows how to use most APIs

Remark: All binaries will be in the `build` directory (same depth level as `src`).

-------------------------------------------
## CLEANING TEMPORARY FILES AND BINARIES

In order to remove binaries and temporary files that are produced during a build you can se the 'clean_xl' script.

`xl.py clean [optional project name]`

If you provide a project name, then project specific temporary files are removed but *ALL* built binaries are removed anyway.

Examples:
- `xl.py clean` -> It deletes all built binaries and some generic temporary files
- `xl.py clean foo` -> It deletes the same files as `xl.py clean` and also deletes specific temporary files found inside `foo` folder.

-------------------------------------------
## LISTING PROJECTS

You can list all current game and test projects with 

`xl.py list`

which searches both `tests` and `games` folders for projects (both built-in and user-defined) and produces a list with all of them.

--------------------------------------------
## DELETING PROJECTS

Non-built-in game projects can be deleted trhough the `./delete_xl.py` script in a very simple way:

`xl.py delete [game project name]`

Example: 

`xl.py delete foo` -> It removes the `foo` source code, assets and Makefile files

-------------------------------------------
## CROSS-LIB APIs

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


## HARDWARE-AGNOSTIC CODE

Thanks to Cross-Lib APIs, the game code can be *hardware-agnostic*.
Using only Cross-Lib APIs is not enough, though, as Cross-Lib covers targets that have different screen size, screen shape and targets that lack some hardware features such as graphics, colors, sounds.

### ANSI C89 (A sub-set of)
The code has to be compiled by several different compilers. Therefore it should be written in a sub-set of ANSI C89 that is common among all supported compilers.

This sub-set is mostly ANSI C89 without:
- `float` types,
- `struct` copies, assignments, parameters (use pointers to `struct` instead).

Moreover for performance reasons it is better to avoid recursion (as this has an extreme high cost on MOS 6502 targets).

Remark: Modern `//` comments are allowed.

### Screen size
Using Cross-Lib APIs is not enough as the screen size and shape may be different.
So the game code should rely on fractions of the height (`YSize`) and width (`XSize`) of the screen and never use hard-coded sizes.

### Colors
Some targets have no colors. So do not write any logic that relies only on colors or if you do, use conditional directives to implement an alternative for targets with no colors.

### Sounds
Some targets may have no sounds. So do not write any logic that relies only on sounds or if you do, use conditional directives to implement an alternative for targets with no sounds.

### Graphics
Some targets have no graphics. So do not write logic that only depends on the presence of graphics or if you do, use some conditional directives to implement a version with no graphics.

### Tile Shapes
Cross-Lib APIs only support tile-based graphics with pre-defined (at compilation time) tile shapes in order to support all targets. 
Most targets with graphics have 8x8 pixel tiles but some other targets with graphics have different shapes.
So do not implement logic that only assumes 8x8 pixel tiles (e.g. software sprites with pre-shifted tiles). 

### Learn from the built-in games and tests
Cross-Lib comes with games and tests whose code can be used to learn how to code universal games.

The code of the games is in:

https://github.com/Fabrizio-Caruso/CROSS-LIB/tree/master/src/games

The code of some tests is in:

https://github.com/Fabrizio-Caruso/CROSS-LIB/tree/master/src/tests


-------------------------------------------

## ADAPTIVE GRAPHICS

Graphics in Cross-Lib is implemented in a way that can be rendered in the same way on all targets.

### Tile-based 
Graphics in Cross-Lib is tile-based as this is the only possible graphics that can be supported by all targets. 
For targets with graphics, tiles are only defined at compilation time and cannot be re-defined.
So, for targets with graphics, smoothly moving sprites can only be implemented as software sprites through pre-shifted tiles.

### Adaptivity
If colors, graphics and sounds are available on a specific target, the tool-chain will produce a game with possibly some sound effects and colored graphics.
Otherwise Cross-Lib will produce a game with just ASCII graphics or no sound or no colors on a specific target that lacks some hardware feature.

For example for the game Cross Snake you can see how it is rendered on the MSX (graphics, sounds, colors) and on the Game Boy (graphics, sounds but no colors):

![MSX](snapshots/XSnake_MSX2.png)
![GB](snapshots/XSnake_GB.png)

For more snapshots we refer to: 
https://github.com/Fabrizio-Caruso/CROSS-LIB/blob/master/docs/SNAPSHOTS.md



-------------------------------------------


## THE FUTURE

The main future goals are
- improving the exposed APIs 
- supporting more cross-compilers, native compilers, systems and sub-targets
- adding more features to Cross-Lib (e.g., more redefinable tiles, more sound effects, etc.)
- coding more universal games and demos


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


