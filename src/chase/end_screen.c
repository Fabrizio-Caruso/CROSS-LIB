#if !defined(TINY_GAME)

#include "settings.h"

#include "end_screen.h"
#include "text.h"
#include "text_strings.h"
#include "move_player.h"

#if defined(END_SCREEN)
	#include "strategy.h"
	#include "level.h"
	#include "character.h"
	#include "enemy.h"
	
	extern Character player;
	extern Character skull;
	extern Character ghosts[GHOSTS_NUMBER];
	extern Character missile;
	extern unsigned char playerFire;
	extern unsigned char level;
	extern unsigned char loop;
	extern unsigned char missileDirection;
	extern unsigned char guns;
#endif



#if defined(END_SCREEN) || defined(BETWEEN_SCREEN)
	void dance(Character * characterPtr)
	{
		deleteCharacter(characterPtr);
		if(!(missileDirection&3))
		{
			++(characterPtr->_x);
		}
		else if((missileDirection&3)==1)
		{
			++(characterPtr->_y);			
		}
		else if ((missileDirection&3)==2)
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
		skull._x = player._x-5;
		skull._y = player._y;	
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
			dance(&skull);
		
			printCenteredMessageOnRow(2+(loop&15),  YOU_MADE_IT_STRING);
			#if defined(SLOW_DOWN)
				for(k=0;k<GAME_SLOW_DOWN*4;++k) {};
			#endif
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

#endif

