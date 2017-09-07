#include <stdio.h>
#include <conio.h>

#define POKE(addr,val)     (*(unsigned char*) (addr) = (val))
#define POKEW(addr,val)    (*(unsigned*) (addr) = (val))
#define PEEK(addr)         (*(unsigned char*) (addr))
#define PEEKW(addr) (*(unsigned*) (addr))

#if defined(__MSX__)
	#define XSIZE 40
#else
	#define XSIZE 40
#endif




char GET_CHAR(void)
{
	if(PEEK(14351) == 6)
		return PEEK(14346);
	else
		return '\0';
}

int main()
{
	unsigned char i;
	for(i=0;i<XSIZE;++i)
	{
		gotoxy(i,0); cputs("X");
	}
	for(i=0;i<12;++i)
	{
		gotoxy(0,i); cputs("O");
	}
	for(i=0;i<XSIZE;++i)
	{
		gotoxy(i,5); putch(i+65);
	}	
	while(1)
	{
	}
	
	// while(1)
	// {
		// int i;
		// printf("%c \n", getk());
		// for(i=0;i<2000;++i)
		// {
		// }		
	// }
	// {	
		// int i;
		// printf("%d -- %d -- %d -- %d\n", PEEK(0x380A), PEEK(0x380F), PEEK(0x380E), PEEK(0x380F));
		// for(i=0;i<2000;++i)
		// {
		// }
	// }
	return 0;
}


