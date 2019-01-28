#include "cross_lib.h"

#include "text_macros.h"


#if !defined(NO_MESSAGE)
	#if !defined(ALT_DISPLAY_STATS)
	void printCenteredMessageOnRow(unsigned char row, char *Text)
	{
		PRINT(((unsigned char) (XSize - (unsigned char) strlen(Text))>>1), row, Text);	
	}
	#endif


	#if !defined(NO_TEXT_COLOR)
		#if defined(__ATMOS__)
			#include <peekpoke.h>
			void printCenteredMessageOnRowWithCol(unsigned char row, unsigned char col, char *Text)
			{
				POKE(0xBB80+3+(row)*40,16);POKE(0xBB80+3+1+(row)*40,col);
				printCenteredMessageOnRow(row, Text);						
				POKE(0xBB80+35+(row)*40,16);POKE(0xBB80+35+1+(row)*40,3);			
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


