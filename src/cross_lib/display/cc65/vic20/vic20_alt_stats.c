extern unsigned short points;
extern unsigned short highScore;

#if defined(ALT_DISPLAY_STATS) || defined(ALT_HISCORE)
	#include <peekpoke.h>
	void _displayShort(unsigned short value) 
	{ 
		unsigned char i; 
		unsigned short tmp; 
		
		tmp = value; 
		
		for(i=1;i<6;++i) 
		{ 
			tmp -= POKE(7686-i,(unsigned char) ((tmp)%10)); 
			tmp/=10; 
			POKE(7686-i,PEEK(7686-i)+48); 
		} 
		POKE(7686,48); 	
	}
	
	#if defined(ALT_DISPLAY_STATS)
	void displayStats(void)
	{
		_displayShort(points);
	}
	#endif
	
	
	#if defined(ALT_HIGHSCORE) 
	void highScoreScreen(void)
	{
		_displayShort(highScore);
		POKE(7686+2,'h'); 
		POKE(7686+3,'i');
		// POKE(7686+5,'s'); POKE(7686+6,'c');		
		// POKE(7686+7,'o'); POKE(7686+8,'r');		
		// POKE(7686+9,'e');				
	}
	#endif
#endif
