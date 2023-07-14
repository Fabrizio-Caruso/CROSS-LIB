# CROSS-LIB
by Fabrizio Caruso

## INTRODUCTION

The goal of this article is to present Cross-Lib, briefly show how to use it and present the 6 "universal" games I have written with it.
By "universal" here I mean that these games will run on about 200 different systems (consoles, computers, arcade boards, pocket calculators, etc.) despite using the very same game code.
For more details we refer to the GitHub page: http://www.github.com/Fabrizio-Caruso/CROSS-LIB/



## WHAT IS CROSS-LIB?

Cross-Lib (https://github.com/Fabrizio-Caruso/CROSS-LIB/) is a free open source tool for retro-coding for a multitude of 8-bit and vintage systems from the ‘80s (computers, consoles, pocket calculators, arcade boards, etc.) 
such as Apple //, all 8-bit Commodore computers, Atari 800, Amstrad CPC, Sinclair ZX 81, Sinclair ZX Spectrum, TRS-80 CoCo, Dragon 32, TRS-80 MC-10, Thomson Mo5 and To7, MSX, Oric-1, Oric Atmos, BBC Micro, etc. as well as consoles such as GameBoy, Sega Master System, GameGear, Nintendo NES, etc. 
and other devices such as Texas Instruments pocket calculators. The main CPU architectures are Zilog 80, MOS 6502, Intel 8080, Motorola 6803, Motorola 6809 and RCA 1802. 
Cross-Lib also supports several lesser known systems such as the Philips VG-5000, the Jupiter Ace and several exotic and rare systems such as the yugoslav Galaksija, the Hungarian Homlab-2, 
several computers from the Eastern German Robotron series such as the Robotron KC 85, etc. 
A large, yet partial list of the supported systems is at the page: 
https://github.com/Fabrizio-Caruso/CROSS-LIB/blob/master/docs/STATUS.md. 

Cross-Lib has two main components:

1. A "universal" C library (e.g., a collection of C APIs mostly for input/output);
2. A Python script to manage game projects (e.g., to build, create, clone, etc.) and manipulate graphics assets (e.g., to create, import or rip assets)

The main goal of Cross-Lib is to provide a *hardware abstraction layer* for hundreds of vintage systems so that the *very same* game code can be used without any adaptation for all its supported systems.

Several articles have been published on Cross-Lib ([rg], [prog]) and its games ([ceo] and, in by this very magazine, [app]).



## CROSS-LIB VS OTHER DEV-KITS

Historically, 8-bit systems were programmed directly on the system and mostly in Assembly, or for simpler programs in interpreted BASIC.

Today cross-development simplifies developers’ tasks a lot (modern editors, simplified debugging, efficient and quick compilation, etc.) and can be done in C.



The problem with the portability of C code comes from the diversity of the machines and from the fact that ANSI C does not provide a standard library for graphics, sounds, input, etc.
A partial solution is already provided by some dev-kits such as CC65 (for MOS 6502), Z88DK (for the Zilog 80 and Intel 8080), LCC1802 (for the RCA 1802), CMOC (for the Motorola 6890), etc. 
By using one of these dev-kits, we can exploit their cross-compatible libraries for input and output.
On the other hand:
1. Each dev-kit only covers a subset of all 8-bit architectures.
2. Even within a given dev-kit, its libraries do not necessarily provide the very same input/output APIs across the entirety of its supported targets.



Cross-Lib improves on these points and lets you do things that you cannot do with other tools:
1. cover many more vintage 8-bit architectures, including very exotic ones;
2. writing 100% WORA code, i.e., "write once, run anywhere" (after re-compilation);
3. using WORA graphics resources, i.e., define abstract graphics assets for several totally different systems;
4. using a simplified tool-chain to create, compile and test the games.



The closest dev-kits to Cross-Lib with respect to these points are 
- 8-bit-Unity [un8] (meant to be used with CC65), which does incredible things but only for very few MOS 6502-based systems;
- Z88DK, which has huge libraries but only covers Zilog 80 and Intel 8080 systems.



On the other hand Cross-Lib is not a compiler. It uses several compilers to build the binary files (executable files, disk images, tape images, cartridge images, etc.) for retro-consoles and retro-computers.



By providing an abstraction layer for all 8-bit systems, Cross-Lib makes some compromises with respect to what we could do if we were coding only for a specific system.
In particular, graphics is somehow limited by what is possible on all systems. Sound effects are even more limited.





## COMPILERS

Cross-Lib is supposed to be used with C compilers but which ones? 

A native compiler (for example GCC) can be used to build native binaries that can be run, for instance, under Windows. 
Today the support for native compilation is limited to the text terminal, which means no graphics other than ASCII on the host machine.



Currently Cross-Lib fully supports the following cross-compilers and dev-kits with graphics and sound effects for most of its targets:
- CC65 [cc65] for most systems based on the MOS 6502 architecture; 
- Z88DK [z88dk] for systems based on the Zilog 80 and Intel 8080 architectures;
- CMOC [cmoc] for systems that use the Motorola 6809;
- LCC1802 [lcc1802] for systems that use the COSMAC RCA 1802 ;
- GCC for TI [ti] for the Texas Instruments TI99/4A based on the 16-bit TMS9900.



Cross-Lib also supports other compilers (currently with limited or absent graphics and sound effects) such as 
- ACK [ack] for 8088/8086, Intel 8080 CP/M, 386/68K/PPC/MIPS Linux and PDP11, 
- XTC68 [xtc68] for the Sinclair QL, 
- VBCC [vbcc] for the BBC Micro, BBC Master, the Amiga and others, 
- CC6303 [cc6303] for the Motorola 6803; 
- several versions of modded GCC to target vintage computers (Atari ST [st], Olivetti M20 [m20]).



Cross-Lib supports a large sub-set of ANSI C89 because most available cross-compilers for 8-bit architectures only implement a sub-set of ANSI C89.
Therefore we write in C (a sub-set of ANSI C C89) with Cross-Lib APIs for graphics, sounds, input.
In particular we must avoid:
- "float" and "double" types, 
- copies and parameters by value of "struct" objects,
- any use of the heap.


For hints on how to code efficiently for 8-bit systems we refer to [effc] and for specific hints on how to code with these compilers we refer to [c8b].





## INSTALLATION

Cross-Lib itself does not require any real installation. 
It is enough to download the repository files (from the GitHub page) or to clone the repository with:
git clone https://www.github.com/Fabrizio-Caruso/CROSS-LIB/



On the other hand it requires:
- a POSIX environment (e.g., Windows with Cygwin, Linux, FreeBSD, etc.);
- Make (GNU Make);
- Python 3.* [optional but necessary to simplify many operations];
- GCC compiler with ncurses libraries [optional];
- any compiler necessary to build for a desired target architecture (e.g., CC65, Z88DK, etc.) 
- JAVA (only to be disk images for the Apple //).


Once you have Cross-Lib with its dependencies in a POSIX environment, you should move to the "src" directory.
Inside "src" you will find the "xl" Python script and the following directories 
- "cross-lib", which contains the library code;
- "games" with built-in games;
- "examples" with built-in examples;
- "projects" with user-defined projects.





If you have correctly installed just Makefile, Python and (native) GCC in a POSIX environment, it should already be possible to minimally use Cross-Lib to build the Chase game by running the following command inside the "src" directory (you will have to make sure that the "." directory is in your PATH environment variable otherwise you will have to explicitly use "./xl" instead of "xl"):

"xl chase gcc", which will build the game Chase in turn-based mode for the native console.





## HOW TO USE THE XL SCRIPT

If just one of the supported compilers is installed, Cross-Lib can be used with the "xl" Python script inside the "src" directory.
If GCC with ncurses is installed, a native version of the games and examples can be built with:

"xl build \<project name\>" or just "xl \<project name\>"

For example 

"xl snake" 

builds the "snake" game for the native console.



If a cross compiler (CC65, Z88DK, CMOC, CC6303, LCC1802, etc) is installed, the games and examples can be built for the corresponding systems.

For example, if CC65 is installed, 

"xl horde apple2" 

builds the "horde" game for the Apple // with 6-color HGR graphics. 



The "xl" script takes several commands. In particular you can see a list of the available commands with "xl commands".

You can get an explanation of each command with examples with 

"xl help \<command\>".



If we want to create a new project, we need to use 

"xl create \<project name\>", 

which will create a project with a trivial "hello world" example inside the "projects" directory.

We can also start with a template code for a game if we pass the additional parameter "game": 

"xl create \<project name\> game".



 

## HELLO WORLD

Coding with Cross-Lib is like normal coding in C (with ANSI C89) with the only addition that all input/output is done through Cross-Lib APIs.

Cross-Lib comes with many examples in the src/examples/ directory as well as a few games in the src/games directory.
Both of which can be used to learn how to code with Cross-Lib.
We could start to look at src/examples/helloworld/, whose main.c files contains: 


```
#include "cross_lib.h"

int main(void)
{        
    _XL_INIT_GRAPHICS();
    _XL_CLEAR_SCREEN();
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT_CENTERED("HELLO WORLD");
    while(1){};
    return EXIT_SUCCESS;
}
```


We remark that all Cross-Lib specific functions start with the _XL_ prefix: 

1. _XL_INIT_GRAPHICS() required to initialize the display ;

2. _XL_CLEAR_SCREEN()  which cleans the content of the screen ;

3. _XL_SET_TEXT_COLOR(_XL_WHITE) which defines the color to be used by the text and characters ;

4. _XL_PRINT_CENTERED("HELLO WORLD") which displays "HELLO WORLD" on the center of the screen.



If we want to use one of the examples or games as a starting point we can clone the project with:

xl clone \<existing project to clone\> \<new project name\>



Or we can start from (almost) scratch with

xl create \<new project name\>



Once we have a new project, we can build it with

xl \<project\> \<target\>



or



xl \<project\>

if we want to build the project for the native terminal.



We can run the project for the native terminal with:

xl run \<project\>



For example we could run these commands:

xl clone helloworld myhelloworld [to clone helloworld and create myhellworld]

We modify the project myhelloworld (found inside ./src/projects/myhelloworld/)

xl build myhelloworld (or equivalently xl myhelloworld) [to build a native binary of the new project]

xl run myhelloworld [to run the new project]

xl build myhelloworld c64 [to build the same project for the Commodore 64]





## CODING WITH ABSTRACTIONS



In order to code with Cross-Lib for all targets at the same time, Cross-Lib uses several abstractions.



Graphics is only tile-based. In the code, tiles are abstract as their exact shape will depend on the target type.
The game code only sees tiles as macros _TILE_0, _TILE_1, _TILE_2, etc.

Each concrete target will interpret tiles in different ways depending on whether the target can or cannot assign free shapes to tiles and 
on the number of pixels that define tiles on that system.


Sound is currently only limited to few sound effects that are rendered somehow similarly across multiple targets.



Input is limited to 4 directions and a single fire button. On each target this will be mapped to either 5 keys (usually I J K L SPACE)
or the main joystick/controller/gamepad.



Running "xl manual" will output a manual with all commands.





## MAIN DEVELOPMENT FLOW



Producing "universal" games with Cross-Lib for the developer boils down to these main steps:

- Creating or cloning a project (with xl create or xl clone);

- Writing the abstract game code in ANSI C with Cross-Lib APIs for input/output;

- Manually creating or automatically importing abstract assets (with commands such as e.g., xl shapes, xl import, xl rip, xl tile);

- Running the xl script to build the project for one or multiple targets (with xl build).



The "xl build \<project\> \<target\>" command (or its equivalent short form "xl \<project\> \<target\>") 

executes one or several Makefile targets, which generally:

- generate a concrete asset file from the abstract asset files

- compile the source code (game code and target-specific portions of Cross-Lib library);

- include or link the asset files with the compiled source code;

- if necessary a tool is used to create a usable disk/floppy/rom/cartridge image (this is the case for the Apple // for which disk images are generated).



[INSERT FLOW PICTURE HERE]





## GRAPHICS



Graphics is performed by displaying "tiles", whose shape can be defined. The number of tiles is currently limited to 27 but future versions of Cross-Lib will overcome this constraint.
We can also display "characters" (letters, digits and the space character) but their shape is (currently) fixed.
Characters and tiles have the same size. Characters are used for the text, whereas tiles are used for graphics.


ASCII-only targets will be able to only assign ASCII shapes to tiles, whereas targets with real graphics will assign a "freely defined" shape to tiles.



Currently each tile only has one tile-specific foreground color and a common background color.
The definition of their shapes is not done in the C code.
Instead it is done inside the \<project\>/tiles directory, which contains sub-directories with files with tile definitions for the ASCII case and 
for the graphics-enabled targets, e.g., 8x8 (for most targets), 7x8 (for the Apple //), 6x8 (for the Oric series and NTSC COMX-35), etc... 



In the aforementioned files the shapes are described by either ASCII codes for ASCII-only targets or sequences of bytes for graphics-enabled targets.

These files can be:

1. edited manually or 

2. automatically imported from several programs such as CharPad [chPad] (with "xl import \<file\> \<project\>") or 

3. automatically ripped from BASIC or Assembly listings (with "xl rip \<file\> \<project\>") or

4. generated by some simple "pictorial" text files with drawings made of "#" and "." characters 

(with "xl shapes \<project\> \<x_size\> \<y_size\> or "xl tile \<file\> \<project\> \<tile number\>".



You can see how to use these commands by running "xl help \<command\>".



In the game code tile graphics is primarily performed by invoking these functions (to display or delete tiles):

void _XL_DRAW_TILE(uint8_t x, uint8_t y, uint8_t tile_number, uint8_t tile_color);

void _XL_DELETE(uint8_t x, uint8_t y);





## SOUND



Sound is even more limited than graphics and it is restricted to a few predefined effects.

The sound commands have this form:

void _XL_\<effect name\>_SOUND(void);



For example:

XL_EXPLOSION_SOUND() produces an explosion sound (or something vaguely similar to it depending on the target)




## INPUT


Input is limited to four directions and a single fire button.

The primary input APIs are:

uint8_t _XL_INPUT(void);

uint8_t _XL_LEFT(uint8_t input);

uint8_t _XL_RIGHT(uint8_t input);

uint8_t _XL_UP(uint8_t input);

uint8_t _XL_DOWN(uint8_t input);

uint8_t _XL_FIRE(uint8_t input);



where _XL_INPUT polls the input and the other functions are used to determined the specific input.





## GRAPHICS, SOUNDS AND INPUT IN ONE EXAMPLE



Inside the src/examples/animate directory you can find the following example that could be used as a blueprint for a simple game.

This simple example has graphics, input and sound.

When compiled for any of the possible systems, it will produce the very same program but the exact tile shapes and sounds will depend on the target.

For example it can be compiled for the native console with:

xl animate

or for the Apple // with

xl animate apple2

or for the MSX with

xl animate msx


```
#include "cross_lib.h"



#define MIN_X 1

#define MAX_X (XSize-2)

#define MIN_Y 1

#define MAX_Y (YSize-2)

#define DOWN_TILE  _TILE_0

#define UP_TILE    _TILE_1 

#define RIGHT_TILE _TILE_2

#define LEFT_TILE  _TILE_3 

#define FIRE_TILE  _TILE_4



int main(void)

{ 

 uint8_t x;

 uint8_t y;

 uint8_t input;

 uint8_t tile;



 _XL_INIT_GRAPHICS();

 _XL_INIT_SOUND();

 _XL_INIT_INPUT();



 _XL_CLEAR_SCREEN();



 x = XSize/2;

 y = YSize/2;

 tile = DOWN_TILE;

 while(1)

 {

        _XL_SET_TEXT_COLOR(_XL_WHITE);

        _XL_PRINTD(0,0,3,x);

        _XL_PRINTD(5,0,3,y);

        

        input = _XL_INPUT();

        if(_XL_UP(input))

        {

            _XL_DELETE(x,y);

            tile = UP_TILE;

            --y;

        }

        else if (_XL_DOWN(input))

        {

            _XL_DELETE(x,y);

            tile = DOWN_TILE;

            ++y;

        }

        else if (_XL_LEFT(input))

        {

            _XL_DELETE(x,y);

            tile = LEFT_TILE;

            --x;

        }

        else if(_XL_RIGHT(input))

        {

            _XL_DELETE(x,y);

            tile = RIGHT_TILE;

            ++x;

        }

        else if(_XL_FIRE(input))

        {

            _XL_DRAW(x,y,FIRE_TILE,_XL_WHITE);

            _XL_EXPLOSION_SOUND();

            _XL_SLOW_DOWN(16*_XL_SLOW_DOWN_FACTOR);  

        }

        

        if((y>=MIN_Y)&&(y<=MAX_Y)&&(x>=MIN_X)&&(x<=MAX_X))

        {

            _XL_DRAW(x,y,tile,_XL_WHITE);

        }

        else

        {

            _XL_DELETE(x,y);

            _XL_ZAP_SOUND();

            _XL_SLOW_DOWN(16*_XL_SLOW_DOWN_FACTOR);  

            x = XSize/2;

            y = YSize/2;

        }    

        _XL_SLOW_DOWN(4*_XL_SLOW_DOWN_FACTOR);    

    }

    return EXIT_SUCCESS;

}

```



## THE GAMES



Cross-Lib comes with some games I have written with it. As for July 2023, it includes seven games:

- Shuriken

- Verbix

- Horde

- Snake

- Bomber

- Shoot

- Chase


### SHURIKEN

Shuriken is somehow similar to PAC-MAN in that you need to collect all diamonds to complete non-boss levels while you need to avoid being hit by the shurikens.
In boss levels you have to additionally kill all the shurikens by using the movable blocks that are deadly for them.
You can also collect special items that make you temporarily invincible (white ring) or that slow down the shurikens (blue large diamond).


### VERBIX



Verbix is a Scrabble-like game that looks also a bit like Tetris.
In this game you have to form valid 5-letter English words on the bottom row before the tower of letters reaches the top level.
You can rotate the bottom row or any of the columns by moving in the 4 directions. You press fire to confirm the word.



### HORDE

Horde is a zombie-themed shooter with many power-ups and several different enemies, levels and special items to pick.

You control a bow with a limited number of arrows. You get more arrows as a possible reward for killing some zombies.
If you run out of arrows, you will get a few arrows after a short period.
In each level you are confronted with a horde of zombies that become harder as you progress.



### SNAKE

Snake is my personal interpretation of a hybrid game between the arcade game Nibbler and the Nokia cell phone game Snake to which I have added more game elements. 
As in Nibbler the walls do not kill you and can be used to stop you and let you better control your movements.
As in Nokia Snake the main goal is to eat apples. If you do not eat any apple for too long, the snake becomes faster and harder to control, consumes its energy and eventually dies.
In each level you are also confronted with mines that kill you if the snake's head touches them.
As a secondary goal you can collect rings. If you manage to get 3 or more rings without dying on a level, you will be rewarded with bonus items.
The game has several secrets and level to discover.


### BOMBER

Bomber is a mini-game and clone of Air Attack (aka Blitz) that first appeared on the Commodore Pet. 
The game concept is horrible (not my own concept) but it can be addictive nevertheless.
Your plane has to carpet-bomb an entire city in order to safely land on the road. Let us assume the city has no inhabitants.
You do not control the plane and can only press fire to drop a bomb at a time.



### SHOOT

Shoot is a shooter somehow similar to Robotron. You have to kill a horde of ghosts on each level.
You can kill them by hitting them many times or by hitting them enough times to push them against the walls (mortal only for them).
You can also kill them by placing mines but you need to get the mine item (with 3 mines). A mine kills instantly common enemies.
At the end of each level when only few ghosts are left or if you survive without killing enemies long enough or in boss levels, you are confronted by some hard-to-kill skull-shaped enemies.
As a secondary goal you can complete the level by killing the skulls before the last ghosts and by doing so you unlock extra items and secrets in the next levels.
Destroying the missile bases can also unlock items.
The game has many secrets, levels and items to discover.


### CHASE

Cross Chase is somehow similar to Daleks / Gnome Robots. You need to lure your pursuers into the mines.
The main difference is that it is a real-time game and that it has several secrets items and power-ups to pick.
Similarly to Shoot, at the end of each level or in boss levels, you are confronted by a boss (a skull), which can only be killed by several bullets fired by the gun item.
You can complete the level by killing the skull before the last enemy and by doing so you unlock extra items and secrets in the next levels.
Destroying the missile bases can also unlock items.




## FUTURE DEVELOPMENT

Cross-Lib is actively developed. Planned future developments are: 

- support for more targets and architectures (e.g., Apple ||gs, Apple 1, etc.);

- improved targets (e.g, DHGR mode for the 80-column capable Apple // models);

- improved APIs (more tiles, improved sounds, etc.);

- simplified setup (docker file with all compilers, tools and dependencies);

- improved scripts to handle graphics assets;

- improved support for the native target; 

- more games.







## BIBLIOGRAPHY



[ack] ACK, Tanenbaum A., Jacobs C. et al., C compiler for Intel 8088, PDP 11, CP/M-80,etc., https://github.com/davidgiven/ack



[app] Cross Chase: A Massively 8-bi Multi-System Game, Caruso F., Call-A.P.P.L.E., vol. 28, No. 1, Feb 2018, pages 31-33, http://www.callapple.org



[c8b] 8-bit C, Caruso F., https://github.com/Fabrizio-Caruso/8bitC



[cc6303] CC6303, Cox A., C compiler for the Motorola 6800 and 6803, https://github.com/EtchedPixels/CC6303



[cc65] CC65, C compiler for the MOS 6502, https://sourceforge.net/projects/cc65/



[ceo] Cross Chase, Caruso F., CEO-MAG, No. 327-328, Juillet-Août 2017, https://ceo.oric.org/



[chPad] CharPad, Subchrist Software, https://subchristsoftware.itch.io/charpad-c64-free



[cmoc] CMOC, Sarrazin P., C compiler for the Motorola 6809, https://perso.b2b2c.ca/~sarrazip/dev/cmoc.html



[effc] Efficient C Code for 8-bit Microcontrollers, Jones N., https://barrgroup.com/embedded-systems/how-to/efficient-c-code



[m20] Z8KGCC, Groessler C., GCC modded for the Olivetti M20, http://www.z80ne.com/m20/sections/download/z8kgcc/z8kgcc.html



[lcc1802] LCC1802, Rowe B., C compiler for the RCA 1802, https://github.com/bill2009/lcc1802



[prog] Cross-Lib, Caruso F., Programmez! Hors Série #6, pages 72-82.



[rg] Interview to F. Caruso about Cross-Lib, RetroGamer #240, page 100, https://www.retrogamer.net/



[st] GCC, Rivière V., modded for the Atari ST, http://vincent.riviere.free.fr/soft/m68k-atari-mint/



[ti] GCC, (Insomnia), modded for the TI99/4A, http://atariage.com/forums/topic/164295-gcc-for-the-ti



[un8] 8-Bit Unity, Beaucamp A. (8-bit Dude), http://8bit-unity.com/



[vbcc] VBCC, Barthelmann V., C compiler for the BBC Micro, BBC Master, Amiga, etc., http://www.compilers.de/vbcc.html



[xtc68] XTC68, Hudson J., C compiler Sinclair QL, https://github.com/stronnag/xtc68



[z88dk] Z88DK, C compiler and dev-kit for the Zilog 80 and Intel 8080 architectures.
