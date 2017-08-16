#ifndef _SPECTRUM_SLEEP
#define _SPECTRUM_SLEEP

void sleep(unsigned int sec) 
{ 
#if !defined(DEBUG_CRASH)
	unsigned int i; 
	for(i=0;i<sec*20000;++i){}; 
#else
	unsigned long i;
	for(i=0;i<sec*400UL;++i){}; 
#endif
} 

#endif // _SPECTRUM_SLEEP