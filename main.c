/*
** Portable barely playable chasing game
**
** Fabrizio Caruso (fabrizio_caruso@hotmail.com)
**
*/

#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <joystick.h>

#include <unistd.h>

#include <time.h>

#define GHOST_VS_GHOST_BONUS 2500ul
#define GHOST_VS_BOMBS_BONUS 1000ul
#define POWER_UP_BONUS 500ul
#define LOOP_POINTS 10ul

unsigned int ghostLevel = 1u;
unsigned long points = 0ul;
unsigned int ghostSmartness = 3u;
unsigned int ghostSlowDown;
unsigned int powerUpCoolDown;


struct CharacterStruct
{
	// character coordinates
	int _x;
	int _y;
	
	// how to display the character (i.e., which ASCII character to use
	char _ch;
	
	// _status decides whether the character is active
	short _status;
	
	//_alive decides whether it is dead or alive
	short _alive;
	
};

typedef struct CharacterStruct Character;

void setCharacterPosition(Character* characterPtr, int x, int y)
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

int areCharctersAtSamePosition(Character* lhs, Character* rhs)
{
	return (lhs->_x==rhs->_x)&&(lhs->_y==rhs->_y);
}

int leftBomb(Character* characterPtr, Character* bombPtr)
{
	return (characterPtr->_y == bombPtr->_y) && (characterPtr->_x-1 == bombPtr->_x);
}

int rightBomb(Character* characterPtr, Character* bombPtr)
{
	return (characterPtr->_y == bombPtr->_y) && (characterPtr->_x+1 == bombPtr->_x);	  
}

int upBomb(Character* characterPtr, Character* bombPtr)
{
	return (characterPtr->_x == bombPtr->_x) && (characterPtr->_y-1 == bombPtr->_y);
}

int downBomb(Character* characterPtr, Character* bombPtr)
{
	return (characterPtr->_x == bombPtr->_x) && (characterPtr->_y+1 == bombPtr->_y);	  
}

int leftBombs(Character* characterPtr, 
              Character* bombPtr1,  Character* bombPtr2, 
              Character* bombPtr3,  Character* bombPtr4)
{
	return leftBomb(characterPtr, bombPtr1) || leftBomb(characterPtr, bombPtr2) || 
	       leftBomb(characterPtr, bombPtr3) || leftBomb(characterPtr, bombPtr4);
}

int rightBombs(Character* characterPtr, 
              Character* bombPtr1,  Character* bombPtr2, 
              Character* bombPtr3,  Character* bombPtr4)
{
	return rightBomb(characterPtr, bombPtr1) || rightBomb(characterPtr, bombPtr2) || 
	       rightBomb(characterPtr, bombPtr3) || rightBomb(characterPtr, bombPtr4);
}

int upBombs(Character* characterPtr, 
              Character* bombPtr1,  Character* bombPtr2, 
              Character* bombPtr3,  Character* bombPtr4)
{
	return upBomb(characterPtr, bombPtr1) || upBomb(characterPtr, bombPtr2) || 
	       upBomb(characterPtr, bombPtr3) || upBomb(characterPtr, bombPtr4);
}

int downBombs(Character* characterPtr, 
              Character* bombPtr1,  Character* bombPtr2, 
              Character* bombPtr3,  Character* bombPtr4)
{
	return downBomb(characterPtr, bombPtr1) || downBomb(characterPtr, bombPtr2) || 
	       downBomb(characterPtr, bombPtr3) || downBomb(characterPtr, bombPtr4);
}


void chaseCharacterXAvoidBombStrategy(Character* hunterPtr, Character* preyPtr, 
                    Character* bombPtr1, Character* bombPtr2,
					Character* bombPtr3, Character* bombPtr4)
{
	if(!rightBombs(hunterPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4) && 
	   hunterPtr->_x<preyPtr->_x)
	{
		deleteCharacter(hunterPtr);
		++hunterPtr->_x;
	}
	else if(!leftBombs(hunterPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4) && 
	         hunterPtr->_x>preyPtr->_x)
	{
		deleteCharacter(hunterPtr);
		--hunterPtr->_x;
	}
	else if(!downBombs(hunterPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4) && 
	        hunterPtr->_y<preyPtr->_y)
	{
		deleteCharacter(hunterPtr);
		++hunterPtr->_y;
	}
	else if(!upBombs(hunterPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4) && 
	        hunterPtr->_y>preyPtr->_y)
	{
		deleteCharacter(hunterPtr);
		--hunterPtr->_y;
	}
	displayCharacter(hunterPtr);
}

