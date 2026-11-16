#include <stdlib.h>
#include <stdio.h>

int main(void)

{
	short i;
	
	for(i=0;i<10;++i)
	{
		printf("%d\n", i);
		sleep(1);
	}
	while(1){};
	return 0;
}
