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

#if !defined(NO_WALL)
extern Image HORIZONTAL_BRICK_IMAGE;
extern Image VERTICAL_BRICK_IMAGE;
#endif

#if !defined(NO_SET_SCREEN_COLORS) && !defined(NO_GRAPHICS)
    void setScreenColors(void)
    {
        SET_TEXT_COLOR(TEXT_COLOR);
        SET_BORDER_COLOR(BORDER_COLOR);
        SET_BACKGROUND_COLOR(BACKGROUND_COLOR);
    }
#endif


#if defined(__ATARI__) && (defined(ATARI_MODE1) ) 
extern uint16_t BASE_ADDR;
#endif


#if defined(VDP_WAIT_V_SYNC)
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

#if !defined(INLINE_LOC) && \
    (defined(CREATIVISION_COLOR) || defined(MEMORY_MAPPED) || defined(ORIC_COLOR))
    uint16_t loc(uint8_t x, uint8_t y)
    {
        #if !defined(__CIDELSA__)
            return ((uint16_t) BASE_ADDR)+x+(uint8_t)y*((uint16_t) ((XSize) + X_OFFSET));
        #else
            return ((uint16_t) 0xF800+XSize*YSize-YSize)-x*YSize+(uint8_t)y;
    #endif
    }

#elif defined(Z88DK_SPRITES)

    #include "z88dk_sprites_definitions.h"
    extern uint8_t sprites[];

#elif defined(BUFFERED)
    #if defined(NO_LOWER_BORDER)
        #define LOWER_BORDER_OFFSET 2
    #else
        #define LOWER_BORDER_OFFSET 1
    #endif
    uint8_t video_buffer[YSize+Y_OFFSET][XSize];
    
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
#elif defined(__MO5__)
    #define POKE(addr,val)     (*(uint8_t*) (addr) = (val))

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
    
    

#elif defined(__SUPERVISION__)
    uint8_t reversed_map_one_to_two_lookup[16] = 
    {
        0x00, 0xC0, 0x30, 0xF0, 0x0C, 0xCC, 0x3C, 0xFC,
        0x03, 0xC3, 0x33, 0xF3, 0x0F, 0xCF, 0x3F, 0xFF
    };
    
    uint8_t left_map_one_to_two(uint8_t n)
    {
        return reversed_map_one_to_two_lookup[n >> 4];
    }
    
    uint8_t right_map_one_to_two(uint8_t n)
    {
        return reversed_map_one_to_two_lookup[n&0x0F];
    }

#endif


#ifndef X_OFFSET
  COMPILATION ERROR
#endif
    
#ifndef Y_OFFSET
  COMPILATION ERROR
#endif


#if defined(Z88DK_SPRITES) && defined(ALT_PRINT)
    void _draw_ch(uint8_t x, uint8_t y, uint8_t ch)
    {
        __DELETE(x,y);
        putsprite(spr_or,x*(SPRITE_X_STEP),y*(SPRITE_Y_STEP),sprites + ((ch-32)*(2+SPRITE_Y_SIZE))); \
    }
#endif


#if !defined(NO_BLINKING)
void _blink_draw(uint8_t x, uint8_t y, Image * image, uint8_t *blinkCounter) 
{
    if(*blinkCounter) 
    {
        _draw(x,y,image);
        *blinkCounter=0;
    } 
    else 
    {
        _delete(x,y);
        *blinkCounter=1;
    }    
}
#endif


#if defined(DEFAULT_CLEAR_SCREEN)
    void CLEAR_SCREEN(void)
    {
        uint8_t i;
        uint8_t j;
        
        for(i=0;i<XSize;++i)
        {

        #if !defined(NO_STATS) \
            || X_OFFSET!=0 || Y_OFFSET!=0    
            
            for(j=0;j<YSize+Y_OFFSET;++j)
            {
                _delete_stat(i,j);
            }
        #else
            for(j=0;j<YSize;++j)
            {
                _delete(i,j);
            }
        #endif
        }
    }
    
#endif


#if !defined(NO_WALL)
    void DRAW_HORIZONTAL_LINE(uint8_t x,uint8_t y, uint8_t length) 
    {
        uint8_t i;
        for(i=0;i<length;++i)
        {
            _draw(x+i,y,&HORIZONTAL_BRICK_IMAGE);
        }
    }

    void DRAW_VERTICAL_LINE(uint8_t x,uint8_t y, uint8_t length) 
    {
        uint8_t i;
        for(i=0;i<length;++i)
        {
            _draw(x,y+i,&VERTICAL_BRICK_IMAGE);
        }        
    }
#endif






