
// Code by Marcel van Tongeren
void vidclr(unsigned char * vidmem, int vidlen){ //write 0's to video memory
	asm( //vidmem pointer is R12, vidlen is R13
	"$$cpy:\n"
	" ldi 0 ;source a 0 for clearing the screen\n"
	" b1  $	;wait til video is quiet\n"
	" str R12 ;move the byte\n"
	" inc R12 ;++\n"
	" dec R13 ;decrement count\n"
	" glo R13 ; check bottom byte\n"
	" bnz $$cpy\n"
	" ghi R13 ;top byte\n"
	" bnz $$cpy\n");
	return;
}

void CLEAR_SCREEN(void)
{
    vidclr((unsigned char*)0xf800, 40*24);
}
// Code by Marcel van Tongeren
void vidchar(int vidmem, int character){ //write character to vidmem location in video memory
	asm(//vidmem pointer is R12, character is R13.0
	" glo r13\n"
	" b1  $	;wait til video is quiet\n"
	" str R12 ;move the byte\n");
	return;
}


void shapechar(const unsigned char * shapelocation, int number)
{
	asm( //shapelocation pointer is R12, number of shapes is R13
	" ldi 0xf7\n"
	" phi R8\n"
	" ldi 0xfb\n"
	" phi R9\n"
	"$$nextshape:\n"
	" ldi 0xC0 \n"
	" plo R8 ; R8 = charmem pointer\n"
	" plo r9 ; R9 = vidmem pointer\n"
	" ldi 9 \n"
	" plo R10 ; R10.0 = number of lines, we need to somehowe make a NTSC version for 8 lines\n"
	" lda 12\n" 
	" phi R10 ; R10.1 = character\n"
	"$$nextline:\n"
	" ghi R10\n"
	" b1  $	; wait til video is quiet\n"
	" str R9 ; store character in vidmem\n"
	" inc R9\n"
	" lda R12\n"
	" str R8 ; store first shape line in charmem\n"
	" inc R8\n"
	" dec R10\n"
	" glo R10\n"
	" bnz $$nextline ; number of lines - 1 if not zero do next line\n"
	" dec r13\n"
	" glo r13\n"
	" bnz $$nextshape\n");
}


void redefine_char(const unsigned char * shapelocation, int color)
{
    unsigned char colored_shape[10];
    unsigned char i;
    
    colored_shape[0] = *shapelocation;
    for(i=1;i<9;++i)
    {
       colored_shape[i]=shapelocation[i]+color*64;
    }
    colored_shape[9]=0;
    shapechar(colored_shape, 1);
}

#define __PLAYER_DOWN_UDG              12,18,12,51,45,12,18,51
#define __PLAYER_UP_UDG                12,30,12,51,45,12,18,51
#define __PLAYER_LEFT_UDG              12,22,12,62,13,12,20,20
#define __PLAYER_RIGHT_UDG             12,26,12,31,44,12,10,10


#define __GHOST_UDG                    33,30,33,51,33,45,33,30
#define __SKULL_UDG                    30,33,51,33,33,18,18,12
#define __GUN_UDG                       0,32,31,40,56,32, 0, 0
#define __POWERUP_UDG                   0,30,51,55,51,26,18,12
#define __BULLET_UDG                    0, 0, 4,28,14, 8, 0, 0
#define __BOMB_UDG                     30,33,51,45,45,51,33,30

const unsigned char player_down[10] = {  97, __PLAYER_DOWN_UDG, 0 };
const unsigned char player_up[10] = { 98 , __PLAYER_UP_UDG, 0 };
const unsigned char player_right[10] = { 99 , __PLAYER_LEFT_UDG, 0 };
const unsigned char player_left[10] = { 100 , __PLAYER_RIGHT_UDG, 0};

const unsigned char ghost[10] = { 102 , __GHOST_UDG, 0 };
const unsigned char bomb[10] =  { 101 , __BOMB_UDG, 0};


void INIT_GRAPHICS(void)
{

    
	asm(" ldiReg R8,0xF800\n"
	    " sex R8\n"
	    " out 7\n"
	    " sex R2\n");
        
    redefine_char(player_down, 3);
    redefine_char(player_up, 3);
    redefine_char(player_right, 3);
    redefine_char(player_left, 3);
    redefine_char(ghost, 3);
    redefine_char(bomb, 0);
    
    // vidchar(0xf800, 0xe1);
    // vidchar(0xf801, 0xe2);
    // vidchar(0xf802, 0xe3);
    // vidchar(0xf803, 0xe4);
    // while(1){};
}
static unsigned long int next = 1;

int rand(void) // RAND_MAX assumed to be 32767
{
    next = next * 1103515245 + 12345;
    return (unsigned int)(next/65536) % 32768;
}/* --------------------------------------------------------------------------------------- */ 
// 
// CROSS CHASE by Fabrizio Caruso
//
// Fabrizio_Caruso@hotmail.com
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from
// the use of this software.

// Permission is granted to anyone to use this software for non-commercial applications, 
// subject to the following restrictions:

// 1. The origin of this software must not be misrepresented; you must not
// claim that you wrote the original software. If you use this software in
// a product, an acknowledgment in the product documentation would be
// appreciated but is not required.

// 2. Altered source versions must be plainly marked as such, and must not
// be misrepresented as being the original software.

// 3. This notice may not be removed or altered from any source distribution.
/* --------------------------------------------------------------------------------------- */ 
 
#include "character.h"
#include "item.h"
#include "item.h"
#include "settings.h"
#include "ghost.h"


#include "cross_lib.h"

#include "level.h"

extern uint8_t level;

extern Image PLAYER_IMAGE;
extern Image GHOST_IMAGE;
extern Image BOMB_IMAGE;
extern Image DEAD_GHOST_IMAGE;

#if !defined(TINY_GAME)
	extern Image SKULL_IMAGE;
	extern Image BULLET_IMAGE;
	extern Image POWERUP_IMAGE;
	extern Image GUN_IMAGE;
	extern Image EXTRA_POINTS_IMAGE;
#endif

extern Character player; 

#if !defined(TINY_GAME)
	extern Character skull;

	extern Item powerUp;
	extern Item gun;
	extern Item extraPoints;

	extern Character bullet;

#endif

extern Character ghosts[GHOSTS_NUMBER];
extern Character bombs[BOMBS_NUMBER];


#if defined(FULL_GAME) 
	extern Item powerUp2;

	extern Item chase;
	extern Character chasingBullet;

	extern uint8_t innerVerticalWallY;
	extern uint8_t innerVerticalWallLength;

	extern Image ROCKET_IMAGE;

	extern Image FREEZE_IMAGE;
	extern Image LEFT_HORIZONTAL_MISSILE_IMAGE;
	extern Image RIGHT_HORIZONTAL_MISSILE_IMAGE;

	extern Image EXTRA_LIFE_IMAGE;
	extern Image INVINCIBILITY_IMAGE;
	extern Image SUPER_IMAGE;
	extern Image CONFUSE_IMAGE;
	extern Image ZOMBIE_IMAGE;

	extern Character leftHorizontalMissile;
	extern Character rightHorizontalMissile;

	extern Item freeze;
	extern Item extraLife;
	extern Item invincibility;
	extern Item super;
	extern Item confuse;
	extern Item zombie;
	
	extern Character rockets[ROCKETS_NUMBER];

	extern char rockets_x[ROCKETS_NUMBER];
	
	extern char skullsKilled;
#endif


#if defined(FULL_GAME)
	void updateInnerWallVerticalData(void)
	{	
		uint8_t lvmod = level%5;

		if((lvmod==1)||(lvmod==0))
		{
			innerVerticalWallLength = 0;
		}
		else
		{
			#if defined(WIDE)
				innerVerticalWallLength = YSize-10+(lvmod-2)*2;
			#elif YSize<=14
				innerVerticalWallLength = 4;
			#else
				innerVerticalWallLength = YSize-14+(lvmod-2)*2;		
			#endif
		}

		innerVerticalWallY = (YSize>>1)-(innerVerticalWallLength>>1);
	}

	uint8_t oneMissileLevel(void)
	{
		return ((level%5)==3) || (level==5);
	}

	uint8_t rocketLevel(void)
	{
		return (level >= FIRST_ROCKETS_LEVEL) && ((level%5)==2 || (level%5)==3);
	}

	uint8_t missileLevel(void)
	{
		return level%5==4;
	}	

	uint8_t bossLevel(void)
	{
		return !(level%5);
	}
	
	uint8_t horizontalWallsLevel(void)
	{
		return ((level >= FIRST_HORIZONTAL_WALLS_LEVEL) && ((level%5==1) || (level%5==4)));
	}	

	void initializeAwayFromWall(Character * characterPtr, uint8_t x, uint8_t y, uint8_t status, Image *imagePtr)
	{
		do{
			initializeCharacter(characterPtr, x, y, status, imagePtr);
			relocateCharacter(characterPtr);
		} while(innerWallReached(characterPtr));
	}

#endif 

#if defined(BETWEEN_LEVEL)

void _spiral_slow_down()
{
	uint8_t k;
	
	for(k=0;k<254;++k){};
}

void spiral(register Character *characterPtr, uint8_t length)
{
	uint8_t i;
	uint8_t j;
	
	characterPtr->_x = XSize/2;
	characterPtr->_y = YSize/2;
	for(i=0;i<length;++i)
	{
		for(j=0;j<i/2;++j)
			{
				displayCharacter(characterPtr);		
				if(i&2)
				{
					++(*((uint8_t *) characterPtr + (i&1)));
				}
				else
				{
					--(*((uint8_t *) characterPtr + (i&1)));				
				}
				#if defined(SLOW_DOWN)
				_spiral_slow_down();	
				#endif
			}
	}
}

#endif


