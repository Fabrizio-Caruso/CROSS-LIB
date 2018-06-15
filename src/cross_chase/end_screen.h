#ifndef _THE_END_HEADER
#define _THE_END_HEADER

#include "character.h"

#if defined(END_SCREEN) || defined(BETWEEN_SCREEN)
	void dance(Character *characterPtr);
#endif

#if defined(END_SCREEN)
	void gameCompleted(void);
#endif
	
#endif // _THE_END_HEADER

