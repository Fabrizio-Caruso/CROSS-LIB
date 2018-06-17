#if defined(FULL_GAME)

#include "../cross_lib/display/display_macros.h"

#include "settings.h"
#include "character.h"
#include "level.h"

extern Character rockets[ROCKETS_NUMBER];

extern unsigned char ghostCount;

	void handle_rockets(void)
	{	
		if((ghostCount<=MAX_GHOST_COUNT_FOR_ROCKETS && rocketLevel()) || bossLevel())
		{ 
			unsigned char i;

			for(i=0;i<ROCKETS_NUMBER;++i)
			{
				if(rockets[i]._status)
				{
					if(playerKilledBy(&rockets[i]))
					{
						playerDies();
					}
					
					#if defined(TURNED_BASED)
					if(1)
					#else
					if(rand()&1)
					#endif
					{
						deleteMissile(&rockets[i]);
						--(rockets[i]._y);
					}

					displayMissile(&rockets[i]);			
					if(rockets[i]._y<=1)
					{	
						deleteMissile(&rockets[i]);
						//
						rockets[i]._x = (i+1)*(XSize/(ROCKETS_NUMBER+1));
						rockets[i]._y = YSize-2;							
					}
				}
			}
		}
	}


#endif // defined(FULL_GAME)

