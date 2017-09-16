#ifndef _SPECTRUM_SLEEP
#define _SPECTRUM_SLEEP

void sleep(unsigned int sec) 
{ 
	unsigned long i;
	for(i=0;i<sec*400UL;++i){}; 
} 

#endif // _SPECTRUM_SLEEP