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

#ifndef _SMS_CONIO_IMPLEMENTATION
#define _SMS_CONIO_IMPLEMENTATION
	#include <stdio.h>
	#include <arch/sms/SMSlib.h>
	
	#define gotoxy(a,b) SMS_setNextTileatXY(x,y)
	
	#define clrscr() SMS_VRAMmemset(SMS_PNTAddress, 0, 32*28*2);
	
	#define cprintf 
	//printf
	//tgi_outtext

	#define cputc(c) SMS_setTile(c);
	//putchar(c); 	

	#define cgetc() (char) getchar();
	
	#define textcolor 

	
	#define COLOR_BLACK 0	
	#define COLOR_BLUE 1

	#define COLOR_RED 2
	#define COLOR_MAGENTA 3
	
	#define COLOR_GREEN 4
	#define COLOR_CYAN 5
	
	#define COLOR_YELLOW 6
	#define COLOR_WHITE 7
#endif // SMS_CONIO_IMPLEMENTATION

