/* --------------------------------------------------------------------------------------- */ 
// 
// CROSS CHASE by Fabrizio Caruso
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

#ifndef POKE
    #define POKE(addr,val)     (*(uint8_t*) (addr) = (val))
#endif

#if defined(__ATARI__) && (defined(__ANTIC_MODE6_GRAPHICS) ) 
extern uint16_t BASE_ADDR;
#endif

#if defined(__AQUARIUS__) && defined(__MEMORY_MAPPED_GRAPHICS)
void _XL_SET_TEXT_COLOR(uint8_t c)
{
    if(c==_XL_WHITE)
    {
        textcolor(WHITE);
    }
    else if(c==_XL_RED)
    {
        textcolor(RED);
    }
    else if(c==_XL_YELLOW)
    {
        textcolor(YELLOW);
    }
    else if(c==_XL_GREEN)
    {
        textcolor(GREEN);
    }
    else if(c==_XL_CYAN)
    {
        textcolor(CYAN);
    }
    else if(c==_XL_BLUE)
    {
        textcolor(BLUE);
    }
}
#endif

// #if defined(__SUPERVISION__) || (defined(__COCO__) && defined(__BIT_MAPPED_4_GRAPHICS)) || (defined(__MC10__) && defined(__BIT_MAPPED_4_GRAPHICS))
#if defined(__BIT_MAPPED_4_GRAPHICS)
    extern uint8_t udgs[];

    uint8_t _bitmap4_text_color;

    void _color_draw(uint8_t x, uint8_t y, uint8_t tile, uint8_t color)
    {
        uint8_t k;
        uint16_t offset = (_XL_TILE_Y_SIZE*(uint16_t)(tile)); // uint8_t does not work on CoCo and Dragon but it does work on Supervision
        
        uint16_t base = 2*x+BYTES_PER_LINE*_XL_TILE_Y_SIZE*(y);
		uint8_t udg;
        
        for(k=0;k<_XL_TILE_Y_SIZE;++k)
        {
			udg=udgs[offset++];
            SV_VIDEO[base]    = left_map_one_to_two(udg)&color;
            SV_VIDEO[++base]  = right_map_one_to_two(udg)&color;
            base+=BYTES_PER_LINE-1;
        }
    }

    void _color_delete(uint8_t x, uint8_t y)
    {
        uint8_t k;
        uint16_t base = 2*x+BYTES_PER_LINE*_XL_TILE_Y_SIZE*(y);
        // uint16_t delta = 0;
        
        for(k=0;k<_XL_TILE_Y_SIZE;++k)
        {

            // SV_VIDEO[base]=0;
            // SV_VIDEO[base+1]=0;
            // base+=BYTES_PER_LINE;
			SV_VIDEO[base]=0;
            SV_VIDEO[++base]=0;
			base+=BYTES_PER_LINE-1;
            // base+=BYTES_PER_LINE;
        }
    }
#elif defined(__BIT_MAPPED_16_GRAPHICS)

    extern uint8_t udgs[];

    uint8_t _bitmap16_text_color;

    void _color_draw(uint8_t x, uint8_t y, uint8_t tile, uint8_t color)
    {
        uint8_t k;
        uint16_t offset = (_XL_TILE_Y_SIZE*(uint16_t)(tile)); // uint8_t does not work on CoCo and Dragon but it does work on Supervision
        
        uint16_t base = 4*x+BYTES_PER_LINE*_XL_TILE_Y_SIZE*(y);
        // uint16_t delta = 0;
		uint8_t udg;
        
        for(k=0;k<_XL_TILE_Y_SIZE;++k)
        {
			udg = udgs[offset++];
            SV_VIDEO[base]    = first_map_one_to_four(udg)&color;
            SV_VIDEO[++base]  = second_map_one_to_four(udg)&color;
            SV_VIDEO[++base]  = third_map_one_to_four(udg)&color;
            SV_VIDEO[++base]  = fourth_map_one_to_four(udg)&color;
            // SV_VIDEO[base+delta]    = 0xFF;
            // SV_VIDEO[base+delta+1]  = 0xFF;
            // SV_VIDEO[base+delta+2]  = 0xFF;
            // SV_VIDEO[base+delta+3]  = 0xFF;			
            base+=BYTES_PER_LINE-3;
        }
    }

    void _color_delete(uint8_t x, uint8_t y)
    {
        uint8_t k;
        uint16_t base = 4*x+BYTES_PER_LINE*_XL_TILE_Y_SIZE*(y);
		
        for(k=0;k<_XL_TILE_Y_SIZE;++k)
        {

            SV_VIDEO[base]=0;
            SV_VIDEO[++base]=0;
			SV_VIDEO[++base]=0;
            SV_VIDEO[++base]=0;
            base+=BYTES_PER_LINE-3;
        }
    }
