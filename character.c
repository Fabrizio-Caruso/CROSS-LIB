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

void displayDeadGhosts(Character * ghostPtr1, Character * ghostPtr2, 
						Character * ghostPtr3, Character * ghostPtr4,
						Character * ghostPtr5, Character * ghostPtr6, 
						Character * ghostPtr7, Character * ghostPtr8)
{
	SET_TEXT_COLOR(COLOR_RED);
	if(!(ghostPtr1->_alive))
		DRAW(ghostPtr1->_x,ghostPtr1->_y,'X');
	
	if(!(ghostPtr2->_alive))
		DRAW(ghostPtr2->_x,ghostPtr2->_y,'X');
	
	if(!(ghostPtr3->_alive))
		DRAW(ghostPtr3->_x,ghostPtr3->_y,'X');
	
	if(!(ghostPtr4->_alive))
		DRAW(ghostPtr4->_x,ghostPtr4->_y,'X');
	
	if(!(ghostPtr5->_alive))
		DRAW(ghostPtr5->_x,ghostPtr5->_y,'X');
	
	if(!(ghostPtr6->_alive))
		DRAW(ghostPtr6->_x,ghostPtr6->_y,'X');

	if(!(ghostPtr7->_alive))
		DRAW(ghostPtr7->_x,ghostPtr7->_y,'X');

	if(!(ghostPtr8->_alive))
		DRAW(ghostPtr8->_x,ghostPtr8->_y,'X');
	SET_TEXT_COLOR(TEXT_COLOR);
}

int isCharacterAtLocation(short x, short y, Character * characterPtr)
{
	return(characterPtr->_x==x) && (characterPtr->_y==y);
}

int areCharctersAtSamePosition(Character* lhs, Character* rhs)
{
	return (lhs->_x==rhs->_x)&&(lhs->_y==rhs->_y);
}

int leftDanger(Character* characterPtr, Character* bombPtr)
{
	return (characterPtr->_y == bombPtr->_y) && (characterPtr->_x-1 == bombPtr->_x);
}

int rightDanger(Character* characterPtr, Character* bombPtr)
{
	return (characterPtr->_y == bombPtr->_y) && (characterPtr->_x+1 == bombPtr->_x);	  
}

int upDanger(Character* characterPtr, Character* bombPtr)
{
	return (characterPtr->_x == bombPtr->_x) && (characterPtr->_y-1 == bombPtr->_y);
}

int downDanger(Character* characterPtr, Character* bombPtr)
{
	return (characterPtr->_x == bombPtr->_x) && (characterPtr->_y+1 == bombPtr->_y);	  
}


int leftBombs(Character* characterPtr, 
              Character* bombPtr1,  Character* bombPtr2, 
              Character* bombPtr3,  Character* bombPtr4)
{
	return leftDanger(characterPtr, bombPtr1) || leftDanger(characterPtr, bombPtr2) || 
	       leftDanger(characterPtr, bombPtr3) || leftDanger(characterPtr, bombPtr4);
}

int rightBombs(Character* characterPtr, 
              Character* bombPtr1,  Character* bombPtr2, 
              Character* bombPtr3,  Character* bombPtr4)
{
	return rightDanger(characterPtr, bombPtr1) || rightDanger(characterPtr, bombPtr2) || 
	       rightDanger(characterPtr, bombPtr3) || rightDanger(characterPtr, bombPtr4);
}

int upBombs(Character* characterPtr, 
              Character* bombPtr1,  Character* bombPtr2, 
              Character* bombPtr3,  Character* bombPtr4)
{
	return upDanger(characterPtr, bombPtr1) || upDanger(characterPtr, bombPtr2) || 
	       upDanger(characterPtr, bombPtr3) || upDanger(characterPtr, bombPtr4);
}

int downBombs(Character* characterPtr, 
              Character* bombPtr1,  Character* bombPtr2, 
              Character* bombPtr3,  Character* bombPtr4)
{
	return downDanger(characterPtr, bombPtr1) || downDanger(characterPtr, bombPtr2) || 
	       downDanger(characterPtr, bombPtr3) || downDanger(characterPtr, bombPtr4);
}


