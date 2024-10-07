#ifndef _STANDARD_LIBS_H
#define _STANDARD_LIBS_H

#if defined(__COCO__) || defined(__DRAGON__) || (defined(__CMOC__) && !defined(__WINCMOC__))
	#include <cmoc.h>
    #include <coco.h>

#elif defined(__MC10__)
    #include <stdint.h>
    #include <stdio.h>
    int rand(void);

#elif defined(__BBC__) && defined(__NO_GRAPHICS)
    #include <stdint.h>
    #include <stdlib.h>
#elif defined(__LCC1802__) 
    #include <stdint.h>
#elif defined(__TI99__)
        typedef unsigned char uint8_t;
        typedef unsigned int uint16_t;
        #include <string.h>
#elif defined(__NO_GRAPHICS) 
    #if !defined(_STDINT)
        #define _STDINT
        typedef unsigned char uint8_t;
        typedef unsigned int uint16_t;
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

#if defined(__ALT_RAND) && !defined(__LCC1802__)
    int rand(void);
#endif

#endif // _STANDARD_LIBS_H

