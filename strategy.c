
#include "settings.h"
#include "character.h"
#include "strategy.h"


extern unsigned short level;
extern unsigned short ghostCount;
extern unsigned int invincibleSlowDown;

void chaseCharacterXAvoidBombStrategy(Character* hunterPtr, Character* preyPtr, 
                    Character* bombPtr1, Character* bombPtr2,
					Character* bombPtr3, Character* bombPtr4,
					Character* ghostPtr1, Character *ghostPtr2, Character* ghostPtr3, Character *ghostPtr4,
					Character* ghostPtr5, Character *ghostPtr6, Character* ghostPtr7)
{
	if(!rightBombs(hunterPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4) && 
	   !rightGhosts(hunterPtr, ghostPtr1, ghostPtr2, ghostPtr3, ghostPtr4, ghostPtr5, ghostPtr6, ghostPtr7) &&
	   hunterPtr->_x<preyPtr->_x)
	{
		deleteCharacter(hunterPtr);
		++hunterPtr->_x;
	}
	else if(!leftBombs(hunterPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4) && 
	        !leftGhosts(hunterPtr, ghostPtr1, ghostPtr2, ghostPtr3, ghostPtr4, ghostPtr5, ghostPtr6, ghostPtr7) &&
	         hunterPtr->_x>preyPtr->_x)
	{
		deleteCharacter(hunterPtr);
		--hunterPtr->_x;
	}
	else if(!downBombs(hunterPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4) && 
			!downGhosts(hunterPtr, ghostPtr1, ghostPtr2, ghostPtr3, ghostPtr4, ghostPtr5, ghostPtr6, ghostPtr7) &&
	        hunterPtr->_y<preyPtr->_y)
	{
		deleteCharacter(hunterPtr);
		++hunterPtr->_y;
	}
	else if(!upBombs(hunterPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4) && 
			!upGhosts(hunterPtr, ghostPtr1, ghostPtr2, ghostPtr3, ghostPtr4, ghostPtr5, ghostPtr6, ghostPtr7) &&
	        hunterPtr->_y>preyPtr->_y)
	{
		deleteCharacter(hunterPtr);
		--hunterPtr->_y;
	}
	// displayCharacter(hunterPtr);
}



void chaseCharacterYAvoidBombStrategy(Character* hunterPtr, Character* preyPtr, 
                    Character* bombPtr1, Character* bombPtr2,
					Character* bombPtr3, Character* bombPtr4,
					Character* ghostPtr1, Character *ghostPtr2, Character* ghostPtr3, Character *ghostPtr4,
					Character* ghostPtr5, Character *ghostPtr6, Character* ghostPtr7)
{
    if(!downBombs(hunterPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4) && 
	   !downGhosts(hunterPtr, ghostPtr1, ghostPtr2, ghostPtr3, ghostPtr4, ghostPtr5, ghostPtr6, ghostPtr7) &&
		hunterPtr->_y<preyPtr->_y)
	{
		deleteCharacter(hunterPtr);
		++hunterPtr->_y;
	}
	else if(!upBombs(hunterPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4) && 
	        !upGhosts(hunterPtr, ghostPtr1, ghostPtr2, ghostPtr3, ghostPtr4, ghostPtr5, ghostPtr6, ghostPtr7) &&
	        hunterPtr->_y>preyPtr->_y)
	{
		deleteCharacter(hunterPtr);
		--hunterPtr->_y;
	}
	else if(!rightBombs(hunterPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4) && 
			!rightGhosts(hunterPtr, ghostPtr1, ghostPtr2, ghostPtr3, ghostPtr4, ghostPtr5, ghostPtr6, ghostPtr7) &&
			hunterPtr->_x<preyPtr->_x)
	{
		deleteCharacter(hunterPtr);
		++hunterPtr->_x;
	}
	else if(!leftBombs(hunterPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4) && 
			!leftGhosts(hunterPtr, ghostPtr1, ghostPtr2, ghostPtr3, ghostPtr4, ghostPtr5, ghostPtr6, ghostPtr7) &&
			hunterPtr->_x>preyPtr->_x) 
	{
		deleteCharacter(hunterPtr);
		--hunterPtr->_x;
	}

	// displayCharacter(hunterPtr);
}

void blindChaseCharacterXStrategy(Character* hunterPtr, Character* preyPtr)
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

void blindChaseCharacterYStrategy(Character* hunterPtr, Character* preyPtr)
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



