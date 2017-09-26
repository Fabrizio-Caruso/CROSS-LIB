#include <stdio.h>
#include <string.h>
#include <sound/bit.h>

#define PING_SOUND() bit_fx(bitfx_19)
#define CLEAR_SCREEN() {printf("\xc");};
#define EXPLOSION_SOUND() bit_fx(bitfx_16);

#define INVINCIBLE_GHOST_POINTS 2000

unsigned char lives;
 unsigned char innerVerticalWallX;
 unsigned char innerVerticalWallY;
 unsigned char innerVerticalWallLength;
 unsigned char level;
 unsigned char XSize;
 unsigned char YSize;

 unsigned long points = 10000UL;

void sleep(unsigned int sec) 
{ 
	unsigned long i; 
	for(i=0;i<sec*400UL;++i){}; 
} 

#define X_OFFSET 0
#define Y_OFFSET 3

#define XSize 32
#define YSize 24

#define COLOR_WHITE 2
#define TEXT_COLOR COLOR_WHITE
#define SET_TEXT_COLOR(c) {printf("\020%c",c);};

#define gotoxy(x,y) printf("\x16%c%c",x+1,y+1); 
#define PRINTF(x,y,str,val) {gotoxy(x+X_OFFSET,y+Y_OFFSET); printf(str,val); };

void printCenteredMessage(char *Text)
{
	SET_TEXT_COLOR(TEXT_COLOR);
	PRINTF((XSize - strlen (Text)) / 2, YSize / 2,"%s", Text);
}

void printExtraLife(void)
{
	printCenteredMessage("E X T R A   L I F E ! ! !"); 
	sleep(1);
}

void updateInnerWallVerticalData(void)
{		
	switch(level)
	{
		case 1:
			innerVerticalWallLength = 0;
		break;
		case 2:
			innerVerticalWallLength = YSize-12;
		break;
		case 3:
			innerVerticalWallLength = YSize-8;
		break;
		case 4:
			innerVerticalWallLength = YSize-6;
		break;
		case 5:
			innerVerticalWallLength = 0;		
		break;
		case 6:
			innerVerticalWallLength = 0;
		break;
		case 7:
			innerVerticalWallLength = YSize-12;
		break;
		case 8:
			innerVerticalWallLength = YSize-8;
		break;
		case 9:
			innerVerticalWallLength = YSize-6;
		break;
		case 10:
			innerVerticalWallLength = 0;
		break;
		case 11:
			innerVerticalWallLength = 0;
		break;
		case 12:
			innerVerticalWallLength = YSize-12;
		break;
		case 13:
			innerVerticalWallLength = YSize-8;
		break;
		case 14:
			innerVerticalWallLength = YSize-6;
		break;
		case 15:
			innerVerticalWallLength = 0;
		break;
		case 16:
			innerVerticalWallLength = 0;
		break;
		case 17:
			innerVerticalWallLength = YSize-12;
		break;
		case 18:
			innerVerticalWallLength = YSize-8;
		break;
		case 19:
			innerVerticalWallLength = YSize-6;
		break;
		case 20:
			innerVerticalWallLength = 0;
		break;
		case 21:
			innerVerticalWallLength = YSize-14;
		break;
		case 22:
			innerVerticalWallLength = YSize-12;
		break;
		case 23:
			innerVerticalWallLength = YSize-8;
		break;
		case 24:
			innerVerticalWallLength = YSize-6;
		break;
		case 25:
			innerVerticalWallLength = 0;		
		break;
		case 26:
			innerVerticalWallLength = YSize-14;
		break;
		case 27:
			innerVerticalWallLength = YSize-12;
		break;
		case 28:
			innerVerticalWallLength = YSize-8;
		break;
		case 29:
			innerVerticalWallLength = YSize-6;
		break;
		case 30:
			innerVerticalWallLength = YSize-14;
		break;
		case 31:
			innerVerticalWallLength = YSize-14;
		break;
		case 32:
			innerVerticalWallLength = YSize-12;
		break;
		case 33:
			innerVerticalWallLength = YSize-8;
		break;
		case 34:
			innerVerticalWallLength = YSize-6;
		break;
		case 35:
			innerVerticalWallLength = YSize-14;
		break;
		case 36:
			innerVerticalWallLength = YSize-14;
		break;
		case 37:
			innerVerticalWallLength = YSize-12;
		break;
		case 38:
			innerVerticalWallLength = YSize-8;
		break;
		case 39:
			innerVerticalWallLength = YSize-6;
		break;
		case 40:
			innerVerticalWallLength = YSize-12;		
	}
	innerVerticalWallX = XSize / 2;
    innerVerticalWallY = YSize/2-(innerVerticalWallLength/2);
}

void displayStats(void)
{	
	SET_TEXT_COLOR(TEXT_COLOR);

	PRINTF(8,1-Y_OFFSET,"%06lu",points);	

}


void main(void)
{
	unsigned char i;
	//invincibleGhost._status = 0;
	//invincibleGhostAlive = 0;
	//DELETE_INVINCIBLE_GHOST(invincibleGhost._x,invincibleGhost._y, invincibleGhost._imagePtr);
	//invincibleGhost._x=XSize-2; invincibleGhost._y=YSize-2;
	for(i=0;i<4;++i)
	{
		EXPLOSION_SOUND();
	}
	points+=INVINCIBLE_GHOST_POINTS;
	displayStats();	
	
	
	sleep(1);
	CLEAR_SCREEN();				

	//ghostCount = 7;	
	//
	
	CLEAR_SCREEN();
	sleep(1);
	PING_SOUND();
	printExtraLife();
	sleep(1);
   ++lives;
   //

	//++level;
	
	updateInnerWallVerticalData();
   
}