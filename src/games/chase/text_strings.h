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

#if defined(TINY_GAME)
    #define AUTHOR_STRING "F C"
#else
    #define AUTHOR_STRING "FABRIZIO CARUSO"
#endif

#if defined(TINY_GAME) || XSize<=22
    #define CROSS_CHASE_STRING "CHASE"
	// #define CROSS_LEN 11
#else
    #define CROSS_CHASE_STRING "C H A S E"

	// #define CROSS_LEN 20

#endif
#define THE_STRING _XL_t _XL_h _XL_e
#define SKULL_STRING _XL_s _XL_k _XL_u _XL_l _XL_l
#define MISSILE_STRING _XL_m _XL_i _XL_s _XL_s _XL_i _XL_l _XL_e
#define MISSILES_STRING  MISSILE_STRING _XL_s
#define AND_STRING _XL_a _XL_n _XL_d
#define KILL_THE_SKULL_STRING _XL_K _XL_i _XL_l _XL_l _XL_SPACE THE_STRING _XL_SPACE SKULL_STRING

#if XSize<=22
    #define DESTROY_MISSILES_STRING _XL_D _XL_e _XL_s _XL_t _XL_r _XL_o _XL_y _XL_SPACE MISSILES_STRING
	#define DESTROY_LEN 16
#else
    #define DESTROY_MISSILES_STRING _XL_D _XL_e _XL_s _XL_t _XL_r _XL_o _XL_y _XL_SPACE THE_STRING _XL_SPACE MISSILES_STRING
	#define DESTROY_LEN 20
#endif
#define LURE_THE_ENEMIES_STRING "LURE THE ENEMIES"
#define INTO_THE_MINES_STRING   "INTO THE MINES"
#define SCORE_STRING _XL_S _XL_C _XL_O _XL_R _XL_E
#define LEVEL_STRING _XL_L _XL_E _XL_V _XL_E _XL_L
#define START_LEVEL_STRING LEVEL_STRING _XL_SPACE 
#define USE_THE_GUN_AGAINST_STRING _XL_S _XL_h _XL_o _XL_o _XL_t _XL_SPACE _XL_a _XL_t
#define THE_SKULL_AND_STRING THE_STRING _XL_SPACE SKULL_STRING _XL_SPACE AND_STRING
#define MISSILE_BASES_STRING MISSILE_STRING _XL_SPACE _XL_b _XL_a _XL_s _XL_e _XL_s
#define FOR_POINTS_AND___STRING _XL_f _XL_o _XL_r _XL_SPACE _XL_p _XL_o _XL_i _XL_n _XL_t _XL_s
#define EXTRA_POWERUPS__STRING AND_STRING _XL_SPACE _XL_i _XL_t _XL_e _XL_m _XL_s


#if defined(LESS_TEXT)
    #define PRESS_STRING _XL_G _XL_O
	#define PRESS_LEN 2
#else
    #if !defined(_XL_NO_JOYSTICK)
        #define PRESS_STRING _XL_P _XL_r _XL_e _XL_s _XL_s _XL_SPACE _XL_f _XL_i _XL_r _XL_e
		#define PRESS_LEN 10
    #else
        #define PRESS_STRING _XL_P _XL_r _XL_e _XL_s _XL_s _XL_SPACE _XL_k _XL_e _XL_y
		#define PRESS_LEN 9
    #endif
#endif

#define GAME_OVER_STRING _XL_G _XL_A _XL_M _XL_E _XL_SPACE _XL_O _XL_V _XL_E _XL_R
#define YOU_MADE_IT_STRING THE_STRING _XL_SPACE _XL_E _XL_N _XL_D

#define DEFEAT_STRING _XL_Y _XL_o _XL_u _XL_SPACE _XL_l _XL_o _XL_s _XL_t
#define VICTORY_STRING _XL_Y _XL_o _XL_u _XL_SPACE _XL_w _XL_o _XL_n
#define EXTRA_LIFE_STRING _XL_E _XL_x _XL_t _XL_r _XL_a _XL_SPACE _XL_l _XL_i _XL_f _XL_e

#define _USE_STRING _XL_U _XL_s _XL_e

#if !defined(_XL_NO_JOYSTICK)
    #define USE_STRING _USE_STRING _XL_SPACE THE_STRING _XL_SPACE _XL_j _XL_o _XL_y _XL_s _XL_t _XL_i _XL_c _XL_k
	#define USE_LEN 16
#elif defined(TINY_GAME) 
    #define USE_STRING _USE_STRING _XL_SPACE _XL_I _XL_J _XL_K _XL_L
	#define USE_LEN 8
#elif XSize<=16
    #define USE_STRING _USE_STRING _XL_SPACE _XL_I _XL_J _XL_K _XL_L _XL_SPACE _XL_S _XL_P _XL_A _XL_C _XL_E	
	#define USE_LEN 14
#else
    #define USE_STRING "USE I J K L SPACE"		
	#define USE_LEN 17
#endif	


#endif // _TEXT_STRINGS_H

