#include "end_screen.h"
#include "text.h"
#include "text_strings.h"
#include "settings.h"
#include "display_macros.h"
#include "sleep_macros.h"


#if defined(END_SCREEN)
	#include "strategy.h"
	#include "level.h"
	#include "character.h"
	#include "enemy.h"
	#include "input_macros.h"
	
	extern Character player;
	extern Character invincibleGhost;
	extern Character ghosts[GHOSTS_NUMBER];
	extern Character missile;
	extern unsigned char playerFire;
	extern unsigned char level;
	extern unsigned char loop;
	extern unsigned char guns;
#endif



#if defined(END_SCREEN) || defined(BETWEEN_SCREEN)
	void dance(Character * characterPtr)
	{
		deleteCharacter(characterPtr);
		if(!(loop&3))
		{
			++(characterPtr->_x);
		}
		else if((loop&3)==1)
		{
			++(characterPtr->_y);			
		}
		else if ((loop&3)==2)
		{
			--(characterPtr->_x);			
		}
		else
		{
			--(characterPtr->_y);			
		}
		displayCharacter(characterPtr);	
	}

#endif

#if defined(END_SCREEN)
	void gameCompleted(void)
	{
		unsigned short k;
		
		level = 1;
		
		CLEAR_SCREEN();
		fillLevelWithCharacters(GHOSTS_NUMBER);	
		
		DRAW_BORDERS();

		playerFire = 0;
		loop = 0;
		invincibleGhost._x = player._x-5;
		invincibleGhost._y = player._y;	
		missile._status = 0;
		guns = 1;
		while(!playerFire)
		{
			displayBombs();
			for(k=0;k<GHOSTS_NUMBER;++k)
			{
				dance(&ghosts[k]);
			}
			dance(&player);
			dance(&invincibleGhost);
		
			printCenteredMessageOnRow(2+(loop&15),  YOU_MADE_IT_STRING);
			for(k=0;k<GAME_SLOW_DOWN*4;++k) {};
			printCenteredMessageOnRow(2+(loop&15), "             ");
			
			++loop;
			
			MOVE_PLAYER();
			if(wallReached(&player))
			{
				playerFire = 1;
			}
		}
		printGameOver();
		sleep(2);
		printPressKeyToStart();
		WAIT_PRESS();
		playerFire = 0;
	}
#else
	void gameCompleted(void)	
	{
		printCenteredMessage(YOU_MADE_IT_STRING); 
	}
#endif

