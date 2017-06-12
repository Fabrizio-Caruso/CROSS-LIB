/*****************************************************************************/
/*                                                                           */
/*                                		                                     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* (C) 2017      Fabrizio Caruso                                  		     */
/*                					                                         */
/*              				                                             */
/* EMail:        Fabrizio_Caruso@hotmail.com                                 */
/*                                                                           */
/*                                                                           */
/* This software is provided 'as-is', without any expressed or implied       */
/* warranty.  In no event will the authors be held liable for any damages    */
/* arising from the use of this software.                                    */
/*                                                                           */
/* Permission is granted to anyone to use this software for any purpose,     */
/* including commercial applications, and to alter it and redistribute it    */
/* freely, subject to the following restrictions:                            */
/*                                                                           */
/* 1. The origin of this software must not be misrepresented; you must not   */
/*    claim that you wrote the original software. If you use this software   */
/*    in a product, an acknowledgment in the product documentation would be  */
/*    appreciated but is not required.                                       */
/* 2. Altered source versions must be plainly marked as such, and must not   */
/*    be misrepresented as being the original software.                      */
/* 3. This notice may not be removed or altered from any source              */
/*    distribution.                                                          */
/*                                                                           */
/*****************************************************************************/
 
#include "settings.h"
#include "character.h"
#include "strategy.h"


extern unsigned short level;
extern unsigned short ghostCount;
extern unsigned int invincibleSlowDown;

void blindChaseCharacterXStrategy(Character* hunterPtr, Character* preyPtr)
{
	if(hunterPtr->_x<preyPtr->_x)
	{
		DELETE_CHARACTER(hunterPtr);
		++hunterPtr->_x;
	}
	else if(hunterPtr->_x>preyPtr->_x)
	{
		DELETE_CHARACTER(hunterPtr);
		--hunterPtr->_x;
	}
	else if(hunterPtr->_y<preyPtr->_y)
	{
		DELETE_CHARACTER(hunterPtr);
		++hunterPtr->_y;
	}
	else
	{
		DELETE_CHARACTER(hunterPtr);
		--hunterPtr->_y;
	}
}


void blindChaseCharacterYStrategy(Character* hunterPtr, Character* preyPtr)
{
    if(hunterPtr->_y<preyPtr->_y)
	{
		DELETE_CHARACTER(hunterPtr);
		++hunterPtr->_y;
	}
	else if(hunterPtr->_y>preyPtr->_y)
	{
		DELETE_CHARACTER(hunterPtr);
		--hunterPtr->_y;
	}
	else if(hunterPtr->_x<preyPtr->_x)
	{
		DELETE_CHARACTER(hunterPtr);
		++hunterPtr->_x;
	}
	else 
	{
		DELETE_CHARACTER(hunterPtr);
		--hunterPtr->_x;
	}
}

void chaseCharacter(Character *hunterPtr, Character *preyPtr)
{
	if(hunterPtr->_status)
	{
		if(rand()%2) // Select blind chase strategy
			{
				blindChaseCharacterXStrategy(hunterPtr, preyPtr);
			}
			else
			{
				blindChaseCharacterYStrategy(hunterPtr, preyPtr);
			}
	}
}

// This is a very experimental optimized version (fewer rand() computations)
void chasePlayer(Character ** ghosts, 
                 Character* preyPtr, 
                 int slowDown)
{
	char i;

	// Experimental
	#if ALTERNATE_STRATEGY==1
		int r = rand();
		//if(r>slowDown)
		if(!(r%11)) // BOGUS
		{ 
			int mid = GHOSTS_NUMBER/2;
			if(r%2)
			{	
				for(i=0;i<mid;++i)
				{
					blindChaseCharacterXStrategy(ghosts[i], preyPtr);
				}
			}
			else
			{	
				for(i=mid;i<GHOSTS_NUMBER;++i)
				{
					blindChaseCharacterYStrategy(ghosts[i], preyPtr);
				}
			}
		}
	// Experimental
	#elif ALTERNATE_STRATEGY==2
		int r = rand();
		//if(r>slowDown)
		if(r%11) // BOGUS
		{ 
			if(r%4==0)
			{	
				for(i=0;i<GHOSTS_NUMBER/4;++i)
				{
					chaseCharacter(ghosts[i], preyPtr);
				}
			}
			else if(r%4==1)
			{	
				for(i=GHOSTS_NUMBER/4;i<GHOSTS_NUMBER;++i)
				{
					chaseCharacter(ghosts[i], preyPtr);
				}
			}
			else if(r%4==2)
			{	
				for(i=GHOSTS_NUMBER/2;i<GHOSTS_NUMBER/2+GHOSTS_NUMBER/4;++i)
				{
					chaseCharacter(ghosts[i], preyPtr);
				}
			}
			else
			{	
				for(i=GHOSTS_NUMBER/2+GHOSTS_NUMBER/4;i<GHOSTS_NUMBER;++i)
				{
					chaseCharacter(ghosts[i], preyPtr);
				}
			}
		}
	// Standard
	#else
		for(i=0;i<GHOSTS_NUMBER;++i)
		{
			if(rand()>slowDown)
			{
				chaseCharacter(ghosts[i], preyPtr);
			}
		}
	#endif
}
