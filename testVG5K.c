#include <stdio.h>
#include <stdlib.h>

#define POKE(addr,val) (*(unsigned char*) (addr) = (val))

int main(void)
{
	unsigned char i;
	
	printf("Hello world!\n");
	for(i=0;i<25;++i)
	{
		POKE(16384+80*i,32);
		POKE(16385+80*i,1);
	}
	for(i=0;i<10;++i)
	{
		POKE(16386+2*i,65);
		POKE(16387+2*i,7);
	}
	printf("Hello world!\n");	
	getchar();	
	return 0;
}


