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


LICENSE
This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from
the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software in
a product, an acknowledgment in the product documentation would be
appreciated but is not required.

2. Altered source versions must be plainly marked as such, and must not
be misrepresented as being the original software.

3. This notice may not be removed or altered from any source distribution.
