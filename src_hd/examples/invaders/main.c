/* --------------------------------------------------------------------------------------- */ 
// 
// CROSS SHOOT by Fabrizio Caruso
//
// Fabrizio_Caruso@hotmail.com
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from
// the use of this software.

// Permission is granted to anyone to use this software for non-commercial applications, 
// subxect to the following restrictions:

// 1. The origin of this software must not be misrepresented; you must not
// claim that you wrote the original software. If you use this software in
// a product, an acknowledgment in the product documentation would be
// appreciated but is not required.

// 2. Altered source versions must be plainly marked as such, and must not
// be misrepresented as being the original software.

// 3. This notice may not be removed or altered from any source distribution.
/* --------------------------------------------------------------------------------------- */ 

#include "cross_lib.h"

#include "tile_aliases.h"

#if !defined(_XL_SLOW_DOWN_FACTOR)
    #define _XL_SLOW_DOWN_FACTOR 0
#endif

#  if _XL_SLOW_DOWN_FACTOR<20
    #define LEVEL_SPEED_UP 0
#elif _XL_SLOW_DOWN_FACTOR<100
    #define LEVEL_SPEED_UP 2
#elif _XL_SLOW_DOWN_FACTOR<200
    #define LEVEL_SPEED_UP 4
#elif _XL_SLOW_DOWN_FACTOR<400
    #define LEVEL_SPEED_UP 8
#elif _XL_SLOW_DOWN_FACTOR<800
    #define LEVEL_SPEED_UP 16
#elif _XL_SLOW_DOWN_FACTOR<1600
    #define LEVEL_SPEED_UP 32
#elif _XL_SLOW_DOWN_FACTOR<3200
    #define LEVEL_SPEED_UP 64
#else
    #define LEVEL_SPEED_UP 128
#endif 


#define MAX_Y ((YSize)+(Y_OFFSET))

#define SPACE_SHIP_Y (MAX_Y-4)


#if !defined(_XL_NO_UDG)

    #define draw_ship_1() \
    { \
        _XL_DRAW(ship_x/4,SPACE_SHIP_Y,SPACE_SHIP_1_W_TILE, _XL_CYAN); \
        _XL_DRAW(ship_x/4+1,SPACE_SHIP_Y,SPACE_SHIP_1_E_TILE, _XL_CYAN); \
    }


    #define draw_ship_2() \
    { \
        _XL_DRAW(ship_x/4,SPACE_SHIP_Y,SPACE_SHIP_2_W_TILE, _XL_CYAN); \
        _XL_DRAW(ship_x/4+1,SPACE_SHIP_Y,SPACE_SHIP_2_E_TILE, _XL_CYAN); \
    }

    #define draw_ship_3() \
    { \
        _XL_DRAW(ship_x/4,SPACE_SHIP_Y,SPACE_SHIP_3_W_TILE, _XL_CYAN); \
        _XL_DRAW(ship_x/4+1,SPACE_SHIP_Y,SPACE_SHIP_3_C_TILE, _XL_CYAN); \
        _XL_DRAW(ship_x/4+2,SPACE_SHIP_Y,SPACE_SHIP_3_E_TILE, _XL_CYAN); \
    }


    #define draw_ship_4() \
    { \
        _XL_DRAW(ship_x/4,SPACE_SHIP_Y,SPACE_SHIP_4_W_TILE, _XL_CYAN); \
        _XL_DRAW(ship_x/4+1,SPACE_SHIP_Y,SPACE_SHIP_4_E_TILE, _XL_CYAN); \
    }

    #define delete_at_the_right() \
    { \
        _XL_DELETE(ship_x/4+2,SPACE_SHIP_Y); \
    }

    #define delete_at_the_left() \
    { \
        _XL_DELETE(ship_x/4,SPACE_SHIP_Y); \
    }

#else
    #define draw_ship() \
        _XL_DRAW(ship_x,SPACE_SHIP_Y,SPACE_SHIP_1_W_TILE, _XL_WHITE); \
        _XL_DRAW(ship_x+1,SPACE_SHIP_Y,SPACE_SHIP_1_E_TILE, _XL_WHITE); \
  
    #define draw_ship_1() \
        draw_ship()
  
    #define draw_ship_2() \
        draw_ship()

    #define draw_ship_3() \
        draw_ship()

    #define draw_ship_4() \
        draw_ship() 
        
    #define delete_at_the_left() \
        _XL_DELETE(ship_x-1,SPACE_SHIP_Y); \
        _XL_DELETE(ship_x,SPACE_SHIP_Y); \

        
    #define delete_at_the_right() \
        _XL_DELETE(ship_x+1,SPACE_SHIP_Y);  \
        _XL_DELETE(ship_x,SPACE_SHIP_Y)

