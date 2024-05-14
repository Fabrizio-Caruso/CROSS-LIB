#include "conio_patch.h"
#include "display_macros.h"


void _XL_INIT_GRAPHICS(void)
{               
		// Hide blinking cursor
		PUTCH(0x14);	
	
		
        #if defined(__BACKGROUND_COLOR) && __BACKGROUND_COLOR==1
            
            // SCREEN 7 - TEXT COLOR
            PUTCH(0x1B);PUTCH(32);PUTCH(64);PUTCH(32);
        
            // SCREEN ,0 - BACKGROUND COLOR
            PUTCH(0x1B);PUTCH(32);PUTCH(87);PUTCH(32);

            // BORDER COLOR
            PUTCH(0x1B);PUTCH(103);

		#else
            // SCREEN 7 - TEXT COLOR
            PUTCH(0x1B);PUTCH(32);PUTCH(71);PUTCH(32);
            
            // SCREEN ,0 - BACKGROUND COLOR
            PUTCH(0x1B);PUTCH(32);PUTCH(80);PUTCH(32);  

            // BORDER COLOR
            PUTCH(0x1B);PUTCH(96);

        #endif
                
		// SCREEN ,,,0 (Fix attributes)
		PUTCH(0x1B);PUTCH(118);

}