void chaseCharacterXYStrategy(Character* hunterPtr, Character* preyPtr, 
                    Character* bombPtr1, Character* bombPtr2,
					Character* bombPtr3, Character* bombPtr4,
					Character* ghostPtr1, Character *ghostPtr2, Character* ghostPtr3, Character *ghostPtr4,
					Character* ghostPtr5, Character *ghostPtr6, Character* ghostPtr7, 
					int ghostSmartness)
{
	if(rand()%10 > ghostSmartness)
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
	else
	{
		if(rand()%2) // Select chase strategy that avoids collisions
		{
			chaseCharacterXAvoidBombStrategy(hunterPtr, preyPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4,
											 ghostPtr1, ghostPtr2, ghostPtr3, ghostPtr4, ghostPtr5, ghostPtr6, ghostPtr7);
		}
		else
		{
			chaseCharacterYAvoidBombStrategy(hunterPtr, preyPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4,
											 ghostPtr1, ghostPtr2, ghostPtr3, ghostPtr4, ghostPtr5, ghostPtr6, ghostPtr7);
		}
	}
}

void chaseCharacterMaxAndXStrategy(Character* hunterPtr, Character* preyPtr, 
                    Character* bombPtr1, Character* bombPtr2,
					Character* bombPtr3, Character* bombPtr4,
					Character* ghostPtr1, Character *ghostPtr2, Character* ghostPtr3, Character *ghostPtr4,
					Character* ghostPtr5, Character *ghostPtr6, Character* ghostPtr7, 
					int ghostSmartness)
{
	short deltaX = abs(hunterPtr->_x - preyPtr->_x);
	short deltaY = abs(hunterPtr->_y - preyPtr->_y);
	
	if(rand()%10 > ghostSmartness)
	{
		if(deltaX>=deltaY)
		{
			blindChaseCharacterXStrategy(hunterPtr, preyPtr);
		}
		else
		{
			blindChaseCharacterYStrategy(hunterPtr, preyPtr);
		}
	}
	else
	{
		if(deltaX>=deltaY)
		{
			chaseCharacterXAvoidBombStrategy(hunterPtr, preyPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4,
			ghostPtr1, ghostPtr2, ghostPtr3, ghostPtr4, ghostPtr5, ghostPtr6, ghostPtr7);
		}
		else
		{
			chaseCharacterYAvoidBombStrategy(hunterPtr, preyPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4,
			ghostPtr1, ghostPtr2, ghostPtr3, ghostPtr4, ghostPtr5, ghostPtr6, ghostPtr7);
		}
	}
}


void chaseCharacterMaxAndYStrategy(Character* hunterPtr, Character* preyPtr, 
                    Character* bombPtr1, Character* bombPtr2,
					Character* bombPtr3, Character* bombPtr4,
					Character* ghostPtr1, Character *ghostPtr2, Character* ghostPtr3, Character *ghostPtr4,
					Character* ghostPtr5, Character *ghostPtr6, Character* ghostPtr7,
					int ghostSmartness)
{
	short deltaX = abs(hunterPtr->_x - preyPtr->_x);
	short deltaY = abs(hunterPtr->_y - preyPtr->_y);
	
	if(rand()%10 > ghostSmartness)
	{
		if(deltaY>=deltaX)
		{
			blindChaseCharacterYStrategy(hunterPtr, preyPtr);
		}
		else
		{
			blindChaseCharacterXStrategy(hunterPtr, preyPtr);
		}
	}
	else
	{
		if(deltaY>=deltaY)
		{
			chaseCharacterYAvoidBombStrategy(hunterPtr, preyPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4,
											 ghostPtr1, ghostPtr2, ghostPtr3, ghostPtr4, ghostPtr5, ghostPtr6, ghostPtr7);
		}
		else
		{
			chaseCharacterXAvoidBombStrategy(hunterPtr, preyPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4,
											 ghostPtr1, ghostPtr2, ghostPtr3, ghostPtr4, ghostPtr5, ghostPtr6, ghostPtr7);
		}
	}
}



