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

#if defined(__Z88DK__)
	uint8_t stick;
#endif

#ifndef POKE
#define POKE(addr,val)     (*(uint8_t*) (addr) = (val))    
#endif

#ifndef PEEK
#define PEEK(addr)         (*(uint8_t*) (addr))    
#endif

#if defined(__TI99__) && !defined(_XL_NO_JOYSTICK)

    #include "kscan.h"

    uint8_t __joystfast(int unit) {
    unsigned int result;

    // read the joystick lines (column 6 or 7, (5 added to unit))
    __asm__("li r12,>0024\n\tai %1,5\n\tswpb %1\n\tldcr %1,3\n\tsrc r12,7\n\tli r12,>0006\n\tclr %0\n\tstcr %0,8" : "=r"(result) : "r"(unit) : "r12");

    KSCAN_JOYY = 0;
    KSCAN_JOYX = 0;

    if ((result & 0x0200) == 0) 
    {
        KSCAN_JOYX = JOY_LEFT;
        return JOY_LEFT;
    }
    if ((result & 0x0400) == 0) 
    {
        KSCAN_JOYX = JOY_RIGHT;
        return JOY_RIGHT;
    }
    if ((result & 0x0800) == 0) 
    {
        KSCAN_JOYY = JOY_DOWN;
        return JOY_DOWN;
    }
    if ((result & 0x1000) == 0) 
    {
        KSCAN_JOYY = JOY_UP;
        return JOY_UP;
    }
    kscan(1);
    if(result==18)
    {
        return 18;
    }
    }
    
    uint8_t JOY_INPUT(void)
    {
        return __joystfast(1);
    }
#endif


#if defined(_XL_NO_JOYSTICK) && !defined(ACK) && !defined(__STDIO) && !defined(__NO_INPUT)
    #if defined(__COMX__) || defined(__PECOM__) || defined(__TMC600__) || defined(__MICRO__)
		#include <devkit/input/keyboard.h>
    #endif 
        
    char GET_CHAR(void)
    {
    // #  if defined(__NO_PRINT)
        // return 0;
    #if defined(_XL_TURN_BASED)
        return _XL_TURN_BASED_INPUT();
    
    // #elif defined(__MSX__)
        // if(!get_trigger(0)) 
        // {
            // return get_stick(0);
        // } 
        // else 
        // {
            // return 9;
        // }
    

    #elif defined(__MC10__)
    
        POKE(2u,0xFFu-0x10u);
        if(!(PEEK((volatile uint16_t) 49151u)&2))
        {
            return 'L';
        }
        POKE(2u,0xFFu-0x08u);
        if(!(PEEK((volatile uint16_t)49151u)&2))
        {
            return 'K';
        }
        POKE(2u,0xFFu-0x04u);
        if(!(PEEK((volatile uint16_t)49151u)&2))
        {
            return 'J';
        }
        POKE(2u,0xFFu-0x2u);
        if(!(PEEK((volatile uint16_t)49151u)&2))
        {
            return 'I';
        }
        POKE(2u,0xFFu-0x80u);
        if(!(PEEK((volatile uint16_t)49151u)&8))
        {
            return ' ';
        }
        return 0;

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
        #define PEEK(addr)         (*(uint8_t*) (addr))    
    
    #elif defined(__MO5__) 

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

    #elif defined(__NCURSES__) || defined(__TERMINAL__)

        #if defined(__TERMINAL__)
            #include <ncurses.h>
        #endif
        
        #define INPUT_LOOPS 10
        
        unsigned long delay = 0;
        volatile char _ch;
        volatile char ch = 0;

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

    #elif defined(__COCO3__) 
        #include <cmoc.h>
        #include <coco.h>
        
        uint8_t res;
        uint8_t machine;
        
        asm {
            lda #253
            sta machine
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
out         stb res
        }
        
        #define _COCO_SPACE_BIT_MASK 0x08
                
        POKE(0xFF02,0x7F);
        if(!(PEEK(0xFF00)&_COCO_SPACE_BIT_MASK))
        {
            return ' ';
        }

        return res;
        
    
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
_dragon     lda #247
            sta machine
pia         lda #253        
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
out         stb res
        }
        
        #define _COCO_SPACE_BIT_MASK 0x08
        
        #define _DRAGON_SPACE_BIT_MASK 0x20
        
        POKE(0xFF02,0x7F);
        if(!(PEEK(0xFF00)&_COCO_SPACE_BIT_MASK) || !(PEEK(0xFF00)&_DRAGON_SPACE_BIT_MASK))
        {
            return ' ';
        }

        return res;
        
    #elif defined(__SRR__)
        return getk_inkey();     
    #elif defined(__TI99__)
        
        POKE(0x83C8,0);
        POKE(0x83CA,0);
        if(kbhit())
            return cgetc();
        else
            return 0;    
        
    #elif defined(__BBC__)
        #define OSSCANKEY(x) OSBYTE1(121,(x))
        
        char OSBYTE1(__reg("a") char,__reg("r0") char)="\tldx\tr0\n\tjsr\t$fff4\n\ttxa";
        
        return OSSCANKEY('')+1;
        
    #else
        return getk();
    #endif
    }