void chaseCharacterXStrategy(Character* hunterPtr, Character* preyPtr)
{
	if(hunterPtr->_x<preyPtr->_x)
	{
		deleteCharacter(hunterPtr);
		++hunterPtr->_x;
	}
	else if(hunterPtr->_x>preyPtr->_x)
	{
		deleteCharacter(hunterPtr);
		--hunterPtr->_x;
	}
	else if(hunterPtr->_y<preyPtr->_y)
	{
		deleteCharacter(hunterPtr);
		++hunterPtr->_y;
	}
	else if(hunterPtr->_y>preyPtr->_y)
	{
		deleteCharacter(hunterPtr);
		--hunterPtr->_y;
	}
	displayCharacter(hunterPtr);
}

void chaseCharacterYAvoidBombStrategy(Character* hunterPtr, Character* preyPtr, 
                    Character* bombPtr1, Character* bombPtr2,
					Character* bombPtr3, Character* bombPtr4)
{
    if(!downBombs(hunterPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4) && hunterPtr->_y<preyPtr->_y)
	{
		deleteCharacter(hunterPtr);
		++hunterPtr->_y;
	}
	else if(!upBombs(hunterPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4) && hunterPtr->_y>preyPtr->_y)
	{
		deleteCharacter(hunterPtr);
		--hunterPtr->_y;
	}
	else if(!rightBombs(hunterPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4) && hunterPtr->_x<preyPtr->_x)
	{
		deleteCharacter(hunterPtr);
		++hunterPtr->_x;
	}
	else if(!leftBombs(hunterPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4) && hunterPtr->_x>preyPtr->_x)
	{
		deleteCharacter(hunterPtr);
		--hunterPtr->_x;
	}

	displayCharacter(hunterPtr);
}

void chaseCharacterYStrategy(Character* hunterPtr, Character* preyPtr)
{
    if(hunterPtr->_y<preyPtr->_y)
	{
		deleteCharacter(hunterPtr);
		++hunterPtr->_y;
	}
	else if(hunterPtr->_y>preyPtr->_y)
	{
		deleteCharacter(hunterPtr);
		--hunterPtr->_y;
	}
	else if(hunterPtr->_x<preyPtr->_x)
	{
		deleteCharacter(hunterPtr);
		++hunterPtr->_x;
	}
	else if(hunterPtr->_x>preyPtr->_x)
	{
		deleteCharacter(hunterPtr);
		--hunterPtr->_x;
	}

	displayCharacter(hunterPtr);
}

void chaseCharacter(Character* hunterPtr, Character* preyPtr, 
                    Character* bombPtr1, Character* bombPtr2,
					Character* bombPtr3, Character* bombPtr4)
{
	if(rand()%10 > ghostSmartness)
	{
		if(rand()%2) // Select chase strategy
		{
			chaseCharacterXStrategy(hunterPtr, preyPtr);
		}
		else
		{
			chaseCharacterYStrategy(hunterPtr, preyPtr);
		}
	}
	else
	{
		if(rand()%2) // Select chase strategy
		{
			chaseCharacterXAvoidBombStrategy(hunterPtr, preyPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4);
		}
		else
		{
			chaseCharacterYAvoidBombStrategy(hunterPtr, preyPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4);
		}
	}
}

void chaseCharacterIf(Character* hunterPtr, Character* preyPtr, 
                    Character* bombPtr1, Character* bombPtr2,
					Character* bombPtr3, Character* bombPtr4)
{
	if(((hunterPtr->_status==1) && (hunterPtr->_alive==1)) && rand()>ghostSlowDown)
	{
		chaseCharacter(hunterPtr, preyPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4);
	}
	else
	{
		displayCharacter(hunterPtr);
	}
}					 

