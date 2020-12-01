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

#include "init_images.h"

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


extern Image SPACE_SHIP_1_W_IMAGE;
extern Image SPACE_SHIP_1_E_IMAGE;

extern Image SPACE_SHIP_2_W_IMAGE;
extern Image SPACE_SHIP_2_E_IMAGE;

extern Image SPACE_SHIP_3_W_IMAGE;
extern Image SPACE_SHIP_3_C_IMAGE;
extern Image SPACE_SHIP_3_E_IMAGE;

extern Image SPACE_SHIP_4_W_IMAGE;
extern Image SPACE_SHIP_4_E_IMAGE;

extern Image MID_INVADER_CLOSED_W_IMAGE;
extern Image MID_INVADER_CLOSED_E_IMAGE;

extern Image MID_INVADER_OPEN_W_IMAGE;
extern Image MID_INVADER_OPEN_E_IMAGE;

extern Image LOW_INVADER_CLOSED_W_IMAGE;
extern Image LOW_INVADER_CLOSED_E_IMAGE;

extern Image LOW_INVADER_OPEN_W_IMAGE;
extern Image LOW_INVADER_OPEN_E_IMAGE;

extern Image TOP_INVADER_OPEN_W_IMAGE;
extern Image TOP_INVADER_OPEN_E_IMAGE;

extern Image TOP_INVADER_CLOSED_IMAGE;

uint8_t x;

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
        _XLIB_DRAW(x,SPACE_SHIP_Y,&SPACE_SHIP_1_W_IMAGE); \
        _XLIB_DRAW(x+1,SPACE_SHIP_Y,&SPACE_SHIP_1_E_IMAGE); \
    }


    #define draw_ship_2() \
    { \
        _XLIB_DRAW(x,SPACE_SHIP_Y,&SPACE_SHIP_2_W_IMAGE); \
        _XLIB_DRAW(x+1,SPACE_SHIP_Y,&SPACE_SHIP_2_E_IMAGE); \
    }

    #define draw_ship_3() \
    { \
        _XLIB_DRAW(x,SPACE_SHIP_Y,&SPACE_SHIP_3_W_IMAGE); \
        _XLIB_DRAW(x+1,SPACE_SHIP_Y,&SPACE_SHIP_3_C_IMAGE); \
        _XLIB_DRAW(x+2,SPACE_SHIP_Y,&SPACE_SHIP_3_E_IMAGE); \
    }


    #define draw_ship_4() \
    { \
        _XLIB_DRAW(x,SPACE_SHIP_Y,&SPACE_SHIP_4_W_IMAGE); \
        _XLIB_DRAW(x+1,SPACE_SHIP_Y,&SPACE_SHIP_4_E_IMAGE); \
    }


    #define delete_ship() \
    { \
        _XLIB_DELETE(x,SPACE_SHIP_Y); \
    }


    #define draw_mid_invader_closed(x,y) \
    { \
        _XLIB_DRAW(x,y,&MID_INVADER_CLOSED_W_IMAGE); \
        _XLIB_DRAW(x+1,y,&MID_INVADER_CLOSED_E_IMAGE); \
    }

    #define draw_mid_invader_open(x,y) \
    { \
        _XLIB_DRAW(x,y,&MID_INVADER_OPEN_W_IMAGE); \
        _XLIB_DRAW(x+1,y,&MID_INVADER_OPEN_E_IMAGE); \
    }


    #define delete_mid_invader(x,y) \
    { \
        _XLIB_DELETE(x,y); \
    }


    #define draw_low_invader_open(x,y) \
    { \
        _XLIB_DRAW(x,y,&LOW_INVADER_OPEN_W_IMAGE); \
        _XLIB_DRAW(x+1,y,&LOW_INVADER_OPEN_E_IMAGE); \
    }

    #define draw_low_invader_closed(x,y) \
    { \
        _XLIB_DRAW(x,y,&LOW_INVADER_CLOSED_W_IMAGE); \
        _XLIB_DRAW(x+1,y,&LOW_INVADER_CLOSED_E_IMAGE); \
    }


    #define delete_low_invader(x,y) \
    { \
        _XLIB_DELETE(x,y); \
    }


    #define draw_top_invader_closed(x,y) \
    { \
        _XLIB_DRAW(x,y,&TOP_INVADER_CLOSED_IMAGE); \
    }

    #define draw_top_invader_open(x,y) \
    { \
        _XLIB_DRAW(x,y,&TOP_INVADER_OPEN_W_IMAGE); \
        _XLIB_DRAW(x+1,y,&TOP_INVADER_OPEN_E_IMAGE); \
    }


    #define delete_top_invader(x,y) \
    { \
        _XLIB_DELETE(x,y); \
    }
