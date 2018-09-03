
#include "../graphics_mode/memory_mapped_graphics.h"
#include "memory_mapped_alt_print.h"

#if defined(BUFFERED)	
	//#include "../display_target_settings.h"
	
	// TODO: THIS IS BAD! You should use definitions in display_target_settings.h
	#if defined(__PC86__)
		#define YSize 22
		#define Y_OFFSET 2
		#define XSize 78
	#endif
	
	extern unsigned char video_buffer[22+2][78];
#endif

#if defined(__CMOC__) && !defined(__WINCMOC__)
	#include <cmoc.h>
#else
	#include <stdlib.h>
	#include <string.h>
	#include <stdio.h>
#endif


#if defined(CBM_SCREEN_CODES)
	char screenCode(char ch)
	{
		if(ch>64)
		{
			return ch-64;
		}
		else
		{
			return ch;
		}
		return ch;
	}
#elif defined(__CMOC__) && !defined(__WINCMOC__)
	char screenCode(char ch)
	{
		if(ch==32) 
		{
			return 32+64;
		}
		else
		{
			return ch-32;
		}	
	}
#endif


void PRINT(unsigned char x, unsigned char y, char * str)
{
	unsigned char i = 0;
	while(str[i]!='\0')
	{
		#if defined(CBM_SCREEN_CODES) || (defined(__CMOC__) && !defined(__WINCMOC__))
			DISPLAY_POKE(loc(x+i,y), screenCode(str[i])); 		
		#elif defined(BUFFERED)
			video_buffer[y][x+i] = str[i];
		#else
			DISPLAY_POKE(loc(x+i,y), str[i]);
		#endif
		++i;
	}
}

void print_05u0(unsigned char x, unsigned char y, unsigned short val)
{
	unsigned char i;
	unsigned char digits[6];
	
	digits[0] = 0;
	for(i=1;i<6;++i)
	{
		digits[i] = (unsigned char) ((val)%10);
		val-= digits[i];
		val/=10;
	}
	
	for(i=0;i<6;++i)
	{
		#if defined(BUFFERED)
			video_buffer[y][x+i] = (unsigned char) (digits[5-i])+48;
		#else
			DISPLAY_POKE(loc(x+i,y), (unsigned char) (digits[5-i])+48);
		#endif
	}
}	

void print_02u(unsigned char x, unsigned char y, unsigned short val)
{
	#if defined(BUFFERED)
	//
	#else
	DISPLAY_POKE((loc(x,y)), ((unsigned char) val)/10+48);
	DISPLAY_POKE((1+loc(x,y)), ((unsigned char) val)%10+48);
	#endif
}	


void print_u(unsigned char x, unsigned char y, unsigned short val)
{
	#if defined(BUFFERED)
	//
	#else
	DISPLAY_POKE(loc(x,y), (unsigned char) (val+48));
	#endif
}


void PRINTF(unsigned char x, unsigned char y, char * str, unsigned short val)
{
	if(strlen(str)==5)
	{	
		print_05u0(x,y,val);
	}
	else if(strlen(str)==4)
	{
		print_02u(x,y,val);		
	}
	else
	{
		print_u(x,y,val);		
	}
}



