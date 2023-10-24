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
	extern uint8_t playerFire;
	extern uint8_t level;
	extern uint8_t guns;
	extern uint8_t ind;
#endif



#if defined(END_SCREEN) || defined(DANCE)
	extern uint8_t bulletDirection;

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
	#define MESSAGE_START 5
#else
	#define MESSAGE_START 6
#endif

#if defined(END_SCREEN)
	void gameCompleted(void)
	{
		level = 0;
		
		_XL_CLEAR_SCREEN();
        ghostCount = GHOSTS_NUMBER;
		fillLevelWithCharacters();
        displayScore();

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
		
			PRINT_CENTERED_ON_ROW(MESSAGE_START,  YOU_MADE_IT_STRING);		
			#if _XL_SLOW_DOWN_FACTOR>0
				for(ind=0;ind<253U;++ind){};
			#endif
					
			MOVE_PLAYER();
		}
	}
#endif

#endif

