/* --------------------------------------------------------------------------------------- */ 
// 
// CROSS CHASE by Fabrizio Caruso
//
// Fabrizio_Caruso@hotmail.com
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from
// the use of this software.

// Permission is granted to anyone to use this software for non-commercial applications, 
// subject to the following restrictions:

// 1. The origin of this software must not be misrepresented; you must not
// claim that you wrote the original software. If you use this software in
// a product, an acknowledgment in the product documentation would be
// appreciated but is not required.

// 2. Altered source versions must be plainly marked as such, and must not
// be misrepresented as being the original software.

// 3. This notice may not be removed or altered from any source distribution.
/* --------------------------------------------------------------------------------------- */ 


#ifndef _TEXT_STRINGS_H
#define _TEXT_STRINGS_H


#if defined(ONLY_SMALL_LETTERS)
	#  if defined(NO_SPACE_TEXT)
		#define CROSS_CHASE_STRING "cross" _XL_SPACE "chase"
		#define AUTHOR_STRING "fabrizio" _XL_SPACE "caruso"
	#elif defined(TINY_GAME) || XSize<=20
		#define CROSS_CHASE_STRING "cross" _XL_SPACE "chase"
		#define AUTHOR_STRING "fabrizio" _XL_SPACE "caruso"
	#else
		#define CROSS_CHASE_STRING \
            "c" _XL_SPACE "r" _XL_SPACE "o" _XL_SPACE "s" _XL_SPACE "s" _XL_SPACE "c" _XL_SPACE "h" _XL_SPACE "a" _XL_SPACE "s" _XL_SPACE "e"
		#define AUTHOR_STRING "by fabrizio caruso"
	#endif
	#define KILL_THE_SKULL_STRING       "kill" _XL_SPACE "the" _XL_SPACE "skull"
	#if XSize<=20
		#define DESTROY_MISSILES_STRING "destroy" _XL_SPACE "missiles"
	#else
		#define DESTROY_MISSILES_STRING "destroy the missiles"
	#endif
	#define LURE_THE_ENEMIES_STRING     "lure" _XL_SPACE "the" _XL_SPACE "enemies"
	#define INTO_THE_MINES_STRING       "into" _XL_SPACE "the" _XL_SPACE "mines"
	#define SCORE_STRING "score"
	#define LEVEL_STRING "level"
	#define START_LEVEL_STRING "level" _XL_SPACE 
	#define USE_THE_GUN_AGAINST_STRING "shoot" _XL_SPACE "at"
	#define THE_SKULL_AND_STRING "the" _XL_SPACE "skull" _XL_SPACE "and"
	#define MISSILE_BASES_STRING "missile" _XL_SPACE "bases"
	#define FOR_POINTS_AND___STRING "for" _XL_SPACE "points"
	#define EXTRA_POWERUPS__STRING "and" _XL_SPACE "items"
#elif defined(ONLY_CAPITAL_LETTERS)
	#if defined(TINY_GAME) || XSize<=20
		#define CROSS_CHASE_STRING "CROSS CHASE"
		#define AUTHOR_STRING "FABRIZIO CARUSO"
	#else
		#define CROSS_CHASE_STRING "C R O S S   C H A S E"	
		#define AUTHOR_STRING "BY FABRIZIO CARUSO"
	#endif
	#define KILL_THE_SKULL_STRING       "KILL THE SKULL"
	#if XSize<=20
		#define DESTROY_MISSILES_STRING "DESTROY MISSILES"		
	#else
		#define DESTROY_MISSILES_STRING "DESTROY THE MISSILES"
	#endif
	#define LURE_THE_ENEMIES_STRING     "LURE THE ENEMIES"
	#define INTO_THE_MINES_STRING     "INTO THE MINES"
	#define SCORE_STRING "SCORE"
	#define LEVEL_STRING "LEVEL"
	#define START_LEVEL_STRING "LEVEL" _XL_SPACE
	#define USE_THE_GUN_AGAINST_STRING "SHOOT AT"
	#define THE_SKULL_AND_STRING "THE SKULL AND"
	#define MISSILE_BASES_STRING "MISSILE BASES"
	#define FOR_POINTS_AND___STRING "FOR POINTS"
	#define EXTRA_POWERUPS__STRING "AND ITEMS"
