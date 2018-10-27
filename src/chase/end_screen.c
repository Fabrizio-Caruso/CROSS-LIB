#if !defined(TINY_GAME)

#include "settings.h"

#include "end_screen.h"
#include "game_text.h"
#include "text_strings.h"
#include "move_player.h"

#if defined(END_SCREEN)
	#include "strategy.h"
	#include "level.h"
	#include "character.h"
	#include "ghost.h"
	
	extern Character player;
	extern Character skull;
	extern Character ghosts[GHOSTS_NUMBER];
	extern Character bullet;
	extern unsigned char playerFire;
	extern unsigned char level;
	extern unsigned char loop;
	extern unsigned char guns;
	extern unsigned char ind;
#endif



#if defined(END_SCREEN) || defined(DANCE)
	extern unsigned char bulletDirection;

	void dance(Character * characterPtr)
	{
		deleteCharacter(characterPtr);
		if(!(bulletDirection&3))
		{
			++(characterPtr->_x);
		}
		else if((bulletDirection&3)==1)
		{
			++(characterPtr->_y);			
		}
		else if ((bulletDirection&3)==2)
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

#if YSize < 20
	#define MESSAGE_START 1
	#define MESSAGE_RANGE 7
#else
	#define MESSAGE_START 4
	#define MESSAGE_RANGE 15
#endif

#if defined(END_SCREEN)
void gameCompleted(void)
{

	level = 1;
	
	CLEAR_SCREEN();
	fillLevelWithCharacters(GHOSTS_NUMBER);	
	
	DRAW_BORDERS();

	playerFire = 0;
	skull._x = player._x-4;
	skull._y = player._y;	
	bullet._status = 0;
	guns = 1;
	while(!playerFire && !wallReached(&player))
	{
		++bulletDirection;
		displayBombs();
		for(ind=0;ind<GHOSTS_NUMBER;++ind)
		{
			dance(&ghosts[ind]);
		}
		
		displayPlayer(&player);
		
		dance(&skull);
	
		printCenteredMessageOnRow(MESSAGE_START+(bulletDirection&MESSAGE_RANGE),  YOU_MADE_IT_STRING);
		#if defined(SLOW_DOWN)
			for(ind=0;ind<254;++ind){};
		#endif
		printCenteredMessageOnRow(MESSAGE_START+(bulletDirection&MESSAGE_RANGE), "             ");
				
		MOVE_PLAYER();
	}

	// printPressKeyToStart();
	// WAIT_PRESS();
	playerFire = 0;
}
#endif

#endif

