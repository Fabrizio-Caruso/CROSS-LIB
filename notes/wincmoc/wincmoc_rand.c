#include <coco.h>
#include <stdio.h>
#include <stdlib.h>
			#include <time.h>
			#include <unistd.h>

int main()
{
	unsigned char i;
	unsigned char j;

	for(j=0; j<10;++j)
	{
		for(i=0; i<10; ++i)
		{
			if(rand()>16000)
			{
				printf("%d: YES\n",i);
			}
			else
			{
				printf("%d: NO\n",i);
			}
		}
		sleep(1);
		printf("\n");
	}
	printf("hello world\n");
	return 0;
}

