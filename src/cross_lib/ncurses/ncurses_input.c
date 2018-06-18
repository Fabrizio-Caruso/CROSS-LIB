#define INPUT_LOOPS 10

#include <ncurses.h>

#if !defined(TURN_BASED)
	unsigned char GET_CHAR(void)
	{
		unsigned long delay = 0;
		char _ch;
		char ch;

		while(delay<INPUT_LOOPS)
		{	
			_ch = getch();
			if(_ch!=ERR)
			{
				ch = _ch;
			}
			++delay;
		}
		
		return ch;
	}
#else
	//
#endif
