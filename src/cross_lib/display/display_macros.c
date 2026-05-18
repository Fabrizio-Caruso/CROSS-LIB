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

#ifndef PEEK
    #define PEEK(addr)         (*(uint8_t*)addr)
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
            // SV_VIDEO[base]    = 0xFF;
            // SV_VIDEO[++base]  = 0xFF;
            // SV_VIDEO[++base]  = 0xFF;
            // SV_VIDEO[++base]  = 0xFF;	
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

#if (defined(__VDP_MODE1_GRAPHICS) || defined(__MEMORY_MAPPED_GRAPHICS) || defined(__ORIC_COLOR_GRAPHICS) || defined(__ANTIC_MODE6_GRAPHICS) || defined(__ATARI7800_COLOR_GRAPHICS))
    
	

    #define X_MULT 1
    #define Y_MULT 1

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
        #define LOWER_BORDER_OFFSET 1
    #else
        #define LOWER_BORDER_OFFSET 0
    #endif
    uint8_t video_buffer[YSize+Y_OFFSET][XSize];
    
    #if !defined(__DOUBLE_BUFFER)
    void __display_all(void)
    {
        uint8_t j; 
        uint8_t i;
        
        for(j=0;j<YSize-1+Y_OFFSET-LOWER_BORDER_OFFSET;++j)
        {
            for(i=0;i<XSize;++i)
            {
                putchar(video_buffer[j][i]);
            }
            putchar('\n');
        }
        for(i=0;i<XSize;++i)
        {
            putchar(video_buffer[YSize-1+Y_OFFSET-LOWER_BORDER_OFFSET][i]);
        }
        putchar('\n');
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

        putsprite(spr_or,x*(__SPRITE_X_STEP),y*(__SPRITE_Y_STEP),sprites + ((ch-32U)*(2+SPRITE_Y_SIZE)));
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
            }
        }
        // {
            // uint8_t c;
            // uint16_t i;
            // for(c=0,i=0;i<40*200;i+=40,++c)
            // {
               // POKE(0xA000+i,3+3*(c&1));
            // }
        // }
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


#if defined(__BBC_GRAPHICS)

    #include <stdint.h>
    void osputc(__reg("a") char)="\tjsr\t0xffee";

    void _gotoxy(uint8_t x, uint8_t y)
    {
        osputc(31);
        osputc(x);
        osputc(y);
    }

    void _select_color(uint8_t color)
    {
        osputc(17);
        osputc(color);
    }

    #if !defined(_XL_NO_COLOR)
        void _bbc_draw(uint8_t x, uint8_t y, uint8_t tile, uint8_t color)
        {
            _gotoxy(x,y);
            _select_color(color);
            osputc(tile);
        }
    
    #else
        void _bbc_draw(uint8_t x, uint8_t y, uint8_t tile)
        {
            _gotoxy(x,y);
            osputc(tile);
        }
    
    #endif
    
    void _bbc_delete(uint8_t x, uint8_t y)
    {
        _gotoxy(x,y);
        putchar(' ');
    }
#endif




#if defined(__TERMINAL__)

    #if _XL_TILE_X_SIZE==8
        #define _MAX_BIT_VALUE 128

        #if _XL_TILE_Y_SIZE==8
            #include "8x8_chars.h"

        #elif _XL_TILE_Y_SIZE==6
            #include "8x6_chars.h"
        #endif
    #elif _XL_TILE_X_SIZE==7
        #define _MAX_BIT_VALUE 64

        #include "7x8_chars.h"

    #elif _XL_TILE_X_SIZE==6
        #define _MAX_BIT_VALUE 32

        #if _XL_TILE_Y_SIZE==8
            #include "6x8_chars.h"
            
        #elif _XL_TILE_Y_SIZE==9
            #include "6x9_chars.h"
        #endif
    #endif



    #define _WIDTH 4
    
    
    #define _gotoxy(x,y) do { move(y,x); } while(0)
    #define _cputc(c) do { addch(c);  } while(0)

    #if defined(WIN32)
        #include <ncurses/curses.h>
    #else
        #include <ncurses.h>
    #endif    

    extern uint8_t _tiles[_XL_NUMBER_OF_TILES][_XL_TILE_Y_SIZE];
    
    void _terminal_draw(uint8_t x, uint8_t y, uint8_t tile, uint8_t color)
    {
        uint8_t i;
        uint8_t j;
        uint8_t k;
        uint8_t n;
        

        for(i=0;i<_XL_TILE_Y_SIZE;++i)
        {
            k = _MAX_BIT_VALUE;
            for(j=0;j<_XL_TILE_X_SIZE;++j)
            {
                if(k & _tiles[tile][i])
                {
                    attron(COLOR_PAIR(color));
                }
                else
                {   
                    attron(COLOR_PAIR(_XL_BLACK));
                }
                
                for(n=1;n<=_WIDTH;++n)
                {
                    _gotoxy(_WIDTH*(_XL_TILE_X_SIZE*(x)+j)+n,_XL_TILE_Y_SIZE*(y)+i);
                    _cputc(' ');
                }
                
                k/=2;
            }
        }
        refresh();
    }

    void _terminal_delete(uint8_t x, uint8_t y)
    {
        uint8_t i;
        uint8_t j;
        uint8_t n;
        
        attron(COLOR_PAIR(_XL_BLACK));

        for(i=0;i<_XL_TILE_Y_SIZE;++i)
        {
            for(j=0;j<_XL_TILE_X_SIZE;++j)
            {
                for(n=1;n<=_WIDTH;++n)
                {
                    _gotoxy(_WIDTH*(_XL_TILE_X_SIZE*(x)+j)+n,_XL_TILE_Y_SIZE*(y)+i);
                    _cputc(' ');
                }
                
            }
        }
        refresh();
    }

