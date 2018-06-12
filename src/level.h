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
 
#ifndef _LEVEL
#define _LEVEL

#include "character.h"

#if defined(TINY_GAME) || defined(NO_DEAD_GHOSTS)
	#define PLACE_DEAD_GHOST() \
		initializeCharacter(&ghosts[count], 0,(unsigned char) 0,0,&GHOST_IMAGE);
#else
	#define PLACE_DEAD_GHOST() \
		initializeCharacter(&ghosts[count],(unsigned char) (GHOSTS_NUMBER-count),(unsigned char) 1,0,&DEAD_GHOST_IMAGE);
#endif


// Check XSize to avoid overflow
#if XSize>48
	#define ROUND_X_POS (unsigned char) ((2*j-1)*(unsigned char)(XSize/6))
#else
	#define ROUND_X_POS (unsigned char) ((2*j-1)*XSize/6)
#endif

// Check YSize to avoid overflow
#if YSize>48
	#define ROUND_Y_POS (unsigned char) ((2*i-1)*(unsigned char)(YSize/6))
#else
	#define ROUND_Y_POS (unsigned char) ((2*i-1)*YSize/6)
#endif


#define PLACE_ROUND_GHOST() \
	initializeCharacter(&ghosts[count], \
		ROUND_X_POS, \
		ROUND_Y_POS, \
		1 ,&GHOST_IMAGE);

#define PLACE_FLAT_GHOST() \
	initializeCharacter(&ghosts[count],(unsigned char) (j*(XSize/5)),(i*(YSize/5)),1,&GHOST_IMAGE);

#define PLACE_FLAT_GHOST_6() \
	initializeCharacter(&ghosts[count],(unsigned char) (j*(XSize>>2)),(i*(YSize/5)),1,&GHOST_IMAGE);	
	
#define PLACE_NINTH_GHOST() \
	initializeCharacter(&ghosts[4],(unsigned char) (XSize-4),(unsigned char) (YSize-4),1,&GHOST_IMAGE);

	
#define ROUND_NINE_GHOSTS() \
	for(i=1;i<=3;++i) \
	{ \
		for(j=1;j<=3;++j) \
		{ \
			if(nGhosts>count) \
			{ \
				if(count!=4) \
				{ \
					PLACE_ROUND_GHOST(); \
				} \
				else \
				{ \
					PLACE_NINTH_GHOST(); \
				} \
			} \
			else \
			{ \
				PLACE_DEAD_GHOST(); \
			} \
			++count; \
		} \
	}


#define ROUND_EIGHT_GHOSTS() \
	for(i=1;i<=3;++i) \
	{ \
		for(j=1;j<=3;++j) \
		{ \
			if(i!=2 || j !=2) \
			{ \
				if(nGhosts>count) \
				{ \
					PLACE_ROUND_GHOST(); \
				} \
				else \
				{ \
					PLACE_DEAD_GHOST(); \
				} \
				++count; \
			} \
		} \
	}


#define FLAT_EIGHT_GHOSTS() \
	for(j=1;j<=4;++j) \
	{ \
		for(i=1;i<=4;i+=3) \
		{ \
			if(nGhosts>count) \
			{ \
				PLACE_FLAT_GHOST(); \
			} \
			else \
			{ \
				PLACE_DEAD_GHOST(); \
			} \
			++count; \
		} \
	}

	
#define FLAT_SIX_GHOSTS() \
	for(j=1;j<=3;++j) \
	{ \
		for(i=1;i<=4;i+=3) \
		{ \
			if(nGhosts>count) \
			{ \
				PLACE_FLAT_GHOST_6(); \
			} \
			else \
			{ \
				PLACE_DEAD_GHOST(); \
			} \
			++count; \
		} \
	}
	
	

#if defined(BETWEEN_LEVEL)
	void spiral(register Character *characterPtr, unsigned char length);
#endif

void fillLevelWithCharacters(unsigned char nGhosts);

#if defined(FULL_GAME)
	void updateInnerWallVerticalData(void);
	
	unsigned char oneMissileLevel(void);
	
	unsigned char rocketLevel(void);

	unsigned char missileLevel(void);	

	unsigned char bossLevel(void);
	
	unsigned char horizontalWallsLevel(void);
	
	void initializeAwayFromWall(Character * characterPtr, unsigned char x, unsigned char y, unsigned char status, Image *imagePtr);

#endif 



#endif // _LEVEL
