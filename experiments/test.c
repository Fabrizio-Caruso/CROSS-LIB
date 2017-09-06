#include <stdio.h>
#include <conio.h>

// void color1(unsigned char c)
// {
	// printf("\x1B[%d;40m",c);
// }


// void color2(unsigned char c)
// {
	// printf("\x1B[37;%dm",c);
// }

int main()
{
	unsigned short i; unsigned char j;

	for(i=0;i<19;++i)
	{
		textcolor(i);
		printf("%d - \n", i);
		for(j=0;j<255;++j)
		{
		}
	}


	
	while(1)
	{
	}
	return 0;
}