int leftGhosts(Character* characterPtr, 
              Character* ghostPtr1,  Character* ghostPtr2, 
              Character* ghostPtr3,  Character* ghostPtr4,
              Character* ghostPtr5,  Character* ghostPtr6, 
              Character* ghostPtr7)
{
	return leftDanger(characterPtr, ghostPtr1) || leftDanger(characterPtr, ghostPtr2) || 
	       leftDanger(characterPtr, ghostPtr3) || leftDanger(characterPtr, ghostPtr4) ||
		   leftDanger(characterPtr, ghostPtr5) || leftDanger(characterPtr, ghostPtr6) || 
		   leftDanger(characterPtr, ghostPtr7);
}

int rightGhosts(Character* characterPtr, 
              Character* ghostPtr1,  Character* ghostPtr2, 
              Character* ghostPtr3,  Character* ghostPtr4,
              Character* ghostPtr5,  Character* ghostPtr6, 
              Character* ghostPtr7)
{
	return rightDanger(characterPtr, ghostPtr1) || rightDanger(characterPtr, ghostPtr2) || 
	       rightDanger(characterPtr, ghostPtr3) || rightDanger(characterPtr, ghostPtr4) ||
		   rightDanger(characterPtr, ghostPtr5) || rightDanger(characterPtr, ghostPtr6) || 
		   rightDanger(characterPtr, ghostPtr7);
}

int upGhosts(Character* characterPtr, 
              Character* ghostPtr1,  Character* ghostPtr2, 
              Character* ghostPtr3,  Character* ghostPtr4,
              Character* ghostPtr5,  Character* ghostPtr6, 
              Character* ghostPtr7)
{
	return upDanger(characterPtr, ghostPtr1) || upDanger(characterPtr, ghostPtr2) || 
		   upDanger(characterPtr, ghostPtr3) || upDanger(characterPtr, ghostPtr4) || 
	       upDanger(characterPtr, ghostPtr5) || upDanger(characterPtr, ghostPtr6) ||
		   upDanger(characterPtr, ghostPtr7);
}

int downGhosts(Character* characterPtr, 
              Character* ghostPtr1,  Character* ghostPtr2, 
              Character* ghostPtr3,  Character* ghostPtr4,
              Character* ghostPtr5,  Character* ghostPtr6, 
              Character* ghostPtr7)
{
	return downDanger(characterPtr, ghostPtr1) || downDanger(characterPtr, ghostPtr2) || 
	       downDanger(characterPtr, ghostPtr3) || downDanger(characterPtr, ghostPtr4) ||
		   downDanger(characterPtr, ghostPtr5) || downDanger(characterPtr, ghostPtr6) ||
		   downDanger(characterPtr, ghostPtr7);
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
}

int playerReached(Character * hunterPtr1, Character * hunterPtr2, Character * hunterPtr3, Character * hunterPtr4, 
				  Character * hunterPtr5, Character * hunterPtr6, Character * hunterPtr7, Character * hunterPtr8, 
                  Character* preyPtr)
{
	return(areCharctersAtSamePosition(hunterPtr1,preyPtr) || areCharctersAtSamePosition(hunterPtr2,preyPtr) ||
		   areCharctersAtSamePosition(hunterPtr3,preyPtr) || areCharctersAtSamePosition(hunterPtr4,preyPtr) ||
		   areCharctersAtSamePosition(hunterPtr5,preyPtr) || areCharctersAtSamePosition(hunterPtr6,preyPtr) ||
		   areCharctersAtSamePosition(hunterPtr7,preyPtr) || areCharctersAtSamePosition(hunterPtr8,preyPtr));
}

