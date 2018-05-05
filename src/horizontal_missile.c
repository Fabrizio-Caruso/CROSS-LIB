#if defined(FULL_GAME) 

#include "level.h"
#include "strategy.h"
#include "display_macros.h"

extern unsigned short loop;
extern unsigned char level;

extern Character leftEnemyMissile;
extern Character rightEnemyMissile;
extern Character player;

extern unsigned char arrowRange;
unsigned char arrowYPosition;

	unsigned char _playerInArrowRange()
	{
		return (loop&1 && player._y<=(arrowYPosition+arrowRange) && player._y>=(arrowYPosition-arrowRange));
	}
	
	void _handle_enemy_missile_from_the_left(void)
	{
		if(leftEnemyMissile._status)
		{
			deleteMissile(&leftEnemyMissile);
			if(leftEnemyMissile._x==XSize-2)
			{
				leftEnemyMissile._x=0;
				leftEnemyMissile._y =  arrowYPosition;
			}
			else
			{
				ADVANCED_LEFT_MISSILE();
				if(_playerInArrowRange())
				{
					if(player._x>=leftEnemyMissile._x)
					{
						move(&leftEnemyMissile, &player, Y_MOVE);			
					}
				}
			}
			displayMissile(&leftEnemyMissile);
			
			if(playerKilledBy(&leftEnemyMissile))
			{
				playerDies();
			}
		}		
	}
	
	void _handle_enemy_missile_from_the_right(void)
	{
		if(rightEnemyMissile._status)
		{
			deleteMissile(&rightEnemyMissile);
			if(rightEnemyMissile._x==1)
			{
				rightEnemyMissile._x= XSize-1;
				rightEnemyMissile._y = arrowYPosition;
			}
			else
			{
				ADVANCED_RIGHT_MISSILE();
				if(_playerInArrowRange())
				{
					if(player._x<= rightEnemyMissile._x)	
					{
						move(&rightEnemyMissile, &player,Y_MOVE);			
					}
				}
			}
			displayMissile(&rightEnemyMissile);	
			
			if(playerKilledBy(&rightEnemyMissile))
			{
				playerDies();
			}
		}		
	}
	
	void handle_enemy_missiles(void)
	{	
		if(oneMissileLevel())
		{
			arrowYPosition = YSize/2;
			_handle_enemy_missile_from_the_right();
		}	
		else if(missileLevel() || bossLevel())
		{
			arrowYPosition = ENEMY_MISSILE_OFFSET;
			_handle_enemy_missile_from_the_right();
			
			arrowYPosition = YSize-1-ENEMY_MISSILE_OFFSET; 
			_handle_enemy_missile_from_the_left();
		}

	}
	
#endif // defined(FULL_GAME)