void fillLevelWithCharacters(uint8_t nGhosts)
{
	uint8_t i;
	uint8_t j;
	uint8_t count = 0;
	
	#if defined(FULL_GAME)
		if(rocketLevel() || bossLevel())
		{
			for(i=0;i<ROCKETS_NUMBER;i++)
			{
				rockets_x[i] = (uint8_t) (i+1)*(XSize/(ROCKETS_NUMBER+1));
				initializeCharacter(&rockets[i],(uint8_t) rockets_x[i],(uint8_t)(YSize-1),1,&ROCKET_IMAGE);
				displayRocket(&rockets[i]);
			}
		}
	#endif
	
	#if GHOSTS_NUMBER >= 9
		ROUND_NINE_GHOSTS();
	#elif GHOSTS_NUMBER>=7 
		#if (!defined(TINY_GAME) || defined(ROUND_ENEMIES)) && !defined(FLAT_ENEMIES)
			ROUND_EIGHT_GHOSTS();	
		#else
			FLAT_EIGHT_GHOSTS();
		#endif
	#else
		FLAT_SIX_GHOSTS();
	#endif
	
	#if BOMBS_NUMBER==4
		FOUR_BOMBS();
	#elif BOMBS_NUMBER==3	
		THREE_BOMBS();
	#elif BOMBS_NUMBER==2
		TWO_BOMBS();
	#elif BOMBS_NUMBER==1
		ONE_BOMB();
	#endif
	
	#if defined(FULL_GAME)
		initializeAwayFromWall(&(powerUp._character),(XSize>>1),(YSize>>1),1,&POWERUP_IMAGE);
		initializeAwayFromWall(&(powerUp2._character),(XSize>>1),(YSize>>1),0,&POWERUP_IMAGE);
		initializeAwayFromWall(&(freeze._character),(XSize>>1),(YSize>>1),0,&FREEZE_IMAGE);
		initializeAwayFromWall(&(extraPoints._character), (XSize>>1), (YSize>>1), 0, &EXTRA_POINTS_IMAGE);
		initializeAwayFromWall(&(super._character), (XSize>>1), (YSize>>1), 0, &SUPER_IMAGE);
		initializeAwayFromWall(&(confuse._character), (XSize>>1), (YSize>>1), 0, &CONFUSE_IMAGE);		
		initializeAwayFromWall(&(zombie._character), (XSize>>1), (YSize>>1), 0, &ZOMBIE_IMAGE);	
		
		initializeAwayFromWall(&(gun._character),(XSize>>1), (YSize>>1), (bossLevel() ? 1 : 0), &GUN_IMAGE);

		initializeAwayFromWall(&player,(uint8_t) ((XSize>>1)+(rand()&1)),(uint8_t) ((YSize>>1)+(rand()&1)),1,&PLAYER_IMAGE);
				
		initializeAwayFromWall(&(extraLife._character), (XSize>>1), (YSize>>1), 0, &EXTRA_LIFE_IMAGE);

		initializeAwayFromWall(&(invincibility._character), (XSize>>1), (YSize>>1), 0, &INVINCIBILITY_IMAGE);

		if(oneMissileLevel())
		{
			initializeCharacter(&rightHorizontalMissile,         XSize-1,                      (YSize>>1), 1,&RIGHT_HORIZONTAL_MISSILE_IMAGE);			
		}
		else if(missileLevel() || bossLevel())
		{	
			initializeCharacter(&rightHorizontalMissile,         XSize-1,         HORIZONTAL_MISSILE_OFFSET, 1,&RIGHT_HORIZONTAL_MISSILE_IMAGE);
			initializeCharacter(&leftHorizontalMissile,                0, YSize-1-HORIZONTAL_MISSILE_OFFSET, 1,&LEFT_HORIZONTAL_MISSILE_IMAGE);		
		}		
		
		initializeAwayFromWall(&(chase._character), (XSize>>1), (YSize>>1),0,&BULLET_IMAGE);
		initializeCharacter(&chasingBullet, 0,0, 0, &BULLET_IMAGE);
	#else
		#if !defined(TINY_GAME)
			initializeCharacter(&(powerUp._character),(XSize>>1),(YSize>>1),1,&POWERUP_IMAGE);
			initializeCharacter(&(gun._character),(XSize>>1), (YSize>>1), 0, &GUN_IMAGE);	
			initializeCharacter(&(extraPoints._character), (XSize>>1), (YSize>>1), 0, &EXTRA_POINTS_IMAGE);	
		#endif
		
		#if defined(NO_RANDOM_LEVEL) || defined(TINY_GAME) || defined(SIMPLE_RANDOM_LEVEL)
			initializeCharacter(&player,(uint8_t) ((XSize>>1)),(uint8_t) ((YSize>>1)),1,&PLAYER_IMAGE);			
		#else
			initializeCharacter(&player,(uint8_t) ((XSize>>1)+(uint8_t) (rand()&1)),
										(uint8_t) ((YSize>>1)+(uint8_t) (rand()&1)),1,&PLAYER_IMAGE);	
		#endif
	#endif
	#if !defined(TINY_GAME)
		displayPlayer(&player);
			
		initializeCharacter(&bullet, 0, 0,0,&BULLET_IMAGE);

		#if defined(FULL_GAME)
			initializeCharacter(&skull,XSize-2,YSize-2, NON_BOSS_SKULL_HITS + (bossLevel()<<2), &SKULL_IMAGE);		
		#else
			initializeCharacter(&skull,XSize-2,YSize-2, NON_BOSS_SKULL_HITS, &SKULL_IMAGE);					
		#endif
		
	#endif
	
}

#include "display_macros.h"

#include "graphics_settings.h"

#if !defined(ANIMATE_PLAYER)
	extern Image PLAYER_IMAGE;
#else
	extern Image PLAYER_DOWN;
	extern Image PLAYER_UP;
	extern Image PLAYER_RIGHT;
	extern Image PLAYER_LEFT;	
#endif
	
extern Image GHOST_IMAGE;
extern Image BOMB_IMAGE;

#if !defined(NO_DEAD_GHOSTS)
	extern Image DEAD_GHOST_IMAGE;
#endif

#if !defined(TINY_GAME)
	extern Image SKULL_IMAGE;
	extern Image POWERUP_IMAGE;
	extern Image GUN_IMAGE;
	extern Image BULLET_IMAGE;
	extern Image EXTRA_POINTS_IMAGE;
	
	extern Image HORIZONTAL_BRICK_IMAGE;
	extern Image VERTICAL_BRICK_IMAGE;
#endif

#if defined(FULL_GAME)
	extern Image LEFT_HORIZONTAL_MISSILE_IMAGE;
	extern Image RIGHT_HORIZONTAL_MISSILE_IMAGE;

	extern Image ROCKET_IMAGE;

	extern Image FREEZE_IMAGE;
	extern Image EXTRA_LIFE_IMAGE;
	extern Image INVINCIBILITY_IMAGE;
	extern Image SUPER_IMAGE;	
	extern Image CONFUSE_IMAGE;
	extern Image ZOMBIE_IMAGE;
	
	extern Image BROKEN_BRICK_IMAGE;
#endif


// TODO: MOVE THIS TO TARGET SPECIFIC SETTINGS
#define CPC_WHITE 1
#define CPC_YELLOW 2 
#define CPC_RED 3
#define CPC_BLACK 4

#define _AQUARIUS_RED 16
#define _AQUARIUS_WHITE (16+32+64)
#define _AQUARIUS_YELLOW (16+32) 
#define _AQUARIUS_VIOLET (16+64) 
#define _AQUARIUS_CYAN (32+64)


#ifndef COLOR_CYAN
	#define COLOR_CYAN COLOR_BLUE
#endif

#if defined(CPCRSLIB)
	#define _PLAYER_COLOR CPC_YELLOW
	#define _GHOST_COLOR CPC_WHITE
	#define _DEAD_GHOST_COLOR CPC_RED
	#define _SKULL_COLOR CPC_YELLOW
	#define _BOMB_COLOR CPC_RED
	#define _MISSILE_COLOR CPC_WHITE
	#define _ROCKET_COLOR CPC_WHITE
	#define _POWERUP_COLOR CPC_YELLOW
	#define _FREEZE_COLOR CPC_WHITE
	#define _SUPER_COLOR CPC_RED
	#define _GUN_COLOR CPC_YELLOW
	#define _BULLET_COLOR CPC_WHITE
	#define _BRICK_COLOR CPC_YELLOW
	#define _EXTRA_POINTS_COLOR CPC_YELLOW
	#define _EXTRA_LIFE_COLOR CPC_YELLOW
	#define _INVINCIBILITY_COLOR CPC_YELLOW
	#define _CONFUSE_COLOR CPC_RED
	#define _ZOMBIE_COLOR CPC_YELLOW	
#elif defined(__AQUARIUS__) && defined(MEMORY_MAPPED)
	#define _PLAYER_COLOR _AQUARIUS_CYAN
	#define _GHOST_COLOR _AQUARIUS_WHITE
	#define _DEAD_GHOST_COLOR _AQUARIUS_RED
	#define _SKULL_COLOR _AQUARIUS_YELLOW
	#define _BOMB_COLOR _AQUARIUS_RED
	#define _MISSILE_COLOR _AQUARIUS_WHITE
	#define _ROCKET_COLOR _AQUARIUS_WHITE
	#define _POWERUP_COLOR _AQUARIUS_YELLOW
	#define _FREEZE_COLOR _AQUARIUS_CYAN
	#define _SUPER_COLOR _AQUARIUS_RED
	#define _GUN_COLOR _AQUARIUS_YELLOW
	#define _BULLET_COLOR _AQUARIUS_WHITE
	#define _BRICK_COLOR _AQUARIUS_YELLOW
	#define _EXTRA_POINTS_COLOR _AQUARIUS_YELLOW
	#define _EXTRA_LIFE_COLOR _AQUARIUS_YELLOW
	#define _INVINCIBILITY_COLOR _AQUARIUS_YELLOW
	#define _CONFUSE_COLOR _AQUARIUS_RED
	#define _ZOMBIE_COLOR _AQUARIUS_YELLOW	
#elif defined(__PC6001__) || defined(__SPC1000__)
	#define _PLAYER_COLOR COLOR_BLUE 
	#define _GHOST_COLOR COLOR_CYAN
	#define _DEAD_GHOST_COLOR COLOR_RED
	#define _SKULL_COLOR COLOR_YELLOW
	#define _BOMB_COLOR COLOR_RED
	#define _MISSILE_COLOR COLOR_CYAN
	#define _ROCKET_COLOR COLOR_CYAN
	#define _POWERUP_COLOR COLOR_CYAN
	#define _FREEZE_COLOR COLOR_BLUE
	#define _SUPER_COLOR COLOR_RED
	#define _GUN_COLOR COLOR_BLUE
	#define _BULLET_COLOR COLOR_CYAN
	#define _BRICK_COLOR COLOR_YELLOW
	#define _EXTRA_POINTS_COLOR COLOR_YELLOW
	#define _EXTRA_LIFE_COLOR COLOR_YELLOW
	#define _INVINCIBILITY_COLOR COLOR_YELLOW
	#define _CONFUSE_COLOR COLOR_RED
	#define _ZOMBIE_COLOR COLOR_YELLOW
#else
	#define _PLAYER_COLOR COLOR_CYAN
	#define _GHOST_COLOR COLOR_WHITE
	#define _DEAD_GHOST_COLOR COLOR_RED
	#define _SKULL_COLOR COLOR_YELLOW
	#define _BOMB_COLOR COLOR_RED
	#define _MISSILE_COLOR COLOR_WHITE
	#define _ROCKET_COLOR COLOR_WHITE
	#define _POWERUP_COLOR COLOR_GREEN
	#define _FREEZE_COLOR COLOR_CYAN
	#define _SUPER_COLOR COLOR_RED
	#if !defined(__NCURSES__)
		#define _GUN_COLOR COLOR_BLUE
	#else
		#define _GUN_COLOR COLOR_YELLOW
	#endif
	#define _BULLET_COLOR COLOR_WHITE
	#define _BRICK_COLOR COLOR_YELLOW
	#define _EXTRA_POINTS_COLOR COLOR_YELLOW
	#define _EXTRA_LIFE_COLOR COLOR_YELLOW
	#define _INVINCIBILITY_COLOR COLOR_YELLOW
	#define _CONFUSE_COLOR COLOR_RED
	#define _ZOMBIE_COLOR COLOR_YELLOW
#endif


