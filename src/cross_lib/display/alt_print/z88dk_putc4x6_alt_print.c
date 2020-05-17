
extern uint8_t XSize;
extern uint8_t YSize;

#include "cross_lib.h"

# if defined(Z88DK_PUTC4X6)
	void PRINT(uint8_t x, uint8_t y, char * str)
	{
		uint8_t i = 0;
		while(str[i]!='\0')
		{
			gotoxy(x+i,y);
			cputc(str[i++]);
		}
	}

	void print_05u0(uint8_t x, uint8_t y, uint16_t val)
	{
		uint8_t i;
		uint8_t digits[6];
		uint16_t tmp = val;
		
		digits[0] = 0;
		for(i=1;i<6;++i)
		{
			digits[i] = (uint8_t) ((tmp)%10);
			tmp-= digits[i];
			tmp/=10;
		}
		
		for(i=0;i<6;++i)
		{
			gotoxy(x+i,y);
			cputc((digits[5-i])+48);
		}
	}	

	void print_02u(uint8_t x, uint8_t y, uint16_t val)
	{
		gotoxy(x,y);
		cputc(((uint8_t) val)/10+48);
		gotoxy((x+1),y);
		cputc(((uint8_t) val)%10+48);
	}	


	void print_u(uint8_t x, uint8_t y, uint16_t val)
	{
		gotoxy(x,y);
		cputc(val+48);
	}

	void print_level(uint16_t val)
	{
		PRINT(XSize/2-4,YSize/2,"level");
		print_u(XSize/2+2, YSize/2, val);
	}
	
	void print_score(x,y,str,val)
	{
		print_05u0(XSize/2+2, YSize/2, val);
	}

	void PRINTD(uint8_t x, uint8_t y, char * str, uint16_t val)
	{
		if(strlen(str)==5)
		{	
			print_05u0(x,y,val);
		}
		else if(strlen(str)==4)
		{
			print_02u(x,y,val);		
		}
		else if(strlen(str)==2)
		{
			print_u(x,y,val);		
		}
		else if(str[0]=='B')
		{
			print_score(x,y,"BONUS:",val);
		}
		else if(str[0]=='b')
		{
			print_score(x,y,"bonus:",val);
		}	
		else if(str[0]=='S')
		{
			print_score(x,y,"SCORE:",val);
		}
		else if(str[0]=='s')
		{
			print_score(x,y,"score:",val);
		}		
		else if(str[0]=='H')
		{
			print_score(x,y,"HIGH SCORE:",val);
		}
		else if(str[0]=='h')
		{
			print_score(x,y,"high score:",val);
		}		
		else
		{
			print_level(val);
		}
	}
#endif
