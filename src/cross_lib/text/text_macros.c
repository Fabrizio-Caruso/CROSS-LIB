#include "cross_lib.h"

#include "text_macros.h"
#include "standard_libs.h"

#if defined(__MC10__)
extern uint8_t strlen(char *);
#endif 

#if !defined(NO_PRINT) && !defined(NO_MESSAGE)
	#if !defined(ALT_DISPLAY_STATS)
	void _XL_PRINT_CENTERED_ON_ROW(uint8_t row, char *Text)
	{
		_XL_PRINT(((uint8_t) (XSize - (uint8_t) strlen(Text))>>1), row, Text);	
	}
	#endif

#endif


