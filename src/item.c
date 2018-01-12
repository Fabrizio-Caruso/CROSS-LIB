#include "character.h"
#include "item.h"
#include "enemy.h"
#include "display_macros.h"
#include "settings.h"
#include "text.h"
#include "sound_macros.h"


extern unsigned int points;
extern unsigned char freezeActive;
extern unsigned char invincibilityActive;
extern unsigned char confuseActive;
extern unsigned char zombieActive;
extern unsigned char freeze_count_down;
extern unsigned char guns;
extern unsigned char lives;
extern unsigned char missileBasesDestroyed;
extern unsigned char skullsKilled;
extern unsigned char level;
extern unsigned char freeze_count_down;
extern unsigned char invincibility_count_down;
extern unsigned char confuse_count_down;
extern unsigned char zombie_count_down;

extern Image DEAD_GHOST_IMAGE;

extern Character ghosts[GHOSTS_NUMBER];
extern Character bombs[BOMBS_NUMBER];
extern Character chasingBullet;
extern Character invincibleGhost;
extern Character *chasedEnemyPtr;
extern Character player;

extern Item powerUp;
extern Item powerUp2;
extern Item gun;
extern Item extraPoints;

extern Item freeze;
extern Item invincibility;

extern Item chase;

extern Item super;
extern Item extraLife;

extern Item confuse;
extern Item zombie;
	
#if !defined(TINY_GAME)
	

	void powerUpReached(Character * powerUpPtr)
	{
		ZAP_SOUND();
		DELETE_GUN(powerUpPtr->_x,powerUpPtr->_y,powerUpPtr->_imagePtr);
		DRAW_PLAYER(player._x, player._y, player._imagePtr);
		powerUpPtr->_status = 0;
		displayStats();
	}

	void relocatePowerUp(Character * powerUpPtr)
	{
			powerUpPtr->_status = 1;
			
			#if defined(FULL_GAME)
			do
			{
				relocateCharacter(powerUpPtr, bombs,4);
			} while(nearInnerWall(powerUpPtr));		
			#else
				relocateCharacter(powerUpPtr, bombs,4);
			#endif	
	}	
	
	void _commonPowerUpEffect(void)
	{
		points+=POWER_UP_BONUS;
		decreaseGhostLevel();
		freezeActive = 1;	
		freeze_count_down += FROZEN_COUNT_DOWN;	
	}

	void powerUpEffect(void)
	{
		_commonPowerUpEffect();
		powerUp._coolDown = POWER_UP_INITIAL_COOLDOWN;		
	}

	void powerUp2Effect(void)
	{
		_commonPowerUpEffect();
		powerUp2._coolDown = POWER_UP2_INITIAL_COOLDOWN;	
	}

	void _gunEffect(void)
	{
		guns = GUNS_NUMBER;
		printGunsStats();		
		points+=GUN_BONUS;		
	}

	void gunEffect(void)
	{
		_gunEffect();
		gun._coolDown = GUN_INITIAL_COOLDOWN;	
	}

	void extraPointsEffect(void)
	{
		points+=EXTRA_POINTS+level*EXTRA_POINTS_LEVEL_INCREASE;
		extraPoints._coolDown = EXTRA_POINTS_COOL_DOWN*2; // second time is harder		
	}

	void handle_item(Item *itemPtr)
	{
		// Manage item
		if(itemPtr->_character._status == 1)
		{	
			if(areCharctersAtSamePosition(&player, (Character *) itemPtr))
			{
				itemPtr->_effect();
				powerUpReached((Character *) itemPtr);			
			}
			else
			{
				_blink_draw(itemPtr->_character._x, itemPtr->_character._y, itemPtr->_character._imagePtr, &(itemPtr->_blink));
			}		
		}
		else if (itemPtr->_coolDown <= 0)
		{
			relocatePowerUp((Character *) itemPtr);

			_blink_draw(itemPtr->_character._x, itemPtr->_character._y, itemPtr->_character._imagePtr, &(itemPtr->_blink));
		}
		else
		{
			--itemPtr->_coolDown;
		}
	}

	void handle_count_down(unsigned char * flagPtr, unsigned short * countDownPtr)
	{
		if(*flagPtr)
		{
			if(*countDownPtr<=0)
			{
				*flagPtr=0;
			}
			else
			{
				--(*countDownPtr);
			}
		}
	}	
	
#endif // !defined(TINY_GAME)

	
#if defined(FULL_GAME)
	void reducePowerUpsCoolDowns(void)
	{
		extraPoints._coolDown/=2;
		invincibility._coolDown/=2;
		freeze._coolDown/=2;
		TICK_SOUND();		
	}
#elif !defined(TINY_GAME)
	void reducePowerUpsCoolDowns(void)
	{
		extraPoints._coolDown/=2;
		TICK_SOUND();		
	}
#else	
#endif	


	
#if defined(FULL_GAME)

	void _freezeEffect(void)
	{
		unsigned char i;
		for(i=0;i<3;++i)
		{
			_commonPowerUpEffect();
		}	
	}

	void freezeEffect(void)
	{
		_freezeEffect();
		freeze._coolDown = FREEZE_INITIAL_COOLDOWN*2;	
	}
	
	
	void extraLifeEffect(void)
	{
		++lives;
		skullsKilled=1;
		missileBasesDestroyed = 0;
		// extraLife._coolDown = EXTRA_LIFE_COOL_DOWN*10; // second time is harder
		printLivesStats();		
	}

	void _invincibilityEffect(void)
	{
		invincibilityActive = 1;
		invincibility_count_down = INVINCIBILITY_COUNT_DOWN;			
	}
	
	void invincibilityEffect(void)
	{
		_invincibilityEffect();
		invincibility._coolDown = INVINCIBILITY_COOL_DOWN;		
	}
	
	void superEffect(void)
	{
		_freezeEffect();
		_gunEffect();
		_invincibilityEffect();
		super._coolDown = SUPER_COOL_DOWN*10;
	}

	void confuseEffect(void)
	{
		confuseActive = 1;
		confuse._coolDown = CONFUSE_COOL_DOWN*20;
		confuse_count_down = CONFUSE_COUNT_DOWN;
	}
	
	void zombieEffect(void)
	{
		unsigned char i;
		
		zombieActive = 1;
		zombie._coolDown = ZOMBIE_COOL_DOWN;
		zombie_count_down = ZOMBIE_COUNT_DOWN;
		for(i=ghostCount;i<GHOSTS_NUMBER;++i)
		{
			ghosts[i]._imagePtr = &DEAD_GHOST_IMAGE;
			// TODO: Implement some better point system for this item
			// points+=ZOMBIE_BONUS;
			// ZAP_SOUND();
			// sleep(1);
		}
	}
	
	void chaseEffect(void)
	{
		unsigned firstAliveIndex;
		
		chasingBullet._status = 1;
		chasingBullet._x = chase._character._x;
		chasingBullet._y = chase._character._y;
		
		chase._coolDown = CHASE_COOL_DOWN*2;
		
		firstAliveIndex = firstAlive();
		if(firstAliveIndex == GHOSTS_NUMBER)
		{
			chasedEnemyPtr = &invincibleGhost;
		}
		else
		{
			chasedEnemyPtr = &ghosts[firstAliveIndex];
		}
	}
#endif