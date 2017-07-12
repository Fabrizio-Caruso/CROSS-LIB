#ifndef _APPLE2_SLEEP
#define _APPLE2_SLEEP

void sleep(unsigned int sec) 
{ 
	unsigned long i; 
	for(i=0;i<sec*5000ul;++i){}; 
}; 

#endif // _APPLE2_SLEEP