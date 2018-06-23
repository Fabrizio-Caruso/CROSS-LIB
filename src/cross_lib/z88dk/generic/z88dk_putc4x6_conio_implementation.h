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

#ifndef _Z88DK_PUTC4X6_CONIO_IMPLEMENTATION
#define _Z88DK_PUTC4X6_CONIO_IMPLEMENTATION
	#include <stdio.h>
	#include <graphics.h>
	#include <games.h>

	extern int _LIB_putc4x6(char c);
	extern unsigned char x_4x6;
	extern unsigned char y_4x6;

	#undef gotoxy
	#define gotoxy(x,y) do \
		{ \
			x_4x6 = (x)*4; \
			y_4x6 = (y)*6; \
		\
		} while(0)
		
	#undef clrscr
	#define clrscr() clg();

	#define cprintf printf
			
	#define cputc(c) do \
		{ \
			putc4x6(c); \
		} \
		while (0)
	
	#define cgetc() getk()

	#undef textcolor
	#define textcolor 
	
	// #define COLOR_BLACK 0	
	// #define COLOR_BLUE 1

	// #define COLOR_RED 2
	// #define COLOR_MAGENTA 3
	
	// #define COLOR_GREEN 4
	// #define COLOR_CYAN 5
	
	// #define COLOR_YELLOW 6
	// #define COLOR_WHITE 7
	

#endif // _Z88DK_PUTC4X6_CONIO_IMPLEMENTATION
