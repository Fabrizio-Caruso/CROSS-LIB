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

#ifndef _CMOC_CONIO_PATCH
#define _CMOC_CONIO_PATCH

	#define cprintf printf
	#define gotoxy(x,y)
	#define cputc
	// #if defined(__ZX80__)
		// #define cputc(c) {gen_tv_field(); putch(c);}
	// #else
		// #define cputc(c) cputc(c);
	// #endif
	//
	// #define cgetc() (char) getch();

	// RED 6
	// BLUE 1
	// MAGENTA 5
	// WHITE 7
	// CYAN 3
	// OTHER RED 4 but should be YELLOW
	// OTHER CYAN 9
	// OTHER GREEN 10
	// #if defined(__AQUARIUS__) || defined(__SPECTRUM__) || defined(__SVI__) 

		#define COLOR_BLACK 0
		#define COLOR_BLUE 1		
		#define COLOR_GREEN 2
		#define COLOR_CYAN 3	
		#define COLOR_MAGENTA 5
		#define COLOR_RED 6	
		#define COLOR_WHITE 7		
		#define COLOR_YELLOW 14
	// #else
		// #define COLOR_WHITE 1
		// #define COLOR_BLACK 0
		// #define COLOR_RED 2
		// #define COLOR_BLUE 3
		// #define COLOR_CYAN 4
	// #endif

	#ifndef CH_ULCORNER
		#define CH_ULCORNER 'X'
		#define CH_URCORNER 'X'
		#define CH_LLCORNER 'X'
		#define CH_LRCORNER 'X'	
	#endif	
#endif // _CMOC_CONIO_PATCH