void INIT_IMAGES(void)
{
	// Set color data 
	#if !defined(NO_COLOR)
		
		#if defined(ANIMATE_PLAYER)
			PLAYER_DOWN._color = _PLAYER_COLOR;
			PLAYER_UP._color = _PLAYER_COLOR;
			PLAYER_RIGHT._color = _PLAYER_COLOR;
			PLAYER_LEFT._color = _PLAYER_COLOR;
		#else
			PLAYER_IMAGE._color = _PLAYER_COLOR;
		#endif	
	
		BOMB_IMAGE._color = _BOMB_COLOR;
		GHOST_IMAGE._color = _GHOST_COLOR;		
		
		#if !defined(NO_DEAD_GHOSTS)
			DEAD_GHOST_IMAGE._color = _DEAD_GHOST_COLOR;
		#endif
		
		#if !defined(TINY_GAME)
			SKULL_IMAGE._color = _SKULL_COLOR;
			POWERUP_IMAGE._color = _POWERUP_COLOR;
			GUN_IMAGE._color = _GUN_COLOR;
			EXTRA_POINTS_IMAGE._color = _EXTRA_POINTS_COLOR;
			BULLET_IMAGE._color = _BULLET_COLOR;
			
			VERTICAL_BRICK_IMAGE._color = _BRICK_COLOR;
			HORIZONTAL_BRICK_IMAGE._color = _BRICK_COLOR;			
		#endif
		
		#if defined(FULL_GAME)
			RIGHT_HORIZONTAL_MISSILE_IMAGE._color = _MISSILE_COLOR;		
			LEFT_HORIZONTAL_MISSILE_IMAGE._color = _MISSILE_COLOR;
			
			ROCKET_IMAGE._color = _ROCKET_COLOR;

			FREEZE_IMAGE._color = _FREEZE_COLOR;
			EXTRA_LIFE_IMAGE._color = _EXTRA_LIFE_COLOR;
			
			INVINCIBILITY_IMAGE._color = _INVINCIBILITY_COLOR;
			SUPER_IMAGE._color = _SUPER_COLOR;
			CONFUSE_IMAGE._color = _CONFUSE_COLOR;
			ZOMBIE_IMAGE._color = _ZOMBIE_COLOR;
			
			BROKEN_BRICK_IMAGE._color = _BRICK_COLOR;
		#endif	
	#endif	

	// Set Image Data
	#if defined(ANIMATE_PLAYER)
		PLAYER_DOWN._imageData = _PLAYER_DOWN;
		PLAYER_UP._imageData = _PLAYER_UP;	
		PLAYER_RIGHT._imageData = _PLAYER_RIGHT;
		
		PLAYER_LEFT._imageData = _PLAYER_LEFT;	
	#else
		PLAYER_IMAGE._imageData = _PLAYER;			
	#endif
	
	GHOST_IMAGE._imageData = _GHOST;
	BOMB_IMAGE._imageData = _BOMB;
	
	#if !defined(NO_DEAD_GHOSTS)
		DEAD_GHOST_IMAGE._imageData = _DEAD_GHOST;	
	#endif
	
	#if !defined(TINY_GAME)
		SKULL_IMAGE._imageData = _SKULL;

		POWERUP_IMAGE._imageData = _POWERUP;
		GUN_IMAGE._imageData = _GUN;
		EXTRA_POINTS_IMAGE._imageData = _EXTRA_POINTS;
		
		BULLET_IMAGE._imageData = _BULLET;
		
		VERTICAL_BRICK_IMAGE._imageData = _VERTICAL_BRICK;
		HORIZONTAL_BRICK_IMAGE._imageData = _HORIZONTAL_BRICK;		
	#endif
	
	#if defined(FULL_GAME)
		LEFT_HORIZONTAL_MISSILE_IMAGE._imageData = _LEFT_HORIZONTAL_MISSILE;
		
		RIGHT_HORIZONTAL_MISSILE_IMAGE._imageData = _RIGHT_HORIZONTAL_MISSILE;
		
		ROCKET_IMAGE._imageData = _ROCKET;

		// If colors are present POWERUP/FREEZE/SUPER can be differentiated by the color attribute -> same _POWERUP for all of them
		#if !defined(NO_COLOR) && !defined(UDG_GRAPHICS) && !defined(__NES__) && !defined(__MO5__)
			FREEZE_IMAGE._imageData = _POWERUP;		
			SUPER_IMAGE._imageData = _POWERUP;		
		#else
			FREEZE_IMAGE._imageData = _FREEZE;
			SUPER_IMAGE._imageData = _SUPER;		
		#endif
		
		EXTRA_LIFE_IMAGE._imageData = _EXTRA_LIFE;
		INVINCIBILITY_IMAGE._imageData = _INVINCIBILITY;	
		CONFUSE_IMAGE._imageData = _SKULL;
		ZOMBIE_IMAGE._imageData = _GHOST;
		
		BROKEN_BRICK_IMAGE._imageData = _BROKEN_BRICK;
	#endif
}



/* --------------------------------------------------------------------------------------- */ 
// 
// CROSS CHASE by Fabrizio Caruso
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from
// the use of this software.

// Permission is granted to anyone to use this software for non-commercial applications, 
// subject to the following restrictions:

// 1. The origin of this software must not be misrepresented; you must not
// claim that you wrote the original software. If you use this software in
// a product, an acknowledgment in the product documentation would be
// appreciated but is not required.

// 2. Altered source versions must be plainly marked as such, and must not
// be misrepresented as being the original software.

// 3. This notice may not be removed or altered from any source distribution.
/* --------------------------------------------------------------------------------------- */ 

#include "cross_lib.h"
#include "images.h"


#if !defined(NO_SET_SCREEN_COLORS)
	void setScreenColors(void)
	{
		SET_TEXT_COLOR(TEXT_COLOR);
		SET_BORDER_COLOR(BORDER_COLOR);
		SET_BACKGROUND_COLOR(BACKGROUND_COLOR);
	}
#endif


#if defined(NO_MESSAGE)	
	extern short highScore;
#endif



#include "graphics_settings.h"


#if defined(MEMORY_MAPPED)		
	uint16_t loc(uint8_t x, uint8_t y)
	{
		return ((uint16_t) BASE_ADDR)+x+(uint8_t)y*((uint16_t) (XSize + X_OFFSET));
	}
#elif defined(REX)
	char char_list[UDG_N*2] = 
	{ 
	'*', '\0', // PLAYER
	'O', '\0', // GHOST
	'X', '\0', // BOMB
	'+', '\0', // SKULL
	'.', '\0', // BULLET
	'P', '\0', // POWERUP
	'!', '\0', // GUN
	'$', '\0', // EXTRA_POINTS_IMAGE
	'V', '\0', // INVINCIBILITY
	'*', '\0', // EXTRA LIFE
	'>', '\0', // LEFT_HORIZONTAL_MISSILE
	'<', '\0', // RIGHT_HORIZONTAL_MISSILE
	'^', '\0', // ROCKET
	'*', '\0', // PLAYER_DOWN
	'*', '\0', // PLAYER_UP
	'*', '\0', // PLAYER_RIGHT
	'*', '\0', // PLAYER_LEFT	
	'|', '\0', // VERTICAL_BRICK
	'-', '\0' // HORIZONTAL_BRICK 
	};

	char space_str[2] = {' ', '\0'};
#elif defined(CPCRSLIB)
	char char_list[UDG_N*2] = 
	{ 
	33, '\0', // PLAYER
	37, '\0', // GHOST
	44, '\0', // BOMB
	40, '\0', // SKULL
	43, '\0', // BULLET
	42, '\0', // POWERUP
	41, '\0', // GUN
	49, '\0', // EXTRA_POINTS_IMAGE
	46, '\0', // INVINCIBILITY
	33, '\0', // EXTRA LIFE
	39, '\0', // LEFT_HORIZONTAL_MISSILE
	38, '\0', // RIGHT_HORIZONTAL_MISSILE
	45, '\0', // ROCKET
	33, '\0', // PLAYER_DOWN
	34, '\0', // PLAYER_UP
	35, '\0', // PLAYER_RIGHT
	36, '\0', // PLAYER_LEFT
	47, '\0', // VERTICAL_BRICK
	48, '\0' // HORIZONTAL_BRICK 
	};

	char space_str[2] = {' ', '\0'};

#elif defined(Z88DK_SPRITES)

	extern uint8_t sprites[];

#elif defined(BUFFERED)
	uint8_t video_buffer[YSize+Y_OFFSET][XSize];
	
	void display_all(void)
	{
		uint8_t j; 
		uint8_t i;
		
		for(j=0;j<YSize+Y_OFFSET;++j)
		{
			for(i=0;i<XSize;++i)
			{
				putchar(video_buffer[j][i]);
			}
			putchar('\n');
		}
	}
#elif defined(__MO5__)
	#define POKE(addr,val)     (*(uint8_t*) (addr) = (val))

	void SWITCH_COLOR_BANK_ON(void)
	{
		asm
		{
			swi
			.byte 4
		}	
	}
	
	void SWITCH_COLOR_BANK_OFF(void)
	{
		asm
		{
			swi
			.byte 6
		}	
	}	
	
	void PUTCH(char ch)
	{
		POKE(0x201B,0);
		asm
		{
			ldb ch
			swi
			.byte 2
		}
	}
	

	void gotoxy(uint8_t xx, uint8_t yy)
	{
		PUTCH(0x1F);
		PUTCH(0x40+yy);
		PUTCH(0x41+xx);
	}	
	
#elif defined(__TO7__)
	void PUTCH(char ch)
	{
		asm
		{
			ldb ch
			jsr	0xE803
		}
	}
#elif defined(__SUPERVISION__)
    uint8_t reversed_map_one_to_two_lookup[16] = 
    {
        0x00, 0xC0, 0x30, 0xF0, 0x0C, 0xCC, 0x3C, 0xFC,
        0x03, 0xC3, 0x33, 0xF3, 0x0F, 0xCF, 0x3F, 0xFF
    };
    
    uint8_t left_map_one_to_two(uint8_t n)
    {
        return reversed_map_one_to_two_lookup[n >> 4];
    }
    
    uint8_t right_map_one_to_two(uint8_t n)
    {
        return reversed_map_one_to_two_lookup[n&0x0F];
    }

#endif


#ifndef X_OFFSET
  COMPILATION ERROR
#endif
	
#ifndef Y_OFFSET
  COMPILATION ERROR
#endif


#if defined(Z88DK_SPRITES) && defined(ALT_PRINT)
	void _draw_ch(uint8_t x, uint8_t y, uint8_t ch)
	{
		__DELETE(x,y);
		putsprite(spr_or,x*(SPRITE_X_STEP),y*(SPRITE_Y_STEP),sprites + (uint8_t *) ((ch-32)*(2+SPRITE_Y_SIZE))); \
	}
#endif

#if !defined(NO_STATS) \
	|| X_OFFSET!=0 || Y_OFFSET!=0
	
	void _draw_stat(uint8_t x, uint8_t y, Image * image) 
	{
		__DRAW((X_OFFSET+x),(y),image);
	}

	void _draw(uint8_t x, uint8_t y, Image * image) 
	{
		_draw_stat(x,Y_OFFSET+y,image);
	}
#else
	void _draw(uint8_t x, uint8_t y, Image * image) 
	{
		__DRAW(x,y,image);
	}	
#endif

#if !defined(NO_STATS) \
	|| X_OFFSET!=0 || Y_OFFSET!=0
	void _delete_stat(uint8_t x, uint8_t y)
	{
		__DELETE(X_OFFSET+x,y);
	}
	
	void _delete(uint8_t x, uint8_t y)
	{
		_delete_stat(x,Y_OFFSET+y);
	}	
#else
	void _delete(uint8_t x, uint8_t y)
	{
		__DELETE(x,y);
	}
#endif


#if defined(FULL_GAME)

	void DRAW_BROKEN_BRICK(uint8_t x, uint8_t y)
	{
		_draw(x,y,&BROKEN_BRICK_IMAGE);		
	}
#endif


#if !defined(NO_BLINKING)
void _blink_draw(uint8_t x, uint8_t y, Image * image, uint8_t *blinkCounter) 
{
	if(*blinkCounter) 
	{
		_draw(x,y,image);
		*blinkCounter=0;
	} 
	else 
	{
		_delete(x,y);
		*blinkCounter=1;
	}	
}
#endif


#if defined(DEFAULT_CLEAR_SCREEN)
	void CLEAR_SCREEN(void)
	{
		uint8_t i;
		uint8_t j;
		
		for(i=0;i<XSize;++i)
		{

		#if !defined(NO_STATS) \
			|| X_OFFSET!=0 || Y_OFFSET!=0	
			
			for(j=0;j<YSize+Y_OFFSET;++j)
			{
				_delete_stat(i,j);
			}
		#else
			for(j=0;j<YSize;++j)
			{
				_delete(i,j);
			}
		#endif
		}
	}
	
#endif


#if !defined(TINY_GAME)

void DRAW_HORIZONTAL_LINE(uint8_t x,uint8_t y, uint8_t length) 
{
	uint8_t i;
	for(i=0;i<length;++i)
	{
		_draw(x+i,y,&HORIZONTAL_BRICK_IMAGE);
	}
}

void DRAW_VERTICAL_LINE(uint8_t x,uint8_t y, uint8_t length) 
{
	uint8_t i;
	for(i=0;i<length;++i)
	{
		_draw(x,y+i,&VERTICAL_BRICK_IMAGE);
	}		
}
#endif