#endif 


#if defined(__QUAD_MEMORY_MAPPED_GRAPHICS)
// TODO: MOVE THIS TO GRAPHICS INIT?
    uint8_t quad_tile[1+27+10+26][4] = {
        {      ' ',      ' ',      ' ',      ' '},     // 32 [SPACE]
        
        {_TILE_0_0,_TILE_0_1,_TILE_0_2,_TILE_0_3},     // 33
        {_TILE_1_0,_TILE_1_1,_TILE_1_2,_TILE_1_3},     // 34
        {_TILE_2_0,_TILE_2_1,_TILE_2_2,_TILE_2_3},     // 35
        {_TILE_3_0,_TILE_3_1,_TILE_3_2,_TILE_3_3},     // 36
        {_TILE_4_0,_TILE_4_1,_TILE_4_2,_TILE_4_3},     // 37
        {_TILE_5_0,_TILE_5_1,_TILE_5_2,_TILE_5_3},     // 38
        {_TILE_6_0,_TILE_6_1,_TILE_6_2,_TILE_6_3},     // 39
        {_TILE_7_0,_TILE_7_1,_TILE_7_2,_TILE_7_3},     // 40
        {_TILE_8_0,_TILE_8_1,_TILE_8_2,_TILE_8_3},     // 41
        {_TILE_9_0,_TILE_9_1,_TILE_9_2,_TILE_9_3},     // 42
        {_TILE_10_0,_TILE_10_1,_TILE_10_2,_TILE_10_3}, // 43
        {_TILE_11_0,_TILE_11_1,_TILE_11_2,_TILE_11_3}, // 44
        {_TILE_12_0,_TILE_12_1,_TILE_12_2,_TILE_12_3}, // 45
        {_TILE_13_0,_TILE_13_1,_TILE_13_2,_TILE_13_3}, // 48
        {_TILE_14_0,_TILE_14_1,_TILE_14_2,_TILE_14_3}, // 47

        {      '0',      ' ',      ' ',      ' '},     // 48 [0]
        {      '1',      ' ',      ' ',      ' '},     // 49
        {      '2',      ' ',      ' ',      ' '},     // 50
        {      '3',      ' ',      ' ',      ' '},     // 51
        {      '4',      ' ',      ' ',      ' '},     // 52
        {      '5',      ' ',      ' ',      ' '},     // 53
        {      '6',      ' ',      ' ',      ' '},     // 54
        {      '7',      ' ',      ' ',      ' '},     // 55
        {      '8',      ' ',      ' ',      ' '},     // 56
        {      '9',      ' ',      ' ',      ' '},     // 57 [9]        
        
        {_TILE_15_0,_TILE_15_1,_TILE_15_2,_TILE_15_3}, // 58
        {_TILE_16_0,_TILE_16_1,_TILE_16_2,_TILE_16_3}, // 59
        {_TILE_17_0,_TILE_17_1,_TILE_17_2,_TILE_17_3}, // 60
        {_TILE_18_0,_TILE_18_1,_TILE_18_2,_TILE_18_3}, // 61
        {_TILE_19_0,_TILE_19_1,_TILE_19_2,_TILE_19_3}, // 62
        {_TILE_20_0,_TILE_20_1,_TILE_20_2,_TILE_20_3}, // 63
        {_TILE_21_0,_TILE_21_1,_TILE_21_2,_TILE_21_3}, // 64
                
        {      'A',      ' ',      ' ',      ' '},     // 65 [A]
        {      'B',      ' ',      ' ',      ' '},     // 66
        {      'C',      ' ',      ' ',      ' '},     // 67
        {      'D',      ' ',      ' ',      ' '},     // 68
        {      'E',      ' ',      ' ',      ' '},     // 69
        {      'F',      ' ',      ' ',      ' '},     // 70
        {      'G',      ' ',      ' ',      ' '},     // 71
        {      'H',      ' ',      ' ',      ' '},     // 72
        {      'I',      ' ',      ' ',      ' '},     // 73
        {      'J',      ' ',      ' ',      ' '},     // 74
        {      'K',      ' ',      ' ',      ' '},     // 75
        {      'L',      ' ',      ' ',      ' '},     // 76
        {      'M',      ' ',      ' ',      ' '},     // 77
        {      'N',      ' ',      ' ',      ' '},     // 78
        {      'O',      ' ',      ' ',      ' '},     // 79
        {      'P',      ' ',      ' ',      ' '},     // 80
        {      'Q',      ' ',      ' ',      ' '},     // 81
        {      'R',      ' ',      ' ',      ' '},     // 82
        {      'S',      ' ',      ' ',      ' '},     // 83
        {      'T',      ' ',      ' ',      ' '},     // 84
        {      'U',      ' ',      ' ',      ' '},     // 85
        {      'V',      ' ',      ' ',      ' '},     // 86
        {      'W',      ' ',      ' ',      ' '},     // 87
        {      'X',      ' ',      ' ',      ' '},     // 88
        {      'Y',      ' ',      ' ',      ' '},     // 89
        {      'Z',      ' ',      ' ',      ' '},     // 90 [Z]

        {_TILE_22_0,_TILE_22_1,_TILE_22_2,_TILE_22_3}, // 91
        {_TILE_23_0,_TILE_23_1,_TILE_23_2,_TILE_23_3}, // 92
        {_TILE_24_0,_TILE_24_1,_TILE_24_2,_TILE_24_3}, // 93
        {_TILE_25_0,_TILE_25_1,_TILE_25_2,_TILE_25_3}, // 94
        {_TILE_26_0,_TILE_26_1,_TILE_26_2,_TILE_26_3}, // 95
    };   
