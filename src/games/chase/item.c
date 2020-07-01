#include "settings.h"

#include "character.h"
#include "item.h"
#include "ghost.h"
#include "game_text.h"

extern uint16_t points;

extern uint8_t guns;
extern uint8_t lives;

extern uint8_t level;

extern uint8_t freezeActive;
extern uint16_t freeze_count_down;


#if defined(FULL_GAME)
	extern uint8_t invincibilityActive;
	extern uint8_t confuseActive;
	extern uint8_t zombieActive;
	
	extern uint16_t invincibility_count_down;
	extern uint16_t confuse_count_down;
	extern uint16_t zombie_count_down;
	
	extern uint8_t missileBasesDestroyed;
	extern uint8_t skullsKilled;	
	
	extern uint8_t extraLife_present_on_level;
	extern uint8_t zombie_present_on_level;
#endif

extern Image DEAD_GHOST_IMAGE;

extern Character ghosts[GHOSTS_NUMBER];
extern Character bombs[BOMBS_NUMBER];
extern Character skull;

extern Character player;

extern Item powerUp;
extern Item gun;
extern Item extraPoints;


#if defined(FULL_GAME)
	extern Character *chasedEnemyPtr;
	extern Character chasingBullet;

	extern Item powerUp2;	
	
	extern Item freeze;

	extern Item invincibility;

	extern Item chase;

	extern Item super;
	extern Item extraLife;

	extern Item confuse;
	extern Item zombie;
#endif
	
#if !defined(TINY_GAME)

	void itemReached(Character * itemPtr)
	{
		ZAP_SOUND();
		#if defined(TURN_BASED)
			displayPlayer(&player);
		#endif
		itemPtr->_status = 0;
		displayStats();
	}

	void relocateItem(Character * itemPtr)
	{
			itemPtr->_status = 1;
			
			#if defined(FULL_GAME)
			do
			{
				relocateCharacter(itemPtr);
			} while(innerWallReached(itemPtr));		
			#else
				relocateCharacter(itemPtr);
			#endif	
	}	
	
	#if defined(FULL_GAME)
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
			powerUp._coolDown = POWER_UP_COOL_DOWN;		
		}

		void _gunEffect(void)
		{
			guns = GUNS_NUMBER;
			#if !defined(NO_STATS)
			printGunsStats();	
			#endif
			points+=GUN_BONUS;		
		}

		void gunEffect(void)
		{
			_gunEffect();
			gun._coolDown = GUN_COOL_DOWN;	
		}
	#else

		void powerUpEffect(void)
		{
			points+=POWER_UP_BONUS;
			freezeActive = 1;	
			freeze_count_down += FROZEN_COUNT_DOWN;	
			powerUp._coolDown = POWER_UP_COOL_DOWN;		
		}

		void gunEffect(void)
		{
			guns = GUNS_NUMBER;
			#if !defined(NO_STATS)
			printGunsStats();	
			#endif
			points+=GUN_BONUS;	
			gun._coolDown = GUN_COOL_DOWN;	
		}		
	#endif

	void extraPointsEffect(void)
	{
		points+=EXTRA_POINTS+level*EXTRA_POINTS_LEVEL_INCREASE;
		extraPoints._coolDown = SECOND_EXTRA_POINTS_COOL_DOWN;//(EXTRA_POINTS_COOL_DOWN<<4); // second time is harder		
	}

	void handle_item(register Item *itemPtr)
	{
		// Manage item
		if(itemPtr->_character._status == 1)
		{	
			if(areCharctersAtSamePosition(&player, (Character *) itemPtr))
			{
				itemPtr->_effect();
				itemReached((Character *) itemPtr);			
			}
			else
			{
				_blink_draw(itemPtr->_character._x, itemPtr->_character._y, itemPtr->_character._imagePtr, &(itemPtr->_blink));
			}		
		}
		else if (itemPtr->_coolDown == 0)
		{
			relocateItem((Character *) itemPtr);
		}
		else
		{
			--(itemPtr->_coolDown);
		}
	}

	void handle_count_down(uint8_t * activeItemFlagPtr, uint16_t * countDownPtr)
	{
		if(*activeItemFlagPtr)
		{
			if(*countDownPtr<=0)
			{
				*activeItemFlagPtr=0;
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
	void powerUp2Effect(void)
	{
		_commonPowerUpEffect();
		powerUp2._coolDown = POWER_UP2_COOL_DOWN;	
	}

	void _freezeEffect(void)
	{
		_commonPowerUpEffect();
		_commonPowerUpEffect();
		_commonPowerUpEffect();
	}

	void freezeEffect(void)
	{
		_freezeEffect();
		freeze._coolDown = ((uint16_t) (FREEZE_COOL_DOWN)*2);	
	}
	
	
	void extraLifeEffect(void)
	{
		++lives;
		skullsKilled=1;
		extraLife_present_on_level = 0;
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
		invincibility._coolDown = ((uint16_t) (INVINCIBILITY_COOL_DOWN)*4);
	}
	
	void superEffect(void)
	{
		_freezeEffect();
		_gunEffect();
		_invincibilityEffect();
		super._coolDown = ((uint16_t) (SUPER_COOL_DOWN)*8);
	}

	void confuseEffect(void)
	{
		confuseActive = 1;
		confuse._coolDown = SECOND_CONFUSE_COOL_DOWN;
		confuse_count_down = CONFUSE_COUNT_DOWN;
	}
	
	void zombieEffect(void)
	{
		uint8_t i;
		
		zombieActive = 1;
		missileBasesDestroyed = 1;
		zombie._coolDown = SECOND_ZOMBIE_COOL_DOWN; 
		zombie_count_down = ZOMBIE_COUNT_DOWN;
		for(i=0;i<GHOSTS_NUMBER;++i)
		{
			if(!(ghosts[i]._status))
			{
				ghosts[i]._imagePtr = &DEAD_GHOST_IMAGE;
			}
		}
	}
	
	void chaseEffect(void)
	{
		unsigned firstAliveIndex;
		
		chasingBullet._status = 1;
		chasingBullet._x = chase._character._x;
		chasingBullet._y = chase._character._y;
		
		chase._coolDown = ((uint16_t)(CHASE_COOL_DOWN)*2);
		
		firstAliveIndex = firstAlive();
		if(firstAliveIndex == GHOSTS_NUMBER)
		{
			chasedEnemyPtr = &skull;
		}
		else
		{
			chasedEnemyPtr = &ghosts[firstAliveIndex];
		}
	}
#endif