#else
    #define draw_ship() \
        _XLIB_DRAW(x,SPACE_SHIP_Y,&SPACE_SHIP_1_W_IMAGE); \
        _XLIB_DRAW(x+1,SPACE_SHIP_Y,&SPACE_SHIP_1_E_IMAGE); \
        
    #define draw_ship_2() \
        draw_ship()
        
    #define draw_ship_4() \
        draw_ship() 
        
    #define delete_ship() \
        _XLIB_DELETE(x,SPACE_SHIP_Y); \

    #define draw_top_invader_closed(x,y) \
        _XLIB_DRAW(x,y,&TOP_INVADER_CLOSED_IMAGE)

    #define draw_top_invader_open(x,y) \
        _XLIB_DRAW(x,y,&TOP_INVADER_OPEN_W_IMAGE)

    #define draw_mid_invader_closed(x,y) \
        _XLIB_DRAW(x,y,&MID_INVADER_CLOSED_W_IMAGE)

    #define draw_mid_invader_open(x,y) \
        _XLIB_DRAW(x,y,&MID_INVADER_OPEN_W_IMAGE)

    #define draw_low_invader_closed(x,y) \
        _XLIB_DRAW(x,y,&LOW_INVADER_CLOSED_W_IMAGE)

    #define draw_low_invader_open(x,y) \
        _XLIB_DRAW(x,y,&LOW_INVADER_OPEN_W_IMAGE)
    
    #define delete_top_invader(x,y) \
        _XLIB_DELETE(x,y)

    #define delete_mid_invader(x,y) \
        _XLIB_DELETE(x,y)

    #define delete_low_invader(x,y) \
        _XLIB_DELETE(x,y)


#endif

#if !defined(INVADERS_PER_LINE)
    #define INVADERS_PER_LINE (XSize/4)
#endif
#define SPACE_BETWEEN_INVADERS 2

int main(void)
{        

    uint8_t i;
    
    uint8_t line_counter;
    
    INIT_GRAPHICS();

    INIT_INPUT();    
    
    INIT_IMAGES();
    
    ship_x = 1;
    line_counter = 0;

    CLEAR_SCREEN();

    // PRINT(0,SPACE_SHIP_Y-3,"01234567890123456789");
    x = 0;
    while(1)
    {
        for(i=0;i<5;++i)
        {
            invader[i] = 0;
        }
        CLEAR_SCREEN();
        
        for(i=0;i<INVADERS_PER_LINE*SPACE_BETWEEN_INVADERS;i+=SPACE_BETWEEN_INVADERS)
        {
            draw_top_invader_closed(invader[4]+i,TOP_INVADER_Y);
            draw_mid_invader_closed(invader[3]+i,MID_INVADER_Y);
            draw_mid_invader_closed(invader[2]+i,MID_INVADER_Y+2);
            draw_low_invader_open(invader[1]+i,LOW_INVADER_Y);
            draw_low_invader_open(invader[0]+i,LOW_INVADER_Y+2);
        }
        
        while(invader[0]<XSize-INVADERS_PER_LINE*SPACE_BETWEEN_INVADERS)
        {

            #if !defined(CHAR_GRAPHICS)
            draw_ship_1();
            
            WAIT_V_SYNC();
            REFRESH();
            DO_SLOW_DOWN(SLOW_DOWN);
            #endif
            
            if(!(x&3))
            {
                for(i=0;i<INVADERS_PER_LINE*SPACE_BETWEEN_INVADERS;i+=SPACE_BETWEEN_INVADERS)
                {
                    switch(line_counter%5)
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
                
                ++invader[line_counter%5];

                for(i=0;i<INVADERS_PER_LINE*SPACE_BETWEEN_INVADERS;i+=SPACE_BETWEEN_INVADERS)
                {
                    switch(line_counter%5)
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
            if(++x==XSize-2)
            {
                x=0;
                CLEAR_SCREEN();
                REFRESH();
            };
            #endif
            
            draw_ship_2();
            PRINTD(0,0,2,x);
            
            WAIT_V_SYNC();
            REFRESH();
            // WAIT_PRESS();
            DO_SLOW_DOWN(SLOW_DOWN);
            
            
            #if !defined(CHAR_GRAPHICS)
            delete_ship();
            draw_ship_3();
            
            
            WAIT_V_SYNC();
            REFRESH();
            DO_SLOW_DOWN(SLOW_DOWN);
            #endif
            
            if((x&3)==1)
            {
                for(i=0;i<INVADERS_PER_LINE*SPACE_BETWEEN_INVADERS;i+=SPACE_BETWEEN_INVADERS)
                {
                    switch(line_counter%5)
                    {
                        case 4:
                            draw_top_invader_open(invader[4]+i,TOP_INVADER_Y);
                        break;
                        case 3:
                            draw_mid_invader_open(invader[3]+i,MID_INVADER_Y);
                        break;
                        case 2:
                            draw_mid_invader_open(invader[2]+i,MID_INVADER_Y+2);
                        break;
                        case 1:
                            draw_low_invader_closed(invader[1]+i,LOW_INVADER_Y);
                        break;
                        case 0:
                            draw_low_invader_closed(invader[0]+i,LOW_INVADER_Y+2);
                        break;
                    }
                }
            }
            delete_ship();
            if(++x==XSize-2)
            {
                x=0;
                CLEAR_SCREEN();
                REFRESH();
            };
            draw_ship_4();
            PRINTD(0,0,2,x);
            
            WAIT_V_SYNC();
            REFRESH();
            DO_SLOW_DOWN(SLOW_DOWN);
            // WAIT_PRESS();
        }
        
    }


    return EXIT_SUCCESS;
}

