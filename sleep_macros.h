#ifndef _SLEEP_MACROS
#define _SLEEP_MACROS

#if defined(__APPLE2__)
	void sleep(unsigned int sec) { /* BOGUS */ sec = 1; } 
#else
	#include <time.h>
	#include <unistd.h>
#endif

#endif // _SLEEP_MACROS