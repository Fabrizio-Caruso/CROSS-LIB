#include "../cross_lib.h"

#include "text_macros.h"


#if !defined(NO_MESSAGE)
	void printCenteredMessageOnRow(unsigned char row, char *Text)
	{
		PRINT(((XSize - strlen(Text))>>1), row, Text);	
	}


	#if !defined(NO_TEXT_COLOR)
		#if defined(__ATMOS__)
			#include <peekpoke.h>
			void printCenteredMessageOnRowWithCol(unsigned char row, unsigned char col, char *Text)
			{
				POKE(0xBB80+3+(row+Y_OFFSET)*40,16);POKE(0xBB80+3+1+(row+Y_OFFSET)*40,col);
				printCenteredMessageOnRow(row, Text);						
				POKE(0xBB80+35+(row+Y_OFFSET)*40,16);POKE(0xBB80+35+1+(row+Y_OFFSET)*40,3);			
			}			
		#else	
			void printCenteredMessageOnRowWithCol(unsigned char row, unsigned char col, char *Text)
			{
				SET_TEXT_COLOR(col);
				printCenteredMessageOnRow(row, Text);
			}
					
		#endif
	#endif

#endif


