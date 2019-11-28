#if defined(__CMOC__) && !defined(__WINCMOC__)
	#include <cmoc.h>
    #include <coco.h>
#elif defined(__BBC__) && defined(NO_GRAPHICS)
    #include <stdint.h>
    #include <stdlib.h>
#elif defined(NO_GRAPHICS)
	//
#else
	#include <stdlib.h>
	#include <string.h>
	#include <stdio.h>
	#include <stdint.h>
#endif