void chaseCharacterYStrategy(Character* hunterPtr, Character* preyPtr, 
                    Character* bombPtr1, Character* bombPtr2,
					Character* bombPtr3, Character* bombPtr4,
					Character* ghostPtr1, Character *ghostPtr2, Character* ghostPtr3, Character *ghostPtr4,
					Character* ghostPtr5, Character *ghostPtr6, Character* ghostPtr7,
					int ghostSmartness)
{
	if(rand()%10 > ghostSmartness)
	{
			blindChaseCharacterYStrategy(hunterPtr, preyPtr);
	}
	else
	{
			chaseCharacterYAvoidBombStrategy(hunterPtr, preyPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4,
											 ghostPtr1, ghostPtr2, ghostPtr3, ghostPtr4, ghostPtr5, ghostPtr6, ghostPtr7);
	}
}

void chaseCharacterXStrategy(Character* hunterPtr, Character* preyPtr, 
                    Character* bombPtr1, Character* bombPtr2,
					Character* bombPtr3, Character* bombPtr4,
					Character* ghostPtr1, Character *ghostPtr2, Character* ghostPtr3, Character *ghostPtr4,
					Character* ghostPtr5, Character *ghostPtr6, Character* ghostPtr7,
					int ghostSmartness)
{
	if(rand()%10 > ghostSmartness)
	{
			blindChaseCharacterXStrategy(hunterPtr, preyPtr);
	}
	else
	{
			chaseCharacterXAvoidBombStrategy(hunterPtr, preyPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4,
											 ghostPtr1, ghostPtr2, ghostPtr3, ghostPtr4, ghostPtr5, ghostPtr6, ghostPtr7);	
	}
}




void chaseCharacterXStrategyIf(Character* ghostPtr1, Character* preyPtr, 
                    Character* bombPtr1, Character* bombPtr2,
					Character* bombPtr3, Character* bombPtr4,
					Character *ghostPtr2, Character* ghostPtr3, Character *ghostPtr4,
					Character* ghostPtr5, Character *ghostPtr6, Character* ghostPtr7, Character* ghostPtr8,
					int ghostSmartness, int ghostSlowDown)
{
	// TODO: to fix
	
	if((ghostPtr1->_status==1) && (ghostPtr1->_alive==1))
	{
		if(rand()>ghostSlowDown)
		{
			chaseCharacterXStrategy(ghostPtr1, preyPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4, 
									ghostPtr2, ghostPtr3, ghostPtr4, ghostPtr5, ghostPtr6, ghostPtr7, ghostPtr8,
									ghostSmartness);
		}
		displayCharacter(ghostPtr1);
	}
	
	// displayCharacter(ghostPtr1);
}					 

void chaseCharacterYStrategyIf(Character* ghostPtr1, Character* preyPtr, 
                    Character* bombPtr1, Character* bombPtr2,
					Character* bombPtr3, Character* bombPtr4,
					Character *ghostPtr2, Character* ghostPtr3, Character *ghostPtr4,
					Character* ghostPtr5, Character *ghostPtr6, Character* ghostPtr7, Character* ghostPtr8,
					int ghostSmartness, int ghostSlowDown)
{
	// TODO: to fix
	
	if((ghostPtr1->_status==1) && (ghostPtr1->_alive==1))
	{
		if(rand()>ghostSlowDown)
		{
			chaseCharacterYStrategy(ghostPtr1, preyPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4, 
									ghostPtr2, ghostPtr3, ghostPtr4, ghostPtr5, ghostPtr6, ghostPtr7, ghostPtr8, 
									ghostSmartness);
		}
		displayCharacter(ghostPtr1);
	}
	
	// displayCharacter(ghostPtr1);
}

void chaseCharacterIf(Character* ghostPtr1, Character* preyPtr, 
                    Character* bombPtr1, Character* bombPtr2,
					Character* bombPtr3, Character* bombPtr4,
					Character *ghostPtr2, Character* ghostPtr3, Character *ghostPtr4,
					Character* ghostPtr5, Character *ghostPtr6, Character* ghostPtr7, Character* ghostPtr8, 
					int ghostSmartness, int ghostSlowDown)
{
	// TODO: to fix
	
	if((ghostPtr1->_status==1) && (ghostPtr1->_alive==1))
	{
		if(rand()>ghostSlowDown)
		{
			chaseCharacterXYStrategy(ghostPtr1, preyPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4, 
									 ghostPtr2, ghostPtr3, ghostPtr4, ghostPtr5, ghostPtr6, ghostPtr7, ghostPtr8, 
									 ghostSmartness);
		}
		displayCharacter(ghostPtr1);
	}
	
	// displayCharacter(ghostPtr1);
}	

