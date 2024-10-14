#ifndef _SETTINGS_H
#define _SETTINGS_H


#define INITIAL_LEVEL 1
// #define DEBUG_LEVELS
// #define DEBUG_SLOWDOWN
// #define DEBUG_APPLES
// #define DEBUG_ACHIEVEMENTS
// #define NO_EXTRA_TITLE
// #define DEBUG_FREEZE
// #define NO_ACHIEVEMENTS_SCREEN
// #define NO_DISPLAY_LEVEL_ANIMATION
// #define DEBUG_SECRET_HOLE

#define FINAL_LEVEL 32

#define INITIAL_LIVES       5U

#if XSize>27
    #define INITIAL_SNAKE_LENGTH (XSize/7)
#else
    #define INITIAL_SNAKE_LENGTH (XSize/5)
#endif

#define EXTRA_POINTS        5U
#define RING_POINTS        10U
#define APPLE_POINTS       20U
#define SUPER_RING_POINTS  50U

#define EXTRA_LIFE_THRESHOLD 5000U

#define INITIAL_APPLE_COUNT 10U

#define APPLE_COUNT_INCREASE 2U

#define SOME_EXTRA 10
#define MANY_EXTRA (15+(XSize)+(XSize)/2)

#if XSize<=40
    #define SPEED_INCREASE_THRESHOLD 18U
#else
    #define SPEED_INCREASE_THRESHOLD 25U
#endif

#define SPAWNED_APPLE_START 2U

#define EXTRA_RING_SPAWN_THRESHOLD 7U

// BONUS/SECRET LEVELS THRESHOLD
#define MAGIC_WALL_THRESHOLD 12
#define RING_THRESHOLD ((MAGIC_WALL_THRESHOLD)+((XSize)/2)+15)
#define EXTRA_1UP_THRESHOLD ((RING_THRESHOLD)+20+((XSize)/2))

#define TRANSPARENT_TRIGGER 20

#define FEW_MINES_SLOWDOWN ((_XL_SLOW_DOWN_FACTOR)/8)

#define MAX_APPLES 24

// This is to accout for extra length gained by eating extra points ($)
#define EXTRA_LENGTH 4

#if !defined(MAX_SNAKE_LENGTH)
    #define MAX_SNAKE_LENGTH ((XSize)-2+(YSize)-2-1)
    
    //((XSize/2)+((NUMBER_OF_MAPS-1)/4)+(MAX_APPLES)+((MAX_APPLES)/8)+EXTRA_LENGTH)
#endif

#define MAX_NUMBER_OF_HORIZONTAL_MINES 4
#define MAX_NUMBER_OF_VERTICAL_MINES 2

#define NUMBER_OF_MAPS 16

#define NUMBER_OF_STRINGS 5

#define MAX_STRING_SIZE (6+1)

#define EMPTY 0
#define DEADLY 1
#define SUPER_RING 2
#define RING 3
#define EXTRA 4
#define APPLE 5
#define EXTRA_LIFE 6
#define SECRET 7
#define WALL  8
#define HORIZONTAL_WALL 9
#define VERTICAL_WALL 10
#define TRANSPARENT 11


#endif // _SETTINGS_H