/* --------------------------------------------------------------------------------------- */ 
// 
// CROSS CHASE by Fabrizio Caruso
//
// Fabrizio_Caruso@hotmail.com
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from
// the use of this software.

// Permission is granted to anyone to use this software for non-commercial applications, 
// subject to the following restrictions:

// 1. The origin of this software must not be misrepresented; you must not
// claim that you wrote the original software. If you use this software in
// a product, an acknowledgment in the product documentation would be
// appreciated but is not required.

// 2. Altered source versions must be plainly marked as such, and must not
// be misrepresented as being the original software.

// 3. This notice may not be removed or altered from any source distribution.
/* --------------------------------------------------------------------------------------- */ 

#include "cross_lib.h"

#include "sleep_macros.h"

#include "input_macros.h"

#if defined(__MSX__)
    #include<msx/gfx.h>
#endif
        

#if defined(KEYBOARD_CONTROL) && !defined(ACK) && !defined(STDLIB)
    char GET_CHAR(void)
    {
    #  if defined(NO_INPUT)
        return 0;
    #elif defined(TURN_BASED)
        return TURN_BASED_INPUT();
    
    #elif defined(__MSX__)
        if(!get_trigger(0)) 
        {
            return get_stick(0);
        } 
        else 
        {
            return 9;
        }
    
    #elif defined(__VIC20__) || defined(__SUPERVISION__) || defined(__CREATIVISION__) || defined(__OSIC1P__) \
    || defined(__APPLE2__) || defined(__APPLE2ENH__) || defined(__CBM610__) || defined(__C16__) || defined(__CX16__)
        if(kbhit())
            return cgetc();
        else
            return 0;

    // Code by Marcel van Tongeren
    #elif defined(__COMX__)
        asm(
        " ldiReg R8,0x41A3\n"
        " ldn R8\n"
        " plo R15\n"
        " bz  $$nokey\n"
        " ldi 0\n"
        " str R8\n"
        "$$nokey:\n"
        " phi R15\n"
        " Cretn\n");
        return 0; //this statement will never be executed but it keeps the compiler happy

    #elif defined(__ATMOS__) || defined(__TELESTRAT__)
        #include <peekpoke.h>    
        
        uint8_t polledValue = PEEK(0x208);

        switch(polledValue)
        {
            case 141:
                return 'I';
            break;
            case 129:
                return 'J';
            break;
            case 131:
                return 'K';
            break;
            case 143:
                return 'L';
            break;
            case 132:
                return ' ';
            break;
        }
        return '\0';
    #elif defined(__TO7__)
        uint8_t res;
        
        asm
        {
            jsr 0xE806
            stb res
        }
        return res; 
    
    #elif defined(__MO5__) 
        #define POKE(addr,val)     (*(uint8_t*) (addr) = (val))    
        #define PEEK(addr)         (*(uint8_t*) (addr))    

        #define KEYREG 0xA7C1
        
        POKE(KEYREG,0x18);
        if(!(PEEK(KEYREG)&128))
        {
            return 'I';
        }
        else 
        {
            POKE(KEYREG,0x04);
            if(!(PEEK(KEYREG)&128))
            {
                return 'J';
            }
            else
            {
                POKE(KEYREG,0x14);
                if(!(PEEK(KEYREG)&128))
                {
                    return 'K';
                }
                else
                {
                    POKE(KEYREG,0x24);
                    if(!(PEEK(KEYREG)&128))
                    {
                        return 'L';
                    }
                    else
                    {
                        POKE(KEYREG,0x40);
                        if(!(PEEK(KEYREG)&128))
                        {
                            return ' ';
                        }
                        else
                        {
                            return 0;
                        }
                    }
                }
            }
        }
            
    #elif defined(__NCURSES__)
        #define INPUT_LOOPS 10
        
        unsigned long delay = 0;
        char _ch;
        char ch;

        while(delay<INPUT_LOOPS)
        {    
            _ch = getch();
            if(_ch!=ERR)
            {
                ch = _ch;
            }
            ++delay;
        }
        
        return ch;
    
    #elif defined(__COCO__) || defined(__DRAGON__)
        #include <cmoc.h>
        #include <coco.h>
        
        uint8_t res;
        uint8_t machine;
        
        asm {
            ldd $8000
            cmpd #$7EBB
            beq _dragon
            lda #253
            sta machine
            bra pia
_dragon        lda #247
            sta machine
pia            lda #253        
            sta $FF02
            ldb #73
test        lda $ff00
            cmpa machine
            beq out
            incb
            rol $ff02
            inc $ff02
            cmpb #77
            bne test
            clrb 
out            stb res
        }
        
        if(res == 0)
            return inkey();
        return res;
        
    #elif defined(__SRR__)
        return getk_inkey();     
    #else
        #if defined(ALT_MOVE)
            return getch();
        #else
            return getk();
        #endif
    #endif
    }
#endif    
    
#if defined(NO_WAIT) && !defined(NO_SLEEP)
    void WAIT_PRESS(void)
    {
        SLEEP(2);
    }
#elif defined(NO_WAIT)
//
#elif defined(WAIT_FOR_KEY)
    #  if defined(__GCC_BUFFERED__) || defined(STDLIB)
        void WAIT_PRESS(void)
        {
            getchar();
        }
    #elif defined(__NCURSES__)
        #if defined(__ATARI_ST__)
            #include <ncurses/curses.h>
        #else
            #include <ncurses.h>
        #endif
        
        void WAIT_PRESS(void)
        {
            #if !defined(TURN_BASED)
                while(getch()==ERR)
                {}
            #else
                getch();
            #endif
        }
    #elif defined(__CMOC__) && !defined(__WINCMOC__) && !defined(__MO5__) && !defined(__TO7__)
        #include <cmoc.h>
        
        void WAIT_PRESS(void)
        {
            waitkey(0);
        }    
    #else 
        #if defined(CONIO_LIB)
            #include<conio.h>
        #endif

        #if defined(Z88DK)
            #undef cgetc
            #define cgetc() getch()
        #endif
        
        void WAIT_PRESS(void)
        {
            while(kbhit())
                cgetc();
            while(!kbhit())
            { 
            }; 
            cgetc();
        }
    #endif    
#else
    #if defined(Z88DK_JOYSTICK)
        #include <games.h>
        
        extern uint8_t stick;
        
        void WAIT_PRESS(void)
        {
            while ((joystick(stick) & MOVE_FIRE))
            {
            }
            while (!(joystick(stick) & MOVE_FIRE))
            {
            }
        }    
    #elif defined(__SMS__)
        #include <arch/sms/SMSlib.h>
                
        void WAIT_PRESS(void)
        {
            while ((SMS_getKeysStatus() | PORT_A_KEY_1))
            {
            }
            while (!(SMS_getKeysStatus() | PORT_A_KEY_1))
            {
            }
        }        
    #else
        #include<joystick.h>

        void WAIT_PRESS(void)
        {
            while ((joy_read(JOY_1) & JOY_BTN_1_MASK))
            {
            }
            while (! (joy_read(JOY_1) & JOY_BTN_1_MASK))
            {
            }
        }    
    #endif
#endif

#include "cross_lib.h"

#include "text_macros.h"


#if !defined(NO_MESSAGE)
	#if !defined(ALT_DISPLAY_STATS)
	void printCenteredMessageOnRow(uint8_t row, char *Text)
	{
		PRINT(((uint8_t) (XSize - (uint8_t) strlen(Text))>>1), row, Text);	
	}
	#endif


	#if !defined(NO_TEXT_COLOR)
		#if defined(__ATMOS__)
			#include <peekpoke.h>
			void printCenteredMessageOnRowWithCol(uint8_t row, uint8_t col, char *Text)
			{
				POKE(0xBB80+3+(row)*40,16);POKE(0xBB80+3+1+(row)*40,col);
				printCenteredMessageOnRow(row, Text);						
				POKE(0xBB80+35+(row)*40,16);POKE(0xBB80+35+1+(row)*40,3);			
			}			
		#else	
			void printCenteredMessageOnRowWithCol(uint8_t row, uint8_t col, char *Text)
			{
				SET_TEXT_COLOR(col);
				printCenteredMessageOnRow(row, Text);
			}
					
		#endif
	#endif

#endif



#include "settings.h"

#include "move_player.h"

#include "character.h"
#include "skull.h"

#include "definitions.h"


extern Character player;

#if !defined(TINY_GAME)
	extern uint8_t skullXCountDown;
	extern uint8_t skullYCountDown;
	extern uint8_t playerFire;
	extern Character bullet;
	extern uint8_t guns;
	extern uint8_t playerDirection;
#endif

#if defined(FULL_GAME)
	extern uint8_t player_invincibility;
	extern uint8_t playerBlink;	
#endif

#if !defined(TINY_GAME) 
	#if !defined(ALT_MOVE)
		#define _DO_MOVE_UP \
			deletePlayer(&player); \
			--player._y; \
			SHOW_UP(); \
			skullYCountDown = SKULL_COUNT_DOWN; \
			playerDirection = UP;
		#define _DO_MOVE_DOWN \
			deletePlayer(&player); \
			++player._y; \
			SHOW_DOWN(); \
			skullYCountDown = SKULL_COUNT_DOWN; \
			playerDirection = DOWN;
		#define _DO_MOVE_LEFT \
			deletePlayer(&player); \
			--player._x; \
			SHOW_LEFT(); \
			skullXCountDown = SKULL_COUNT_DOWN; \
			playerDirection = LEFT;
		#define _DO_MOVE_RIGHT \
			deletePlayer(&player); \
			++player._x; \
			SHOW_RIGHT(); \
			skullXCountDown = SKULL_COUNT_DOWN; \
			playerDirection = RIGHT;
	#else
		#define _DO_MOVE_UP \
			--player._y; \
			SHOW_UP(); \
			skullYCountDown = SKULL_COUNT_DOWN; \
			playerDirection = UP;
		#define _DO_MOVE_DOWN \
			++player._y; \
			SHOW_DOWN(); \
			skullYCountDown = SKULL_COUNT_DOWN; \
			playerDirection = DOWN;	
		#define _DO_MOVE_LEFT \
			--player._x; \
			SHOW_LEFT(); \
			skullXCountDown = SKULL_COUNT_DOWN; \
			playerDirection = LEFT;
		 
		#define _DO_MOVE_RIGHT \
			++player._x; \
			SHOW_RIGHT(); \
			skullXCountDown = SKULL_COUNT_DOWN; \
			playerDirection = RIGHT;
	#endif
#else
	#if !defined(ALT_MOVE)
		#define _DO_MOVE_UP \
			deletePlayer(&player); \
			--player._y; \
			SHOW_UP();
			
		#define _DO_MOVE_DOWN \
			deletePlayer(&player); \
			++player._y; \
			SHOW_DOWN();
			
		#define _DO_MOVE_LEFT \
			deletePlayer(&player); \
			--player._x; \
			SHOW_LEFT();
			
		#define _DO_MOVE_RIGHT \
			deletePlayer(&player); \
			++player._x; \
			SHOW_RIGHT();
	#else
		#define _DO_MOVE_UP \
			--player._y; \
			SHOW_UP();
			
		#define _DO_MOVE_DOWN \
			++player._y; \
			SHOW_DOWN();
			
		#define _DO_MOVE_LEFT \
			--player._x; \
			SHOW_LEFT();
			
		#define _DO_MOVE_RIGHT \
			++player._x; \
			SHOW_RIGHT();		
	#endif
#endif