void chaseCharacterMaxAndXStrategyIf(Character* ghostPtr1, Character* preyPtr, 
                    Character* bombPtr1, Character* bombPtr2,
					Character* bombPtr3, Character* bombPtr4,
					Character *ghostPtr2, Character* ghostPtr3, Character *ghostPtr4,
					Character* ghostPtr5, Character *ghostPtr6, Character* ghostPtr7, Character* ghostPtr8,
					int ghostSmartness, int ghostSlowDown)
{
	// TODO: to fix
	
	if((ghostPtr1->_status==1) && (ghostPtr1->_alive==1))
	{
		if(rand()>ghostSlowDown)
		{
			chaseCharacterMaxAndXStrategy(ghostPtr1, preyPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4, 
										  ghostPtr2, ghostPtr3, ghostPtr4, ghostPtr5, ghostPtr6, ghostPtr7, ghostPtr8, 
										  ghostSmartness);
		}
		displayCharacter(ghostPtr1);
	}
	
	// displayCharacter(ghostPtr1);
}	


void chaseCharacterMaxAndYStrategyIf(Character* ghostPtr1, Character* preyPtr, 
                    Character* bombPtr1, Character* bombPtr2,
					Character* bombPtr3, Character* bombPtr4,
					Character *ghostPtr2, Character* ghostPtr3, Character *ghostPtr4,
					Character* ghostPtr5, Character *ghostPtr6, Character* ghostPtr7, Character* ghostPtr8,
					int ghostSmartness, int ghostSlowDown)
{
	// TODO: to fix
	
	if((ghostPtr1->_status==1) && (ghostPtr1->_alive==1))
	{
		if(rand()>ghostSlowDown)
		{
			chaseCharacterMaxAndYStrategy(ghostPtr1, preyPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4, 
										  ghostPtr2, ghostPtr3, ghostPtr4, ghostPtr5, ghostPtr6, ghostPtr7, ghostPtr8, 
										  ghostSmartness);
		}
		displayCharacter(ghostPtr1);
	}
	
	//displayCharacter(ghostPtr1);
}	


