#if defined(FULL_GAME)


#include "settings.h"
#include "character.h"
#include "level.h"


#include "cross_lib.h"


extern Character rockets[ROCKETS_NUMBER];

extern uint8_t ghostCount;
extern uint8_t isRocketLevel;
extern uint8_t isBossLevel;

	void handle_rockets(void)
	{	
		if((ghostCount<=MAX_GHOST_COUNT_FOR_ROCKETS && isRocketLevel) || isBossLevel)
		{ 
			uint8_t i;

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
					if(_XL_RAND()&1)
					#endif
					{
						deleteRocket(&rockets[i]);
						--(rockets[i]._y);
					}

					displayRocket(&rockets[i]);			
					if(rockets[i]._y<=1)
					{	
						deleteRocket(&rockets[i]);
						rockets[i]._x = (1+i)*(XSize/(ROCKETS_NUMBER+1));
						rockets[i]._y = YSize-2;							
					}
				}
			}
		}
	}


#endif // defined(FULL_GAME)

