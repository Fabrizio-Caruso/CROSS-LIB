#if defined(__CMOC__) && !defined(__WINCMOC__)
	#include <cmoc.h>
    #include <coco.h>
#elif defined(__BBC__) && defined(NO_GRAPHICS)
    #include <stdint.h>
    #include <stdlib.h>
#elif defined(NO_GRAPHICS)
    #if !defined(_STDINT)
        #define _STDINT
        typedef unsigned char uint8_t;
        typedef unsigned short uint16_t;
    #endif // _STDINT
#else
	#include <stdlib.h>
	#include <string.h>
	#include <stdio.h>
    #if !defined(__XTC68__) && !defined(__OLIVETTI_M20__)
        #include <stdint.h>
    #else
        #if !defined(_STDINT)
            #define _STDINT
            typedef unsigned char uint8_t;
            typedef unsigned short uint16_t;
        #endif // _STDINT
    #endif
#endif

