#ifndef _GAME_TEXT_H
#define _GAME_TEXT_H

#define MAX_ENERGY 20

#if XSize<22
    #define HISCORE_OFFSET 2
#else
    #define HISCORE_OFFSET 0
#endif

#define RED_ENERGY_THRESHOLD ((MAX_ENERGY)/2)

void DISPLAY_ENERGY(void);

void PRESS_KEY(void);

void DISPLAY_POINTS(void);

void DISPLAY_REMAINING_APPLES_COUNT(void);

void DISPLAY_LIVES(void);

#endif // _GAME_TEXT_H

