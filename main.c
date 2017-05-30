/*
** Portable Chase 
** 
** by
** Fabrizio Caruso (fabrizio_caruso@hotmail.com)
**
*/

#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <joystick.h>

#include <unistd.h>


/*****************************************************************************/
/*                                   Data                                    */
/*****************************************************************************/


/*****************************************************************************/
/*                                   Code                                    */
/*****************************************************************************/

int ghostSlowDown = 500; 
int points = 0;

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

void chaseCharacter(Character* hunterPtr, Character* preyPtr)
{
	if(rand()>16000) // Select chase strategy
	{
		chaseCharacterXStrategy(hunterPtr, preyPtr);
	}
	else
	{
		chaseCharacterYStrategy(hunterPtr, preyPtr);
	}
}

void chaseCharacterIf(Character* hunterPtr, Character* preyPtr)
{
	if(((hunterPtr->_status==1) && (hunterPtr->_alive==1)) && rand()>ghostSlowDown)
	{
		chaseCharacter(hunterPtr, preyPtr);
	}
	else
	{
		displayCharacter(hunterPtr);
	}
}					 

void chasePlayer(Character * hunterPtr1, Character * hunterPtr2, 
                 Character * hunterPtr3, Character * hunterPtr4, 
                 Character* preyPtr)
{
	chaseCharacterIf(hunterPtr1, preyPtr);
	chaseCharacterIf(hunterPtr2, preyPtr);
	chaseCharacterIf(hunterPtr3, preyPtr);
	chaseCharacterIf(hunterPtr4, preyPtr);
}

void displayScore(int points)
{
	gotoxy(9,2);
	cputs("       ");
	gotoxy(9,2);
	cprintf("%d",points);
}

