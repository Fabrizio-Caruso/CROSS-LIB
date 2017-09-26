#include <stdio.h>
#include <stdlib.h>

#define POKE(addr,val) (*(unsigned char*) (addr) = (val))

int draw(unsigned char x, unsigned char y, unsigned char ch, unsigned char col)
{
	int xy = 0;
	int chCol = 0;
	xy = (y<<8) | x;
	chCol = (ch<<8) | col;
	return _draw_aux(chCol,xy);
}

int _draw_aux(int chCol, int xy)
{
	#asm
	pop bc   ; bc = ret address
	pop hl   ; hl = int b
	pop de  ; de = int a

	push de    ; now restore stack
	push hl
	push bc
	
	call 0x0092	
	#endasm
}

int main(void)
{
	unsigned char i;
	
	for(i=0;i<10;++i)
	{
		draw(10,i+8,65,1);
		getchar();
	}	
	
	for(i=0;i<10;++i)
	{
		draw(i+1,10,66,1);
		getchar();
	}	
	
	return 0;
}