#if defined(JOYSTICK_CONTROL)
	#if defined(Z88DK_JOYSTICK)
		#include <games.h>
		
		void movePlayerByJoystick(uint8_t joyInput)
		{
			if(joyInput & MOVE_UP)
			{
				_DO_MOVE_UP
			}
			else if(joyInput & MOVE_DOWN)
			{
				_DO_MOVE_DOWN
			}
			else if(joyInput & MOVE_LEFT)
			{
				_DO_MOVE_LEFT
			}
			else if(joyInput & MOVE_RIGHT)
			{
				_DO_MOVE_RIGHT
			}
			#if !defined(TINY_GAME)
			else if(joyInput & MOVE_FIRE && guns>0 && !bullet._status)
			{
				playerFire = 1;
			}
			#endif
		}	
	#elif defined(__SMS__)
	// TODO: BOGUS - IMPLEMENT THIS!
		#include <arch/sms/SMSLib.h>
		
		void movePlayerByJoystick(uint8_t joyInput)
		{
			if(joyInput & PORT_A_KEY_UP)
			{
				_DO_MOVE_UP
			}
			else if(joyInput & PORT_A_KEY_DOWN)
			{
				_DO_MOVE_DOWN
			}
			else if(joyInput & PORT_A_KEY_LEFT)
			{
				_DO_MOVE_LEFT
			}
			else if(joyInput & PORT_A_KEY_RIGHT)
			{
				_DO_MOVE_RIGHT
			}
			#if !defined(TINY_GAME)
			else if(joyInput & PORT_A_KEY_1 && guns>0 && !bullet._status)
			{
				playerFire = 1;
			}
			#endif			
		}
	#else
		#include <joystick.h>
		
		void movePlayerByJoystick(uint8_t joyInput)
		{
			if(JOY_UP(joyInput))
			{
				_DO_MOVE_UP
			}
			else if(JOY_DOWN(joyInput))
			{
				_DO_MOVE_DOWN
			}
			else if(JOY_LEFT(joyInput))
			{
				_DO_MOVE_LEFT
			}
			else if(JOY_RIGHT(joyInput))
			{
				_DO_MOVE_RIGHT
			}
			#if !defined(TINY_GAME)
			else if(JOY_BTN_1(joyInput) && guns>0 && !bullet._status)
			{
				playerFire = 1;
			}
			#endif
		}	
	#endif
#else
	void movePlayerByKeyboard(uint8_t kbInput)
	{
		#if defined(ALT_MOVE)
			deletePlayer(&player);
		#endif
		if(kbInput==_MOVE_UP)
		{
			_DO_MOVE_UP
		}
		else if(kbInput==_MOVE_DOWN)
		{
			_DO_MOVE_DOWN
		}
		else if(kbInput==_MOVE_LEFT)
		{
			_DO_MOVE_LEFT
		}
		else if(kbInput==_MOVE_RIGHT)
		{
			_DO_MOVE_RIGHT
		}
		#if !defined(TINY_GAME)
		else if(kbInput==_FIRE && guns>0 && !bullet._status)
		{
			playerFire = 1;
		}
		#endif
	}
#endif

	
#if defined(NO_INPUT)
	void MOVE_PLAYER(void) {}
#elif defined(KEYBOARD_CONTROL)
	void MOVE_PLAYER(void) 
	{ 
		#if defined(ALT_MOVE)
			if(kbhit())
			{
		#endif
				movePlayerByKeyboard(GET_CHAR()); 
		#if defined(ALT_MOVE)
			}
		#endif
	}
#else
	
	#if defined(Z88DK_JOYSTICK)
		extern uint8_t stick;
		
		#define JOY_INPUT() joystick(stick)
	#elif defined(__SMS__)
		#include <arch/sms/SMSLib.h>
		
		#define JOY_INPUT() (SMS_getKeysStatus() & 0xFF)
	#else
		#define JOY_INPUT() joy_read(JOY_1)
	#endif	

	#if defined(TURN_BASED)
		void MOVE_PLAYER(void) 
		{ 
			uint8_t joyInput; 
			while(!(joyInput=JOY_INPUT())) 
			{ 
			}; 
			movePlayerByJoystick(joyInput); 
		}	
	#else
		void MOVE_PLAYER(void) { movePlayerByJoystick(JOY_INPUT()); }
	#endif
#endif


/* --------------------------------------------------------------------------------------- */ 
// 
// CROSS CHASE by Fabrizio Caruso
//
// Fabrizio_Caruso@hotmail.com
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from
// the use of this software.

// Permission is granted to anyone to use this software for non-commercial applications, 
// subject to the following restrictions:

// 1. The origin of this software must not be misrepresented; you must not
// claim that you wrote the original software. If you use this software in
// a product, an acknowledgment in the product documentation would be
// appreciated but is not required.

// 2. Altered source versions must be plainly marked as such, and must not
// be misrepresented as being the original software.

// 3. This notice may not be removed or altered from any source distribution.
/* --------------------------------------------------------------------------------------- */ 


#include "ghost.h"
#include "settings.h"

extern uint16_t ghostLevel;
extern uint8_t level;

extern Character ghosts[GHOSTS_NUMBER];
extern Character bombs[BOMBS_NUMBER];

#if defined(FULL_GAME)
uint16_t computeGhostSlowDown(void)
{
	if(ghostLevel<MAX_GHOST_LEVEL)
	{
		#if defined(TURN_BASED)
			return INITIAL_GHOST_SLOWDOWN-(uint16_t)level*256-ghostLevel*8;		
		#else
			return INITIAL_GHOST_SLOWDOWN-(uint16_t)level*256-ghostLevel*16;
		#endif
	}
	return GHOST_MIN_SLOWDOWN;
}
#endif

#if defined(FULL_GAME)
	void decreaseGhostLevel(void)
	{
		if(ghostLevel>GHOST_LEVEL_DECREASE)
			ghostLevel-=GHOST_LEVEL_DECREASE;
		else
			ghostLevel=0;
	}
#endif

void displayBombs(void)
{
	uint8_t i;

	for(i=0;i<BOMBS_NUMBER;++i)
	{
		displayBomb(&bombs[i]);
	}
}	

#if defined(FULL_GAME)
	uint8_t firstAlive(void)
	{
		uint8_t i;
		
		i=0;
		while(i<GHOSTS_NUMBER)
		{
			if(ghosts[i]._status)
				return i;
			++i;
		}
		return GHOSTS_NUMBER;
	}
#endif
/* --------------------------------------------------------------------------------------- */ 
// 
// CROSS CHASE by Fabrizio Caruso
//
// Fabrizio_Caruso@hotmail.com
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from
// the use of this software.

// Permission is granted to anyone to use this software for non-commercial applications, 
// subject to the following restrictions:

// 1. The origin of this software must not be misrepresented; you must not
// claim that you wrote the original software. If you use this software in
// a product, an acknowledgment in the product documentation would be
// appreciated but is not required.

// 2. Altered source versions must be plainly marked as such, and must not
// be misrepresented as being the original software.

// 3. This notice may not be removed or altered from any source distribution.
/* --------------------------------------------------------------------------------------- */ 

#include "character.h"
#include "settings.h"

#include "cross_lib.h"

#include "game_text.h"

extern uint16_t points;

extern uint8_t ghostCount;
extern uint16_t loop;
extern uint8_t level;

extern Image DEAD_GHOST_IMAGE;
extern Image GHOST_IMAGE;
extern Image BOMB_IMAGE;

extern Character ghosts[GHOSTS_NUMBER];
extern Character bombs[BOMBS_NUMBER];
extern Character player;


void displayCharacter(register Character * characterPtr)
{
	DRAW_CHARACTER(characterPtr->_x, characterPtr->_y, characterPtr->_imagePtr);
}

void deleteCharacter(Character * characterPtr)
{
	DELETE_CHARACTER(characterPtr->_x, characterPtr->_y);
}

#if defined(FULL_GAME)
	extern uint8_t invincibilityActive;
	extern uint8_t innerVerticalWallY;
	extern uint8_t innerVerticalWallLength;
	
	extern uint8_t horizontalWallsLength;
	extern uint8_t zombieActive;	
#endif 

void playerDies(void)
{
	EXPLOSION_SOUND();
	player._status=0;
	#if !defined(LESS_TEXT)
		printDefeatMessage();
	#endif
	SLEEP(1);	
}

#if defined(FULL_GAME)
	uint8_t playerKilledBy(Character *enemyPtr)
	{
		return !invincibilityActive && areCharctersAtSamePosition(enemyPtr,&player);
	}
#endif

void initializeCharacter(register Character* characterPtr, uint8_t x, uint8_t y, uint8_t status, Image * imagePtr)
{
	characterPtr->_x = x;
	characterPtr->_y = y;
	characterPtr->_status = status;
	characterPtr->_imagePtr = imagePtr;
	
	#if defined(DEBUG)
		displayCharacter(characterPtr);
	#endif
}


uint8_t isCharacterAtLocation(uint8_t x, uint8_t y, Character * characterPtr)
{
	return(characterPtr->_x==x) && (characterPtr->_y==y);
}


uint8_t wallReached(register Character *characterPtr)
{
	return (characterPtr->_x==0)||(characterPtr->_x==XSize-1) || 
		   (characterPtr->_y==0)||(characterPtr->_y==YSize-1);
}


void ghostDies(Character * ghostPtr)
{
	EXPLOSION_SOUND();
	
	ghostPtr->_status=0;
	displayStats();
	
	#if defined(FULL_GAME)
		if(level>=FIRST_MOVING_BOMBS_LEVEL)
			{
				uint8_t i;

				for(i=0;i<BOMBS_NUMBER;++i)
				{
					deleteCharacter(&bombs[i]);
					if(ghostCount&1)
					{
						--bombs[i]._x;
						--bombs[i]._y;
					}
					else
					{
						++bombs[i]._x;
						++bombs[i]._y;
					}
				}
			}		
	#endif
		
	#if !defined(NO_DEAD_GHOSTS)
		ghostPtr->_imagePtr = (Image *)&DEAD_GHOST_IMAGE;
	#elif !defined(TINY_GAME)
		ghostPtr->_imagePtr = (Image *)&SKULL_IMAGE;			
	#endif
	
	--ghostCount;
	printGhostCountStats();
}

void checkBombsVsGhost(register Character * ghostPtr)
{
	
	if(ghostPtr->_status && characterReachedBombs(ghostPtr))
	{		
		points+=GHOST_VS_BOMBS_BONUS;
		

		#	if defined(TINY_GAME) && defined(MOVE_DEAD_GHOST)
			ghostPtr->_y=1;
		#elif defined(TINY_GAME) && defined(BOMB_DEAD_GHOST)
			ghostPtr->_imagePtr = &BOMB_IMAGE;			
		#elif defined(TINY_GAME) && defined(HIDE_DEAD_GHOST)
			//
		#elif defined(TINY_GAME)
			ghostPtr->_x=ghostCount;
			ghostPtr->_y=1;				
		#else
			ghostPtr->_x=1+GHOSTS_NUMBER-ghostCount;
			ghostPtr->_y=1;	
		#endif
		
		ghostDies(ghostPtr);
	}
	
}
						

uint8_t sameLocationAsAnyLocation(uint8_t x, uint8_t y, Character *characterList, uint8_t length)
{
	uint8_t i;

	for(i=0;i<length;++i)
	{
		if(isCharacterAtLocation(x,y,&characterList[i]))
			return 1;
	}	
	return 0;
}


#if YSize<XSize
	#define SAFETY YSize/4
#else
	#define SAFETY XSize/4
#endif

// TODO: To be replaced with something cleaner
// also used with things different from global bombs
uint8_t safeLocation(uint8_t x, uint8_t y)
{
	return !(sameLocationAsAnyLocation(x,y,ghosts,GHOSTS_NUMBER) 
	      || sameLocationAsAnyLocation(x,y,bombs, BOMBS_NUMBER)
		  || (x<SAFETY) || (x>XSize-SAFETY) || (y<=SAFETY) || (y>YSize-SAFETY));
}

