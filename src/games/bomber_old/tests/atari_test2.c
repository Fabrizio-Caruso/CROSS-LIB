/* --------------------------------------------------------------------------------------- */ 
// 
// CROSS SHOOT by Fabrizio Caruso
//
// Fabrizio_Caruso@hotmail.com
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from
// the use of this software.

// Permission is granted to anyone to use this software for non-commercial applications, 
// subxect to the following restrictions:

// 1. The origin of this software must not be misrepresented; you must not
// claim that you wrote the original software. If you use this software in
// a product, an acknowledgment in the product documentation would be
// appreciated but is not required.

// 2. Altered source versions must be plainly marked as such, and must not
// be misrepresented as being the original software.

// 3. This notice may not be removed or altered from any source distribution.
/* --------------------------------------------------------------------------------------- */ 


#include <conio.h>
#include <stdint.h>
#include <atari.h>

uint8_t x;
uint8_t y;

// #define XSize 20
// #define YSize 24

#include "cross_lib.h"

// void _GOTOXY(uint8_t x, uint8_t y)
// { 
	// if((y)&1) 
	// { 
		// gotoxy(x+20,(y)/2);
	// } 
	// else 
	// { 
		// gotoxy(x, (y)/2);
	// } 
// };

// void PRINT(uint8_t x, uint8_t y, char * str)
// { 
    // _GOTOXY(x,y);
    // cprintf(str); 
// };

int main(void)
{        
    
    INIT_GRAPHICS();

    INIT_INPUT();    


    while(1)
    {

        // CLEAR_SCREEN();
        gotoxy(0,0);
        for(y=2;y<YSize-1;++y)
        {
            for(x=1;x<XSize-2;++x)
            {
                cprintf("X");
            }
        }
        cgetc();
        PRINT(3,9,"game over");
        cgetc();
        CLEAR_SCREEN(); sleep(2);
        // cgetc();
        PRINT(3,7,"game completed"); while(1); sleep(2); while(1);
        cgetc();
    } // while(1) -> restart from level 1

    return 0;
}

