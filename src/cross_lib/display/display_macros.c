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
#include "images.h"


#if !defined(NO_SET_SCREEN_COLORS)
    void setScreenColors(void)
    {
        SET_TEXT_COLOR(TEXT_COLOR);
        SET_BORDER_COLOR(BORDER_COLOR);
        SET_BACKGROUND_COLOR(BACKGROUND_COLOR);
    }
#endif


#include "graphics_settings.h"


#if defined(MEMORY_MAPPED)        
    uint16_t loc(uint8_t x, uint8_t y)
    {
        return ((uint16_t) BASE_ADDR)+x+(uint8_t)y*((uint16_t) (XSize + X_OFFSET));
    }
#elif defined(REX)
    char char_list[UDG_N*2] = 
    { 
    '*', '\0', // PLAYER
    'O', '\0', // GHOST
    'X', '\0', // BOMB
    '+', '\0', // SKULL
    '.', '\0', // BULLET
    'P', '\0', // POWERUP
    '!', '\0', // GUN
    '$', '\0', // EXTRA_POINTS_IMAGE
    'V', '\0', // INVINCIBILITY
    '*', '\0', // EXTRA LIFE
    '>', '\0', // LEFT_HORIZONTAL_MISSILE
    '<', '\0', // RIGHT_HORIZONTAL_MISSILE
    '^', '\0', // ROCKET
    '*', '\0', // PLAYER_DOWN
    '*', '\0', // PLAYER_UP
    '*', '\0', // PLAYER_RIGHT
    '*', '\0', // PLAYER_LEFT    
    '|', '\0', // VERTICAL_BRICK
    '-', '\0' // HORIZONTAL_BRICK 
    };

    char space_str[2] = {' ', '\0'};
#elif defined(CPCRSLIB)
    char char_list[UDG_N*2] = 
    { 
    33, '\0', // PLAYER
    37, '\0', // GHOST
    44, '\0', // BOMB
    40, '\0', // SKULL
    43, '\0', // BULLET
    42, '\0', // POWERUP
    41, '\0', // GUN
    49, '\0', // EXTRA_POINTS_IMAGE
    46, '\0', // INVINCIBILITY
    33, '\0', // EXTRA LIFE
    39, '\0', // LEFT_HORIZONTAL_MISSILE
    38, '\0', // RIGHT_HORIZONTAL_MISSILE
    45, '\0', // ROCKET
    33, '\0', // PLAYER_DOWN
    34, '\0', // PLAYER_UP
    35, '\0', // PLAYER_RIGHT
    36, '\0', // PLAYER_LEFT
    47, '\0', // VERTICAL_BRICK
    48, '\0' // HORIZONTAL_BRICK 
    };

    char space_str[2] = {' ', '\0'};

#elif defined(Z88DK_SPRITES)

    extern uint8_t sprites[];

#elif defined(BUFFERED)
    uint8_t video_buffer[YSize+Y_OFFSET][XSize];
    
    void display_all(void)
    {
        uint8_t j; 
        uint8_t i;
        
        for(j=0;j<YSize+Y_OFFSET;++j)
        {
            for(i=0;i<XSize;++i)
            {
                putchar(video_buffer[j][i]);
            }
            putchar('\n');
        }
    }
#elif defined(__MO5__)
    #define POKE(addr,val)     (*(uint8_t*) (addr) = (val))

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
    
    void PUTCH(char ch)
    {
        POKE(0x201B,0);
        asm
        {
            ldb ch
            swi
            .byte 2
        }
    }
    

    void gotoxy(uint8_t xx, uint8_t yy)
    {
        PUTCH(0x1F);
        PUTCH(0x40+yy);
        PUTCH(0x41+xx);
    }    
    
#elif defined(__TO7__)
    void PUTCH(char ch)
    {
        asm
        {
            ldb ch
            jsr    0xE803
        }
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
        putsprite(spr_or,x*(SPRITE_X_STEP),y*(SPRITE_Y_STEP),sprites + (uint8_t *) ((ch-32)*(2+SPRITE_Y_SIZE))); \
    }
#endif

#if !defined(NO_STATS) \
    || X_OFFSET!=0 || Y_OFFSET!=0
    
    void _draw_stat(uint8_t x, uint8_t y, Image * image) 
    {
        __DRAW((X_OFFSET+x),(y),image);
    }

    void _draw(uint8_t x, uint8_t y, Image * image) 
    {
        _draw_stat(x,Y_OFFSET+y,image);
    }
#else
    void _draw(uint8_t x, uint8_t y, Image * image) 
    {
        __DRAW(x,y,image);
    }    
#endif

#if !defined(NO_STATS) \
    || X_OFFSET!=0 || Y_OFFSET!=0
    void _delete_stat(uint8_t x, uint8_t y)
    {
        __DELETE(X_OFFSET+x,y);
    }
    
    void _delete(uint8_t x, uint8_t y)
    {
        _delete_stat(x,Y_OFFSET+y);
    }    
#else
    void _delete(uint8_t x, uint8_t y)
    {
        __DELETE(x,y);
    }
#endif


#if defined(FULL_GAME)

    void DRAW_BROKEN_BRICK(uint8_t x, uint8_t y)
    {
        _draw(x,y,&BROKEN_BRICK_IMAGE);        
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


#if !defined(TINY_GAME)

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