#if !defined(TINY_GAME)
void relocateCharacter(register Character * characterPtr)
{
	uint8_t x; 
	uint8_t y;  
	
	do
	{
		x = characterPtr->_x  + (uint8_t)(rand() % RELOCATE_RANGE) - (uint8_t)(RELOCATE_RANGE/2);
		y = characterPtr->_y  + (uint8_t)(rand() % RELOCATE_RANGE) - (uint8_t)(RELOCATE_RANGE/2);

	} while(!safeLocation(x,y));

	characterPtr->_x = x;
	characterPtr->_y = y;
}
#endif

#if defined(FULL_GAME)
	uint8_t innerWallReached(register Character *characterPtr)
	{
		return (characterPtr->_x==XSize/2) && (characterPtr->_y >= innerVerticalWallY) && (characterPtr->_y<= (innerVerticalWallY + innerVerticalWallLength-1));
	}

	uint8_t horizontalWallsReached(void)
	{
		return (player._y==YSize/2) && 
		       ((player._x<=horizontalWallsLength) ||
			    (player._x>=-1+XSize-horizontalWallsLength));
	}	
	
#endif

/* --------------------------------------------------------------------------------------- */ 
// 
// CROSS CHASE by Fabrizio Caruso
//
// Fabrizio_Caruso@hotmail.com
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from
// the use of this software.

// Permission is granted to anyone to use this software for non-commercial applications, 
// subject to the following restrictions:

// 1. The origin of this software must not be misrepresented; you must not
// claim that you wrote the original software. If you use this software in
// a product, an acknowledgment in the product documentation would be
// appreciated but is not required.

// 2. Altered source versions must be plainly marked as such, and must not
// be misrepresented as being the original software.

// 3. This notice may not be removed or altered from any source distribution.
/* --------------------------------------------------------------------------------------- */ 

#include "settings.h"
#include "cross_lib.h"

#include "game_text.h"
#include "character.h"
#include "settings.h"
#include "text_strings.h"

#define _YELLOW COLOR_YELLOW
#define _WHITE COLOR_WHITE
#define _RED   COLOR_RED
#define SCORE_COLOR COLOR_IF_NO_BLUE_THEN_YELLOW

#if !defined(NO_TEXT_COLOR)
	#define SET_COLOR(c) SET_TEXT_COLOR(c)
#else
	#define SET_COLOR(c)
#endif

#if XSize<20
	#define EXTRA_TINY 1
#else
	#define EXTRA_TINY 0
#endif


#if Y_OFFSET==3
	#define SKIP_ROW 1
#else
	#define SKIP_ROW 0
#endif

#if defined(WIDE) && !defined(TINY_GAME)
	#define GUN_IMAGE_X 17
	#define GUN_IMAGE_Y 0
	#define GHOST_IMAGE_X 13
	#define GHOST_IMAGE_Y 0
	#define PLAYER_IMAGE_X 16
	#define PLAYER_IMAGE_Y (1+SKIP_ROW)
	#define LEVEL_X 6
	#define LEVEL_Y (1+SKIP_ROW)
#else
	#define GUN_IMAGE_X (11-EXTRA_TINY)
	#define GUN_IMAGE_Y 0
	#define GHOST_IMAGE_X (8-EXTRA_TINY)
	#define GHOST_IMAGE_Y 0
	#define PLAYER_IMAGE_X (14-EXTRA_TINY)
	#define PLAYER_IMAGE_Y 0
	#define LEVEL_X 18
	#define LEVEL_Y 0
#endif


#if defined(ALT_DISPLAY_STATS)
	#define printCenteredMessageOnRow(row, text) PRINT(6,row,text)
#endif

#if defined(NO_TEXT_COLOR)

	#define printCenteredMessageOnRowWithCol(row,col,text) printCenteredMessageOnRow(row,text)
#endif	

extern uint8_t level;
extern uint8_t lives;

extern uint16_t points;
extern uint8_t ghostCount;
extern uint16_t ghostLevel;
extern uint16_t highScore;


#if defined(FULL_GAME) && !defined(NO_MESSAGE)
	void printKillTheSkull(void)
	{
		printCenteredMessage(KILL_THE_SKULL_STRING);	
		printCenteredMessageOnRow(((uint8_t)YSize)/2+2,DESTROY_MISSILES_STRING);
	}
	
#endif

#if defined(NO_TITLE_LINE)
	#define TITLE_Y 0
	#define TITLE_LINE()
#else
	#define TITLE_Y 1
	#define TITLE_LINE() PRINT(XSize-11,+0,  "-----------")
#endif


#define PRINT_WIDE_TITLE() \
	SET_COLOR(SCORE_COLOR); \
	PRINT(0,       0, SCORE_STRING); \
	PRINT(0, LEVEL_Y, LEVEL_STRING); \
	\
	SET_COLOR(_RED); \
	TITLE_LINE(); \
	PRINT(XSize-11,TITLE_Y,TITLE_LINE_STRING);	


// TODO: This is SLOW
#if !defined(TINY_GAME) && !defined(NO_STATS)
	void displayStatsTitles(void)
	{				
		#if defined(WIDE)
				PRINT_WIDE_TITLE();
		#endif
		
		SET_COLOR(TEXT_COLOR);
		
		#if (X_OFFSET==0) && (Y_OFFSET==0)
			#define _draw_stat _draw
		#endif
	
		_draw_stat(GUN_IMAGE_X, GUN_IMAGE_Y, &GUN_IMAGE);
		_draw_stat(GHOST_IMAGE_X, GHOST_IMAGE_Y, &GHOST_IMAGE);
		_draw_stat(PLAYER_IMAGE_X, PLAYER_IMAGE_Y, &PLAYER_IMAGE);					
	}

	
	void printGunsStats(void)
	{
		SET_COLOR(TEXT_COLOR);	
		
		#if defined(WIDE)
			PRINTF(GUN_IMAGE_X+2,0+0,"%u",guns);
		#else
			PRINTF(GUN_IMAGE_X+1,0+0,"%u",guns);
		#endif
	}
#endif

#if !defined(NO_STATS)
	void printLevelStats(void)
	{	
		SET_COLOR(TEXT_COLOR);
	
		#if defined(WIDE) && !defined(TINY_GAME)
			PRINTF(LEVEL_X,LEVEL_Y,"%02u", level);
		#elif XSize>16
			PRINTF(LEVEL_X,LEVEL_Y,"%02u",level);
		#else
			// No space for level
		#endif	
	}


	void printGhostCountStats(void)
	{
		SET_COLOR(TEXT_COLOR);		
		
		#if defined(WIDE) && !defined(TINY_GAME)
			PRINTF(GHOST_IMAGE_X+2,+0,"%u",ghostCount);
		#else
			PRINTF(GHOST_IMAGE_X+1,+0,"%u",ghostCount);	
		#endif	
	}


	void printLivesStats(void)
	{
		SET_COLOR(TEXT_COLOR);
		
		#if defined(WIDE) && !defined(TINY_GAME)
			PRINTF(PLAYER_IMAGE_X+2,PLAYER_IMAGE_Y,"%02u",lives);
		#else
			PRINTF(PLAYER_IMAGE_X+1,PLAYER_IMAGE_Y,"%02u",lives);	
		#endif
	}	
	
#endif

#if !defined(NO_MESSAGE)
	void printPressKeyToStart(void)
	{
		printCenteredMessage(PRESS_STRING);
	}	
#endif


void displayStats(void)
{	
	SET_COLOR(TEXT_COLOR);
	
	#if defined(WIDE) && !defined(TINY_GAME)
		PRINTF(6,+0,"%05u0",points);
	#else
		PRINTF(!EXTRA_TINY,0,"%05u0",points);	
	#endif	
}

#if !defined(LESS_TEXT)	
	void printLevel(void)
	{
		PRINTF(((XSize -7)>>1), (YSize>>1), START_LEVEL_STRING, level);
	}
#endif


#if !defined(TINY_GAME) && !defined(NO_MESSAGE)
	void _printScoreOnRow(uint8_t row, char * text, uint16_t score)
	{
		PRINTF((uint8_t) ((XSize-strlen(text))>>1), row, text, score);
	}	
	
	#if !defined(LESS_TEXT)
	void _printScore(char * text, uint16_t score)
	{
		_printScoreOnRow((YSize>>1), text, score);
	}
	#endif
#endif

#if !defined(END_SCREEN) && !defined(NO_MESSAGE)
	void gameCompleted(void)	
	{
		CLEAR_SCREEN();
		printCenteredMessage(YOU_MADE_IT_STRING); 
	}
#endif


#if !defined(LESS_TEXT)
	void printExtraLife(void)
	{
		printCenteredMessageWithCol(_RED, EXTRA_LIFE_STRING); 
	}

	void printVictoryMessage(void)
	{
		printCenteredMessageWithCol(_RED, VICTORY_STRING);
	}	
#endif


#if !defined(LESS_TEXT)	
	void printDefeatMessage(void)
	{			
		printCenteredMessageWithCol(_RED, DEFEAT_STRING);
	}	
#endif
	
	
#if !defined(NO_MESSAGE)
	void printGameOver(void)
	{
		printCenteredMessageWithCol(_RED, GAME_OVER_STRING);
	}	
#endif



#if YSize>=20
	#define EXTRA_Y 1
	#define AUTHOR_Y 5
	#define CROSS_CHASE_Y 2
	#define INTERLINE 2
	#define INSTR_Y_OFFSET 3
#else
	#define EXTRA_Y 0
	#define AUTHOR_Y 3
	#define CROSS_CHASE_Y 1
	#if defined(Z88DK_SPRITES)
		#define INTERLINE 2
	#else
		#define INTERLINE 1
	#endif
	#define INSTR_Y_OFFSET 1
#endif

 


#if (defined(FULL_GAME) && !defined(NO_HINTS)) || !defined(NO_INITIAL_SCREEN)
	#if defined(FULL_GAME) && !defined(NO_HINTS)
	void _printCrossChase(void)
	{
		printCenteredMessageOnRowWithCol(CROSS_CHASE_Y, _RED,  CROSS_CHASE_STRING);		
		SET_COLOR(TEXT_COLOR);
		
	}
	#else
		#define _printCrossChase() \
			printCenteredMessageOnRowWithCol(CROSS_CHASE_Y, _RED,  CROSS_CHASE_STRING);	\
			SET_COLOR(TEXT_COLOR);	
	#endif
#endif


#if defined(FULL_GAME) && !defined(NO_HINTS)
	void printHints(void)
	{
		_printCrossChase();
		
		printCenteredMessageOnRow(AUTHOR_Y+1*INTERLINE+EXTRA_Y,  USE_THE_GUN_AGAINST_STRING);

		printCenteredMessageOnRow(AUTHOR_Y+2*INTERLINE+EXTRA_Y,  THE_SKULL_AND_STRING);

		printCenteredMessageOnRow(AUTHOR_Y+3*INTERLINE+EXTRA_Y, MISSILE_BASES_STRING);	
		
		#if YSize>=14
			printCenteredMessageOnRow(AUTHOR_Y+4*INTERLINE+EXTRA_Y, FOR_POINTS_AND___STRING);

			printCenteredMessageOnRow(AUTHOR_Y+5*INTERLINE+EXTRA_Y, EXTRA_POWERUPS__STRING);
		#endif
	}
#endif

#if !defined(NO_INITIAL_SCREEN)
	void printStartMessage(void)
	{
		_printCrossChase();
		
		printCenteredMessageOnRow(AUTHOR_Y, AUTHOR_STRING);	

		#if !defined(TINY_GAME) && !defined(NO_TITLE_INFO)
			_printTopScore();
			
			SET_COLOR(COLOR_IF_NO_BLUE_THEN_YELLOW);
			
			printCenteredMessageOnRow(AUTHOR_Y+1*INTERLINE+EXTRA_Y,  LURE_THE_ENEMIES_STRING);
			printCenteredMessageOnRow(AUTHOR_Y+2*INTERLINE+EXTRA_Y,  INTO_THE_MINES_STRING);			
			
			SET_COLOR(TEXT_COLOR);	
			
		#endif

		#if !defined(NO_CONTROL_INSTRUCTIONS)
			printCenteredMessageOnRow(YSize-INSTR_Y_OFFSET, USE_STRING);
		#endif
	}
