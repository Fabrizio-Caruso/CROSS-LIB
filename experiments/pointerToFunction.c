#include <stdio.h>
#include <conio.h>

void hello(void)
{
	gotoxy(3,5);
	textcolor(COLOR_WHITE);
	cprintf("Hello\n");
}

struct objectStruct
{
	void (* foo) (void);
};

typedef struct objectStruct Object;



int main()
{
	Object obj;
	obj.foo = hello;
	
	obj.foo();
	
	gotoxy(3,7); cprintf("world!\n");
	
	while(1){};
	
	return 0;
}