void chasePlayer(Character * ghostPtr1, Character * ghostPtr2, 
                 Character * ghostPtr3, Character * ghostPtr4,
				 Character * ghostPtr5, Character * ghostPtr6, 
                 Character * ghostPtr7, Character * ghostPtr8, 
                 Character* preyPtr, 
                 Character* bombPtr1, Character* bombPtr2,
				 Character* bombPtr3, Character* bombPtr4,
				 int ghostSmartness, int ghostSlowDown)
{
	if(level>=MAX_STRATEGY_START_LEVEL)
	{
		chaseCharacterMaxAndYStrategyIf(ghostPtr8, preyPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4,
		ghostPtr1, ghostPtr2, ghostPtr3, ghostPtr4, ghostPtr5, ghostPtr6, ghostPtr7, ghostSmartness, ghostSlowDown);
			
		chaseCharacterMaxAndYStrategyIf(ghostPtr2, preyPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4,
		ghostPtr1, ghostPtr3, ghostPtr4, ghostPtr5, ghostPtr6, ghostPtr7, ghostPtr8, ghostSmartness, ghostSlowDown);
			
		chaseCharacterMaxAndXStrategyIf(ghostPtr1, preyPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4,
		ghostPtr2, ghostPtr3, ghostPtr4, ghostPtr5, ghostPtr6, ghostPtr7, ghostPtr8, ghostSmartness, ghostSlowDown);
			
		chaseCharacterMaxAndXStrategyIf(ghostPtr3, preyPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4,
		ghostPtr1, ghostPtr2, ghostPtr4, ghostPtr5, ghostPtr6, ghostPtr7, ghostPtr8, ghostSmartness, ghostSlowDown);
		
		chaseCharacterMaxAndYStrategyIf(ghostPtr4, preyPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4,
		ghostPtr1, ghostPtr2, ghostPtr3, ghostPtr5, ghostPtr6, ghostPtr7, ghostPtr8, ghostSmartness, ghostSlowDown);
				
		chaseCharacterMaxAndXStrategyIf(ghostPtr5, preyPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4,
		ghostPtr1, ghostPtr2, ghostPtr3, ghostPtr4, ghostPtr6, ghostPtr7, ghostPtr8, ghostSmartness, ghostSlowDown);
				
		chaseCharacterMaxAndYStrategyIf(ghostPtr6, preyPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4,
		ghostPtr1, ghostPtr2, ghostPtr3, ghostPtr4, ghostPtr5, ghostPtr7, ghostPtr8, ghostSmartness, ghostSlowDown);
				
		chaseCharacterMaxAndXStrategyIf(ghostPtr7, preyPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4,
		ghostPtr1, ghostPtr2, ghostPtr3, ghostPtr4, ghostPtr5, ghostPtr6, ghostPtr8, ghostSmartness, ghostSlowDown);
	}
	else
	// 1 - 3*: approximate straight line
	// 4* - 5: get closer in two groups (one approximating x and the other y)
	// 6* - 8: get closer in three groups (as 4* - 5 and one group as 1-3*)
	if((level>=HARD_COLLECTIVE_STRATEGY_START_LEVEL) && 
	   ((ghostCount>=5) || 
	  ((ghostCount==4) && (!ghostPtr1->_alive && !ghostPtr3->_alive && !ghostPtr5->_alive && !ghostPtr7->_alive))))
	{ // HARD COLLECTIVE STRATEGY
		if(ghostCount>=6)
		{
			chaseCharacterIf(ghostPtr1, preyPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4,
			ghostPtr2, ghostPtr3, ghostPtr4, ghostPtr5, ghostPtr6, ghostPtr7, ghostPtr8, ghostSmartness, ghostSlowDown);
		
			chaseCharacterIf(ghostPtr2, preyPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4,
			ghostPtr1, ghostPtr3, ghostPtr4, ghostPtr5, ghostPtr6, ghostPtr7, ghostPtr8, ghostSmartness, ghostSlowDown);
		}
		else
		{
			chaseCharacterXStrategyIf(ghostPtr1, preyPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4,
			ghostPtr2, ghostPtr3, ghostPtr4, ghostPtr5, ghostPtr6, ghostPtr7, ghostPtr8, 
			ghostSmartness, ghostSlowDown);
			
			chaseCharacterYStrategyIf(ghostPtr2, preyPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4,
			ghostPtr1, ghostPtr3, ghostPtr4, ghostPtr5, ghostPtr6, ghostPtr7, ghostPtr8, ghostSmartness, ghostSlowDown);
		}
		chaseCharacterXStrategyIf(ghostPtr3, preyPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4,
		ghostPtr1, ghostPtr2, ghostPtr4, ghostPtr5, ghostPtr6, ghostPtr7, ghostPtr8, ghostSmartness, ghostSlowDown);

		chaseCharacterYStrategyIf(ghostPtr4, preyPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4,
		ghostPtr1, ghostPtr2, ghostPtr3, ghostPtr5, ghostPtr6, ghostPtr7, ghostPtr8, ghostSmartness, ghostSlowDown);
		
		chaseCharacterXStrategyIf(ghostPtr5, preyPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4,
		ghostPtr1, ghostPtr2, ghostPtr3, ghostPtr4, ghostPtr6, ghostPtr7, ghostPtr8, ghostSmartness, ghostSlowDown);

		chaseCharacterYStrategyIf(ghostPtr6, preyPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4,
		ghostPtr1, ghostPtr2, ghostPtr3, ghostPtr4, ghostPtr5, ghostPtr7, ghostPtr8, ghostSmartness, ghostSlowDown);
		
		chaseCharacterXStrategyIf(ghostPtr7, preyPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4,
		ghostPtr1, ghostPtr2, ghostPtr3, ghostPtr4, ghostPtr5, ghostPtr6, ghostPtr8, ghostSmartness, ghostSlowDown);
		
		chaseCharacterYStrategyIf(ghostPtr8, preyPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4,
		ghostPtr1, ghostPtr2, ghostPtr3, ghostPtr4, ghostPtr5, ghostPtr6, ghostPtr7, ghostSmartness, ghostSlowDown);

		} // END OF HARD COLLECTIVE STRATEGY
	else 
	{ // BEGIN OF NON-HARD COLLECTIVE STRATEGIES (Also a special case of HARD COLLECTIVE for ghostCount=3, 4) 
		int hardness = level - EASY_COLLECTIVE_STRATEGY_START_LEVEL; // 6 meaningful values: from 0 to 5 
		if( (ghostCount>=3) && 
		   ((hardness>=5) || (hardness>=0 && ghostCount>=(8-hardness))))
		{	// BEGIN OF EASY COLLECTIVE STRATEGY 
			chaseCharacterXStrategyIf(ghostPtr8, preyPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4,
			ghostPtr1, ghostPtr2, ghostPtr3, ghostPtr4, ghostPtr5, ghostPtr6, ghostPtr7, ghostSmartness, ghostSlowDown);
	
			chaseCharacterYStrategyIf(ghostPtr2, preyPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4,
			ghostPtr1, ghostPtr3, ghostPtr4, ghostPtr5, ghostPtr6, ghostPtr7, ghostPtr8, ghostSmartness, ghostSlowDown);
		} // END OF EASY COLLECTIVE STRATEGY
		else
		{ // BEGIN OF NON-COLLECTIVE STRATEGY
						// TODO: DEBUG		
			chaseCharacterIf(ghostPtr8, preyPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4,
			ghostPtr1, ghostPtr2, ghostPtr3, ghostPtr4, ghostPtr5, ghostPtr6, ghostPtr7, ghostSmartness, ghostSlowDown);
			
			chaseCharacterIf(ghostPtr2, preyPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4,
			ghostPtr1, ghostPtr3, ghostPtr4, ghostPtr5, ghostPtr6, ghostPtr7, ghostPtr8, ghostSmartness, ghostSlowDown);
						// TODO: DEBUG
		} // END OF NON-COLLECTIVE STRATEGY

		chaseCharacterIf(ghostPtr1, preyPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4,
		ghostPtr2, ghostPtr3, ghostPtr4, ghostPtr5, ghostPtr6, ghostPtr7, ghostPtr8, ghostSmartness, ghostSlowDown);
			
		chaseCharacterIf(ghostPtr3, preyPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4,
		ghostPtr1, ghostPtr2, ghostPtr4, ghostPtr5, ghostPtr6, ghostPtr7, ghostPtr8, ghostSmartness, ghostSlowDown);
		
		chaseCharacterIf(ghostPtr4, preyPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4,
		ghostPtr1, ghostPtr2, ghostPtr3, ghostPtr5, ghostPtr6, ghostPtr7, ghostPtr8, ghostSmartness, ghostSlowDown);
				
		chaseCharacterIf(ghostPtr5, preyPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4,
		ghostPtr1, ghostPtr2, ghostPtr3, ghostPtr4, ghostPtr6, ghostPtr7, ghostPtr8, ghostSmartness, ghostSlowDown);
				
		chaseCharacterIf(ghostPtr6, preyPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4,
		ghostPtr1, ghostPtr2, ghostPtr3, ghostPtr4, ghostPtr5, ghostPtr7, ghostPtr8, ghostSmartness, ghostSlowDown);
				
		chaseCharacterIf(ghostPtr7, preyPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4,
		ghostPtr1, ghostPtr2, ghostPtr3, ghostPtr4, ghostPtr5, ghostPtr6, ghostPtr8, ghostSmartness, ghostSlowDown);
				

	} // END OF NON-HARD COLLECTIVE STRATEGIES
}


