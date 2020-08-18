#ifndef _KEY_PRESSED_H
#define _KEY_PRESSED_H

#if defined(KEYBOARD_CONTROL)
    #define KEY_PRESSED() (kbhit())
#else
    #define KEY_PRESSED() (JOY_INPUT())
#endif

#endif // _KEY_PRESSED_H