// -------------------------

#endif

#if defined(__MC10__)
    void mc10_display_poke(uint16_t addr, uint8_t val)
    {
        #if !defined(__REVERSE_LETTERS)
        if((val>='A')&&(val<='Z')) // TODO: Change this to allow negative letters
        {
            val = val-64;
        }
        #endif
        POKE(addr,val);
    }
#endif


#if defined(__VDP_WAIT_V_SYNC)
    void vdp_waitvsync(void)
    { 
        #asm 
            ld hl,_tick_count 
            ld a,(hl) 
        loop: 
            cp (hl) 
            jr nz,loop 
        #endasm 
        return;
    }
#endif

#if (defined(__VDP_MODE1_GRAPHICS) || defined(__MEMORY_MAPPED_GRAPHICS) || defined(__QUAD_MEMORY_MAPPED_GRAPHICS) || defined(__ORIC_COLOR_GRAPHICS) || defined(__ANTIC_MODE6_GRAPHICS) || defined(__ATARI7800_COLOR_GRAPHICS))
    
	
    #if defined(__QUAD_MEMORY_MAPPED_GRAPHICS)
        #define X_MULT 2
        #define Y_MULT 2
    #else
        #define X_MULT 1
        #define Y_MULT 1
    #endif

    uint16_t loc(uint8_t x, uint8_t y)
    {
        #if !defined(__CIDELSA__)
            #if defined(__CREATIVISION__)
                #if !defined(_CREAT_XSIZE)
                    #define _CREAT_XSIZE XSize
                #endif
                return ((uint16_t) BASE_ADDR)+(x+X_OFFSET)+(uint8_t)(Y_OFFSET+y)*((uint16_t) (_CREAT_XSIZE));
            #else
                return ((uint16_t) BASE_ADDR)+(X_MULT)*x+(uint8_t)((Y_MULT)*y)*((uint16_t) ((XSize) + X_OFFSET));
            #endif
        #else
            return ((uint16_t) 0xF800+XSize*YSize-YSize)-x*YSize+(uint8_t)y;
    #endif
    }

#elif defined(__Z88DK_SPRITES_GRAPHICS)

	#if SPRITE_Y_SIZE==8
		#include "z88dk_sprites_definitions.h"
	#elif SPRITE_Y_SIZE==6
		#include "z88dk_8x6_sprites_definitions.h"
	#endif
    extern uint8_t sprites[];

