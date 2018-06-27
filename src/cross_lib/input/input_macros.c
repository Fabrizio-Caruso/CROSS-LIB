/* --------------------------------------------------------------------------------------- */ 
// 
// CROSS CHASE by Fabrizio Caruso
//
// Fabrizio_Caruso@hotmail.com
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from
// the use of this software.

// Permission is granted to anyone to use this software for non-commercial applications, 
// subject to the following restrictions:

// 1. The origin of this software must not be misrepresented; you must not
// claim that you wrote the original software. If you use this software in
// a product, an acknowledgment in the product documentation would be
// appreciated but is not required.

// 2. Altered source versions must be plainly marked as such, and must not
// be misrepresented as being the original software.

// 3. This notice may not be removed or altered from any source distribution.
/* --------------------------------------------------------------------------------------- */ 

#include "../cross_lib.h"

#include "../sleep/sleep_macros.h"

#include "input_macros.h"

#if defined(__MSX__)
	#include<msx/gfx.h>
#elif defined(__SPECTRUM__)
	#include<input.h>
#endif
		

#if defined(KEYBOARD_CONTROL)
	unsigned char GET_CHAR(void)
	{
	#if defined(TURN_BASED)
		return TURN_BASED_INPUT();
	
	#elif defined(__MSX__)
		if(!get_trigger(0)) 
		{
			return get_stick(0);
		} 
		else 
		{
			return 9;
		}
	
	#elif defined(__VIC20__) || defined(__SUPERVISION__) || defined(__CREATIVISION__) || defined(__OSIC1P__) \
	|| defined(__APPLE2__) || defined(__APPLE2ENH__) || defined(__CBM610__) || defined(__C16__)
		if(kbhit())
			return cgetc();
		else
			return 0;
	
	#elif defined(__ATMOS__)
		#include <peekpoke.h>	
		
		unsigned char polledValue = PEEK(0x208);

		switch(polledValue)
		{
			case 141:
				return 'I';
			break;
			case 129:
				return 'J';
			break;
			case 131:
				return 'K';
			break;
			case 143:
				return 'L';
			break;
			case 132:
				return ' ';
			break;
		}
		return '\0';
	
	#elif defined(__SPECTRUM__)
		#if defined(CLIB_ANSI)
			return in_Inkey();
		#else
			return in_inkey();
		#endif
	
	#elif defined(__TRS80__) || defined(__EG2K__) 
		#define POKE(addr,val)     (*(unsigned char*) (addr) = (val))
		#define PEEK(addr)         (*(unsigned char*) (addr))

		unsigned char ijkl = PEEK(0x3802);
		
		if(ijkl==2)
			return 'I';
		else if(ijkl==4)
			return 'J';
		else if(ijkl==8)
			return 'K'; 
		else if(ijkl==16)
			return 'L';
		
		if(PEEK(0x3840)==128)
			return ' ';
		else
			return '\0';
	
	#elif defined(__NCURSES__)
		#define INPUT_LOOPS 10
		
		unsigned long delay = 0;
		char _ch;
		char ch;

		while(delay<INPUT_LOOPS)
		{	
			_ch = getch();
			if(_ch!=ERR)
			{
				ch = _ch;
			}
			++delay;
		}
		
		return ch;
	
	#elif defined(__CMOC__) && !defined(__WINCMOC__)
		#include <cmoc.h>
		#include <coco.h>
		
		unsigned char res;
		unsigned char machine;
		
		asm {
			ldd $8000
			cmpd #$7EBB
			beq dragon
			lda #253
			sta machine
			bra pia
dragon		lda #247
			sta machine
pia			lda #253		
			sta $FF02
			ldb #73
test    	lda $ff00
			cmpa machine
			beq out
			incb
			rol $ff02
			inc $ff02
			cmpb #77
			bne test
			clrb 
out			stb res
		}
		
		#if !defined(__WINCMOC__)
			if(res == 0)
				return inkey();
			return res;
		#else
			if(kbhit() && cgetc()==' ')
			{
				return ' ';
			}
			else
			{
				return '\0';
			}
		#endif
		
	#else
		return getk();
	#endif
	}
#endif	
	
#if defined(NO_WAIT) && !defined(NO_SLEEP)
	void WAIT_PRESS(void)
	{
		sleep(2);
	}
#elif defined(NO_WAIT)
//
#elif defined(WAIT_FOR_KEY)
	#  if defined(__NCURSES__)
		#include <ncurses.h>
		
		void WAIT_PRESS(void)
		{
			#if !defined(TURN_BASED)
				while(getch()==ERR)
				{}
			#else
				getch();
			#endif
		}
	#elif defined(__SPECTRUM__)
		#if defined(CLIB_ANSI)
			void WAIT_PRESS(void) 
			{ 
				in_WaitForKey();
				in_Inkey();
				in_WaitForNoKey();
			}
		#else
			void WAIT_PRESS(void)
			{ 
				in_wait_key();
				in_wait_nokey();
			}
		#endif
	#elif defined(__CMOC__) && !defined(__WINCMOC__)	
		#include <cmoc.h>
		
		void WAIT_PRESS(void)
		{
			waitkey(0);
		}	
	#else 
		#if defined(CONIO_LIB)
			#include<conio.h>
		#endif
		
		#if defined(Z88DK)
			#define cgetc() getch()
		#endif
		
		void WAIT_PRESS(void)
		{
			while(kbhit())
				cgetc();
			while(!kbhit())
			{ 
			}; 
			cgetc();
		}
	#endif	
#else
	#include<joystick.h>

	void WAIT_PRESS(void)
	{
		while ((joy_read(JOY_1) & JOY_BTN_1_MASK))
		{
		}
		while (! (joy_read(JOY_1) & JOY_BTN_1_MASK))
		{
		}
	}	
#endif

