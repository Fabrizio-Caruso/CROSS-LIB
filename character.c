#include "character.h"


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
	gotoxy(characterPtr->_x,characterPtr->_y);
	cputc(' ');
}

void displayCharacter(Character* characterPtr)
{
	gotoxy(characterPtr->_x,characterPtr->_y);
	cputc(characterPtr->_ch);
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

