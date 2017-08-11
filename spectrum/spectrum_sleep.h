#ifndef _SPECTRUM_SLEEP
#define _SPECTRUM_SLEEP

void sleep(unsigned int sec) 
{ 
	unsigned int i; 
	for(i=0;i<sec*20000;++i){}; 
} 

#endif // _SPECTRUM_SLEEP