#ifndef _CONTROL_PLAYER
#define _CONTROL_PLAYER




#include "cross_lib.h"


#if defined(JOYSTICK_CONTROL)
    void movePlayerByJoystick(uint8_t joyInput);
#else
    void movePlayerByKeyboard(uint8_t kbInput);
#endif

    

void MOVE_PLAYER(void);


#endif // _CONTROL_PLAYER


