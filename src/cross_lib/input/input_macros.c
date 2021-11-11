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

#include "cross_lib.h"

#include "sleep_macros.h"

#include "input_macros.h"

#if defined(Z88DK_JOYSTICK)
	uint8_t stick;
#endif


// GET_CHAR() definitions
#if defined(_XL_NO_JOYSTICK) && !defined(ACK) && !defined(STDLIB)
    #if defined(__COMX__) || defined(__PECOM__) || defined(__TMC600__) || defined(__MICRO__)
        #if !defined(LCC1802_JOYSTICK)
            #include <devkit/input/keyboard.h>
        #endif
    #endif 
    
    char GET_CHAR(void)
    {
    #  if defined(NO_INPUT)
        return 0;
    #elif defined(TURN_BASED)
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
    || defined(__APPLE2__) || defined(__APPLE2ENH__) || defined(__CBM610__) || defined(__C16__) || defined(__CX16__)
        if(kbhit())
            return cgetc();
        else
            return 0;

    // Code by Marcel van Tongeren
    #elif defined(__COMX__) || defined(__PECOM__) || defined(__TMC600__) || defined(__MICRO__)
        return get_stick();
        
    #elif defined(__ATMOS__) || defined(__TELESTRAT__)
        #include <peekpoke.h>    
        
        uint8_t polledValue = PEEK(0x208);

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
    #elif defined(__TO7__)
        
        uint8_t res;
        
        asm
        {
            jsr 0xE806
            stb res
        }
        return res; 
        #define POKE(addr,val)     (*(uint8_t*) (addr) = (val))    
        #define PEEK(addr)         (*(uint8_t*) (addr))    
    
    #elif defined(__MO5__) 
        #define POKE(addr,val)     (*(uint8_t*) (addr) = (val))    
        #define PEEK(addr)         (*(uint8_t*) (addr))    

        #define KEYREG 0xA7C1
        
        POKE(KEYREG,0x18);
        if(!(PEEK(KEYREG)&128))
        {
            return 'I';
        }
        else 
        {
            POKE(KEYREG,0x04);
            if(!(PEEK(KEYREG)&128))
            {
                return 'J';
            }
            else
            {
                POKE(KEYREG,0x14);
                if(!(PEEK(KEYREG)&128))
                {
                    return 'K';
                }
                else
                {
                    POKE(KEYREG,0x24);
                    if(!(PEEK(KEYREG)&128))
                    {
                        return 'L';
                    }
                    else
                    {
                        POKE(KEYREG,0x40);
                        if(!(PEEK(KEYREG)&128))
                        {
                            return ' ';
                        }
                        else
                        {
                            return 0;
                        }
                    }
                }
            }
        }
            
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
    
    #elif defined(__COCO__) || defined(__DRAGON__)
        #include <cmoc.h>
        #include <coco.h>
        
        uint8_t res;
        uint8_t machine;
        
        asm {
            ldd $8000
            cmpd #$7EBB
            beq _dragon
            lda #253
            sta machine
            bra pia
_dragon        lda #247
            sta machine
pia            lda #253        
            sta $FF02
            ldb #73
test        lda $ff00
            cmpa machine
            beq out
            incb
            rol $ff02
            inc $ff02
            cmpb #77
            bne test
            clrb 
out            stb res
        }
        
        if(res == 0)
            return inkey();
        return res;
        
    #elif defined(__SRR__)
        return getk_inkey();     
    #elif defined(__TI99__)
        return cgetc();     
    #else
        return getk();
    #endif
    }
#endif    



// _XL_WAIT_FOR_INPUT() definitions
#  if defined(NO_WAIT) && !defined(NO_SLEEP)
    void _XL_WAIT_FOR_INPUT(void)
    {
        _XL_SLEEP(2);
    }
#elif defined(NO_WAIT) 
//
#elif defined(WAIT_FOR_KEY)
    #  if defined(__GCC_BUFFERED__) || defined(STDLIB)
        void _XL_WAIT_FOR_INPUT(void)
        {
            getchar();
        }
    #elif defined(__NCURSES__)
        #if defined(__ATARI_ST__)
            #include <ncurses/curses.h>
        #else
            #include <ncurses.h>
        #endif
        
        void _XL_WAIT_FOR_INPUT(void)
        {
            #if !defined(TURN_BASED)
                while(getch()==ERR)
                {}
            #else
                getch();
            #endif
        }
    #elif defined(__COCO__) || defined(__DRAGON__)
        #include <cmoc.h>
        
        void _XL_WAIT_FOR_INPUT(void)
        {
            waitkey(0);
        }
    #elif defined(NO_INPUT)
    //
    #elif defined(__MO5__) || defined(__TO7__)
        void _XL_WAIT_FOR_INPUT(void)
        {
            while(GET_CHAR())
            {
            };
            while(!GET_CHAR())
            {
            }
        }
    #else 
        #if defined(CONIO_LIB)
            #include<conio.h>
        #endif

        #if defined(Z88DK)
            #undef cgetc
            #define cgetc() getch()
        #endif 

        void _XL_WAIT_FOR_INPUT(void)
        {
            while(kbhit())
                (void) cgetc();
            while(!kbhit())
            { 
            }; 
            (void) cgetc();
        }
    #endif    
#else
    #if defined(Z88DK_JOYSTICK)
        #include <games.h>
        
        extern uint8_t stick;
        
        void _XL_WAIT_FOR_INPUT(void)
        {
            while ((joystick(stick) & MOVE_FIRE))
            {
            }
            while (!(joystick(stick) & MOVE_FIRE))
            {
            }
        }    
    #elif defined(__SMS__)
        #include <arch/sms/SMSlib.h>
                
        void _XL_WAIT_FOR_INPUT(void)
        {
            while ((SMS_getKeysStatus() | PORT_A_KEY_1))
            {
            }
            while (!(SMS_getKeysStatus() | PORT_A_KEY_1))
            {
            }
        }        
    #elif defined(__LCC1802__)
        #include <devkit/input/joystick.h>
        
        void _XL_WAIT_FOR_INPUT(void)
        {
            // Flush the video buffer to make sure that the latest character is displayed
            vidflush();
            
            while(!get_stick(0))
            {
            }
        }
    #else
        #include<joystick.h>
        
        void _XL_WAIT_FOR_INPUT(void)
        {
            while ((joy_read(STANDARD_JOY) & JOY_BTN_1_MASK))
            {
            }
            while (! (joy_read(STANDARD_JOY) & JOY_BTN_1_MASK))
            {
            }
        }    
    #endif
#endif

