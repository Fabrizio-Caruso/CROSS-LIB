#include "character.h"
#include "settings.h"
#include <joystick.h>
#include "input.h"

extern unsigned short invincibleXCountDown;
extern unsigned short invincibleYCountDown;
extern unsigned short playerDirection;
extern unsigned short playerFire;
extern unsigned short level;

int computeInvincibleCountDown(void)
{
	return 125 - level*5;
}


void movePlayer(Character *playerPtr, char kbInput)
{
	if((kbInput=='W') || (kbInput=='w'))
	{
		deleteCharacter(playerPtr);
		--playerPtr->_y;
		invincibleYCountDown = computeInvincibleCountDown();
		playerDirection = UP;
	}
	else if((kbInput=='S') || (kbInput=='s'))
	{
		deleteCharacter(playerPtr);
		++playerPtr->_y;
		invincibleYCountDown = computeInvincibleCountDown();
		playerDirection = DOWN;
	}
	else if((kbInput=='A') || (kbInput=='a'))
	{
		deleteCharacter(playerPtr);
		--playerPtr->_x;
		invincibleXCountDown = computeInvincibleCountDown();
		playerDirection = LEFT;
	}
	else if((kbInput=='D') || (kbInput=='d'))
	{
		deleteCharacter(playerPtr);
		++playerPtr->_x;
		invincibleXCountDown = computeInvincibleCountDown();
		playerDirection = RIGHT;
	}
	else if(kbInput==' ')
	{
		playerFire = 1;
	}
	/*
	else if((kbInput=='L') || (kbInput=='l'))
	{
		ghostCount = 0;
		playerPtr->_ch = 'Z';
		displayCharacter(playerPtr);
		sleep(1);
	}
	*/
	displayCharacter(playerPtr);
}


void movePlayerByJoystick(Character *playerPtr, unsigned char joyInput)
{
	if(JOY_BTN_UP(joyInput))
	{
		deleteCharacter(playerPtr);
		--playerPtr->_y;
		invincibleYCountDown = computeInvincibleCountDown();
		playerDirection = UP;
	}
	else if(JOY_BTN_DOWN(joyInput))
	{
		deleteCharacter(playerPtr);
		++playerPtr->_y;
		invincibleYCountDown = computeInvincibleCountDown();
		playerDirection = DOWN;
	}
	else if(JOY_BTN_LEFT(joyInput))
	{
		deleteCharacter(playerPtr);
		--playerPtr->_x;
		invincibleXCountDown = computeInvincibleCountDown();
		playerDirection = LEFT;
	}
	else if(JOY_BTN_RIGHT(joyInput))
	{
		deleteCharacter(playerPtr);
		++playerPtr->_x;
		invincibleXCountDown = computeInvincibleCountDown();
		playerDirection = RIGHT;
	}
	else if(JOY_BTN_FIRE(joyInput))
	{
		playerFire = 1;
	}
	displayCharacter(playerPtr);
}



