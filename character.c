#include <stdlib.h>



#include "character.h"
#include "settings.h"

#include "display_macros.h"

extern unsigned long points;
extern unsigned short innerVerticalWallX;
extern unsigned short innerVerticalWallY;
extern unsigned short innerVerticalWallLength;

extern unsigned char XSize;
extern unsigned char YSize;

extern unsigned short ghostCount;


void initializeCharacter(Character* characterPtr, int x, int y, char ch, short status)
{
	characterPtr->_x = x;
	characterPtr->_y = y;
	characterPtr->_ch = ch;
	characterPtr->_status = status;
	characterPtr->_alive = 1; // TODO: Maybe we should initialize this with a parameter
}

void setCharacterPosition(Character* characterPtr, short x, short y)
{
	characterPtr->_x = x;
	characterPtr->_y = y;
}

void setCharacterDisplay(Character* characterPtr, char ch)
{
	characterPtr->_ch = ch;
}

void deleteCharacter(Character* characterPtr)
{
	DRAW(characterPtr->_x,characterPtr->_y,' ');
}

void displayCharacter(Character* characterPtr)
{
	DRAW(characterPtr->_x, characterPtr->_y, characterPtr->_ch);
}

int isCharacterAtLocation(short x, short y, Character * characterPtr)
{
	return(characterPtr->_x==x) && (characterPtr->_y==y);
}

int areCharctersAtSamePosition(Character* lhs, Character* rhs)
{
	return (lhs->_x==rhs->_x)&&(lhs->_y==rhs->_y);
}


int wallReached(Character *characterPtr)
{
	return (characterPtr->_x==0)||(characterPtr->_x==XSize-1) || 
		   (characterPtr->_y==0)||(characterPtr->_y==YSize-1);
}

void die(Character * playerPtr)
{
	SET_TEXT_COLOR(COLOR_RED);
	DRAW(playerPtr->_x,playerPtr->_y,'X');
	SET_TEXT_COLOR(TEXT_COLOR);
	playerPtr->_status = 0;
	playerPtr->_alive = 0;
	
	playerPtr->_ch = 'X';
}

int playerReached(Character ** ghosts, 
                  Character* preyPtr)
{
	int i=0;
	for(;i<GHOSTS_NUMBER;++i)
	{
		if(areCharctersAtSamePosition(ghosts[i],preyPtr))
			return 1;
	}
	return 0;
}

int playerReachedBombs(Character ** bombs,  
					   Character* preyPtr)
{
	int i=0;
	for(;i<BOMBS_NUMBER;++i)
	{
		if(areCharctersAtSamePosition(bombs[i],preyPtr))
			return 1;
	}
	return 0;
}

int charactersMeet(short preyIndex, Character **ghosts)
{
	short i;
	for(i=0;i<GHOSTS_NUMBER;++i)
	{
		if((i!=preyIndex)&&areCharctersAtSamePosition(ghosts[i],ghosts[preyIndex]))
			return 1;
	}
	return 0;
}


void checkBombsVsGhost(Character ** bombs,
					   Character * ghostPtr)
{
	if(ghostPtr->_alive && playerReachedBombs(bombs, ghostPtr))
	{
		DRAW(ghostPtr->_x,ghostPtr->_y,'X');
		die(ghostPtr);
		points+=GHOST_VS_BOMBS_BONUS;
		--ghostCount;
	}
}
						

void checkBombsVsGhosts(Character ** bombs,
						Character ** ghosts)
{
	char i;
	for(i=0;i<GHOSTS_NUMBER;++i)
	{
		checkBombsVsGhost(bombs, ghosts[i]);
	}
}


int safeLocation(int x, int y, 
				Character ** bombs,
				Character ** ghosts)
{
	char i = 0;
	for(;i<GHOSTS_NUMBER;++i)
	{
		if(isCharacterAtLocation(x,y,ghosts[i]))
			return 0;
	}
	for(i=0;i<BOMBS_NUMBER;++i)
	{
		if(isCharacterAtLocation(x,y,bombs[i]))
			return 0;
	}
	return 1;
}


void relocateCharacter(Character * characterPtr, 
						Character ** bombs,
						Character ** ghosts)
{
	int x; int y; int x_offset; int y_offset;
	int safe = 0;
	while(!safe)
	{
	x_offset = rand() % 7;
	y_offset = rand() % 7;
	if((x_offset==0) && (y_offset==0))
		continue;
	x = characterPtr->_x -3 + x_offset; 
	y = characterPtr->_y -3 + y_offset;
	if(y<=3) // Avoid score line
		continue;
	if((x<2) || (x>XSize-2))
		continue;
	if((y<2) || (y>YSize-2))
		continue;
	
	safe = safeLocation(x,y, bombs, 
						ghosts);
	}
	characterPtr->_x = x;
	characterPtr->_y = y;
}


short innerWallReached(Character *characterPtr)
{
	return (characterPtr->_x==innerVerticalWallX) && (characterPtr->_y >= innerVerticalWallY) && (characterPtr->_y<= (innerVerticalWallY + innerVerticalWallLength-1));
}

short nearInnerWall(Character *characterPtr)
{
	return (characterPtr->_x>=innerVerticalWallX-1) && (characterPtr->_x<=innerVerticalWallX+1) &&
		   (characterPtr->_y >= innerVerticalWallY-1) && (characterPtr->_y<= (innerVerticalWallY + innerVerticalWallLength));
}


void checkGhostsVsGhosts(Character ** ghosts)
{
	char i;
	
	for(i=0;i<GHOSTS_NUMBER;++i)
	{
		if(ghosts[i]->_alive && charactersMeet(i, ghosts))
		{
			die(ghosts[i]);
			points+=GHOST_VS_GHOST_BONUS;
			--ghostCount;
		}
	}

}
