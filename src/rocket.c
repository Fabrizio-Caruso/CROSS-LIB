#if defined(FULL_GAME)

#include "character.h"
#include "level.h"
#include "display_macros.h"

extern Character bubbles[BUBBLES_NUMBER];

extern unsigned char ghostCount;

	void handle_rockets(void)
	{	
		if((ghostCount<=MAX_GHOST_COUNT_FOR_BUBBLES && rocketLevel()) || bossLevel())
		{ 
			unsigned char i;

			for(i=0;i<BUBBLES_NUMBER;++i)
			{
				if(bubbles[i]._status)
				{
					if(playerKilledBy(&bubbles[i]))
					{
						playerDies();
					}
					
					if(rand()&1)
					{
						DELETE_MISSILE(bubbles[i]._x, bubbles[i]._y, bubbles[i]._imagePtr);					
						--(bubbles[i]._y);
					}

					DRAW_MISSILE(bubbles[i]._x, bubbles[i]._y, bubbles[i]._imagePtr);			
					if(bubbles[i]._y<=1)
					{	
						DELETE_MISSILE(bubbles[i]._x, bubbles[i]._y, bubbles[i]._imagePtr);
						//
						bubbles[i]._x = (i+1)*(XSize/(BUBBLES_NUMBER+1));
						bubbles[i]._y = YSize-2;							
					}
				}
			}
		}
	}


#endif // defined(FULL_GAME)