int playerReachedBombs(Character * bombPtr1, Character * bombPtr2, Character * bombPtr3, Character * bombPtr4,  
					   Character* ghostPtr)
{
	return(areCharctersAtSamePosition(bombPtr1,ghostPtr) || areCharctersAtSamePosition(bombPtr2,ghostPtr) ||
		   areCharctersAtSamePosition(bombPtr3,ghostPtr) || areCharctersAtSamePosition(bombPtr4,ghostPtr));
}

int charactersMeet(Character * hunterPtr1, Character * hunterPtr2, Character * hunterPtr3, 
				   Character * hunterPtr4, Character * hunterPtr5, Character * hunterPtr6,
				   Character * hunterPtr7,
				   Character* preyPtr)
{
	return(areCharctersAtSamePosition(hunterPtr1,preyPtr) || areCharctersAtSamePosition(hunterPtr2,preyPtr) ||
	       areCharctersAtSamePosition(hunterPtr3,preyPtr) || areCharctersAtSamePosition(hunterPtr4,preyPtr) ||
	       areCharctersAtSamePosition(hunterPtr5,preyPtr) || areCharctersAtSamePosition(hunterPtr6,preyPtr) ||
		   areCharctersAtSamePosition(hunterPtr7,preyPtr));
}


void checkBombsVsGhost(Character * bombPtr1, Character * bombPtr2, 
					   Character * bombPtr3, Character * bombPtr4,
					   Character * ghostPtr)
{
	if(ghostPtr->_alive && playerReachedBombs(bombPtr1, bombPtr2, bombPtr3, bombPtr4, ghostPtr))
	{
		DRAW(ghostPtr->_x,ghostPtr->_y,'X');
		die(ghostPtr);
		points+=GHOST_VS_BOMBS_BONUS;
		--ghostCount;
	}
}
						

void checkBombsVsGhosts(Character * bombPtr1, Character * bombPtr2, 
						Character * bombPtr3, Character * bombPtr4,
						Character * ghostPtr1, Character * ghostPtr2, 
						Character * ghostPtr3, Character * ghostPtr4,
						Character * ghostPtr5, Character * ghostPtr6, 
						Character * ghostPtr7, Character * ghostPtr8)
{
	checkBombsVsGhost(bombPtr1, bombPtr2, bombPtr3, bombPtr4, ghostPtr1);
	checkBombsVsGhost(bombPtr1, bombPtr2, bombPtr3, bombPtr4, ghostPtr2);
	checkBombsVsGhost(bombPtr1, bombPtr2, bombPtr3, bombPtr4, ghostPtr3);
	checkBombsVsGhost(bombPtr1, bombPtr2, bombPtr3, bombPtr4, ghostPtr4);
	checkBombsVsGhost(bombPtr1, bombPtr2, bombPtr3, bombPtr4, ghostPtr5);
	checkBombsVsGhost(bombPtr1, bombPtr2, bombPtr3, bombPtr4, ghostPtr6);
	checkBombsVsGhost(bombPtr1, bombPtr2, bombPtr3, bombPtr4, ghostPtr7);
	checkBombsVsGhost(bombPtr1, bombPtr2, bombPtr3, bombPtr4, ghostPtr8);
}


int safeLocation(int x, int y, 
				Character * bombPtr1, Character * bombPtr2, 
				Character * bombPtr3, Character * bombPtr4,
				Character * ghostPtr1, Character * ghostPtr2, 
				Character * ghostPtr3, Character * ghostPtr4,
				Character * ghostPtr5, Character * ghostPtr6, 
				Character * ghostPtr7, Character * ghostPtr8)
{
	return !isCharacterAtLocation(x,y,bombPtr1) && !isCharacterAtLocation(x,y,bombPtr2) &&
	!isCharacterAtLocation(x,y,bombPtr3) && !isCharacterAtLocation(x,y,bombPtr4) &&
	!isCharacterAtLocation(x,y,ghostPtr1) && !isCharacterAtLocation(x,y,ghostPtr2) &&
	!isCharacterAtLocation(x,y,ghostPtr3) && !isCharacterAtLocation(x,y,ghostPtr4) &&
	!isCharacterAtLocation(x,y,ghostPtr5) && !isCharacterAtLocation(x,y,ghostPtr6) &&
	!isCharacterAtLocation(x,y,ghostPtr7) && !isCharacterAtLocation(x,y,ghostPtr8);
}


