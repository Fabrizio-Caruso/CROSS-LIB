# ASCII CHASE
ASCII CHASE

by Fabrizio Caruso (Fabrizio_Caruso@hotmail.com)

This is a personal project whose goal is to create a simple playable game that should run
on 8 bit computers and consoles using 6502-compatible CPUs.

The program is written in ANSI C and compiled with CC65 (http://www.cc65.org/).

The main version should be as portable as possible.
Therefore it should ONLY use:
1. ANSI C
2. conio.h (as found in CC65) but only relying on the intersection of characters between ASCII and PETSCII (for max compatibility with Commodore 8 bit computers)
3. joystick.h (as found in CC65)

Once I am happy with the gameplay I may implement some version(s) for some specific target(s) with 
re-defined characters and music/sound effects.
