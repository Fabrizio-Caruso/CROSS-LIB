#ifndef _VARIABLES_H
#define _VARIABLES_H

#if defined(Z88DK_JOYSTICK)
	unsigned char stick;
#endif


#if !defined(TINY_GAME) || defined(TURN_BASED)
	#if defined(__VIC20__) || defined(__C16__)
		extern unsigned short loop;
	#else
		unsigned short loop;	
	#endif	
#endif


#if defined(FULL_GAME) && !defined(NO_BLINKING)
	#if defined(__VIC20__) || defined(__C16__)
		extern unsigned char playerBlink;	
	#else
		unsigned char playerBlink;
	#endif
#endif


#if !defined(TINY_GAME) 
	#if defined(__VIC20__) || defined(__C16__)
		extern unsigned short ghostLevel;
	#else
		unsigned short ghostLevel;
	#endif
#endif


#if !defined(TINY_GAME)
	#if defined(__VIC20__) || defined(__C16__)
		extern unsigned short skullSlowDown;
		extern unsigned char skullXCountDown;
		extern unsigned char skullYCountDown;
		extern unsigned char bulletDirection;		
	#else
		unsigned short skullSlowDown;		
		unsigned char skullXCountDown;
		unsigned char skullYCountDown;
		unsigned char bulletDirection;
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
	#if defined(__VIC20__) || defined(__C16__)
		extern Character skull;
		extern Character bullet;	

		extern Item powerUp;
		extern Item powerUp2;
		extern Item gun;
		extern Item extraPoints;	
	#else
		Character skull;
		Item powerUp;
		Item powerUp2;
		Item gun;
		Item extraPoints;	
		
		Character bullet;
	#endif
#endif

#if defined(__VIC20__) || defined(__C16__) || defined(__GAL__)
	extern Character ghosts[];
	extern Character bombs[];
	
	extern Character player;
	
	extern unsigned short ghostSlowDown;
	extern unsigned short points;
	extern unsigned short highScore;

	extern unsigned char lives;
	extern unsigned char level;	
	
	extern unsigned char ghostCount; // = GHOSTS_NUMBER;

#else
	Character ghosts[GHOSTS_NUMBER];
	Character bombs[BOMBS_NUMBER];

	Character player; 

	unsigned short ghostSlowDown;
	unsigned short points;
	unsigned short highScore;	
	
	unsigned char lives;
	unsigned char level;	
	
	unsigned char ghostCount; // = GHOSTS_NUMBER;
	
#endif

#if defined(FULL_GAME)

	#if defined(__VIC20__) || defined(__C16__)
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
		extern unsigned char rockets_x[];	

		extern unsigned char extraLife_present_on_level;
		extern unsigned char super_present_on_level;
		extern unsigned char zombie_present_on_level;
		
		extern unsigned char dead_rockets;
		
		extern unsigned char arrowRange;
		
		extern unsigned char skullsKilled;
		
		extern unsigned char missileBasesDestroyed;
		
		extern unsigned char horizontalWallsLength;
		
		extern unsigned char invincibilityActive;
		
		extern unsigned char confuseActive; 
		
		extern unsigned char zombieActive; 		
		
		extern unsigned char innerVerticalWallY; 
		extern unsigned char innerVerticalWallX; 
		extern unsigned char innerVerticalWallLength;

	#else
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
		unsigned char rockets_x[ROCKETS_NUMBER];		
		
		unsigned char extraLife_present_on_level;
		unsigned char super_present_on_level;
		unsigned char zombie_present_on_level;
		
		unsigned char dead_rockets;
		
		unsigned char arrowRange;
		
		unsigned char skullsKilled;
		
		unsigned char missileBasesDestroyed;
		
		unsigned char horizontalWallsLength;
		
		unsigned char invincibilityActive;
		
		unsigned char confuseActive; 
		
		unsigned char zombieActive; 


		unsigned char innerVerticalWallY; 
		unsigned char innerVerticalWallX; 
		unsigned char innerVerticalWallLength;
		
	#endif

	#define chase_present_on_level_condition skullsKilled
	#define confuse_present_on_level_condition missileBasesDestroyed
	
	#if defined(__VIC20__) || defined(__C16__)
		extern Character rockets[];

		extern unsigned short invincibility_count_down;	
	
		extern unsigned short confuse_count_down;
	
		extern unsigned short zombie_count_down;		
	#else
		Character rockets[ROCKETS_NUMBER];
	
		unsigned short invincibility_count_down;	
	
		unsigned short confuse_count_down;
	
		unsigned short zombie_count_down;	
	#endif
	
#endif

#if !defined(TINY_GAME)
	#if defined(__VIC20__) || defined(__C16__)
		extern unsigned char strategyArray[GHOSTS_NUMBER];	
		extern unsigned short freeze_count_down;
		
		extern unsigned char playerDirection; // 0: right, 1: down, 2: left, 3: up
		extern unsigned char freezeActive; // freezeActive			
		extern unsigned char extraLifeThroughPointsCounter;			
	#else
		unsigned char strategyArray[GHOSTS_NUMBER];
		unsigned short freeze_count_down;
		
		unsigned char playerDirection; // 0: right, 1: down, 2: left, 3: up
		unsigned char freezeActive; // freezeActive	
		unsigned char extraLifeThroughPointsCounter;			
	#endif


#endif


#if !defined(TINY_GAME)
	#if defined(__VIC20__) || defined(__C16__)
		extern unsigned char playerFire;
		extern unsigned char guns; 
		extern unsigned char skullHits;
		extern unsigned char skullAlive;		
	#else
		unsigned char playerFire;
		unsigned char guns; 
		unsigned char skullHits;
		unsigned char skullAlive;	
	#endif
#endif

#if !defined(TINY_GAME)
	#if defined(__VIC20__) || defined(__C16__)
		extern unsigned char arrowYPosition;
	#else
		unsigned char arrowYPosition;
	#endif
#endif

#endif // _VARIABLES_H