#else
	#if defined(TINY_GAME) || XSize<=20
		#define CROSS_CHASE_STRING "C" "R" "O" "S" "S" _XL_SPACE "C" "H" "A" "S" "E"
		#define AUTHOR_STRING "Fabrizio" _XL_SPACE "Caruso"
	#else
		#define CROSS_CHASE_STRING "C" _XL_SPACE "R" _XL_SPACE "O" _XL_SPACE "S" _XL_SPACE "S" _XL_SPACE "C" _XL_SPACE "H" _XL_SPACE "A" _XL_SPACE "S" _XL_SPACE "E"		
		#define AUTHOR_STRING "by Fabrizio Caruso"
	#endif
	#define KILL_THE_SKULL_STRING   "Kill" _XL_SPACE "the" _XL_SPACE "skull"
	#if XSize<=20
		#define DESTROY_MISSILES_STRING "Destroy" _XL_SPACE "missiles"
	#else
		#define DESTROY_MISSILES_STRING "Destroy" _XL_SPACE "the" _XL_SPACE "missiles"
	#endif
	#define LURE_THE_ENEMIES_STRING     "Lure" _XL_SPACE "enemies"
	#define INTO_THE_MINES_STRING   "into" _XL_SPACE "the" _XL_SPACE "mines"
	#define SCORE_STRING "SCORE"
	#define LEVEL_STRING "LEVEL"
	#define START_LEVEL_STRING "LEVE" _XL_SPACE
	#define USE_THE_GUN_AGAINST_STRING "Shoot" _XL_SPACE "at"
	#define THE_SKULL_AND_STRING "the" _XL_SPACE "skull" _XL_SPACE "and"
	#define MISSILE_BASES_STRING "missile" _XL_SPACE "bases"
	#define FOR_POINTS_AND___STRING "for" _XL_SPACE "points"
	#define EXTRA_POWERUPS__STRING "and" _XL_SPACE "items"
#endif

#if defined(ONLY_SMALL_LETTERS)
	#if defined(JOYSTICK_CONTROL)
		#define USE_STRING "use" _XL_SPACE "the" _XL_SPACE "joystick"
	#elif defined(TINY_GAME) 
		#define USE_STRING "use ijkl"
	#elif XSize<=16
		#define USE_STRING "use ijkl space"	
	#else
		#define USE_STRING "use i j k l space"	
	#endif	
#elif defined(ONLY_CAPITAL_LETTERS) 
	#if defined(JOYSTICK_CONTROL)
		#define USE_STRING "USE THE JOYSTICK"
	#elif defined(TINY_GAME) 
		#define USE_STRING "USE IJKL"
	#elif XSize<=16
		#define USE_STRING "USE IJKL SPACE"	
	#else
		#define USE_STRING "USE I J K L SPACE"	
	#endif	
#else
	#if defined(JOYSTICK_CONTROL)
		#define USE_STRING "Use" _XL_SPACE "the" _XL_SPACE "joystick"
	#elif defined(TINY_GAME) 
		#define USE_STRING "Use IJKL"
	#elif XSize<=16
		#define USE_STRING "Use IJKL SPACE"	
	#else
		#define USE_STRING "Use I J K L SPACE"	
	#endif	
#endif

#if defined(QAOPM) && defined(JOYSTICK_CONTROL)
    #undef USE_STRING
    #define USE_STRING "USE Q A O P M"
#endif    

#if defined(ONLY_CAPITAL_LETTERS)
    #define TITLE_LINE_STRING "CROSS CHASE"
#else
    #define TITLE_LINE_STRING "cross chase"
#endif

#if defined(LESS_TEXT)
	#if defined(ONLY_SMALL_LETTERS)
		#define PRESS_STRING "go"
		#if defined(NO_SPACE_TEXT)
			#define GAME_OVER_STRING "game" _XL_SPACE "over"
			#define YOU_MADE_IT_STRING "the" _XL_SPACE "end"
		#else
			#define GAME_OVER_STRING "game over"
			#define YOU_MADE_IT_STRING "the end"
		#endif
	#else
		#define PRESS_STRING "GO"
		#define GAME_OVER_STRING "GAME OVER"
		#define YOU_MADE_IT_STRING "THE END"
	#endif
#else
	#if defined(ONLY_SMALL_LETTERS)
		#if defined(JOYSTICK_CONTROL)
			#define PRESS_STRING "press" _XL_SPACE "fire"
		#else
			#define PRESS_STRING "press a key"
		#endif
		#define GAME_OVER_STRING "game" _XL_SPACE "over"
		#define DEFEAT_STRING "you" _XL_SPACE "lost"
		#define VICTORY_STRING "you" _XL_SPACE "won"
		#define EXTRA_LIFE_STRING "extra" _XL_SPACE "life"
		#define YOU_MADE_IT_STRING "you" _XL_SPACE "made" _XL_SPACE "it"
	#else
		#if defined(JOYSTICK_CONTROL)
			#define PRESS_STRING "PRESS" _XL_SPACE "FIRE"
		#else
			#define PRESS_STRING "PRESS A KEY"
		#endif
		#define GAME_OVER_STRING "GAME OVER"
		#define DEFEAT_STRING "YOU LOST"	
		#define VICTORY_STRING "YOU WON"
		#define EXTRA_LIFE_STRING "EXTRA LIFE"
		#define YOU_MADE_IT_STRING "YOU MADE IT"	
	#endif
#endif

#if defined(ONLY_SMALL_LETTERS)
	#define BONUS_COLON_STRING "bonus "
	#define SCORE_COLON__STRING "score "
	#define HIGH_SCORE_COLON_STRING "high score "
#else
	#define BONUS_COLON_STRING "BONUS "
	#define SCORE_COLON__STRING "SCORE "
	#if XSize<=16
		#define HIGH_SCORE_COLON_STRING "HISCORE "
	#else
		#define HIGH_SCORE_COLON_STRING "HIGH SCORE "
	#endif
#endif

#endif // _TEXT_STRINGS_H

