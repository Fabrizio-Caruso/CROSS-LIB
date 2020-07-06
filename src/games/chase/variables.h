#ifndef _VARIABLES_H
#define _VARIABLES_H

#if defined(BENCHMARK)
    #if !defined(BENCHMARK_MAX)
        #define BENCHMARK_MAX 70
    #endif
    
    clock_t Ticks, TicksDelta;
    unsigned int Sec;
    unsigned int Milli; 

    uint16_t benchmark_count;
#endif

#if defined(Z88DK_JOYSTICK)
	uint8_t stick;
#endif


#if !defined(TINY_GAME) || defined(TURN_BASED)
	#if defined(USE_MEMORY_BUFFERS) || defined(USE_LIGHT_MEMORY_BUFFERS) || (defined(USE_TINY_MEMORY_BUFFERS) && defined(TURN_BASED))
		extern uint16_t loop;
	#else
		uint16_t loop;	
	#endif	
#endif


#if defined(FULL_GAME) && !defined(NO_BLINKING)
	#if defined(USE_MEMORY_BUFFERS)
		extern uint8_t playerBlink;	
	#else
		uint8_t playerBlink;
	#endif
#endif


#if defined(FULL_GAME) 
	#if defined(USE_MEMORY_BUFFERS)
		extern uint16_t ghostLevel;
	#else
		uint16_t ghostLevel;
	#endif
#endif


#if !defined(TINY_GAME)
	#if defined(USE_MEMORY_BUFFERS) || defined(USE_LIGHT_MEMORY_BUFFERS)
		extern uint16_t skullSlowDown;
		extern uint8_t skullXCountDown;
		extern uint8_t skullYCountDown;
		extern uint8_t bulletDirection;		
	#else
		uint16_t skullSlowDown;		
		uint8_t skullXCountDown;
		uint8_t skullYCountDown;
		uint8_t bulletDirection;
	#endif

#endif

#if !defined(TINY_GAME)
	extern Image SKULL_IMAGE;
	extern Image POWERUP_IMAGE;
	extern Image BULLET_IMAGE;
	extern Image GUN_IMAGE;
	extern Image EXTRA_POINTS_IMAGE;	
	extern Image DEAD_GHOST_IMAGE;
#endif


#if !defined(TINY_GAME)
	#if defined(USE_MEMORY_BUFFERS) || defined(USE_LIGHT_MEMORY_BUFFERS)
		extern Character skull;
		extern Character bullet;	

		extern Item powerUp;
		extern Item gun;
		extern Item extraPoints;	
	#else
		Character skull;
		Item powerUp;
		Item gun;
		Item extraPoints;	
		
		Character bullet;
	#endif
#endif

#if defined(USE_MEMORY_BUFFERS) || defined(USE_TINY_MEMORY_BUFFERS) || defined(USE_LIGHT_MEMORY_BUFFERS)
	extern Character ghosts[];
	extern Character bombs[];
	
	extern Character player;
	
	extern uint16_t ghostSlowDown;
	extern uint16_t points;
	extern uint16_t highScore;

	extern uint8_t lives;
	extern uint8_t level;	
	
	extern uint8_t ghostCount; // = GHOSTS_NUMBER;
	
	extern uint8_t ind;

#else
	Character ghosts[GHOSTS_NUMBER];
	Character bombs[BOMBS_NUMBER];

	Character player; 

	uint16_t ghostSlowDown;
	uint16_t points;
	uint16_t highScore;	
	
	uint8_t lives;
	uint8_t level;	
	
	uint8_t ghostCount; // = GHOSTS_NUMBER;
	
	uint8_t ind;
	
#endif