#endif


uint16_t x;

uint8_t invader[5];

uint8_t ship_x;

uint8_t ship_fire;

#define TOP_INVADER_Y (MAX_Y-21)
#define MID_INVADER_Y (MAX_Y-19)
#define LOW_INVADER_Y (MAX_Y-15)

#if !defined(_XL_NO_UDG)

    #define draw_mid_invader_closed(x,y) \
    { \
        _XL_DRAW(x,y,MID_INVADER_CLOSED_W_TILE, _XL_WHITE); \
        _XL_DRAW(x+1,y,MID_INVADER_CLOSED_E_TILE, _XL_WHITE); \
    }

    #define draw_mid_invader_open(x,y) \
    { \
        _XL_DRAW(x,y,MID_INVADER_OPEN_W_TILE, _XL_WHITE); \
        _XL_DRAW(x+1,y,MID_INVADER_OPEN_E_TILE, _XL_WHITE); \
    }

    #define delete_mid_invader(x,y) \
    { \
        _XL_DELETE(x,y); \
    }

    #define draw_low_invader_open(x,y) \
    { \
        _XL_DRAW(x,y,LOW_INVADER_OPEN_W_TILE, _XL_WHITE); \
        _XL_DRAW(x+1,y,LOW_INVADER_OPEN_E_TILE, _XL_WHITE); \
    }

    #define draw_low_invader_closed(x,y) \
    { \
        _XL_DRAW(x,y,LOW_INVADER_CLOSED_W_TILE, _XL_WHITE); \
        _XL_DRAW(x+1,y,LOW_INVADER_CLOSED_E_TILE, _XL_WHITE); \
    }

    #define delete_low_invader(x,y) \
    { \
        _XL_DELETE(x,y); \
    }

    #define draw_top_invader_closed(x,y) \
    { \
        _XL_DRAW(x,y,TOP_INVADER_CLOSED_TILE, _XL_WHITE); \
    }

    #define draw_top_invader_open(x,y) \
    { \
        _XL_DRAW(x,y,TOP_INVADER_OPEN_W_TILE, _XL_WHITE); \
        _XL_DRAW(x+1,y,TOP_INVADER_OPEN_E_TILE, _XL_WHITE); \
    }

    #define delete_top_invader(x,y) \
    { \
        _XL_DELETE(x,y); \
    }
#else
    
    #define draw_top_invader_closed(x,y) \
        _XL_DRAW(x,y,TOP_INVADER_CLOSED_TILE, _XL_WHITE)

    #define draw_top_invader_open(x,y) \
        _XL_DRAW(x,y,TOP_INVADER_OPEN_W_TILE, _XL_WHITE)

    #define draw_mid_invader_closed(x,y) \
        _XL_DRAW(x,y,MID_INVADER_CLOSED_W_TILE, _XL_WHITE)

    #define draw_mid_invader_open(x,y) \
        _XL_DRAW(x,y,MID_INVADER_OPEN_W_TILE, _XL_WHITE)

    #define draw_low_invader_closed(x,y) \
        _XL_DRAW(x,y,LOW_INVADER_CLOSED_W_TILE, _XL_WHITE)

    #define draw_low_invader_open(x,y) \
        _XL_DRAW(x,y,LOW_INVADER_OPEN_W_TILE, _XL_WHITE)
    
    #define delete_top_invader(x,y) \
        _XL_DELETE(x,y)

    #define delete_mid_invader(x,y) \
        _XL_DELETE(x,y)

    #define delete_low_invader(x,y) \
        _XL_DELETE(x,y)

#endif


#if !defined(INVADERS_PER_LINE)
    #define INVADERS_PER_LINE (XSize/5)
#endif
#define SPACE_BETWEEN_INVADERS 2




