#include <conio.h>
#include <joystick.h>

#define IF_KEYBOARD_HIT if(kbhit())
	
#define GET_CHAR() cgetc();

#define WAIT_JOY_OR_KEY_PRESS() 	while(!kbhit() && !joy_read(JOY_1)) {} 

#define WAIT_JOY_PRESS() while(!joy_read(JOY_1)){}

#define WAIT_KEY_PRESS() while(!kbhit()){}

#define JOY_INSTALL() { joy_load_driver (joy_stddrv); joy_install (joy_static_stddrv); }

#define GET_JOY1() joy_read (JOY_1);

#define GET_JOY2() joy_read (JOY_2);