#endif


#if defined(__DOUBLE_BUFFER) && !defined(__BUFFERED_GRAPHICS)
    #if defined(__CC65__)
        void _XL_REFRESH(void)
            {
                memcpy((uint8_t *)REAL_BASE_ADDR, (uint8_t *)BASE_ADDR,XSize*YSize);
            }
    #else
        void _XL_REFRESH(void)
        {
            uint16_t i;
           
            for(i=0;i<(XSize)*(YSize);++i) \
            {
                POKE(REAL_BASE_ADDR+i,PEEK(BASE_ADDR+i));
            }
        }
    #endif
#endif


#if defined(__ATARI_ANTIC_15)
    #include <stdint.h>

    void _XL_PRINT(uint8_t x, uint8_t y, const char * str)
    {
        set_position(x,y);
        set_fore_color(1);
        output_str(str);
    }
    
    void _XL_CHAR(uint8_t x, uint8_t y, char ch)
    {
        set_position(x,y);
        output_char(ch);
    }
    
void _XL_PRINTD(uint8_t x, uint8_t y, uint8_t length, uint16_t val)
{
	uint8_t i;
	uint8_t digit;

	for(i=0;i<length;++i)
	{
		digit = (uint8_t) ((val)%10);
		val-= digit;
		val/=10;
        set_position(x+length-1-i,y);
        output_char((uint8_t) (digit+(uint8_t) 48u));
	}
}

#endif


#if defined(__BBC__)

    #include <stdint.h>
    
    void _XL_PRINT(uint8_t x, uint8_t y, const char * str)
    {
        uint8_t i = 0;
        
        _gotoxy(X_OFFSET+x,Y_OFFSET+y);
        while(str[i]!='\0')
        {
            osputc(str[i++]);
        }
    }

    void _XL_CHAR(uint8_t x, uint8_t y, char ch)
    {
        _gotoxy(x+X_OFFSET,Y_OFFSET+y);
        osputc(ch);
    }

#include <stdio.h>

void _XL_PRINTD(uint8_t x, uint8_t y, uint8_t length, uint16_t val)
{
	uint8_t i;
	uint8_t digit;

	for(i=0;i<length;++i)
	{
		digit = (uint8_t) ((val)%10);
		val-= digit;
		val/=10;
        _gotoxy(x+length-1-i,y);
        osputc((uint8_t) (digit+(uint8_t) 48u));
	}
}

    #if !defined(_XL_NO_TEXT_COLOR) || !defined(_XL_NO_COLOR)
        #include <stdio.h>
        void _XL_SET_TEXT_COLOR(uint8_t c)
        {
            osputc(17);
            osputc(c);
        }
    
    #endif

#endif

#if defined(__CONIO_PRINT) 

    #if !defined(__VT52)
        #include <conio.h>
    #endif
    
    void _XL_PRINT(uint8_t x, uint8_t y, const char * str)
    {
        gotoxy(X_OFFSET+x,Y_OFFSET+y);
        cprintf(str);
    }

    void _XL_CHAR(uint8_t x, uint8_t y, char ch)
    {
        gotoxy(x+X_OFFSET,Y_OFFSET+y);
        cputc(ch);
    }
#endif


#if defined(__C128__) && defined(__80COL_UDG)

    void vdc_write(uint8_t vdc_register, uint8_t value)
    {
        POKE(ADDRESS_PORT,vdc_register);
        while(!(PEEK(ADDRESS_PORT)&(0x80))){};
        POKE(DATA_PORT,value);
    }


    uint8_t vdc_color_map(uint8_t color)
    {
        if(color == _XL_WHITE)
        {
            return 0x8F;
        }
        else if(color == _XL_RED)
        {
            return 0x88;
        }
        else if(color == _XL_CYAN)
        {
            return 0x87;
        }
        else if(color == _XL_GREEN)
        {
            return 0x84;
        }
        else if(color == _XL_YELLOW) 
        {
            return 0x8D;
        }
        else if(color == _XL_MAGENTA) // too dark?
        {
            return 0x8B;
        }
        else if(color == _XL_BLUE)
        {
            return 0x82;
        }
        return 0x80;
    }

    void vdc_tile_write(uint8_t x, uint8_t y, uint8_t tile, uint8_t color)
    {
        uint16_t address;
        
        address = 0x0800 + y*80U+x;

        vdc_write(HIGH_ADDRESS_REGISTER,(uint8_t)(address>>8));
        vdc_write(LOW_ADDRESS_REGISTER,(uint8_t)(address&0xFF));
        // vdc_write(VDC_DATA_REGISTER,0x80+(color&0xF));
        vdc_write(VDC_DATA_REGISTER,vdc_color_map(color));

        address = y*80U+x;


        vdc_write(HIGH_ADDRESS_REGISTER,(uint8_t)(address>>8));
        vdc_write(LOW_ADDRESS_REGISTER,(uint8_t)(address&0xFF));
        vdc_write(VDC_DATA_REGISTER,tile);
    }

#endif


#if defined(__VIC20__) && defined(__CONIO_GRAPHICS) && defined(__VIC20_EXP_8K)
    #define BASE_ADDR 0x1000
    #define COLOR_ADDR 0x9400
    
    void vic20_tile_write(uint8_t x, uint8_t y, uint8_t tile, uint8_t color)
    {
        uint16_t address;
        
        address = 0x9400+x+y*XSize;
        POKE(address,color);
        
        address = 0x1000+x+y*XSize;
        POKE(address,tile);
        
    }

#endif