#if defined(FULL_GAME)

	#if defined(USE_MEMORY_BUFFERS)
		extern Item powerUp2;
		extern Item freeze;
		extern Item invincibility;
		extern Item extraLife;
		extern Item super;
		extern Item confuse;
		extern Item zombie;
		extern Item chase;	

		extern Character leftHorizontalMissile;
		extern Character rightHorizontalMissile;
		extern Character chasingBullet;

		extern Character * chasedEnemyPtr;
		extern Character * chasedByGhosts;
		extern uint8_t rockets_x[];

		extern uint8_t extraLife_present_on_level;
		extern uint8_t super_present_on_level;
		extern uint8_t zombie_present_on_level;
		
		extern uint8_t dead_rockets;
		
		extern uint8_t arrowRange;
		
		extern uint8_t skullsKilled;
		
		extern uint8_t missileBasesDestroyed;
		
		extern uint8_t horizontalWallsLength;
		
		extern uint8_t invincibilityActive;
		
		extern uint8_t confuseActive; 
		
		extern uint8_t zombieActive; 		
		
		extern uint8_t innerVerticalWallY; 
		extern uint8_t innerVerticalWallX; 
		extern uint8_t innerVerticalWallLength;

	#else
		Item powerUp2;		
		Item freeze;
		Item invincibility;
		Item extraLife;
		Item super;
		Item confuse;
		Item zombie;
		Item chase;
		
		Character leftHorizontalMissile;
		Character rightHorizontalMissile;
		Character chasingBullet;			
		
		Character * chasedEnemyPtr;
		Character * chasedByGhosts;
		uint8_t rockets_x[ROCKETS_NUMBER];		
		
		uint8_t extraLife_present_on_level;
		uint8_t super_present_on_level;
		uint8_t zombie_present_on_level;
		
		uint8_t dead_rockets;
		
		uint8_t arrowRange;
		
		uint8_t skullsKilled;
		
		uint8_t missileBasesDestroyed;
		
		uint8_t horizontalWallsLength;
		
		uint8_t invincibilityActive;
		
		uint8_t confuseActive; 
		
		uint8_t zombieActive; 


		uint8_t innerVerticalWallY; 
		uint8_t innerVerticalWallX; 
		uint8_t innerVerticalWallLength;
		
	#endif

	#define chase_present_on_level_condition skullsKilled
	#define confuse_present_on_level_condition missileBasesDestroyed
	
	#if defined(USE_MEMORY_BUFFERS)
		extern Character rockets[];

		extern uint8_t invincibility_count_down;	
	
		extern uint8_t confuse_count_down;
	
		extern uint8_t zombie_count_down;		
	#else
		Character rockets[ROCKETS_NUMBER];
	
		uint8_t invincibility_count_down;	
	
		uint8_t confuse_count_down;
	
		uint8_t zombie_count_down;	
	#endif
	
#endif

#if defined(FULL_GAME) && !defined(SIMPLE_STRATEGY)
	#if defined(USE_MEMORY_BUFFERS)
		extern uint8_t strategyArray[GHOSTS_NUMBER];	
	#else
		uint8_t strategyArray[GHOSTS_NUMBER];
	#endif	
#endif

#if !defined(TINY_GAME)
	#if defined(USE_MEMORY_BUFFERS) || defined(USE_LIGHT_MEMORY_BUFFERS)
		extern uint8_t freeze_count_down;
		
		extern uint8_t playerDirection; // 0: right, 1: down, 2: left, 3: up
		extern uint8_t freezeActive; // freezeActive			
		extern uint8_t extraLifeThroughPointsCounter;			
	#else
		uint8_t freeze_count_down;
		
		uint8_t playerDirection; // 0: right, 1: down, 2: left, 3: up
		uint8_t freezeActive; // freezeActive	
		uint8_t extraLifeThroughPointsCounter;			
	#endif


#endif


#if !defined(TINY_GAME)
	#if defined(USE_MEMORY_BUFFERS) || defined(USE_LIGHT_MEMORY_BUFFERS)
		extern uint8_t playerFire;
		extern uint8_t guns; 
		extern uint8_t skullActive;		
	#else
		uint8_t playerFire;
		uint8_t guns; 
		uint8_t skullActive;	
	#endif
#endif

#if defined(FULL_GAME)
	#if defined(USE_MEMORY_BUFFERS)
		extern uint8_t arrowYPosition;
	#else
		uint8_t arrowYPosition;
	#endif
#endif

#if defined(FULL_GAME)
    uint8_t isBossLevel;
    uint8_t isOneMissileLevel;
    uint8_t isMissileLevel;
    uint8_t isRocketLevel;
    uint8_t isHorizontalWallsLevel;
    
    uint16_t levelSlowDown;
#endif

#endif // _VARIABLES_H