#elif defined(__BUFFERED_GRAPHICS)
    #if defined(_XL_NO_LOWER_BORDER)
        #define LOWER_BORDER_OFFSET 2
    #else
        #define LOWER_BORDER_OFFSET 1
    #endif
    uint8_t video_buffer[YSize+Y_OFFSET][XSize];
    
    #if !defined(__DOUBLE_BUFFER)
    void display_all(void)
    {
        uint8_t j; 
        uint8_t i;
        
        for(j=0;j<YSize+Y_OFFSET-LOWER_BORDER_OFFSET;++j)
        {
            for(i=0;i<XSize;++i)
            {
                putchar(video_buffer[j][i]);
            }
            putchar('\n');
        }
        for(i=0;i<XSize;++i)
        {
            putchar(video_buffer[YSize+Y_OFFSET-LOWER_BORDER_OFFSET][i]);
        }
    }
    #endif
#elif defined(__MO5__)
/*
COLOR:
lda $a7c0
 anda #$f0
 sta $a7c0


FORMS:
lda $a7c0
 ora #$01
 sta $a7c0
*/


    void SWITCH_COLOR_BANK_ON(void)
    {
        asm
        {
            swi
            .byte 4
        }    
    }
    
    void SWITCH_COLOR_BANK_OFF(void)
    {
        asm
        {
            swi
            .byte 6
        }    
    }    
    
    void PUTCH(unsigned char ch)
    {
        asm
        {
            ldb ch
            swi
            .byte 2
        }
    }
    

    void gotoxy(uint8_t x, uint8_t y)
    {
        PUTCH(0x1F);
        PUTCH(0x40+y);
        PUTCH(0x41+x);
    }    
    
#elif defined(__TO7__)
    #define POKE(addr,val)     (*(uint8_t*) (addr) = (val))

    void SWITCH_COLOR_BANK_ON(void)
    {
        asm
        {
            LDA $E7C3
            ANDA #254
            STA $E7C3	
        }  
    }
    
    void SWITCH_COLOR_BANK_OFF(void)
    {
        asm
        {
            LDA $E7C3
            ORA #1
            STA $E7C3
        }    
    }    

    void PUTCH(unsigned char ch)
    {
        asm
        {
            ldb ch
            jsr    0xE803
        }
    }
    
    void gotoxy(uint8_t x, uint8_t y)
    {
        PUTCH(0x1F);
        PUTCH(0x40+y);
        PUTCH(0x41+x);
    }    
    
#elif defined(__BIT_MAPPED_4_GRAPHICS)
    const uint8_t map_one_to_two_lookup[16] = 
    {  
    #if defined(__COCO__) || defined(__DRAGON__) || defined(__MC10__)
        0x00, 0x03, 0x0C, 0x0F, 0x30, 0x33, 0x3C, 0x3F,
        0xC0, 0xC3, 0xCC, 0xCF, 0xF0, 0xF3, 0xFC, 0xFF
    #elif defined(__SUPERVISION__)
        0x00, 0xC0, 0x30, 0xF0, 0x0C, 0xCC, 0x3C, 0xFC,
        0x03, 0xC3, 0x33, 0xF3, 0x0F, 0xCF, 0x3F, 0xFF
    #endif
    }; 
        
    uint8_t left_map_one_to_two(uint8_t n)
    {
        return map_one_to_two_lookup[n >> 4];
    }
    
    uint8_t right_map_one_to_two(uint8_t n)
    {
        return map_one_to_two_lookup[n&0x0F];
    }
#elif defined(__BIT_MAPPED_16_GRAPHICS)
    const uint8_t map_one_to_four_lookup[4] = 
    {  
        // 0x00000000, 
		// 0b00001111, 
		// 0b11110000, 
		// 0b11111111,
		0x00,
		0x0F, 
		0xF0, 
		0xFF,
    }; 
        
    uint8_t first_map_one_to_four(uint8_t n)
    {
        return map_one_to_four_lookup[n >> 6];
    }
    
    uint8_t second_map_one_to_four(uint8_t n)
    {
        return map_one_to_four_lookup[(n >> 4)&0x03];
    }

    uint8_t third_map_one_to_four(uint8_t n)
    {
        return map_one_to_four_lookup[(n >> 2)&0x03];
    }
    
    uint8_t fourth_map_one_to_four(uint8_t n)
    {
        return map_one_to_four_lookup[n&0x03];
    }
