# CROSS CHASE
CROSS CHASE

by Fabrizio Caruso (Fabrizio_Caruso@hotmail.com)

This is a personal project whose goal is to create a simple playable game that should run
on 8 bit computers and consoles using 6502-compatible and Z80-compatible CPUs.

The program is written in ANSI C and currently compiled with CC65 for 6502 targets (http://www.cc65.org/) and Z88DK for Z80 targets (https://www.z88dk.org/).
For other targets, CMOC or GCC6809 for 6809 targets and C99C for TI-99 will be taken into consideration. Such cross compilers are less advanced and lack the same level of support. 

The main version should be as portable as possible.
Therefore it should ONLY use:
1. ANSI C
2. separated input/output and hardware-dependent code from the gameplay code;
3. conio.h (as implemented in CC65 and in Z88DK), if no specialized code for a specific target is implemented;
4. joystick-related libraries as found in CC65 and in Z88DK;
5. whatever possible for TI-99 and 6809 targets (ideally conio.h as found in CMOC and whatever found in C99C).

Some specific target(s) may get specific graphic code with re-defined characters, software/hardware sprites and music/sound effects.

LICENSE

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