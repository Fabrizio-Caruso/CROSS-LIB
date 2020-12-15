#ifndef _SETTINGS_H
#define _SETTINGS_H

    #define INITIAL_LEVEL 1
    // #define DEBUG_LEVELS
    // #define DEBUG_SLOWDOWN
    // #define DEBUG_APPLES

    #define INITIAL_LIVES       5U

    #define EXTRA_POINTS       10U
    #define APPLE_POINTS       20U
    #define COIN_POINTS        50U
    #define SUPER_COIN_POINTS 150U

    #define EXTRA_LIFE_THRESHOLD 5000U

    #define INITIAL_APPLE_COUNT 10U

    #define APPLE_COUNT_INCREASE 2U

    #define MAX_COIN_COUNT 3U

    #define SPEED_INCREASE_THRESHOLD 20U

    #define SPAWNED_APPLE_START 2U

    #define EXTRA_COIN_SPAWN_THRESHOLD 3U

    #define TRANSPARENT_TRIGGER 20

    #define FEW_MINES_SLOWDOWN ((SLOW_DOWN)/8)

    #define INITIAL_SNAKE_LENGTH 3

    #define MAX_APPLES 24

    // This is to accout for extra length gained by eating extra points ($)
    #define EXTRA_LENGTH 10

    #define MAX_SNAKE_LENGTH ((XSize/4)+((NUMBER_OF_LEVELS-1)/4)+(MAX_APPLES)+((MAX_APPLES)/8)+EXTRA_LENGTH)

#endif // _SETTINGS_H

