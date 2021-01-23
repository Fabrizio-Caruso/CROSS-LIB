#ifndef _TEXT_MACROS
#define _TEXT_MACROS


#include "cross_lib.h"
#include "standard_libs.h"

#include "letters.h"

#define _XL_PRINT_CENTERED(Text) \
	_XL_PRINT_CENTERED_ON_ROW((YSize>>1), Text)


#if !defined(NO_TEXT_COLOR) && !defined(NO_PRINT)
	#define _XL_PRINT_CENTERED_WITH_COLOR(col, Text) \
		_XL_PRINT_CENTERED_ON_ROW_WITH_COLOR((YSize>>1), col, Text)	
#else
	#define _XL_PRINT_CENTERED_WITH_COLOR(col, Text) \
		_XL_PRINT_CENTERED_ON_ROW((YSize>>1), Text)
#endif


#if !defined(NO_PRINT)
    #if !defined(NO_TEXT_COLOR) 
        void _XL_PRINT_CENTERED_ON_ROW_WITH_COLOR(uint8_t row, uint8_t col, char *Text);
    #else
        #define _XL_PRINT_CENTERED_ON_ROW_WITH_COLOR(row,col,Text) _XL_PRINT_CENTERED_ON_ROW(row,Text)
    #endif
    void _XL_PRINT_CENTERED_ON_ROW(uint8_t row, char *Text);
#else
    #define _XL_PRINT_CENTERED_ON_ROW_WITH_COLOR(row,col,Text)
	#define _XL_PRINT_CENTERED_ON_ROW(row,Text)
#endif


#endif // _TEXT_MACROS