void relocateCharacter(Character * characterPtr, 
						Character * bombPtr1, Character * bombPtr2, 
						Character * bombPtr3, Character * bombPtr4,
						Character * ghostPtr1, Character * ghostPtr2, 
						Character * ghostPtr3, Character * ghostPtr4,
						Character * ghostPtr5, Character * ghostPtr6, 
						Character * ghostPtr7, Character * ghostPtr8)
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
	
	safe = safeLocation(x,y, bombPtr1, bombPtr2, bombPtr3, bombPtr4, 
						ghostPtr1, ghostPtr2, ghostPtr3, ghostPtr4, 
						ghostPtr5, ghostPtr6, ghostPtr7, ghostPtr8);
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


void checkGhostsVsGhosts(Character *ghostPtr1, Character *ghostPtr2, Character *ghostPtr3, Character *ghostPtr4,
						 Character *ghostPtr5, Character *ghostPtr6, Character *ghostPtr7, Character *ghostPtr8)
{
	if(ghostPtr8->_alive && charactersMeet(ghostPtr1, ghostPtr2, ghostPtr3, ghostPtr4, ghostPtr5, ghostPtr6, ghostPtr7, ghostPtr8))
	{
		die(ghostPtr8);
		points+=GHOST_VS_GHOST_BONUS;
	    --ghostCount;
	}
	if(ghostPtr1->_alive && charactersMeet(ghostPtr2, ghostPtr3, ghostPtr4, ghostPtr5, ghostPtr6, ghostPtr7, ghostPtr8, ghostPtr1))
	{
		die(ghostPtr1);
		points+=GHOST_VS_GHOST_BONUS;
		--ghostCount;
	}
	if(ghostPtr2->_alive && charactersMeet(ghostPtr3, ghostPtr4, ghostPtr5, ghostPtr6, ghostPtr7, ghostPtr8, ghostPtr1, ghostPtr2))
	{
		die(ghostPtr2);
		points+=GHOST_VS_GHOST_BONUS;
		--ghostCount;
	}
	if(ghostPtr3->_alive && charactersMeet(ghostPtr4, ghostPtr5, ghostPtr6, ghostPtr7, ghostPtr8, ghostPtr1, ghostPtr2, ghostPtr3))
	{
		die(ghostPtr3);
		points+=GHOST_VS_GHOST_BONUS;
		--ghostCount;
	}
	if(ghostPtr4->_alive && charactersMeet(ghostPtr5, ghostPtr6, ghostPtr7, ghostPtr8, ghostPtr1, ghostPtr2, ghostPtr3, ghostPtr4))
	{
		die(ghostPtr4);
		points+=GHOST_VS_GHOST_BONUS;
		--ghostCount;
	}
	if(ghostPtr5->_alive && charactersMeet(ghostPtr6, ghostPtr7, ghostPtr8, ghostPtr1, ghostPtr2, ghostPtr3, ghostPtr4, ghostPtr5))
	{
		die(ghostPtr5);
		points+=GHOST_VS_GHOST_BONUS;
		--ghostCount;
	}
	if(ghostPtr6->_alive && charactersMeet(ghostPtr7, ghostPtr8, ghostPtr1, ghostPtr2, ghostPtr3, ghostPtr4, ghostPtr5, ghostPtr6))
	{
		die(ghostPtr6);
		points+=GHOST_VS_GHOST_BONUS;
		--ghostCount;
	}
	if(ghostPtr7->_alive && charactersMeet(ghostPtr8, ghostPtr1, ghostPtr2, ghostPtr3, ghostPtr4, ghostPtr5, ghostPtr6, ghostPtr7))
	{
		die(ghostPtr7);
		points+=GHOST_VS_GHOST_BONUS;
		--ghostCount;
	}
}
