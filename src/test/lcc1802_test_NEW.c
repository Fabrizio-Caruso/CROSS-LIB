/*
    Hello World for the COMX-35
    by Marcel van Tongoren & Bill Rowe
*/

#include <nstdlib.h>
#include "comx\rca_vis_video.h"
#include "comx\rca_vis_sound.h"
#ifdef __PECOM__
#include "comx\rca_joystick.h"
#endif
#ifdef __TMC600__
#include "comx\rca_joystick.h"
#endif
#ifdef __CIDELSA__
#include "comx\rca_joystick.h"
#include "comx\rca_character_set.h"
#endif
#ifdef __COMX__
#include "comx\rca_keyboard_encoder.h"
#endif
#include "comx\rca_rand.h"
#include "comx\rca_system.h"
#ifndef __CIDELSA__
#include "comx\rca_printf.h"
#endif

#ifdef __COMX__
#define key_up 0x82
#define key_right 0x83
#define key_left 0x84
#define key_down 0x85
#define key_exit 0x71
#endif

#ifdef __PECOM__
#define key_up 0x5e
#define key_right 0x5d
#define key_left 0x5c
#define key_down 0x5b
#define key_exit 0x40
#endif

#ifdef __TMC600__
#define key_up 0xb
#define key_right 0x9
#define key_left 0x8
#define key_down 0xa
#define key_exit 0x20
#endif

#ifdef __CIDELSA__
#define key_up 0x40		// UP is really right on the Cidelsa screen, key up = 0x10
#define key_right 0x20	// RIGHT is really down on the Cidelsa screen, key right = 0x40
#define key_left 0x10	// LEFT is really up on the Cidelsa screen, key left = 0x80
#define key_down 0x80	// DOWN is really left on the Cidelsa screen, key down = 0x20
#define key_exit 0x1
#endif

void main(){

#ifndef __CIDELSA__
    static const unsigned char shapes[] =
    {
    //	97, // First byte is the character to shape followed by 9 bytes for the shape
        0x00, 0xcc, 0xde, 0xed, 0xff, 0xf3, 0xed, 0xe1, 0x00,
    //	98, // next character to shape
        0x00, 0x8c, 0x9e, 0xad, 0xbf, 0xb3, 0xad, 0xa1, 0x00,
    //	99,
        0x00, 0x00, 0xcc, 0xde, 0xff, 0xde, 0xcc, 0x00, 0x00,
    };
#endif

	unsigned int vidmem=0xf800;

	unsigned char key;
	unsigned int loop;
	unsigned int oldbkcolor;

	disableinterrupt();
    setvideobase(vidmem);

#ifdef __CIDELSA__
    #ifdef __DRACO__
        vidmem=0xfc10;
    #else
        vidmem=0xfbc0;
    #endif
#endif

	oldbkcolor = bgcolor(0);
    #ifndef __TMC600__
        textcolordefinition(3);
        monochrome(0);
    #else
        textcolor(7);
    #endif

#ifdef __CIDELSA__
	character_set(4);
    #ifdef __DRACO__
        vidclr(0xf800,27*40);
    #else
        vidclr(0xf800,25*40);
    #endif
	vidstrcpy(vidmem, "HELLO WORLD");
	vidstrcpy(vidmem + 4, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	vidstrcpy(vidmem + 6, "0 1 2 3 4 5 6 7 8 9");
	textcolor(1);
	vidstrcpy(vidmem + 8, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	vidstrcpy(vidmem + 10, "0 1 2 3 4 5 6 7 8 9");
	textcolor(2);
	vidstrcpy(vidmem + 12, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	vidstrcpy(vidmem + 14, "0 1 2 3 4 5 6 7 8 9");
	textcolor(3);
	vidstrcpy(vidmem + 16, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	vidstrcpy(vidmem + 18, "0 1 2 3 4 5 6 7 8 9");
	vidmem += 2;
#else
    #ifndef __TMC600__
        shapecolor(0x20, 0x5f, 2);
        shapechar(shapes, 0x61, 3);
    #endif
    vidclr(vidmem,24*40);
    vidstrcpy(vidmem, "HELLO ");
    textcolor(1);
    vidstrcpy(vidmem+6, "WORLD abc");
    vidmem = 0xf828;
#endif

#ifdef __TMC600__
    setcolor(0,3);
#endif
loop = 1;

while (1){
#ifdef __CIDELSA__
        key = get_stick();
        if (get_trigger() != 0)
            key = 1;
#else
        key = getkey();
#endif
    switch (key){
        case key_up:
            vidstrcpy(vidmem,"UP   ");
            generatetone(80,5,7);
        break;
        case key_right:
            vidstrcpy(vidmem,"RIGHT");
            generatenoise(2,7);
        break;
        case key_left:
            vidstrcpy(vidmem,"LEFT ");
            generatenoise(6,7);
        break;
        case key_down:
            vidstrcpy(vidmem,"DOWN ");
            generatetone(80,7,7);
        break;
        case key_exit:  // Q or space
            loop = 0;
        break;	
        default:
        break;			
    }
    if (kbhit() == 0)
    {
        generatetone(0, 0, 0);
        generatenoise(0,0);
    }
    if (loop == 0)  break;
}

while (kbhit() == 1)
{
}

loop = 1;
vidmem = 0xfa00;
vidchar(vidmem,'X');
while (1){
#ifdef __CIDELSA__
        key = get_stick();
        if (get_trigger() != 0)
            key = 1;
#else
        key = getkey();
#endif
    switch (key){
        case key_up:
            vidchar(vidmem,' ');
            vidmem = vidmem - 40;
            vidchar(vidmem,'X');
        break;
        case key_right:
            vidchar(vidmem,' ');
            vidmem++;
            vidchar(vidmem,'X');
        break;
        case key_left:
            vidchar(vidmem,' ');
            vidmem--;
            vidchar(vidmem,'X');
        break;
        case key_down:
            vidchar(vidmem,' ');
            vidmem = vidmem + 40;
            vidchar(vidmem,'X');
        break;
        case key_exit:  // Q or space
            loop = 0;
        break;	
        default:
        break;			
    }
    if (loop == 0)  break;
}

enableinterrupt();
vidclr(vidmem,24*40);
#ifndef __CIDELSA__
    gotoxy(0,0);
    printf("HELLO WORLD!");
    gotoxy(20,0);
    printf("20,0");
    gotoxy(0,20);
    printf("0,20");
    gotoxy(10,10);
    printf("10,10");

    printf(" %i",rand());
    printf(" %i",rand());
    printf(" %i",rand());
    printf(" %i",rand());
    printf(" %i",rand());
#endif
}

#include <nstdlib.c>
#include <comx\rca_character_set.c>