#endif



/* --------------------------------------------------------------------------------------- */ 
// 
// CROSS CHASE by Fabrizio Caruso
//
// Fabrizio_Caruso@hotmail.com
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from
// the use of this software.

// Permission is granted to anyone to use this software for non-commercial applications, 
// subject to the following restrictions:

// 1. The origin of this software must not be misrepresented; you must not
// claim that you wrote the original software. If you use this software in
// a product, an acknowledgment in the product documentation would be
// appreciated but is not required.

// 2. Altered source versions must be plainly marked as such, and must not
// be misrepresented as being the original software.

// 3. This notice may not be removed or altered from any source distribution.
/* --------------------------------------------------------------------------------------- */ 

#include "settings.h"
#include "character.h"
#include "strategy.h"

extern uint8_t level;
extern uint16_t loop;
extern uint8_t ghostCount;

extern Character player; 
extern Character ghosts[GHOSTS_NUMBER];

extern uint8_t strategyArray[GHOSTS_NUMBER];

#if defined(FULL_GAME)
	extern uint8_t zombieActive;
#endif


uint8_t moveCharacter(register uint8_t *hunterOffsetPtr, register uint8_t *preyOffsetPtr)
{
	if((uint8_t) *((uint8_t *)hunterOffsetPtr) < (uint8_t) *((uint8_t *)preyOffsetPtr))
	{
		++(*((uint8_t *) hunterOffsetPtr));		
	}
	else if((uint8_t) *((uint8_t *) hunterOffsetPtr) > (uint8_t) *((uint8_t *)preyOffsetPtr))
	{
		--(*((uint8_t *) hunterOffsetPtr));		
	}	
	else
	{
		return 0;
	}
	return 1;
}


#if defined(FULL_GAME) && !defined(SIMPLE_STRATEGY)
	void blindChaseCharacterXStrategy(Character* hunterPtr, Character* preyPtr)
	{
		if(moveCharacter((uint8_t *)hunterPtr+X_MOVE, (uint8_t *)preyPtr + X_MOVE))
		{
			return;
		}
		else
		{
			moveCharacter((uint8_t *)hunterPtr+Y_MOVE, (uint8_t *)preyPtr + Y_MOVE);
		}
	}

	void blindChaseCharacterYStrategy(Character* hunterPtr, Character* preyPtr)
	{
		if(moveCharacter((uint8_t *)hunterPtr+Y_MOVE, (uint8_t *)preyPtr + Y_MOVE))
		{
			return;
		}
		else
		{
			moveCharacter((uint8_t *)hunterPtr+X_MOVE, (uint8_t *)preyPtr + X_MOVE);
		}
	}
#endif


// strategy: 
// 4 means do no prefer horizontal to vertical movement
// 0 means always horizontal
// 9 means always vertical
#if defined(FULL_GAME) && !defined(SIMPLE_STRATEGY)
	void moveTowardCharacter(Character* preyPtr, Character *hunterPtr, uint8_t strategy)
	{
		if(rand()%10 > strategy) // Select blind chase strategy
			{ // 0 - 4
				blindChaseCharacterXStrategy(hunterPtr, preyPtr);	
			}
			else
			{ // 5 - 9
				blindChaseCharacterYStrategy(hunterPtr, preyPtr);
			}
	}
#elif defined(FULL_GAME) && defined(SIMPLE_STRATEGY)
	void moveTowardCharacter(Character* preyPtr, Character *hunterPtr)
	{
		uint8_t offset = (uint8_t) rand()&1;
		
		moveCharacter((uint8_t *)hunterPtr+offset, (uint8_t *)preyPtr + offset);
	}	
#else
	void moveTowardCharacter(Character *hunterPtr)
	{
		uint8_t offset = (uint8_t) rand()&1;
		
		moveCharacter((uint8_t *)hunterPtr+offset,(uint8_t *)(&player)+offset);
	}
#endif


// #if !defined(TINY_GAME) 
#if defined(FULL_GAME) && !defined(SIMPLE_STRATEGY)
void computeStrategy(void)
{
	uint8_t i;
	uint8_t skew = (level - 1) / 5;
	
	for(i=5; i<GHOSTS_NUMBER; ++i) // 3 (if total=8)
	{
		strategyArray[i] = 5+skew; // 5,6,7,(8 if GHOSTS are 9): prefer Y (60%, 70%, 80%, 90%)
		strategyArray[9-i] = 3-skew; // 4,3,2,(1 if GHOSTS are 9): prefer X (60%, 70%, 80%, 90%)
			
	}
	strategyArray[0] = 4;
	strategyArray[1] = 4;				
}
#endif


#if defined(__NCURSES__) || defined(__GCC_BUFFERED__)
	#define GHOST_RANDOM_CONDITION ((rand()&0x7fff)>slowDown)
#else
	#define GHOST_RANDOM_CONDITION (rand()>slowDown)
#endif

// #if defined(FULL_GAME)
// Ghosts move to new positions if they get their chanche
#if defined(FULL_GAME)
void chaseCharacter(Character *preyPtr, uint16_t slowDown)
#else
void chaseCharacter(uint16_t slowDown)	
#endif
{
	uint8_t i;
	
	for(i=0;i<GHOSTS_NUMBER;++i)
	{
		#if defined(FULL_GAME)
			if((ghosts[i]._status || (zombieActive && loop&1)) && GHOST_RANDOM_CONDITION)
		#else
			if((ghosts[i]._status) && GHOST_RANDOM_CONDITION)	
		#endif
			{
				deleteGhost(&ghosts[i]);
				#if defined(FULL_GAME) && !defined(SIMPLE_STRATEGY)
					moveTowardCharacter(preyPtr, &ghosts[i], strategyArray[i]);	
				#elif defined(FULL_GAME) && defined(SIMPLE_STRATEGY)
					moveTowardCharacter(preyPtr, &ghosts[i]);	
				#else
					moveTowardCharacter(&ghosts[i]);	
				#endif
			}
	}
}
/* --------------------------------------------------------------------------------------- */ 
// 
// CROSS CHASE by Fabrizio Caruso
//
// Fabrizio_Caruso@hotmail.com
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from
// the use of this software.

// Permission is granted to anyone to use this software for non-commercial applications, 
// subject to the following restrictions:

// 1. The origin of this software must not be misrepresented; you must not
// claim that you wrote the original software. If you use this software in
// a product, an acknowledgment in the product documentation would be
// appreciated but is not required.

// 2. Altered source versions must be plainly marked as such, and must not
// be misrepresented as being the original software.

// 3. This notice may not be removed or altered from any source distribution.
/* --------------------------------------------------------------------------------------- */ 


#if !defined EXIT_SUCCESS
	#define EXIT_SUCCESS 0
#endif

// TODO: REMOVE THIS
// #define DEBUG_ITEMS
// #define DEBUG_END

#include "settings.h"

#include "character.h"
#include "item.h"
#include "strategy.h"
#include "game_text.h"
#include "level.h"
#include "ghost.h"
#include "move_player.h"

#include "text_strings.h"

#if !defined(TINY_GAME)
	#include "bullet.h"
	#include "skull.h"
#endif
	
#include "end_screen.h"

#if defined(FULL_GAME)
	#include "horizontal_missile.h"
	#include "rocket.h"
#endif


#if defined(BENCHMARK)
    #include <stdio.h>
    #include <time.h>
#endif

#include "variables.h"


// Level
// The level affects:
// 1. powerUpCoolDown (how long before a new powerUp is spawned)
// 2. ghostSlowDown (how much the power up slows the enemies down)
// 3. skullXCountDown (time needed to activate the skull ghost)
// 4. skullYCountDown
// 5. skullSlowDown (how much the skull ghost is slowed-down)
// 6. skullLoopTrigger (how long before the skull ghost appears)



#if !defined(TINY_GAME)
	void resetItems(void)
	{
		gun._coolDown = GUN_COOL_DOWN;
		powerUp._coolDown = POWER_UP_COOL_DOWN;
		extraPoints._coolDown = EXTRA_POINTS_COOL_DOWN;		
		
		#if defined(FULL_GAME)
			powerUp2._coolDown = POWER_UP2_COOL_DOWN;
			freeze._coolDown = FREEZE_COOL_DOWN;				
			invincibility._coolDown = INVINCIBILITY_COOL_DOWN;

			chase._coolDown = CHASE_COOL_DOWN;
			
			super._coolDown = SUPER_COOL_DOWN;
			extraLife._coolDown = EXTRA_LIFE_COOL_DOWN;
			
			confuse._coolDown = CONFUSE_COOL_DOWN;
			zombie._coolDown = ZOMBIE_COOL_DOWN;				
		#endif
	}

	
	// Constructor for all items
	void constructItems(void)
	{
		powerUp._effect = &powerUpEffect;
		gun._effect = &gunEffect;
		extraPoints._effect = &extraPointsEffect;
		#if defined(FULL_GAME)
			powerUp2._effect = &powerUp2Effect;		
			freeze._effect = &freezeEffect;
			extraLife._effect = &extraLifeEffect;
			invincibility._effect = &invincibilityEffect;
			super._effect = &superEffect;
			confuse._effect = &confuseEffect;
			zombie._effect = &zombieEffect;
			chase._effect = &chaseEffect;
		#endif	
	}	

#endif

#if !defined(NO_INITIAL_SCREEN)			
void initialScreen(void)
{	
	CLEAR_SCREEN();					
	printStartMessage();
	WAIT_PRESS();
	
	#if defined(FULL_GAME) && !defined(NO_HINTS)
		CLEAR_SCREEN();
		printHints();	
	#endif
}
#endif


#if defined(FULL_GAME)
	
	void handle_special_triggers(void)
	{
		// confuse_present_on_level_condition is defined as missileBasesDestroyed
		zombie_present_on_level = missileBasesDestroyed>=MISSILES_FOR_ZOMBIE;
		super_present_on_level = skullsKilled>=SKULLS_FOR_SUPER;
		// chase_present_on_level_condition is defined as skullsKilled;
		extraLife_present_on_level = super_present_on_level && zombie_present_on_level;
	}
	
#endif

#if defined(SLOW_DOWN)
	#  if defined(__NCURSES__)
	
		void _slow_down(void)
		{
			usleep(GAME_SLOW_DOWN*1024);
		}
	#else
	void _slow_down(void)
	{
		short i;
		for(i=0;i<GAME_SLOW_DOWN;++i)
		{	
		}
	}
	#endif
#endif

