#include "conio_patch.h"


void INIT_GRAPHICS(void)
{               
		// Hide blinking cursor
		PUTCH(0x14);	
		
		// SCREEN 7
		PUTCH(0x1B);PUTCH(32);PUTCH(71);PUTCH(32);
		
		// SCREEN ,0
		PUTCH(0x1B);PUTCH(32);PUTCH(80);PUTCH(32);
		
        PUTCH(0x1B);PUTCH(96);
        
		// SCREEN ,,,0 (Fix attributes)
		PUTCH(0x1B);PUTCH(118);
}	
