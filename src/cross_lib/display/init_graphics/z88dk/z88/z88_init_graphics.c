#include <dor.h>

// #define HELP1   "A pointless demo application made with z88dk"
// #define HELP2   "Simply loops, points out menu selections and dies"
// #define HELP3   "..eventually..see source for details"
// #define HELP4   ""
// #define HELP5   "v0.1 - 9.4.99 (djm)"
// #define HELP6   ""

#pragma -no-expandz88

#if !defined(BAD_PAGES)
	#define BAD_PAGES 2
#else
	#define BAD_PAGES 1
#endif 
#pragma -reqpag=BAD_PAGES

#pragma printf = ""

// /* Redirect OZ required callbacks to our functions */
// #pragma redirect handlecmds=_handlecmds
// #pragma redirect redrawscreen=_redrawscreen

#define APP_INFO "by Fabrizio Caruso"
#define APP_KEY  'X'
#define APP_NAME "XChase"
// #define APP_TYPE AT_Good


// #define TOPIC1   "Action"
// #define TOPIC1ATTR      TP_Help
// #define TOPIC1HELP1     "Some help for the topic"
// #define TOPIC1HELP2     "And another line of help"

// #define TOPIC1_1         "Eat"
// #define TOPIC1_1KEY      "AE"
// #define TOPIC1_1CODE     $81
// #define TOPIC1_1ATTR     MN_Help

// #define TOPIC1_1HELP1    "Are you hungry?"

// #define TOPIC1_2         "Drink"
// #define TOPIC1_2KEY      "AD"
// #define TOPIC1_2CODE     $82
// #define TOPIC1_2ATTR     MN_Help
// #define TOPIC1_2HELP1    "Why yes, if you're offering, mine'd a"
// #define TOPIC1_2HELP2    "pint of something nice"

// #define TOPIC1_3         "Sleep"
// #define TOPIC1_3KEY      "AS"
// #define TOPIC1_3CODE     $83
// #define TOPIC1_3ATTR     MN_Help
// #define TOPIC1_3HELP1    "Yawn...."

// #define TOPIC2           "Commands"
// #define TOPIC2ATTR       TP_Help
// #define TOPIC2HELP1      "From this menu you can quit this"
// #define TOPIC2HELP2      "Completely useless application"

// #define TOPIC2_1         "Quit"
// #define TOPIC2_1KEY      "CQ"
// #define TOPIC2_1CODE     $84
// #define TOPIC2_1ATTR     MN_Help
// #define TOPIC2_1HELP1    "Go on, leave me now!"


#include <application.h>

#include <graphics.h> 

#include "display_macros.h"

void _XL_INIT_GRAPHICS(void) 
{ 
	#if defined(__Z88DK_SPRITES_GRAPHICS)
		static struct window win; /* Window structure */ 	
	
		win.graph=1; 
		win.width=255; 
		win.number='4'; 
		window(win); 
	#endif
    _setScreenColors();
}

