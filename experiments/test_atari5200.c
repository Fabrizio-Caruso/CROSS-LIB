#include <stdio.h>
#include <conio.h>
#include <unistd.h>
#include <atari5200.h>

int main()
{
	char ch;
	while(1)
	{
		if(kbhit())
		{
			ch = cgetc();
			printf("%c \n", ch);
		}
	}
}

