#include "conio_patch.h"


void INIT_GRAPHICS(void)
{               
	        // POKE &H2019,PEEK(&H2019) OR 8 -> cursor OFF 

                // POKE &H2019,PEEK(&H2019) AND &HF7 -> cursor ON 
 			
		PUTCH(0x14);
		
		// PUTCH('a');
		
		// cputc('B');
		
		// cputc('C');
		
		// gotoxy(10,10);
		
		// cputc('D');
		
		// cputc('e');
		
		// cputc('f');
		
		// while(1){}
}	
