#ifndef _CHARACTER
#define _CHARACTER

struct CharacterStruct
{
	// character coordinates
	short _x;
	short _y;
	
	// how to display the character (i.e., which ASCII character to use
	char _ch;
	
	// _status decides whether the character is active
	short _status;
	
	//_alive decides whether it is dead or alive
	short _alive;
	
};

typedef struct CharacterStruct Character;

void initializeCharacter(Character* characterPtr, int x, int y, char ch, short status);

void setCharacterPosition(Character* characterPtr, short x, short y);

void setCharacterDisplay(Character* characterPtr, char ch);

void deleteCharacter(Character* characterPtr);

void displayCharacter(Character* characterPtr);

void displayDeadGhosts(Character * ghostPtr1, Character * ghostPtr2, 
						Character * ghostPtr3, Character * ghostPtr4,
						Character * ghostPtr5, Character * ghostPtr6, 
						Character * ghostPtr7, Character * ghostPtr8);

int isCharacterAtLocation(short x, short y, Character * characterPtr);

int areCharctersAtSamePosition(Character* lhs, Character* rhs);

int leftDanger(Character* characterPtr, Character* bombPtr);

int rightDanger(Character* characterPtr, Character* bombPtr);

int upDanger(Character* characterPtr, Character* bombPtr);

int downDanger(Character* characterPtr, Character* bombPtr);

int leftBombs(Character* characterPtr, 
              Character* bombPtr1,  Character* bombPtr2, 
              Character* bombPtr3,  Character* bombPtr4);

int rightBombs(Character* characterPtr, 
              Character* bombPtr1,  Character* bombPtr2, 
              Character* bombPtr3,  Character* bombPtr4);

int upBombs(Character* characterPtr, 
              Character* bombPtr1,  Character* bombPtr2, 
              Character* bombPtr3,  Character* bombPtr4);

int downBombs(Character* characterPtr, 
              Character* bombPtr1,  Character* bombPtr2, 
              Character* bombPtr3,  Character* bombPtr4);

int leftGhosts(Character* characterPtr, 
              Character* ghostPtr1,  Character* ghostPtr2, 
              Character* ghostPtr3,  Character* ghostPtr4,
              Character* ghostPtr5,  Character* ghostPtr6, 
              Character* ghostPtr7);
			  
int rightGhosts(Character* characterPtr, 
              Character* ghostPtr1,  Character* ghostPtr2, 
              Character* ghostPtr3,  Character* ghostPtr4,
              Character* ghostPtr5,  Character* ghostPtr6, 
              Character* ghostPtr7);
			  
int upGhosts(Character* characterPtr, 
              Character* ghostPtr1,  Character* ghostPtr2, 
              Character* ghostPtr3,  Character* ghostPtr4,
              Character* ghostPtr5,  Character* ghostPtr6, 
              Character* ghostPtr7);

int downGhosts(Character* characterPtr, 
              Character* ghostPtr1,  Character* ghostPtr2, 
              Character* ghostPtr3,  Character* ghostPtr4,
              Character* ghostPtr5,  Character* ghostPtr6, 
              Character* ghostPtr7);

int wallReached(Character *characterPtr);


void die(Character * playerPtr);

			int playerReached(Character * hunterPtr1, Character * hunterPtr2, Character * hunterPtr3, Character * hunterPtr4, 
				  Character * hunterPtr5, Character * hunterPtr6, Character * hunterPtr7, Character * hunterPtr8, 
                  Character* preyPtr);

int playerReachedBombs(Character * bombPtr1, Character * bombPtr2, Character * bombPtr3, Character * bombPtr4,  
					   Character* ghostPtr);

int charactersMeet(Character * hunterPtr1, Character * hunterPtr2, Character * hunterPtr3, 
				   Character * hunterPtr4, Character * hunterPtr5, Character * hunterPtr6,
				   Character * hunterPtr7,
				   Character* preyPtr);
  
  
void checkBombsVsGhost(Character * bombPtr1, Character * bombPtr2, 
					   Character * bombPtr3, Character * bombPtr4,
					   Character * ghostPtr);
						

void checkBombsVsGhosts(Character * bombPtr1, Character * bombPtr2, 
						Character * bombPtr3, Character * bombPtr4,
						Character * ghostPtr1, Character * ghostPtr2, 
						Character * ghostPtr3, Character * ghostPtr4,
						Character * ghostPtr5, Character * ghostPtr6, 
						Character * ghostPtr7, Character * ghostPtr8);


int safeLocation(int x, int y, 
				Character * bombPtr1, Character * bombPtr2, 
				Character * bombPtr3, Character * bombPtr4,
				Character * ghostPtr1, Character * ghostPtr2, 
				Character * ghostPtr3, Character * ghostPtr4,
				Character * ghostPtr5, Character * ghostPtr6, 
				Character * ghostPtr7, Character * ghostPtr8);

void relocateCharacter(Character * characterPtr, 
						Character * bombPtr1, Character * bombPtr2, 
						Character * bombPtr3, Character * bombPtr4,
						Character * ghostPtr1, Character * ghostPtr2, 
						Character * ghostPtr3, Character * ghostPtr4,
						Character * ghostPtr5, Character * ghostPtr6, 
						Character * ghostPtr7, Character * ghostPtr8);

short innerWallReached(Character *characterPtr);

short nearInnerWall(Character *characterPtr);


void checkGhostsVsGhosts(Character *ghostPtr1, Character *ghostPtr2, Character *ghostPtr3, Character *ghostPtr4,
						 Character *ghostPtr5, Character *ghostPtr6, Character *ghostPtr7, Character *ghostPtr8);


#endif