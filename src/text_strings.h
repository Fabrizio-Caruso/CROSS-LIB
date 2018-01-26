/*****************************************************************************/
/*                                                                           */
/*                                   vic20.h                                 */
/*                                                                           */
/*                      vic20 system specific definitions                    */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* (C) 1998-2004 Ullrich von Bassewitz                                       */
/*               Römerstraße 52                                              */
/*               D-70794 Filderstadt                                         */
/* EMail:        uz@cc65.org                                                 */
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


#ifndef _TEXT_STRINGS_H
#define _TEXT_STRINGS_H


#if defined(NO_CASE_LETTERS)
	#if defined(TINY_GAME)
		#define CROSS_CHASE_STRING "cross chase"
		#define AUTHOR_STRING "fabrizio caruso"
	#else
		#define CROSS_CHASE_STRING "c r o s s  c h a s e"	
		#define AUTHOR_STRING "by fabrizio caruso"
	#endif
	#define KILL_THE_SKULL_STRING "kill the skull"
	#define LURE_THE_ENEMIES_STRING "lure the enemies"
	#define INTO_THE_MINES_STRING 	"into the mines"
	#define SCORE_STRING "score:"
	#define LEVEL_STRING "level:"
	#define START_LEVEL_STRING "level %u"
	#define USE_THE_GUN_AGAINST_STRING "use the gun against"
	#define THE_SKULL_AND_STRING "the skull and"
	#define MISSILE_BASES_STRING "missile bases"
	#define FOR_POINTS_AND___STRING "for points and  "
	#define EXTRA_POWERUPS__STRING "extra powerups "
#elif defined(__PC6001__)
	#define CROSS_CHASE_STRING "C R O S S  C H A S E"		
	#define AUTHOR_STRING "BY FABRIZIO CARUSO"
	#define KILL_THE_SKULL_STRING "KILL THE SKULL"	
	#define LURE_THE_ENEMIES_STRING "LURE THE ENEMIES"
	#define INTO_THE_MINES_STRING 	"INTO THE MINES"
	#define SCORE_STRING "SCORE:"
	#define LEVEL_STRING "LEVEL:"
	#define START_LEVEL_STRING "LEVEL %u"
	#define USE_THE_GUN_AGAINST_STRING "USE THE GUN AGAINST"
	#define THE_SKULL_AND_STRING "THE SKULL AND"
	#define MISSILE_BASES_STRING "MISSILE BASES"
	#define FOR_POINTS_AND___STRING "FOR POINTS AND  "
	#define EXTRA_POWERUPS__STRING "EXTRA POWERUPS "	
#else
	#if defined(TINY_GAME)
		#define CROSS_CHASE_STRING "CROSS CHASE"
		#define AUTHOR_STRING "Fabrizio Caruso"
	#else
		#define CROSS_CHASE_STRING "C R O S S  C H A S E"		
		#define AUTHOR_STRING "by Fabrizio caruso"
	#endif
	#define KILL_THE_SKULL_STRING "Kill the skull"	
	#define LURE_THE_ENEMIES_STRING "Lure the enemies"
	#define INTO_THE_MINES_STRING "into the mines"
	#define SCORE_STRING "SCORE:"
	#define LEVEL_STRING "LEVEL:"	
	#define START_LEVEL_STRING "LEVEL %u"	
	#define USE_THE_GUN_AGAINST_STRING "Use the gun against"
	#define THE_SKULL_AND_STRING "the skull and"
	#define MISSILE_BASES_STRING "missile bases"
	#define FOR_POINTS_AND___STRING "for points and  "
	#define EXTRA_POWERUPS__STRING "extra powerups "	
#endif

#if defined(JOYSTICK_CONTROL) || defined(__MSX__)
	#define USE_STRING "use the joystick"
#elif !defined(TINY_GAME)		
	#if !defined(__PC6001__)
		#define USE_STRING "use i j k l space"
	#else
		#define USE_STRING "USE I J K L SPACE"
	#endif
#else
	#if !defined(__PC6001__)
		#define USE_STRING "use ijkl"		
	#else
		#define USE_STRING "USE IJKL"
	#endif			
#endif	

#if defined(LESS_TEXT)
	#if defined(NO_CASE_LETTERS)
		#define PRESS_STRING "go"
		#define GAME_OVER_STRING "game over"
		#define YOU_MADE_IT_STRING "the end"		
	#else
		#define PRESS_STRING "GO"	
		#define GAME_OVER_STRING "GAME OVER"
		#define YOU_MADE_IT_STRING "THE END"		
	#endif
#else
	#if defined(NO_CASE_LETTERS)
		#if defined(JOYSTICK_CONTROL) || defined(__MSX__)
			#define PRESS_STRING "press fire"
		#else
			#define PRESS_STRING "press any key"
		#endif
		#define GAME_OVER_STRING "game over"
		#define DEFEAT_STRING "you lost"
		#define VICTORY_STRING "you won"
		#define EXTRA_LIFE_STRING "extra life"
		#define YOU_MADE_IT_STRING "you made it"
	#else
		#if defined(JOYSTICK_CONTROL) || defined(__MSX__)
			#define PRESS_STRING "PRESS FIRE"
		#else
			#define PRESS_STRING "PRESS ANY KEY"
		#endif
		#define GAME_OVER_STRING "GAME OVER"
		#define DEFEAT_STRING "YOU LOST"	
		#define VICTORY_STRING "YOU WON"
		#define EXTRA_LIFE_STRING "EXTRA LIFE"
		#define YOU_MADE_IT_STRING "YOU MADE IT"	
	#endif
#endif
	
#endif // _TEXT_STRINGS_H