#endif    



// _XL_WAIT_FOR_INPUT() definitions
#if !defined(__NO_INPUT)
    #  if defined(__NO_WAIT) && !defined(_XL_NO_SLEEP)
        void _XL_WAIT_FOR_INPUT(void)
        {
            _XL_SLEEP(2);
        }
    #elif defined(__NO_WAIT) 
    //
    #elif defined(WAIT_FOR_KEY)
        #  if defined(__STDIO)
            void _XL_WAIT_FOR_INPUT(void)
            {
                getchar();
            }
        #elif defined(__MC10__)
            void _XL_WAIT_FOR_INPUT(void)
            {
                getchar();
            }
        #elif defined(__NCURSES__) || defined(__TERMINAL__)
            // #if defined(__ATARI_ST__)
                // #include <ncurses/curses.h>
            // #else
                #include <ncurses.h>
            // #endif
            
            void _XL_WAIT_FOR_INPUT(void)
            {
                #if !defined(_XL_TURN_BASED)
                    while(getch()==ERR)
                    {}
                #else
                    getch();
                #endif
            }
            
        #elif  defined(__COCO3__)
            void _XL_WAIT_FOR_INPUT(void)
            {
                do
                {
                    
                } while(!_XL_INPUT());
            }
        #elif defined(__COCO__) || defined(__DRAGON__)
            #include <cmoc.h>
            
            void _XL_WAIT_FOR_INPUT(void)
            {
                waitkey(0);
                // isKeyPressed();
            }
        #elif defined(__NO_INPUT)
        //
        #elif defined(__BBC__)
            void _XL_WAIT_FOR_INPUT(void)
            {
                while(!_XL_KEY_PRESSED())
                {
                }
            }
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
        #elif defined(__TERMINAL__)
            #include <ncurses.h>
            void _XL_WAIT_FOR_INPUT(void)
            {
                // TODO: Implement this
            }
        #else 
            #if defined(__INCLUDE_CONIO_H)
                #include<conio.h>
            #endif

            #if defined(__Z88DK__)
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
        #if defined(__Z88DK__)
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
        #elif defined(__LCC1802__)
            #include <devkit/input/joystick.h>
            
            void _XL_WAIT_FOR_INPUT(void)
            {
                // Flush the video buffer to make sure that the latest character is displayed
                #if !defined(__LCC1802_UNBUFFERED)
                VIDFLUSH();
                #endif
                
                while(!get_stick(0))
                {
                }
            }
        #elif defined(__TI99__)
            // TODO: Implement this
            #include "kscan.h"
            
            void _XL_WAIT_FOR_INPUT(void)
            {
                do
                {
                    kscan(1);
                } while(KSCAN_KEY!=18);
                
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
#endif

