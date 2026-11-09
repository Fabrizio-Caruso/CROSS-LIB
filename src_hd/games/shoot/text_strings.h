/* --------------------------------------------------------------------------------------- */ 
// 
// CROSS SHOOT by Fabrizio Caruso
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

#define AUTHOR_STRING _XL_F _XL_a _XL_b _XL_r _XL_i _XL_z _XL_i _XL_o _XL_SPACE _XL_C _XL_a _XL_r _XL_u _XL_s _XL_o


#if defined(TINY_GAME) || XSize<=22
    #define CROSS_SHOOT_STRING _XL_S _XL_H _XL_O _XL_O _XL_T 
#else
    #define CROSS_SHOOT_STRING \
        _XL_S _XL_SPACE _XL_H _XL_SPACE _XL_O _XL_SPACE _XL_O _XL_SPACE _XL_T 
#endif

#define THE_STRING _XL_t _XL_h _XL_e
#define SKULL_STRING _XL_s _XL_k _XL_u _XL_l _XL_l
#define MISSILE_STRING _XL_m _XL_i _XL_s _XL_s _XL_i _XL_l _XL_e
#define MISSILES_STRING  MISSILE_STRING _XL_s
#define AND_STRING _XL_a _XL_n _XL_d
// #define KILL_THE_SKULL_STRING _XL_K _XL_i _XL_l _XL_l _XL_SPACE THE_STRING _XL_SPACE SKULL_STRING

#if XSize<=22
    #define DESTROY_MISSILES_STRING _XL_D _XL_e _XL_s _XL_t _XL_r _XL_o _XL_y _XL_SPACE MISSILES_STRING
#else
    #define DESTROY_MISSILES_STRING _XL_D _XL_e _XL_s _XL_t _XL_r _XL_o _XL_y _XL_SPACE THE_STRING _XL_SPACE MISSILES_STRING
#endif
#define KILL_THEM_ALL__STRING _XL_K _XL_i _XL_l _XL_l _XL_SPACE _XL_t _XL_h _XL_e _XL_m _XL_SPACE _XL_a _XL_l _XL_l
#define KILL_ALL_SKULLS__STRING _XL_K _XL_I _XL_L _XL_L _XL_SPACE _XL_A _XL_L _XL_L _XL_SPACE _XL_S _XL_K _XL_U _XL_L _XL_L _XL_S
#define KILL_SKULLS__STRING _XL_K _XL_I _XL_L _XL_L _XL_SPACE _XL_S _XL_K _XL_U _XL_L _XL_L _XL_S

#define SCORE_STRING _XL_S _XL_C _XL_O _XL_R _XL_E
#define LEVEL_STRING _XL_L _XL_E _XL_V _XL_E _XL_L
#define START_LEVEL_STRING LEVEL_STRING _XL_SPACE 
#define BEFORE__TO_UNLOCK__STRING _XL_B _XL_E _XL_F _XL_O _XL_R _XL_E _XL_SPACE _XL_SPACE _XL_SPACE _XL_T _XL_O _XL_SPACE _XL_U _XL_N _XL_L _XL_O _XL_C _XL_K
#define ITEMS__STRING _XL_I _XL_T _XL_E _XL_M _XL_S
#define SKULLS___AND__STRING SKULL_STRING _XL_s _XL_SPACE _XL_SPACE _XL_SPACE AND_STRING
#define DESTROY_MISSILES__STRING _XL_D _XL_E _XL_S _XL_T _XL_R _XL_O _XL_Y _XL_SPACE _XL_M _XL_I _XL_S _XL_S _XL_I _XL_L _XL_E _XL_S
#define FOR_POINTS_AND___STRING _XL_f _XL_o _XL_r _XL_SPACE _XL_p _XL_o _XL_i _XL_n _XL_i _XL_s
#define EXTRA_POWERUPS__STRING AND_STRING _XL_SPACE _XL_i _XL_t _XL_e _XL_m _XL_s

#if defined(LESS_TEXT)
    #define PRESS_STRING _XL_G _XL_O
#else
    #if !defined(_XL_NO_JOYSTICK)
        #define PRESS_STRING _XL_P _XL_r _XL_e _XL_s _XL_s _XL_SPACE _XL_f _XL_i _XL_r _XL_e
    #else
        #define PRESS_STRING _XL_P _XL_r _XL_e _XL_s _XL_s _XL_SPACE _XL_k _XL_e _XL_y
    #endif
#endif

#define GAME_OVER_STRING _XL_G _XL_A _XL_M _XL_E _XL_SPACE _XL_O _XL_V _XL_E _XL_R
#define YOU_MADE_IT_STRING THE_STRING _XL_SPACE _XL_E _XL_N _XL_D

#define DEFEAT_STRING _XL_Y _XL_o _XL_u _XL_SPACE _XL_l _XL_o _XL_s _XL_t
#define VICTORY_STRING _XL_Y _XL_o _XL_u _XL_SPACE _XL_w _XL_o _XL_n
#define SECOND_ROUND_STRING _XL_W _XL_E _XL_L _XL_L _XL_SPACE _XL_D _XL_O _XL_N _XL_E


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
    #define USE_STRING _USE_STRING _XL_SPACE _XL_I _XL_SPACE _XL_J _XL_SPACE _XL_K _XL_SPACE _XL_L _XL_SPACE _XL_S _XL_P _XL_A _XL_C _XL_E		
	#define USE_LEN 17
#endif	

#define EMPTY_STRING        _XL_SPACE _XL_SPACE _XL_SPACE _XL_SPACE _XL_SPACE _XL_SPACE _XL_SPACE _XL_SPACE _XL_SPACE _XL_SPACE _XL_SPACE _XL_SPACE

#define _SECRET_STRING _XL_S _XL_E _XL_C _XL_R _XL_E _XL_T
#define DISCOVERED_SECRETS_STRING _SECRET_STRING _XL_S
#define OF_STRING _XL_O _XL_F _XL_SPACE
#define GAME_COMPLETED_STRING _XL_V _XL_I _XL_C _XL_T _XL_O _XL_R _XL_Y
#define FOUND_STRING _XL_F _XL_O _XL_U _XL_N _XL_D
#define SECRET_LEVEL_FOUND_STRING _SECRET_STRING _XL_SPACE LEVEL_STRING _XL_SPACE FOUND_STRING
#define SECRET_FOUND_STRING _SECRET_STRING _XL_SPACE FOUND_STRING
#define _KILL_STRING _XL_K _XL_I _XL_L _XL_L 
#define KILL_THE_SKULLS_STRING _KILL_STRING _XL_SPACE THE_STRING _XL_SPACE _XL_S _XL_K _XL_U _XL_L _XL_L _XL_S
#define KILL_THE_BOSS _KILL_STRING _XL_SPACE THE_STRING _XL_SPACE _XL_B _XL_O _XL_S _XL_S

#define HIGH_SCORE_STRING _XL_H _XL_I _XL_S _XL_C _XL_O _XL_R _XL_E _XL_SPACE

#endif // _TEXT_STRINGS_H

