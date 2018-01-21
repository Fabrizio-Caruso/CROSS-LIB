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


	
	void _handle_enemy_missile_from_the_left(void)
	{
		if(!oneMissileLevel() && leftEnemyMissile._status)
		{
			deleteMissile(&leftEnemyMissile);
			if(leftEnemyMissile._x==XSize-2)
			{
				leftEnemyMissile._x=0;
				leftEnemyMissile._y = YSize-1-ENEMY_MISSILE_OFFSET;
			}
			else
			{
				ADVANCED_LEFT_MISSILE();
				if(loop&1 && player._y>=YSize-1-ENEMY_MISSILE_OFFSET-arrowRange && player._x>=leftEnemyMissile._x)
				{
					move(&leftEnemyMissile, &player,Y_MOVE);			
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
				if(oneMissileLevel())
				{
					rightEnemyMissile._y = (YSize>>1);					
				}
				else
				{
					rightEnemyMissile._y = ENEMY_MISSILE_OFFSET;
				}
			}
			else
			{
				ADVANCED_RIGHT_MISSILE();
				if((loop&1 && (player._x<= rightEnemyMissile._x)) && ((!oneMissileLevel() && player._y<=ENEMY_MISSILE_OFFSET+arrowRange) || (oneMissileLevel() && player._y<=YSize/2+arrowRange && player._y>=YSize/2-arrowRange)))			
				{
					move(&rightEnemyMissile, &player,Y_MOVE);			
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
		if(missileLevel() || bossLevel() || oneMissileLevel())
		{
			_handle_enemy_missile_from_the_left();	
			_handle_enemy_missile_from_the_right();
		}	
	}
	
#endif // defined(FULL_GAME)