// CHASING STRATEGIES FOR THE INVINCIBLE ENEMY

void blindChaseCharacterMaxStrategy(Character* hunterPtr, Character* preyPtr)
{
	short deltaX = abs(hunterPtr->_x - preyPtr->_x);
	short deltaY = abs(hunterPtr->_y - preyPtr->_y);
	
    if(rand()>invincibleSlowDown)
        {
			if(level>NASTY_INVINCIBLE_START_LEVEL)
			{
				if(deltaX<deltaY) // Always on the 
				{
					blindChaseCharacterXStrategy(hunterPtr, preyPtr);
				}
				else
				{
					blindChaseCharacterYStrategy(hunterPtr, preyPtr);
				}
			}
			else
			{
				if(rand()%2)
				{
					blindChaseCharacterXStrategy(hunterPtr, preyPtr);
				}
				else
				{
					blindChaseCharacterYStrategy(hunterPtr, preyPtr);
				}	
			}
		}
}

void blindChaseCharacter(Character* hunterPtr, Character* preyPtr)
{
	if((hunterPtr->_status==1) && (hunterPtr->_alive==1))
	{
		if(rand()>invincibleSlowDown)
		{
			if(rand()%2) // Select chase strategy
			{
				blindChaseCharacterXStrategy(hunterPtr, preyPtr);
			}
			else
			{
				blindChaseCharacterYStrategy(hunterPtr, preyPtr);
			}
		}
	}
}

