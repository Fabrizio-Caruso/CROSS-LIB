#if defined(FULL_GAME)

#include "../cross_lib/display/display_macros.h"

#include "settings.h"
#include "character.h"
#include "level.h"

extern Character bubbles[ROCKETS_NUMBER];

extern unsigned char ghostCount;

	void handle_rockets(void)
	{	
		if((ghostCount<=MAX_GHOST_COUNT_FOR_ROCKETS && rocketLevel()) || bossLevel())
		{ 
			unsigned char i;

			for(i=0;i<ROCKETS_NUMBER;++i)
			{
				if(bubbles[i]._status)
				{
					if(playerKilledBy(&bubbles[i]))
					{
						playerDies();
					}
					
					#if defined(TURNED_BASED)
					if(1)
					#else
					if(rand()&1)
					#endif
					{
						deleteMissile(&bubbles[i]);
						--(bubbles[i]._y);
					}

					displayMissile(&bubbles[i]);			
					if(bubbles[i]._y<=1)
					{	
						deleteMissile(&bubbles[i]);
						//
						bubbles[i]._x = (i+1)*(XSize/(ROCKETS_NUMBER+1));
						bubbles[i]._y = YSize-2;							
					}
				}
			}
		}
	}


#endif // defined(FULL_GAME)

