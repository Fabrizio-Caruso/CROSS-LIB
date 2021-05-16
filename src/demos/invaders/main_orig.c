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


#if !defined EXIT_SUCCESS
    #define EXIT_SUCCESS 0
#endif

#include "cross_lib.h"

#include "images.h"

#if !defined(SLOW_DOWN)
    #define SLOW_DOWN 0
#endif

#  if SLOW_DOWN<20
    #define LEVEL_SPEED_UP 0
#elif SLOW_DOWN<100
    #define LEVEL_SPEED_UP 2
#elif SLOW_DOWN<200
    #define LEVEL_SPEED_UP 4
#elif SLOW_DOWN<400
    #define LEVEL_SPEED_UP 8
#elif SLOW_DOWN<800
    #define LEVEL_SPEED_UP 16
#elif SLOW_DOWN<1600
    #define LEVEL_SPEED_UP 32
#elif SLOW_DOWN<3200
    #define LEVEL_SPEED_UP 64
#else
    #define LEVEL_SPEED_UP 128
#endif 

#define MAX_Y ((YSize)+(Y_OFFSET))


uint16_t x;

uint8_t invader[5];

uint8_t ship_x;

uint8_t ship_fire;


#define SPACE_SHIP_Y (MAX_Y-4)
#define TOP_INVADER_Y (MAX_Y-21)
#define MID_INVADER_Y (MAX_Y-19)
#define LOW_INVADER_Y (MAX_Y-15)

#if !defined(CHAR_GRAPHICS)
    #define draw_ship_1() \
    { \
        _XL_DRAW(ship_x,SPACE_SHIP_Y,SPACE_SHIP_1_W_TILE, _XL_CYAN); \
        _XL_DRAW(ship_x+1,SPACE_SHIP_Y,SPACE_SHIP_1_E_TILE, _XL_CYAN); \
    }


    #define draw_ship_2() \
    { \
        _XL_DRAW(ship_x,SPACE_SHIP_Y,SPACE_SHIP_2_W_TILE, _XL_CYAN); \
        _XL_DRAW(ship_x+1,SPACE_SHIP_Y,SPACE_SHIP_2_E_TILE, _XL_CYAN); \
    }

    #define draw_ship_3() \
    { \
        _XL_DRAW(ship_x,SPACE_SHIP_Y,SPACE_SHIP_3_W_TILE, _XL_CYAN); \
        _XL_DRAW(ship_x+1,SPACE_SHIP_Y,SPACE_SHIP_3_C_TILE, _XL_CYAN); \
        _XL_DRAW(ship_x+2,SPACE_SHIP_Y,SPACE_SHIP_3_E_TILE, _XL_CYAN); \
    }


    #define draw_ship_4() \
    { \
        _XL_DRAW(ship_x,SPACE_SHIP_Y,SPACE_SHIP_4_W_TILE, _XL_CYAN); \
        _XL_DRAW(ship_x+1,SPACE_SHIP_Y,SPACE_SHIP_4_E_TILE, _XL_CYAN); \
    }


    #define delete_ship() \
    { \
        _XL_DELETE(ship_x,SPACE_SHIP_Y); \
    }


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
    #define draw_ship() \
        _XL_DRAW(x,SPACE_SHIP_Y,SPACE_SHIP_1_W_TILE, _XL_WHITE); \
        _XL_DRAW(x+1,SPACE_SHIP_Y,SPACE_SHIP_1_E_TILE, _XL_WHITE); \
        
    #define draw_ship_2() \
        draw_ship()
        
    #define draw_ship_4() \
        draw_ship() 
        
    #define delete_ship() \
        _XL_DELETE(x,SPACE_SHIP_Y); \

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


int main(void)
{        

    uint8_t i;
    
    uint8_t line_counter;
    
    _XL_INIT_GRAPHICS();

    _XL_INIT_INPUT();    
    
    ship_x = 1;
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

            #if !defined(CHAR_GRAPHICS)
            draw_ship_1();
            
            _XL_WAIT_VSYNC();
            REFRESH();
            _XL_SLOW_DOWN(SLOW_DOWN);
            #endif
            
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

            delete_ship();
            
            #if defined(CHAR_GRAPHICS)
            if(++ship_x==XSize-2)
            {
                ship_x=0;
                redraw();
            };
            #endif
            
            draw_ship_2();
            _XL_PRINTD(0,0,2,x);
            
            _XL_WAIT_VSYNC();
            REFRESH();
            // _XL_WAIT_FOR_INPUT();
            _XL_SLOW_DOWN(SLOW_DOWN);
            
            
            #if !defined(CHAR_GRAPHICS)
            delete_ship();
            draw_ship_3();
            
            
            _XL_WAIT_VSYNC();
            REFRESH();
            _XL_SLOW_DOWN(SLOW_DOWN);
            #endif
            
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
            delete_ship();
            if(++ship_x==XSize-2)
            {
                ship_x=0;
                redraw();
            };
            ++x;
            draw_ship_4();
            _XL_PRINTD(0,0,2,x);
            
            _XL_WAIT_VSYNC();
            REFRESH();
            _XL_SLOW_DOWN(SLOW_DOWN);
        }
        
    }


    return EXIT_SUCCESS;
}

