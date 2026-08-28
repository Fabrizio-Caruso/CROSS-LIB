/* --------------------------------------------------------------------------------------- */ 
// 
// CROSS CHASE by Fabrizio Caruso
//
// Fabrizio_Caruso@hotmail.com
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from
// the use of this software.

// Permission is granted to anyone to use this software for non-commercial applications, 
// subject to the following restrictions:

// 1. The origin of this software must not be misrepresented; you must not
// claim that you wrote the original software. If you use this software in
// a product, an acknowledgment in the product documentation would be
// appreciated but is not required.

// 2. Altered source versions must be plainly marked as such, and must not
// be misrepresented as being the original software.

// 3. This notice may not be removed or altered from any source distribution.
/* --------------------------------------------------------------------------------------- */ 

#ifndef _AQUARIUS_CHAR_TILES_H
#define _AQUARIUS_CHAR_TILES_H

    #if defined(ANIMATE_PLAYER)
        // UP
        #define _TILE_1 14
        // 138
        // '*'

        // LEFT
        #define _TILE_3 140
        //155
        // '*'

        // RIGHT
        #define _TILE_2 23
        //154
        // '*'

        // DOWN
        #define _TILE_0 19
        //139
        // '*'
    #else
        // UP
        #define _TILE_1 135
        // '*'

        // LEFT
        #define _TILE_3 135
        // '*'

        // RIGHT
        #define _TILE_2 135
        // '*'

        // DOWN
        #define _TILE_0 135
        // '*'
    #endif


    // BOMB
    #define _TILE_5 216
    // 211
    // 'X'

    // GHOST
    #define _TILE_4 157
    // 'o'

    #define _TILE_6 156
    // '#'

    #define _TILE_9 167
    // '!'

    // SKULL
    #define _TILE_7 146
    // 200
    // '+'

    #define _TILE_25 '$'

    #define _TILE_8 212
    // 'S'

    #define _TILE_17 212
    // 'F'

    #define _TILE_18 212
    // 'H'

    #define _TILE_10 213
    // 'V'

    #define _TILE_11 198
    // '.'
    
    #define _TILE_14 6
    // '>'


    #define _TILE_15 7
    // '<'

    // Rocket
    #define _TILE_16 25
    // ('^')

    #define _TILE_12 214
    // 'I' 

    #define _TILE_13 172
    // '-'


#endif // _AQUARIUS_CHAR_TILES_H


