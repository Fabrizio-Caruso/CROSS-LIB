#include "cross_lib.h"

#include "text_macros.h"


#if !defined(NO_PRINT) && !defined(NO_MESSAGE)
	#if !defined(ALT_DISPLAY_STATS)
	void _XL_PRINT_CENTERED_ON_ROW(uint8_t row, char *Text)
	{
		_XL_PRINT(((uint8_t) (XSize - (uint8_t) strlen(Text))>>1), row, Text);	
	}
	#endif


	#if !defined(_XL_NO_TEXT_COLOR)	
			void _XL_PRINT_CENTERED_ON_ROW_WITH_COLOR(uint8_t row, uint8_t col, char *Text)
			{
				_XL_SET_TEXT_COLOR(col);
				_XL_PRINT_CENTERED_ON_ROW(row, Text);
			}
					
	#endif

#endif