int playerReached(Character * hunterPtr1, Character * hunterPtr2, Character * hunterPtr3, Character * hunterPtr4, 
                 Character* preyPtr)
{
	return(areCharctersAtSamePosition(hunterPtr1,preyPtr) || areCharctersAtSamePosition(hunterPtr2,preyPtr) ||
		   areCharctersAtSamePosition(hunterPtr3,preyPtr) || areCharctersAtSamePosition(hunterPtr4,preyPtr));
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

void pressKeyToStart()
{
	gotoxy(4,8);
	cputs("PRESS ANY KEY TO START");
    cgetc ();
	gotoxy(4,8);
	cputs("                      ");
	gotoxy(2,2);
	cputs("SCORE: ");
}

void toggleHunters(Character * hunterPtr1, Character * hunterPtr2, 
                   Character * hunterPtr3, Character * hunterPtr4, 
				   int loop)
{
	if(loop==2)
		hunterPtr1->_status = 1;
	if(loop==10)
		hunterPtr2->_status = 1;
	if(loop==50)
		hunterPtr3->_status = 1;
	if(loop==200)
		hunterPtr4->_status = 1;
}

void checkHuntersAndPrey(Character * hunterPtr1, Character * hunterPtr2, 
					     Character * hunterPtr3, Character * hunterPtr4,
						 Character * preyPtr)
{
	if(preyPtr->_alive && playerReached(hunterPtr1, hunterPtr2, hunterPtr3, hunterPtr4, preyPtr))
	{
		gotoxy(preyPtr->_x,preyPtr->_y);
		cputc('X');
		sleep(2);
		preyPtr->_alive = 0;
		preyPtr->_status = 0;
		points+=5000;
	}
}
						

void checkHuntersAndPreys(Character * hunterPtr1, Character * hunterPtr2, 
					      Character * hunterPtr3, Character * hunterPtr4,
						  Character * preyPtr1, Character * preyPtr2, 
					      Character * preyPtr3, Character * preyPtr4)
{
	checkHuntersAndPrey(hunterPtr1, hunterPtr2, 
					    hunterPtr3, hunterPtr4,
					    preyPtr1);
	checkHuntersAndPrey(hunterPtr1, hunterPtr2, 
					    hunterPtr3, hunterPtr4,
					    preyPtr2);
	checkHuntersAndPrey(hunterPtr1, hunterPtr2, 
					    hunterPtr3, hunterPtr4,
					    preyPtr3);
	checkHuntersAndPrey(hunterPtr1, hunterPtr2, 
					    hunterPtr3, hunterPtr4,
					    preyPtr4);
}

void initializeCharacters(Character * playerPtr, Character * powerUpPtr, 
						  Character * ghostPtr1, Character * ghostPtr2,
						  Character * ghostPtr3, Character * ghostPtr4,
						  Character * bombPtr1, Character * bombPtr2,
						  Character * bombPtr3, Character * bombPtr4,
						  )
{
	initializeCharacter(ghostPtr1,3,3,'1',0);
	displayCharacter(ghostPtr1);

	initializeCharacter(ghostPtr2,17,17,'2',0);
	displayCharacter(ghostPtr2);

	initializeCharacter(ghostPtr3,17,3,'3',0);
	displayCharacter(ghostPtr3);
	
	initializeCharacter(ghostPtr4,3,17,'4',0);
	displayCharacter(ghostPtr4);

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

int victory(Character *ghostPtr1, Character *ghostPtr2, Character *ghostPtr3, Character *ghostPtr4)
{
	return(!(ghostPtr1->_alive) && !(ghostPtr2->_alive) && !(ghostPtr3->_alive) && !(ghostPtr4->_alive));
}

void printGameOver()
{
	gotoxy(10,10);
	cputs("G A M E   O V E R");
}

void printVictoryMessage()
{
	gotoxy(10,10);
	cputs("Y O U   W O N ! !");
}

int main (void)
{
    unsigned char XSize, YSize;
	
	int i = 0;
	
	char kbInput;
	
	int loop = 0;
	
	Character ghost_1; 	
	Character ghost_2; 
    Character ghost_3; 	
	Character ghost_4; 
	
	Character player; 
	
	Character powerUp;
	
	Character bomb_1;
	Character bomb_2;
	Character bomb_3;
	Character bomb_4;

	// Set Screen Colors
	setScreenColors();

    /* Clear the screen, put cursor in upper left corner */
    clrscr ();

    /* Ask for the screen size */
    screensize (&XSize, &YSize);

    /* Draw a border around the screen */
	drawBorders(XSize, YSize);

    /* Wait for the user to press a key */
	pressKeyToStart();

	// Initialize characters
	initializeCharacters(&player, &powerUp, 
						 &ghost_1, &ghost_2, &ghost_3, &ghost_4, 
						 &bomb_1, &bomb_2, &bomb_3, &bomb_4);
	
	while(1)
	{
		sleep(1);
		
		++loop;
		toggleHunters(&ghost_1, &ghost_2, &ghost_3, &ghost_4, loop);
		
		if(kbhit())
		{		
			kbInput = cgetc();

			movePlayer(&player, kbInput);
		}

		chasePlayer(&ghost_1, &ghost_2, &ghost_3, &ghost_4, &player);
		
		if(playerReached(&ghost_1, &ghost_2, &ghost_3, &ghost_4, &player))
		{
			gotoxy(player._x,player._y);
			cputc('X');
			sleep(2);
			return EXIT_SUCCESS;
		}
	
		if(playerReached(&bomb_1, &bomb_2, &bomb_3, &bomb_4, &player))
		{
			gotoxy(player._x,player._y);
			cputc('X');
			sleep(2);
			printGameOver();
			return EXIT_SUCCESS;
		}
	
	    checkHuntersAndPreys(&bomb_1, &bomb_2, &bomb_3, &bomb_4, &ghost_1, &ghost_2, &ghost_3, &ghost_4);

	    if(powerUp._status == 1)
		{
			if(powerUpReached(&player, &powerUp))
			{
				ghostSlowDown+=10000;	
				points+=1000;
				powerUp._status = 0;	
			}
			else
			{
				displayCharacter(&powerUp);
			}		
		}
		
		displayScore(points);
		
		if(victory(&ghost_1, &ghost_2, &ghost_3, &ghost_4))
		{
			gotoxy(player._x,player._y);
			cputc('!');
			printVictoryMessage();
			sleep(2);
			printGameOver();
			sleep(2);
			return EXIT_SUCCESS;
		}
		
		// Add points to score
		points+=10;
		
		// Increase ghost speed
		ghostSlowDown-= 100;
	};

		
	/* Clear the screen again */
	clrscr ();

	/* Done */
	return EXIT_SUCCESS;
}

