/*****************************************************************************/
/*                                                                           */
/*                                		                                     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* (C) 2017      Fabrizio Caruso                                  		     */
/*                					                                         */
/*              				                                             */
/* EMail:        Fabrizio_Caruso@hotmail.com                                 */
/*                                                                           */
/*                                                                           */
/* This software is provided 'as-is', without any expressed or implied       */
/* warranty.  In no event will the authors be held liable for any damages    */
/* arising from the use of this software.                                    */
/*                                                                           */
/* Permission is granted to anyone to use this software for any purpose,     */
/* including commercial applications, and to alter it and redistribute it    */
/* freely, subject to the following restrictions:                            */
/*                                                                           */
/* 1. The origin of this software must not be misrepresented; you must not   */
/*    claim that you wrote the original software. If you use this software   */
/*    in a product, an acknowledgment in the product documentation would be  */
/*    appreciated but is not required.                                       */
/* 2. Altered source versions must be plainly marked as such, and must not   */
/*    be misrepresented as being the original software.                      */
/* 3. This notice may not be removed or altered from any source              */
/*    distribution.                                                          */
/*                                                                           */
/*****************************************************************************/

#ifndef _VT52_CONIO_IMPLEMENTATION
#define _VT52_CONIO_IMPLEMENTATION
	#include <stdio.h>

	#define gotoxy(x,y) do \
		{ \
		printf("%c%c%c%c",27,'Y',y+32,x+32); \
		\
		} while(0)
		
	
	#define clrscr() printf("%cH%cJ",27,27);
	
	// #if defined(__KAYPRO__)
		// #define clrscr() printf("%c",26);
	// #else
		// #define clrscr() printf("%c%c",27,'*');
	// #endif
	// #define clrscr() do {printf("%c%c",26); printf("%c%c",27,'*');} while(0)

	#define cprintf printf
	
// 		printf("%c%c", 27,'2'); 	

	// #if defined(__KAYPRO__)
		// #define cputc(c) do \
			// { \
			// printf("\033C4"); \
			// fputc_cons(c); \
			// } while(0)	
	// #else
		// #define cputc(c) do \
			// { \
			// fputc_cons(c); \
			// } while(0)
	// #endif
	
	#define cputc(c) do \
	{ \
		fputc_cons(c); \
	} while(0)
	
	#define cgetc() getk()

	#define textcolor 

	
	#define COLOR_BLACK 0	
	#define COLOR_BLUE 1

	#define COLOR_RED 2
	#define COLOR_MAGENTA 3
	
	#define COLOR_GREEN 4
	#define COLOR_CYAN 5
	
	#define COLOR_YELLOW 6
	#define COLOR_WHITE 7
	
#endif // VT52_CONIO_IMPLEMENTATION

