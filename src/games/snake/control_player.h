#ifndef _CONTROL_PLAYER
#define _CONTROL_PLAYER




#include "cross_lib.h"


#if defined(JOYSTICK_CONTROL)
    uint8_t movePlayerByJoystick(uint8_t joyInput);
#else
    uint8_t movePlayerByKeyboard(uint8_t kbInput);
#endif

    

uint8_t MOVE_PLAYER(void);


#endif // _CONTROL_PLAYER


