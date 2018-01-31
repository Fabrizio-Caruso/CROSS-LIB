#include<supervision.h>

int main()
{
	unsigned char i;
	unsigned short j;
	
	i = 0;
	do
	{
		for(j=0;j<40;++j)
		{
			SV_VIDEO[j*i]=i;
		};
		++i;
	}
	while(i!=200);

	return 0;
}