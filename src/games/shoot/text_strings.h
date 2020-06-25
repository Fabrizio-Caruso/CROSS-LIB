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


#if defined(NO_CASE_LETTERS)
    #if defined(TINY_GAME)
        #define CROSS_SHOOT_STRING "cross shoot beta"
        #define AUTHOR_STRING "fabrizio caruso"
    #else
        #define CROSS_SHOOT_STRING "c r o s s  s h o o t  beta"    
        #define AUTHOR_STRING "by fabrizio caruso"
    #endif
    #define KILL_THE_SKULL_STRING "kill the skulls"
    #define DESTROY_MISSILES_STRING "destroy the missiles"
    #define LURE_THE_ENEMIES_STRING "kill all enemies"
    #define INTO_THE_MINES_STRING     ""
    #define SCORE_STRING "score "
    #define LEVEL_STRING "level "
    #define START_LEVEL_STRING "level %u"
    #define USE_THE_GUN_AGAINST_STRING "shoot at"
    #define THE_SKULL_AND_STRING "the skulls and"
    #define MISSILE_BASES_STRING "missile bases"
    #define FOR_POINTS_AND___STRING "for points"
    #define EXTRA_POWERUPS__STRING "and items"
#elif defined(__PC6001__)
    #define CROSS_SHOOT_STRING "C R O S S  S H O O T  BETA"        
    #define AUTHOR_STRING "BY FABRIZIO CARUSO"
    #define KILL_THE_SKULL_STRING "KILL THE SKULLS"    
    #define DESTROY_MISSILES_STRING "destroy the missiles"    
    #define LURE_THE_ENEMIES_STRING "KILL ALL ENEMIES"
    #define INTO_THE_MINES_STRING     ""
    #define SCORE_STRING "SCORE "
    #define LEVEL_STRING "LEVEL "
    #define START_LEVEL_STRING "LEVEL %u"
    #define USE_THE_GUN_AGAINST_STRING "SHOOT AT"
    #define THE_SKULL_AND_STRING "THE SKULLS AND"
    #define MISSILE_BASES_STRING "MISSILE BASES"
    #define FOR_POINTS_AND___STRING "FOR POINTS"
    #define EXTRA_POWERUPS__STRING  "AND ITEMS"    
#else
    #if defined(TINY_GAME)
        #define CROSS_SHOOT_STRING "CROSS SHOOT BETA"
        #define AUTHOR_STRING "Fabrizio Caruso"
    #else
        #define CROSS_SHOOT_STRING "C R O S S  S H O O T  BETA"        
        #define AUTHOR_STRING "by Fabrizio Caruso"
    #endif
    #define KILL_THE_SKULL_STRING "Kill the skulls"    
    #define DESTROY_MISSILES_STRING "Destroy the missiles"    
    #define LURE_THE_ENEMIES_STRING "Kill all enemies"
    #define INTO_THE_MINES_STRING ""
    #define SCORE_STRING "SCORE "
    #define LEVEL_STRING "LEVEL "    
    #define START_LEVEL_STRING "LEVEL %u"    
    #define USE_THE_GUN_AGAINST_STRING "Shoot at"
    #define THE_SKULL_AND_STRING "the skulls and"
    #define MISSILE_BASES_STRING "missile bases"
    #define FOR_POINTS_AND___STRING "for points"
    #define EXTRA_POWERUPS__STRING "and items"    
#endif

#if defined(JOYSTICK_CONTROL) || defined(__MSX__) || defined(__CPC_JOYSTICK__)
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
            #define PRESS_STRING "press a key"
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
            #define PRESS_STRING "PRESS A KEY"
        #endif
        #define GAME_OVER_STRING "GAME OVER"
        #define DEFEAT_STRING "YOU LOST"    
        #define VICTORY_STRING "YOU WON"
        #define EXTRA_LIFE_STRING "EXTRA LIFE"
        #define YOU_MADE_IT_STRING "YOU MADE IT"    
    #endif
#endif

#if defined(NO_CASE_LETTERS)
    #define BONUS_COLON_STRING "bonus "
    #define SCORE_COLON__STRING "score "
    #define HIGH_SCORE_COLON_STRING "high score "
#else
    #define BONUS_COLON_STRING "BONUS "
    #define SCORE_COLON__STRING "SCORE "
    #define HIGH_SCORE_COLON_STRING "HIGH SCORE "
#endif

#define BONUS_DIGITS "%u0"
#define SCORE_DIGITS "%05u0"

#endif // _TEXT_STRINGS_H

