#include<supervision.h>
#include<peekpoke.h>

// #define POKE(addr,val)     (*(unsigned char*) (addr) = (val))
// #define POKEW(addr,val)    (*(unsigned*) (addr) = (val))
// #define PEEK(addr)         (*(unsigned char*) (addr))
// #define PEEKW(addr) (*(unsigned*) (addr))

int main()
{
	unsigned char i;
	unsigned short j;
	
/*
struct __sv_lcd {
    unsigned char       width;
    unsigned char       height;
    unsigned char       xpos;
    unsigned char       ypos;
};
#define SV_LCD  (*(struct __sv_lcd*)0x2000)
*/

	SV_LCD.width = 160;
	SV_LCD.height = 160;
	SV_LCD.xpos = 100;
	SV_LCD.ypos = 100;
	*SV_VIDEO = 255;
	i = 0;
	do
	{
		for(j=0;j<40;++j)
		{
			POKE((unsigned short) (0x4000+i+(unsigned short)(j*40)),i);
			SV_VIDEO[(unsigned short) j*i]=i;
			SV_LCD.xpos=i;
			SV_LCD.ypos=j;
		};
		++i;
	}
	while(i!=160);
	
	while(1){};
	
	return 0;
}