void chasePlayer(Character * hunterPtr1, Character * hunterPtr2, 
                 Character * hunterPtr3, Character * hunterPtr4,
				 Character * hunterPtr5, Character * hunterPtr6, 
                 Character * hunterPtr7, Character * hunterPtr8, 
                 Character* preyPtr, 
                 Character* bombPtr1, Character* bombPtr2,
				 Character* bombPtr3, Character* bombPtr4
				 )
{
	chaseCharacterIf(hunterPtr1, preyPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4);
	chaseCharacterIf(hunterPtr2, preyPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4);
	chaseCharacterIf(hunterPtr3, preyPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4);
	chaseCharacterIf(hunterPtr4, preyPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4);
    chaseCharacterIf(hunterPtr5, preyPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4);
	chaseCharacterIf(hunterPtr6, preyPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4);
	chaseCharacterIf(hunterPtr7, preyPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4);
	chaseCharacterIf(hunterPtr8, preyPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4);
}

void displayScore(unsigned long points)
{
	// Draw score 
	gotoxy(2,3);
	cputs("SCORE: ");
	
	gotoxy(9,3);
	cputs("       ");
	gotoxy(9,3);
	cprintf("%lu",points);
}

void displayGhostLevel()
{
	// Draw score 
	gotoxy(2,2);
	cputs("SPEED: ");
	
	gotoxy(9,2);
	cputs("       ");
	gotoxy(9,2);
	cprintf("%u",ghostLevel);
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

int powerUpReached(Character * hunterPtr, Character* preyPtr)
{
	return(areCharctersAtSamePosition(hunterPtr, preyPtr));
}


void movePlayer(Character *playerPtr, char kbInput)
{
	if((kbInput=='W') || (kbInput=='w'))
	{
		deleteCharacter(playerPtr);
		--playerPtr->_y;
	}
	else if((kbInput=='S') || (kbInput=='s'))
	{
		deleteCharacter(playerPtr);
		++playerPtr->_y;
	}
	else if((kbInput=='A') || (kbInput=='a'))
	{
		deleteCharacter(playerPtr);
		--playerPtr->_x;
	}
	else if((kbInput=='D') || (kbInput=='d'))
	{
		deleteCharacter(playerPtr);
		++playerPtr->_x;
	}
	displayCharacter(playerPtr);
}

void initializeCharacter(Character* characterPtr, int x, int y, char ch, short status)
{
	characterPtr->_x = x;
	characterPtr->_y = y;
	characterPtr->_ch = ch;
	characterPtr->_status = status;
	characterPtr->_alive = 1; // TODO: Maybe we should initialize this with a parameter
}

void drawBorders(int XSize, int YSize)
{
	/* Clear the screen, put cursor in upper left corner */
    clrscr ();
	
	/* Top line */
    cputc (CH_ULCORNER);
    chline (XSize - 2);
    cputc (CH_URCORNER);

    /* Vertical line, left side */
    cvlinexy (0, 1, YSize - 2);

    /* Bottom line */
    cputc (CH_LLCORNER);
    chline (XSize - 2);
    cputc (CH_LRCORNER);

    /* Vertical line, right side */
    cvlinexy (XSize - 1, 1, YSize - 2);
}

void setScreenColors()
{
    (void) textcolor (COLOR_WHITE);
    (void) bordercolor (COLOR_BLACK);
    (void) bgcolor (COLOR_BLACK);
}


void printCenteredMessage(int XSize, int YSize, char *Text)
{
	gotoxy ((XSize - strlen (Text)) / 2, YSize / 2);
    cprintf ("%s", Text);
}


void printPressKeyToStart(int XSize, int YSize)
{
	printCenteredMessage(XSize, YSize, "PRESS ANY KEY TO START");
}

void deleteCenteredMessage(int XSize, int YSize)
{
	gotoxy ((XSize - 22) / 2, YSize / 2);
    cputs( "                      ");
}

void toggleHunters(Character * hunterPtr1, Character * hunterPtr2, 
                   Character * hunterPtr3, Character * hunterPtr4, 
				   Character * hunterPtr5, Character * hunterPtr6, 
                   Character * hunterPtr7, Character * hunterPtr8, 
				   int loop)
{
	if(loop==5)
		hunterPtr1->_status = 1;
	if(loop==10)
		hunterPtr2->_status = 1;
	if(loop==20)
		hunterPtr3->_status = 1;
	if(loop==30)
		hunterPtr4->_status = 1;
	if(loop==40)
		hunterPtr5->_status = 1;
	if(loop==50)
		hunterPtr6->_status = 1;
	if(loop==60)
		hunterPtr7->_status = 1;
	if(loop==70)
		hunterPtr8->_status = 1;
}

void checkBombsVsGhost(Character * bombPtr1, Character * bombPtr2, 
					   Character * bombPtr3, Character * bombPtr4,
					   Character * ghostPtr)
{
	if(ghostPtr->_alive && playerReachedBombs(bombPtr1, bombPtr2, bombPtr3, bombPtr4, ghostPtr))
	{
		gotoxy(ghostPtr->_x,ghostPtr->_y);
		cputc('X');
		ghostPtr->_alive = 0;
		ghostPtr->_status = 0;
		points+=GHOST_VS_BOMBS_BONUS;
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


void initializeCharacters(Character * playerPtr, Character * powerUpPtr, 
						  Character * ghostPtr1, Character * ghostPtr2,
						  Character * ghostPtr3, Character * ghostPtr4,
						  Character * ghostPtr5, Character * ghostPtr6,
						  Character * ghostPtr7, Character * ghostPtr8,
						  Character * bombPtr1, Character * bombPtr2,
						  Character * bombPtr3, Character * bombPtr4,
						  )
{
	initializeCharacter(ghostPtr1,4,4,'1',0);
	displayCharacter(ghostPtr1);

	initializeCharacter(ghostPtr2,17,17,'2',0);
	displayCharacter(ghostPtr2);

	initializeCharacter(ghostPtr3,17,3,'3',0);
	displayCharacter(ghostPtr3);
	
	initializeCharacter(ghostPtr4,3,17,'4',0);
	displayCharacter(ghostPtr4);
	
	initializeCharacter(ghostPtr5,6,6,'5',0);
	displayCharacter(ghostPtr5);

	initializeCharacter(ghostPtr6,19,19,'6',0);
	displayCharacter(ghostPtr6);

	initializeCharacter(ghostPtr7,19,6,'7',0);
	displayCharacter(ghostPtr7);
	
	initializeCharacter(ghostPtr8,6,19,'8',0);
	displayCharacter(ghostPtr8);

	initializeCharacter(playerPtr,10,10,'*',1);
	displayCharacter(playerPtr);

	initializeCharacter(powerUpPtr,15,12,'P',1);
	displayCharacter(powerUpPtr);
	
	initializeCharacter(bombPtr1,16,16,'B',0);
	displayCharacter(bombPtr1);

	initializeCharacter(bombPtr2,8,8,'B',0);
	displayCharacter(bombPtr2);

	initializeCharacter(bombPtr3,8,16,'B',0);
	displayCharacter(bombPtr3);
	
	initializeCharacter(bombPtr4,16,8,'B',0);
	displayCharacter(bombPtr4);
}

int victoryCondition(Character *ghostPtr1, Character *ghostPtr2, Character *ghostPtr3, Character *ghostPtr4,
					 Character *ghostPtr5, Character *ghostPtr6, Character *ghostPtr7, Character *ghostPtr8)
{
	return(!(ghostPtr1->_alive) && !(ghostPtr2->_alive) && !(ghostPtr3->_alive) && !(ghostPtr4->_alive) &&
	       !(ghostPtr5->_alive) && !(ghostPtr6->_alive) && !(ghostPtr7->_alive) && !(ghostPtr8->_alive));
}

void printGameOver(int XSize, int YSize)
{
	printCenteredMessage(XSize, YSize, "G A M E   O V E R");
}

void printVictoryMessage(int XSize, int YSize)
{
	printCenteredMessage(XSize, YSize, "Y O U   W O N ! !");
}

void printDefeatMessage(int XSize, int YSize)
{
	printCenteredMessage(XSize, YSize, "Y O U   L O S T !");
}


unsigned int computeGhostSlowDown()
{
   if(ghostLevel<5)
   {
	   return 32000;
   }
   else if(ghostLevel<100)
   {
	   return 32000-ghostLevel*5;
   }
   else if(ghostLevel<400)
   {
	   return 32000-ghostLevel*10;
   }
   else if(ghostLevel<1000)
   {
	   return 32000-ghostLevel*20;
   }
   else 
   {
	   return 7000;
   }
}

int wallReached(int XSize, int YSize, Character *characterPtr)
{
	return (characterPtr->_x==0)||(characterPtr->_x==XSize-1) || 
		   (characterPtr->_y==0)||(characterPtr->_y==YSize-1);
}

void die(Character * playerPtr)
{
	gotoxy(playerPtr->_x,playerPtr->_y);
	cputc('X');
	playerPtr->_status = 0;
	playerPtr->_alive = 0;
	playerPtr->_ch = 'X';
}

void defeat(int XSize, int YSize)
{
	printDefeatMessage(XSize, YSize);
	sleep(2);
	printGameOver(XSize, YSize);
}

void win(Character * playerPtr)
{
	gotoxy(playerPtr->_x,playerPtr->_y);
	cputc('!');
}

void victory(int XSize, int YSize)
{
	printVictoryMessage(XSize, YSize);
	sleep(2);
	printGameOver(XSize, YSize);
}

void decreaseGhostLevel(int level)
{
	if(ghostLevel>level)
		ghostLevel-=level;
	else
		ghostLevel=0;
}

#if defined(CLOCKS_PER_SEC)
unsigned __fastcall__ microSleep (unsigned int wait)
{
    clock_t goal = clock () + ((clock_t) wait / 1000) * (int) (CLOCKS_PER_SEC);
    while ((long) (goal - clock ()) > 0) ;
    return 0;
}
#endif

void checkGhostsVsGhosts(Character *ghostPtr1, Character *ghostPtr2, Character *ghostPtr3, Character *ghostPtr4,
						 Character *ghostPtr5, Character *ghostPtr6, Character *ghostPtr7, Character *ghostPtr8)
{
	if(ghostPtr8->_alive && charactersMeet(ghostPtr1, ghostPtr2, ghostPtr3, ghostPtr4, ghostPtr5, ghostPtr6, ghostPtr7, ghostPtr8))
	{
		die(ghostPtr8);
		points+=GHOST_VS_GHOST_BONUS;
	}
	if(ghostPtr1->_alive && charactersMeet(ghostPtr2, ghostPtr3, ghostPtr4, ghostPtr5, ghostPtr6, ghostPtr7, ghostPtr8, ghostPtr1))
	{
		die(ghostPtr1);
		points+=GHOST_VS_GHOST_BONUS;
	}
	if(ghostPtr2->_alive && charactersMeet(ghostPtr3, ghostPtr4, ghostPtr5, ghostPtr6, ghostPtr7, ghostPtr8, ghostPtr1, ghostPtr2))
	{
		die(ghostPtr2);
		points+=GHOST_VS_GHOST_BONUS;
	}
	if(ghostPtr3->_alive && charactersMeet(ghostPtr4, ghostPtr5, ghostPtr6, ghostPtr7, ghostPtr8, ghostPtr1, ghostPtr2, ghostPtr3))
	{
		die(ghostPtr3);
		points+=GHOST_VS_GHOST_BONUS;
	}
	if(ghostPtr4->_alive && charactersMeet(ghostPtr5, ghostPtr6, ghostPtr7, ghostPtr8, ghostPtr1, ghostPtr2, ghostPtr3, ghostPtr4))
	{
		die(ghostPtr4);
		points+=GHOST_VS_GHOST_BONUS;
	}
	if(ghostPtr5->_alive && charactersMeet(ghostPtr6, ghostPtr7, ghostPtr8, ghostPtr1, ghostPtr2, ghostPtr3, ghostPtr4, ghostPtr5))
	{
		die(ghostPtr5);
		points+=GHOST_VS_GHOST_BONUS;
	}
	if(ghostPtr6->_alive && charactersMeet(ghostPtr7, ghostPtr8, ghostPtr1, ghostPtr2, ghostPtr3, ghostPtr4, ghostPtr5, ghostPtr6))
	{
		die(ghostPtr6);
		points+=GHOST_VS_GHOST_BONUS;
	}
	if(ghostPtr7->_alive && charactersMeet(ghostPtr8, ghostPtr1, ghostPtr2, ghostPtr3, ghostPtr4, ghostPtr5, ghostPtr6, ghostPtr7))
	{
		die(ghostPtr7);
		points+=GHOST_VS_GHOST_BONUS;
	}
}

int main (void)
{
    unsigned char XSize, YSize;
	
	char kbInput;
	
	int loop = 0;
	
	Character ghost_1; 	
	Character ghost_2; 
    Character ghost_3; 	
	Character ghost_4; 
	Character ghost_5; 	
	Character ghost_6; 
    Character ghost_7; 	
	Character ghost_8; 
	
	Character player; 
	
	Character powerUp;
	
	Character bomb_1;
	Character bomb_2;
	Character bomb_3;
	Character bomb_4;

	/* Clear the screen, put cursor in upper left corner */
    clrscr ();
	
	// Set Screen Colors
	setScreenColors();

    /* Ask for the screen size */
    screensize (&XSize, &YSize);

    /* Wait for the user to press a key */
	printPressKeyToStart(XSize, YSize);
	cgetc();
	deleteCenteredMessage(XSize, YSize);
	
	
    /* Draw a border around the screen */
	drawBorders(XSize, YSize);
	
	// Initialize characters
	initializeCharacters(&player, &powerUp, 
						 &ghost_1, &ghost_2, &ghost_3, &ghost_4, 
						 &ghost_5, &ghost_6, &ghost_7, &ghost_8, 
						 &bomb_1, &bomb_2, &bomb_3, &bomb_4);	
	while(1)
	{
		ghostSlowDown = computeGhostSlowDown();
		microSleep(250);
		
		++loop;
		toggleHunters(&ghost_1, &ghost_2, &ghost_3, &ghost_4, 
		              &ghost_5, &ghost_6, &ghost_7, &ghost_8, loop);
		
		if(kbhit())
		{		
			kbInput = cgetc();
			movePlayer(&player, kbInput);
		}

		chasePlayer(&ghost_1, &ghost_2, &ghost_3, &ghost_4, 
					&ghost_5, &ghost_6, &ghost_7, &ghost_8, &player, 
					&bomb_1, &bomb_2, &bomb_3, &bomb_4);
		
		if(playerReached(&ghost_1, &ghost_2, &ghost_3, &ghost_4, 
						 &ghost_5, &ghost_6, &ghost_7, &ghost_8, &player) ||
		   playerReachedBombs(&bomb_1, &bomb_2, &bomb_3, &bomb_4, &player))
		{
			die(&player);
			defeat(XSize, YSize);
			return EXIT_SUCCESS;
		}
	
	    checkBombsVsGhosts(&bomb_1, &bomb_2, &bomb_3, &bomb_4, 
						   &ghost_1, &ghost_2, &ghost_3, &ghost_4, 
						   &ghost_5, &ghost_6, &ghost_7, &ghost_8);
		
		checkGhostsVsGhosts(&ghost_1, &ghost_2, &ghost_3, &ghost_4, 
							&ghost_5, &ghost_6, &ghost_7, &ghost_8);

	    if(powerUp._status == 1)
		{
			if(powerUpReached(&player, &powerUp))
			{
				decreaseGhostLevel(200);	
				points+=POWER_UP_BONUS;
				powerUp._status = 0;	
				powerUpCoolDown = 20;
			}
			else
			{
				displayCharacter(&powerUp);
			}		
		}
		else if (powerUpCoolDown == 0)
		{
			powerUp._status = 1;
			powerUp._x = powerUp._x-3 * rand() % 7; 
			powerUp._y = powerUp._y-3 * rand() % 7; 
			displayCharacter(&powerUp);
		}
		else
		{
			--powerUpCoolDown;
		}
			
		if(wallReached(XSize, YSize, &player))
		{
			die(&player);
			defeat(XSize, YSize);
			return EXIT_SUCCESS;
		}
		
		displayScore(points);
		displayGhostLevel();
		
		if(victoryCondition(&ghost_1, &ghost_2, &ghost_3, &ghost_4, &ghost_5, &ghost_6, &ghost_7, &ghost_8))
		{
			win(&player);
			victory(XSize, YSize);
			return EXIT_SUCCESS;
		}
		
		// Add points to score
		points+=LOOP_POINTS;
		
		// Increase ghost speed
		++ghostLevel;
	};

		
	/* Clear the screen again */
	clrscr ();

	/* Done */
	return EXIT_SUCCESS;
}

