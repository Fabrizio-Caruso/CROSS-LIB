
#if defined(FORCE_NO_CONIO_LIB) && defined(C16_UNEXPANDED)
	#include<peekpoke.h>
	#define textcolor(c) POKE(0x053B,c)
	#define bgcolor(c)
	#define bordercolor(c)
#endif

#if defined(CONIO_ADM3A) || defined(CONIO_VT52) || defined(CONIO_VT100)
	#include <stdio.h>
	
	#define cprintf printf
	
	#if defined(Z88DK)
		#define cputc(c) fputc_cons(c)
	#else
		#define cputc(c) putchar(c)
	#endif
		
	#if defined(NO_COLOR)
		#define textcolor 
	#endif
#endif

#if defined(__G800__) || defined(__MZ2500__) || defined(__LASER500__) || defined(__PC88__)
	#define cputc(c) fputc_cons(c)	
#endif

#if defined(__SMS__)
	#include <stdio.h>
	#include <stropts.h>
	#include <arch/sms.h>
	#include <rect.h>
	#include <input.h>
	#include <arch/sms/SMSlib.h>
	#include <arch/sms/PSGlib.h>

	#define gotoxy(x,y) \
		SMS_setNextTileatXY(x,y)
		
	#define cputc(c) \
		SMS_setTile(c)

#endif

// TODO: BOGUS! Implement this
#if defined(__MZ2500__)
	#define gotoxy(x,y)
	/*
	#define gotoxy(x,y) \
		do \
		{ \
			unsigned char i; \
			\
			printf("\021"); \
			for(i=0;i<x;++i) \
			{ \
				printf("\019"); \
			} \
			for(i=0;i<y;++i) \
			{ \
				printf("\017"); \
			} \
		} while(0)
		*/			
	#define clrscr() printf("--------------------\n");
	//putchar((unsigned char) 22)
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
	
	#if defined(__ZX80__) && defined(ZX80_GEN_TV_FIELD)
		#define gotoxy(x,y) do {gen_tv_field(); zx_setcursorpos(y,x); gen_tv_field();} while(0)
		#define cputc(c) do { gen_tv_field(); fputc_cons(c); gen_tv_field(); } while(0)
	#else
		#define gotoxy(x,y) zx_setcursorpos(y,x)
		#define cputc(c) fputc_cons(c)
	#endif
	

 
#elif defined(__MSX__) || (defined(__SVI__) && defined(MSX_MODE0))
	#define gotoxy(a,b)     printf("\033Y%c%c",b+31+1,a+31)
	#define clrscr() printf("\033E") 

	#define cputc(c) fputc_cons(c)
	
#elif defined(__C128_Z80__) || defined(__X1__) || defined(__Z9001__) \
	|| defined(__Z1013__) || defined(__OSCA__) || defined(__MC1000__) \
	|| defined(__ABC80__) || defined(__ABC800__) || defined(__PC6001__) || defined(__SRR__) \
	|| defined(__NASCOM__) || defined(__P2000__) || defined(__BEE__) \
	|| defined(__TI8X__) || defined(__TI82__) || defined(__TI83__) \
	|| defined(__TI82__) || defined(__TI83__) || defined(__TI8X__) \
	|| defined(__TI85__) || defined(__TI86__) || defined(__TI86S__) || defined(__MZ__) \
	|| defined(__GAL__) || defined(__SC3000__) || defined(__SPECTRUM__) \
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
		
#elif defined(__CMOC__) && !defined(__WINCMOC__) && !defined(__MO5__)

	#define cprintf printf
	#define gotoxy(x,y) locate(y,x)
	#define cputc(c) printf("%c",c)

#elif defined(__MO5__)
	#define cprint 

	#define cputc(c) \
		asm { \
		   ldb c \
		   swi \
		   .byte 2 \
		}

					
		
	#define gotoxy(x,y)	\
			do \
			{ \
				cputc(0x1B); \
				cputc(0x3F+(y)); \
				cputc(0x40+(x)); \
			} while(0)
			
		
#elif defined(__NCURSES__)
	#include <ncurses.h>
	
	#define gotoxy(x,y) do { move(y,x); refresh(); } while(0)
	#define cputc(c) do { addch(c); refresh(); } while(0)

#else
	//
#endif

#if !defined(NO_COLOR)
	#ifndef COLOR_BLACK
		#if defined(Z88DK)
			#define COLOR_BLUE BLUE
			#define COLOR_BLACK BLACK
			#if defined(__PC6001__)
				#define COLOR_WHITE 7
			#else
				#define COLOR_WHITE WHITE
			#endif
		#endif
	#endif
	
	#ifndef COLOR_RED
		#define COLOR_GREEN GREEN
		#define COLOR_CYAN CYAN
		#define COLOR_MAGENTA MAGENTA
		#define COLOR_RED RED
		#define COLOR_YELLOW YELLOW		
	#endif	
	
#endif


