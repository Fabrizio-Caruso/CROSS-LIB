#include <stdio.h>

#define gotoxy(x,y) printf("\033Y%c%c",y+32,x+32)

#define clrscr() printf("\033H\033J")

#define cprintf printf

int main()
{
	gotoxy(10,10);
	cprintf("Hello world!\n");	
	getch();
	clrscr();
	gotoxy(12,12);
	cprintf("Hello world!\n");
	
}

