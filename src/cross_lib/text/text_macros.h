#ifndef _TEXT_MACROS
#define _TEXT_MACROS


#include "cross_lib.h"
#include "standard_libs.h"

#include "letters.h"

#define _XL_PRINT_CENTERED(Text) \
	_XL_PRINT_CENTERED_ON_ROW((YSize>>1), Text)


#if !defined(NO_PRINT)
    void _XL_PRINT_CENTERED_ON_ROW(uint8_t row, char *Text);
#else
	#define _XL_PRINT_CENTERED_ON_ROW(row,Text)
#endif


#endif // _TEXT_MACROS