void redraw(void)
{
    uint8_t i;
    
    _XL_CLEAR_SCREEN();
    
    for(i=0;i<INVADERS_PER_LINE*SPACE_BETWEEN_INVADERS;i+=SPACE_BETWEEN_INVADERS)
    {
        draw_top_invader_open(invader[4]+i,TOP_INVADER_Y);
        draw_mid_invader_open(invader[3]+i,MID_INVADER_Y);
        draw_mid_invader_open(invader[2]+i,MID_INVADER_Y+2);
        draw_low_invader_closed(invader[1]+i,LOW_INVADER_Y);
        draw_low_invader_closed(invader[0]+i,LOW_INVADER_Y+2);
    }
}


    
#if defined(__NO_INPUT)
    void MOVE_PLAYER(void) {}
#else
    void MOVE_PLAYER(void)
    {
        uint8_t input = _XL_INPUT();
        
        if(_XL_LEFT(input))
        {
            delete_at_the_right();
            --ship_x;
        }
        else if(_XL_RIGHT(input))
        {
            delete_at_the_left();
            ++ship_x;
        }
        else if(_XL_FIRE(input))
        {
            ship_fire = 1;
        }
        
        switch(ship_x&3)
        {
            case 1:
                draw_ship_1();
            break;
            
            case 2:
                draw_ship_2();
            break;
            
            case 3:
                draw_ship_3();
            break;
            
            case 0:
                draw_ship_4();
            break;
        }
    }
#endif



int main(void)
{        

    uint8_t i;
    
    uint8_t line_counter;
    
    _XL_INIT_GRAPHICS();

    _XL_INIT_INPUT();    
    
    ship_x = XSize/2;
    line_counter = 0;

    _XL_CLEAR_SCREEN();

    x = 0;
    while(1)
    {
        for(i=0;i<5;++i)
        {
            invader[i] = 0;
        }
        redraw();
        
        while(invader[4]<XSize-INVADERS_PER_LINE*SPACE_BETWEEN_INVADERS)
        {

            MOVE_PLAYER();
            if(!(x&1))
            {
                for(i=0;i<INVADERS_PER_LINE*SPACE_BETWEEN_INVADERS;i+=SPACE_BETWEEN_INVADERS)
                {
                    switch(line_counter%10)
                    {
                        case 4:
                            delete_top_invader(invader[4]+i,TOP_INVADER_Y);
                        break;
                        case 3:
                            delete_mid_invader(invader[3]+i,MID_INVADER_Y);
                        break;
                        case 2:
                            delete_mid_invader(invader[2]+i,MID_INVADER_Y+2);
                        break;
                        case 1:
                            delete_low_invader(invader[1]+i,LOW_INVADER_Y);
                        break;
                        case 0:
                            delete_low_invader(invader[0]+i,LOW_INVADER_Y+2);
                        break;
                    }
                }
                
                if(line_counter%10<5)
                {
                    ++invader[line_counter%5];
                }
                
                for(i=0;i<INVADERS_PER_LINE*SPACE_BETWEEN_INVADERS;i+=SPACE_BETWEEN_INVADERS)
                {
                    switch(line_counter%10)
                    {
                        case 4:
                            draw_top_invader_closed(invader[4]+i,TOP_INVADER_Y);
                        break;
                        case 3:
                            draw_mid_invader_closed(invader[3]+i,MID_INVADER_Y);
                        break;
                        case 2:
                            draw_mid_invader_closed(invader[2]+i,MID_INVADER_Y+2);
                        break;
                        case 1:
                            draw_low_invader_open(invader[1]+i,LOW_INVADER_Y);
                        break;
                        case 0:
                            draw_low_invader_open(invader[0]+i,LOW_INVADER_Y+2);
                        break;
                    }
                }
                ++line_counter;
                if(line_counter==250)
                {
                    line_counter=0;
                }
            }
            
            _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR/8);
            
            if(x&1)
            {
                for(i=0;i<INVADERS_PER_LINE*SPACE_BETWEEN_INVADERS;i+=SPACE_BETWEEN_INVADERS)
                {
                    switch(line_counter%10)
                    {
                        case 9:
                            draw_top_invader_open(invader[4]+i,TOP_INVADER_Y);
                        break;
                        case 8:
                            draw_mid_invader_open(invader[3]+i,MID_INVADER_Y);
                        break;
                        case 7:
                            draw_mid_invader_open(invader[2]+i,MID_INVADER_Y+2);
                        break;
                        case 6:
                            draw_low_invader_closed(invader[1]+i,LOW_INVADER_Y);
                        break;
                        case 5:
                            draw_low_invader_closed(invader[0]+i,LOW_INVADER_Y+2);
                        break;
                    }
                }
            }

            ++x;
        }
        
    }


    return EXIT_SUCCESS;
}

