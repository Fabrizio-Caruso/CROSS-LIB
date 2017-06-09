#include "character.h"
#include "settings.h"
#include <joystick.h>
#include "display_macros.h"
#include "invincible_enemy.h"

#include "input.h"

extern unsigned short invincibleXCountDown;
extern unsigned short invincibleYCountDown;
extern unsigned short playerDirection;
extern unsigned short playerFire;
extern unsigned short level;

void movePlayerByKeyboard(Character *playerPtr, char kbInput)
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
	else 	
	if(kbInput==' ')
	{
		playerFire = 1;
	}

	#ifdef _TRAINER
	else if((kbInput=='Z') || (kbInput=='z'))
	{
		ghostCount = 0;
		playerPtr->_ch = 'Z';
		SET_TEXT_COLOR(PLAYER_COLOR);
		displayCharacter(playerPtr);
		SET_TEXT_COLOR(TEXT_COLOR);
		sleep(1);
	}
	#endif // TRAINER
	SET_TEXT_COLOR(PLAYER_COLOR);
	displayCharacter(playerPtr);
	SET_TEXT_COLOR(TEXT_COLOR);
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
	SET_TEXT_COLOR(PLAYER_COLOR);
	displayCharacter(playerPtr);
	SET_TEXT_COLOR(TEXT_COLOR);
}



