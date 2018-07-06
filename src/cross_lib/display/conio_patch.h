

#if defined(CONIO_ADM3A) || defined(CONIO_VT52) || defined(CONIO_VT100)
	#include <stdio.h>
	
	#define cprintf printf
	
	#if defined(Z88DK)
		#define cputc(c) fputc_cons(c)
	#else
		#define cputc(c) putchar()
	#endif
		
	#if defined(NO_COLOR)
		#define textcolor 
	#endif
#endif


#  if defined(CONIO_ADM3A)
	#define gotoxy(x,y) printf("\033=%c%c",y+32,x+32);
	#define clrscr() printf("\032")

#elif defined(CONIO_VT52)
	#undef gotoxy
	#define gotoxy(x,y) printf("\033Y%c%c",y+32,x+32)

	#undef clrscr
	#define clrscr() printf("\033E")

#elif defined(CONIO_VT100)
	#include <stdio.h>

	#define gotoxy(x,y) printf("\033[%d;%dH", y+1, x+1)
	#define clrscr() printf("\033[H\033[2J")
	
#elif defined(Z88DK_PUTC4X6)
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
			
	#define cputc(c) putc4x6(c)
	
#elif defined(__ATARI_LYNX__)
	#include <tgi.h>
	#include <stdio.h>
	#include <lynx.h>
	
	#define gotoxy(a,b) tgi_gotoxy(a*8,b*8)
	#define clrscr() tgi_clear()
	#define cprintf 
	#define cputc(c) 
	#define textcolor tgi_setcolor

#elif defined(__SUPERVISION__)
	#include <stdio.h>
	
	#define gotoxy(a,b) 
	#define clrscr() 
	#define cprintf 
	#define cputc(c) 	
	#define textcolor
	
#elif defined(__ZX81__) || defined(__ZX80__) || defined(__LAMBDA__)
	#undef gotoxy
	#define gotoxy(x,y) zx_setcursorpos(y,x)

	#define cputc(c) fputc_cons(c)
 
#elif defined(__MSX__) || (defined(__SVI__) && defined(MSX_MODE0))
	#define gotoxy(a,b)     printf("\033Y%c%c",b+31+1,a+31)
	#define clrscr() printf("\033E") 

	#define cputc(c) fputc_cons(c)
	
#elif defined(__C128_Z80__) || defined(__X1__) || defined(__Z9001__) \
	|| defined(__Z1013__) || defined(__OSCA__) || defined(__MC1000__) \
	|| defined(__ABC80__) || defined(__PC6001__) || defined(__SRR__) \
	|| defined(__NASCOM__) || defined(__P2000__) || defined(__BEE__) \
	|| defined(__TI8X__) || defined(__TI82__) || defined(__TI83__) \
	|| defined(__TI82__) || defined(__TI83__) || defined(__TI8X__) \
	|| defined(__TI85__) || defined(__TI86__) || defined(__TI86S__) || defined(__MZ__) \
	|| defined(__GAL__) || defined(__SC3000__) || (defined(__SPECTRUM__) && defined(CLIB_ANSI)) \
	|| defined(__SAM__) || defined(__CPC__) || (defined(__SVI__) && !defined(MSX_MODE0)) \
	|| defined(__VG5K__) || defined(__AQUARIUS__) || defined(__VZ__) \
	|| defined(__MTX__) || defined(__Z88__) || defined(__PX4__) \
	|| defined(__EG2K__) || defined(__TRS80__) || defined(__ACE__) \
	|| defined(__EINSTEIN__) || defined(__M5__)

	#  if defined(__NASCOM__) || defined(__VG5K__)
		#define cputc(c) fputc_cons(c)		
	#else
		#define cputc(c) putch(c);
	#endif

#elif defined(__KC__)
	#include <stdio.h>
	
	#define gotoxy(x,y) do \
		{ \
		printf("\1B%c%c",y+0x80, x+0x80); \
		\
		} while(0)
		
	#define clrscr() printf("\0C");

	#define cprintf printf
			
	#define cputc(c) do \
		{ \
		fputc_cons(c); \
		} while(0)
		
#elif defined(__SPECTRUM__) && !defined(CLIB_ANSI)
	#define gotoxy(x,y) printf("\x16%c%c",x+1,y+1); 
	#define cprintf printf
	#define cputc(c) printf("\x11%c%c",COLOR_BLACK,c);		

#elif defined(__CMOC__) && !defined(__WINCMOC__)

	#if defined(CMOC_RAND_FIX)
		int seed = 0;
		
		int rand()
		{
			seed = seed * (256 + 128 + 1) + 1;
			return seed & 0x7FFF;
		}
	#endif

	#define cprintf printf
	#define gotoxy(x,y) locate(y,x)
	#define cputc(c) printf("%c",c)

#elif defined(__NCURSES__)
	#include <ncurses.h>
	
	#define gotoxy(x,y) do { move(y,x); refresh(); } while(0)
	#define cputc(c) do { addch(c); refresh(); } while(0)

#else
	//
#endif

#if !defined(NO_COLOR)
	#ifndef COLOR_BLACK
		#define COLOR_BLACK 0	
		#define COLOR_WHITE 7
		#define COLOR_BLUE 1
	#endif
	
	#ifndef COLOR_RED
		#if defined(__SPECTRUM__) && !defined(CLIB_ANSI)
			#define COLOR_RED 2
			#define COLOR_MAGENTA 3
			#define COLOR_GREEN 4
			#define COLOR_CYAN 5
			#define COLOR_YELLOW 6
		#else
			#define COLOR_GREEN 2
			#define COLOR_CYAN 3	
			#define COLOR_MAGENTA 5
			#define COLOR_RED 6		
			#define COLOR_YELLOW 14		
		#endif
	#endif
#endif


