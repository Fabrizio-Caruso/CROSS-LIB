
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
	extern Character skulls[];
	extern Character ghosts[GHOSTS_NUMBER];
	extern Character bullet;
	extern uint8_t playerFire;
	extern uint8_t level;
	extern uint8_t guns;
	extern uint8_t ind;
    
    extern uint8_t maxGhostsOnScreen;
    
    extern uint8_t isBossLevel;
    extern uint8_t innerVerticalWallLength;
#endif


#if YSize < 20
	#define MESSAGE_START 3
	#define MESSAGE_RANGE 7
#else
	#define MESSAGE_START 7
	#define MESSAGE_RANGE 15
#endif

#if defined(END_SCREEN)
	void gameCompleted(void)
	{
		
		_XL_CLEAR_SCREEN();
        ghostCount = 8;
        maxGhostsOnScreen = 8;
        isBossLevel = 0;
		fillLevelWithCharacters();

		// skulls[0]._x = player._x-4;
		// skulls[0]._y = player._y;

        for(ind=0;ind<maxGhostsOnScreen;++ind)
        {
            displayCharacter(&ghosts[ind]);
        }
        
        displayPlayer(&player);
        // displaySkull(&skulls[0]);
    
        _XL_PRINT(XSize/2-3, MESSAGE_START,  YOU_MADE_IT_STRING);

        _XL_WAIT_FOR_INPUT();
        
        #if defined(BETWEEN_LEVEL)
            spiral(&player, 2*MIN_SIZE-18);
            // _XL_SLEEP(1);
        #endif
	}
#endif