int main(void)
{		
	INIT_GRAPHICS();

	INIT_INPUT();

	highScore = 0;
	
	while(1)
	{

		INIT_IMAGES();
		
		#if !defined(NO_SET_SCREEN_COLORS)
			// Set Screen Colors
			setScreenColors();				
		#endif
		
		#if !defined(NO_INITIAL_SCREEN)
			initialScreen();
			
			#if !defined(TINY_GAME)
				WAIT_PRESS();
				CLEAR_SCREEN();	
			#endif
		#else
			CLEAR_SCREEN();	
			
			printPressKeyToStart();				
		#endif

		#if !defined(LESS_TEXT) || defined(ALT_HIGHSCORE)
			highScoreScreen();
			WAIT_PRESS();	
		#endif

		#if !defined(TINY_GAME)

			CLEAR_SCREEN();
		#endif
		
		#if !defined(TINY_GAME)
			extraLifeThroughPointsCounter = 1;
		#endif
		points = 0;
		level = INITIAL_LEVEL; 	
		lives = LIVES_NUMBER;
		ghostCount = GHOSTS_NUMBER;
		#if defined(FULL_GAME)
			#if defined(DEBUG_ITEMS)
				missileBasesDestroyed = 99;
				skullsKilled = 99;
			#else
				missileBasesDestroyed = 0;
				skullsKilled = 0;			
			#endif			
		#endif
		
		
		do // Level (Re-)Start
		{ 	
			#if !defined(TINY_GAME) || defined(TURN_BASED)
				loop = 0;
			#endif
			
			#if !defined(TINY_GAME)
		
				playerFire = 0;
			#endif
		
			#if defined(FULL_GAME)
				ghostLevel = 0;
				
				dead_rockets = 0;
				
				invincibilityActive = 1;				
				invincibility_count_down = INITIAL_INVINCIBILITY_COUNT_DOWN;
				
				#if !defined(INITIAL_GHOST_FREEZE)
					freezeActive = 0;
					freeze_count_down = 0;
				#endif
				confuseActive = 0;
				zombieActive = 0; 
				
				handle_special_triggers();
				
				#if !defined(SIMPLE_STRATEGY)
					computeStrategy();			
				#endif
								
			#endif			
			
			#if !defined(TINY_GAME)
				skullActive = 0;
				
				guns = 0;
				
				resetItems();
				
				#if defined(INITIAL_GHOST_FREEZE)
					freezeActive = 1;
					freeze_count_down = INITIAL_FROZEN_COUNT_DOWN;
				#endif
				
				skullXCountDown = SKULL_COUNT_DOWN;
				skullYCountDown = SKULL_COUNT_DOWN;		
				#if !defined(FULL_GAME)
					skullSlowDown = INITIAL_SKULL_SLOWDOWN;
				#endif
			#endif

			#if !defined(FULL_GAME)
				ghostSlowDown = INITIAL_GHOST_SLOWDOWN-(uint16_t)level*256;
			#else
				ghostSlowDown = computeGhostSlowDown();
			#endif
			
			CLEAR_SCREEN();
			#if !defined(LESS_TEXT)
				// Clear the screen, put cursor in upper left corner

				printLevel();
				SLEEP(1);
				CLEAR_SCREEN();
			#endif
				
			
			#if defined(FULL_GAME)
						
				arrowRange = computeArrowRange();
			
				if(bossLevel())
				{
					printKillTheSkull();
					SLEEP(2);
					ghostCount = 1+(level>>2);
				}
				CLEAR_SCREEN();
				
				updateInnerWallVerticalData();
			#endif
			
			printPressKeyToStart();
			WAIT_PRESS();
			CLEAR_SCREEN();
			
			
			#if !defined(TINY_GAME) && !defined(NO_BORDERS)
				DRAW_BORDERS();
			#endif
			
			fillLevelWithCharacters(ghostCount);			
			
			#if !defined(TINY_GAME)
				constructItems();	
				
				#if !defined(NO_STATS)
				displayStatsTitles();
				#endif
			#endif
			
			displayStats();		
			#if !defined(NO_STATS)	
				printLevelStats();
				printLivesStats();
			#endif
			
			//
			#if !defined(TINY_GAME)
				#if !defined(NO_STATS)
				printGunsStats();
				#endif
				printGhostCountStats();
			#endif		
			
			#if defined(FULL_GAME)
                #if !defined(BENCHMARK)
                    while(player._status && ((ghostCount>0 && !bossLevel()) || (skull._status && bossLevel()))) // while alive && there are still ghosts
                #else
                    Ticks = clock();

                    while(benchmark_count<BENCHMARK_MAX && player._status && ((ghostCount>0 && !bossLevel()) || (skull._status && bossLevel()))) // while alive && there are still ghosts
                #endif
			#else
                while(player._status && (ghostCount>0) )
			#endif
			{
                #if defined(BENCHMARK)
                    ++benchmark_count;
                #endif
				#if defined(DEBUG_END)
					gameCompleted();
				#endif
					
				#if !defined(TURN_BASED)
					MOVE_PLAYER();				
					_DRAW_PLAYER();	
				#endif
				
				#if defined(SLOW_DOWN)
					_slow_down();
				#endif
						
				#if defined(FULL_GAME)
					handle_rockets();
					handle_horizontal_missiles();
				#endif
				
				#if !defined(TINY_GAME) || defined(TURN_BASED)
				++loop;
				#endif
				
				#if !defined(TINY_GAME)
				if(points>(extraLifeThroughPointsCounter*EXTRA_LIFE_THROUGH_POINTS))
				{
					++extraLifeThroughPointsCounter;
					PING_SOUND();
					++lives;
					printLivesStats();
				}
				#endif
				
				#if !defined(FULL_GAME)
					if(ghostSlowDown) 
					{
						--ghostSlowDown;
					}
				#else
					ghostSlowDown = computeGhostSlowDown();
				#endif
			
				#if !defined(TINY_GAME)
					handle_bullet();
				#endif
				
				#if !defined(TINY_GAME)						

					if(!freezeActive)
					{
						#if defined(FULL_GAME)
							if(confuseActive && skullActive && skull._status)
							{
								chasedByGhosts=&skull;
							}
							else
							{
								chasedByGhosts=&player;
							}
							chaseCharacter(chasedByGhosts, ghostSlowDown);


							#if BOMBS_NUMBER==4
								if((level==15 || level==20) && ghostCount<=2)
								{
									deleteCharacter(&bombs[loop&3]);
									#if defined(SIMPLE_STRATEGY)
										moveTowardCharacter(chasedByGhosts, &bombs[loop&3]);
									#else
										moveTowardCharacter(chasedByGhosts, &bombs[loop&3], (uint8_t) (1+(uint8_t)(loop&3))<<1);
									#endif
								}
							#endif							
							++ghostLevel;
						#else
							chaseCharacter(ghostSlowDown);
						#endif
						
					}
					
					if(skull._status)
					{
						handle_skull();
					}

					// This detects collisions of ghosts that have just moved
					if(bullet._status)
					{
						checkBullet(&bullet);
					}
				#else
					#if !defined(NO_CHASE)
						chaseCharacter(ghostSlowDown);
					#endif
				#endif
				
				// Check collisions bombs vs ghosts
				for(ind=0;ind<GHOSTS_NUMBER;++ind)
					{
						checkBombsVsGhost(&ghosts[ind]);
					}
				
				#if !defined(TINY_GAME)
					handle_extraPoints_item();
					handle_gun_item();
					handle_powerup_item();
					handle_freeze_count_down();					
				#endif
				
				#if defined(FULL_GAME)
					handle_powerup2_item();					
					handle_freeze_item();	
					handle_invincibility_item();
					handle_invincibility_count_down();					

					if(chase_present_on_level_condition)
					{
						handle_chase_item();
						handle_chasing_bullet();						
						if(super_present_on_level)
						{
							handle_super_item();
							if(extraLife_present_on_level)
							{
								handle_extraLife_item();
							}
						}
					}
										
					if(confuse_present_on_level_condition)
					{
						handle_confuse_item();
						handle_confuse_count_down();
						if(zombie_present_on_level)
						{
							handle_zombie_item();
							handle_zombie_count_down();	
							if(zombieActive && !(loop&15))
							{
								points+=ZOMBIE_BONUS;
								displayStats();
								reducePowerUpsCoolDowns();
							}
						}
					}
					
				#endif
				
				#if defined(FULL_GAME)
					if(wallReached(&player) || 
					   (!invincibilityActive && (playerReachedGhosts() || characterReachedBombs(&player) || innerWallReached(&player) || (horizontalWallsLevel() && horizontalWallsReached())))
					  )
				#else
					if(wallReached(&player) || playerReachedGhosts() || characterReachedBombs(&player))
				#endif
					{
						playerDies();
					}

				
				#if defined(FULL_GAME)
					SKIP_DRAW
					{
						displayBombs();	
					}
					SKIP_MORE_DRAW
					{						
						DRAW_VERTICAL_LINE(XSize/2, YSize/2-(innerVerticalWallLength/2), innerVerticalWallLength);			
				
						if(horizontalWallsLevel())
						{				
							horizontalWallsLength = HORIZONTAL_WALLS_INITIAL_LENGTH + (level>>4) + (uint8_t) (loop/HORIZONTAL_WALLS_INCREASE_LOOP);		
							DRAW_HORIZONTAL_WALLS(horizontalWallsLength);	
						}						
					}
										
				#else
					SKIP_MORE_DRAW
					{
						displayBombs();
					}
				#endif
				
				// Display ghosts
				SKIP_DRAW
				{
					for(ind=0;ind<GHOSTS_NUMBER;++ind)
					{
						displayGhost(&ghosts[ind]);
					}
				}
					
				#if defined(TURN_BASED) 
					if((loop<TURN_BASED_MAX_LOOP) || loop&1)
					{
						MOVE_PLAYER();
					}					
					_DRAW_PLAYER();	
				#endif	
				REFRESH();
			}; // end inner while [while (player._alive && ghostCount>0), i.e., exit on death or end of level]
    
            #if defined(BENCHMARK)
                TicksDelta = clock() - Ticks;
                Sec = (uint16_t) (TicksDelta / CLOCKS_PER_SEC);
                Milli = ((TicksDelta % CLOCKS_PER_SEC) * 1000) / CLOCKS_PER_SEC;
                printf ("\nTime used: %u.%03u secs = %u ticks\n", Sec, Milli, (uint16_t) TicksDelta);    
                getchar();
            #endif

			if(player._status) // if level finished
			{
				#if defined(BETWEEN_LEVEL)
					chasedEnemyPtr = &player;
					SHOW_DOWN();
				#endif
				
				#if defined(FULL_GAME)
					#if !defined(LESS_TEXT)
						SLEEP(1);
						printVictoryMessage();
						SLEEP(2);

						CLEAR_SCREEN();	
					#endif

					points+= LEVEL_BONUS;
				#endif			

				ghostCount = GHOSTS_NUMBER;
				
				#if defined(FULL_GAME)			
					if(bossLevel())
					{	
						PING_SOUND();
						#if !defined(LESS_TEXT)
							printExtraLife();
							SLEEP(2);
						#endif
						++lives;
						skullsKilled = 1;
						missileBasesDestroyed/=2;
					}
					else
					{
						if(!skull._status)
						{
							++skullsKilled;
						}
						missileBasesDestroyed+=dead_rockets;
					}
				#endif
				++level;

			}
			else // if dead
			{		
				#if defined(END_SCREEN) || defined(DANCE)
					for(bulletDirection=0;bulletDirection<80;++bulletDirection)
					{
						for(ind=0;ind<GHOSTS_NUMBER;++ind)
						{
							if(ghosts[ind]._status)
								dance(&ghosts[ind]);
						}
					}
				#endif
				
				#if defined(BETWEEN_LEVEL)
					chasedEnemyPtr = &skull;	
				#endif
				
				#if !defined(TINY_GAME)
					CLEAR_SCREEN();
				#endif
				if(--lives>0)
				{
					player._status = 1;
				}
			}
			#if defined(BETWEEN_LEVEL)
				spiral(chasedEnemyPtr, 2*MIN_SIZE-18);
				SLEEP(1);
			#endif				
		} while (player._status && (level<(FINAL_LEVEL+1))); // lives left and not completed game game 
			
		if(level==FINAL_LEVEL+1) // if completed game
		{
			gameCompleted();
			#if !defined(NO_SLEEP)
				SLEEP(1);
			#else
				WAIT_PRESS();
			#endif
		}

		// GAME OVER	
		CLEAR_SCREEN();
		printGameOver();
		
		#if !defined(NO_SLEEP)
			SLEEP(1);
		#else
			WAIT_PRESS();
		#endif
		
		#if !defined(TINY_GAME) && !defined(LESS_TEXT)
			CLEAR_SCREEN();
			finalScore();
		
			#if !defined(NO_SLEEP)
				SLEEP(1);
			#else
				WAIT_PRESS();
			#endif
		
		#endif
		if(points>highScore)
		{
			highScore = points;
		}
		
	} // while(1) -> restart from the beginning

	return EXIT_SUCCESS;
}