#endif


#ifndef X_OFFSET
  COMPILATION ERROR
#endif
    
#ifndef Y_OFFSET
  COMPILATION ERROR
#endif

#if defined(__MO5__) || defined(__TO7__)

    #define _THOMSON_BLACK 64
    #define _THOMSON_RED 65
    #define _THOMSON_GREEN 66
    #define _THOMSON_YELLOW 67
    #define _THOMSON_BLUE 68
    #define _THOMSON_MAGENTA 69
    #define _THOMSON_CYAN 70
    #define _THOMSON_WHITE 71
    #define _THOMSON_GREY 72

    void _XL_SET_TEXT_COLOR(uint8_t c)
    {
        PUTCH(0x1B);
        if(c==_XL_WHITE)
        {
            PUTCH(_THOMSON_WHITE);
        }
        else if(c==_XL_RED)
        {
            PUTCH(_THOMSON_RED);
        }
        else if(c==_XL_GREEN)
        {
            PUTCH(_THOMSON_GREEN);
        }
        else if (c==_XL_YELLOW)
        {
            PUTCH(_THOMSON_YELLOW);
        }
        else if(c==_XL_BLUE)
        {
            PUTCH(_THOMSON_BLUE);
        }
        else if(c==_XL_CYAN)
        {
            PUTCH(_THOMSON_CYAN);
        }
        else if(c==_XL_WHITE)
        {
            PUTCH(_THOMSON_WHITE);
        }
        else
        {
            PUTCH(_THOMSON_BLACK);
        }
    }
#endif


#if defined(__Z88DK_SPRITES_GRAPHICS) && defined(__ALT_PRINT)
    void __draw_ch(uint8_t x, uint8_t y, uint8_t ch)
    {
        _XL_DELETE(x,y); 
        // if(ch)
        // {
            putsprite(spr_or,x*(__SPRITE_X_STEP),y*(__SPRITE_Y_STEP),sprites + ((ch-32U)*(2+SPRITE_Y_SIZE)));
        // }
    }
#endif



#if defined(__DEFAULT_CLEAR_SCREEN)
    void _XL_CLEAR_SCREEN(void)
    {
        uint8_t i;
        uint8_t j;
        
        for(i=0;i<XSize;++i)
        {

            for(j=0;j<YSize+Y_OFFSET;++j)
            {
                _XL_DELETE(i,j);
				// _XL_SLEEP(1);
            }
        }
    }
    
#endif


#if defined(__MEMORY_MAPPED_GRAPHICS)
    #if !defined(_XL_NO_COLOR)
        void _XL_DRAW(uint8_t x, uint8_t y, uint8_t tile, uint8_t color)
        {
            COLOR_POKE((uint16_t) ((uint16_t) (COLOR_ADDR+(x)) +(uint16_t)(y)*(XSize)),(color));
            DISPLAY_POKE(loc(x,y), (tile));
        }
    #else
        void __DRAW_NO_COLOR(uint8_t x, uint8_t y, uint8_t tile)
        {
            DISPLAY_POKE(loc(x,y), (tile));
        }   
    #endif
	
	void _XL_DELETE(uint8_t x, uint8_t y)
	{
		DISPLAY_POKE(loc(x,y), _SPACE);
	}
#endif

#if defined(__DOUBLE_BUFFER) && !defined(__BUFFERED_GRAPHICS)
    #if defined(__CC65__)
        void _XL_REFRESH(void)
            {
                memcpy((uint8_t *)REAL_BASE_ADDR, (uint8_t *)BASE_ADDR,XSize*YSize);
                // memcpy((uint8_t *)REAL_COLOR_ADDR, (uint8_t *)COLOR_ADDR,XSize*YSize);
            }
    #else
        void _XL_REFRESH(void)
        {
            uint16_t i;
           
            for(i=0;i<(XSize)*(YSize);++i) \
            {
                POKE(REAL_BASE_ADDR+i,PEEK(BASE_ADDR+i));
                // POKE(REAL_COLOR_ADDR+i,PEEK(COLOR_ADDR+i));
            }
        }
    #endif
#endif
