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
// subject to the following restrictions:

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

// TODO: REMOVE THIS
#define MAX_INIT_Y_POS ((YSize)+(Y_OFFSET)-19)

#if ((YSize)+(Y_OFFSET)-1)>19
    #define MAX_TILES 19
#else
    #define MAX_TILES ((YSize)+(Y_OFFSET)-1)
#endif


extern Image VERTICAL_HEAD_IMAGE;  
extern Image HORIZONTAL_HEAD_IMAGE; 
extern Image RIGHT_HEAD_IMAGE; 
extern Image LEFT_HEAD_IMAGE; 	


extern Image BODY_IMAGE; 
extern Image BOTTOM_TAIL_IMAGE; 

extern Image DEAD_GHOST_IMAGE; 

extern Image SKULL_IMAGE; 

extern Image CALM_DOWN_IMAGE; 
extern Image HORIZONTAL_JOINT_IMAGE; 
extern Image FIRE_POWER_IMAGE; 
extern Image EXTRA_POINTS_IMAGE; 

extern Image BULLET_IMAGE; 

extern Image VERTICAL_BRICK_IMAGE; 
extern Image HORIZONTAL_BRICK_IMAGE; 

extern Image LEFT_TAIL_IMAGE; 

extern Image RIGHT_TAIL_IMAGE; 

extern Image VERTICAL_JOINT_IMAGE; 

extern Image BOTTOM_HEAD_IMAGE; 
extern Image SUPER_IMAGE; 

extern Image EXTRA_LIFE_IMAGE; 
extern Image INVINCIBILITY_IMAGE; 


extern Image CONFUSE_IMAGE; 
extern Image SUICIDE_IMAGE; 

extern Image BROKEN_BRICK_IMAGE; 
    
extern Image TOP_HEAD_IMAGE; 
extern Image TOP_HEAD_JOINT_IMAGE; 
extern Image BOTTOM_HEAD_JOINT_IMAGE; 
extern Image LEFT_HEAD_JOINT_IMAGE; 
extern Image RIGHT_HEAD_JOINT_IMAGE; 
extern Image TOP_TAIL_IMAGE; 



#define COL_OFFSET ((XSize-16)/2-1)
#define ROW_OFFSET 3

#define INIT_SNAKE_LENGTH 10
#define MAX_SNAKE_LENGTH 20

struct SnakeBodyStruct
{
    uint8_t x;
    uint8_t y;
};
typedef struct SnakeBodyStruct SnakeBody;

SnakeBody snake[MAX_SNAKE_LENGTH];

enum Direction {UP, DOWN, LEFT, RIGHT};

enum Direction direction;

uint8_t snake_length;

uint8_t snake_head;

Image *head_image_ptr;

void draw_head(void)
{
    _XLIB_DRAW(snake[snake_head].x,snake[snake_head].y,head_image_ptr);
}

void draw_body_part(uint8_t i)
{
    _XLIB_DRAW(snake[i].x,snake[i].y,&BODY_IMAGE);
}

void draw_snake(void)
{
    uint8_t i;
    
    draw_head();

    for(i=1;i<snake_length;++i)
    {
        draw_body_part((i+snake_head) % snake_length);
    }
}

void init_snake(void)
{

    uint8_t i;
    
    snake_length = INIT_SNAKE_LENGTH;
    snake_head = 0;
    
    for(i=0;i<snake_length;++i)
    {
        snake[(i+snake_head)%snake_length].x = XSize/2+snake_length/2-i;
        snake[(i+snake_head)%snake_length].y = YSize/2;
    }
    
    head_image_ptr = &HORIZONTAL_HEAD_IMAGE;
    
    draw_snake();
}



int main(void)
{        

    uint8_t j;


    INIT_GRAPHICS();

    INIT_INPUT();
    
    INIT_IMAGES();
    
    for(j=0;j<3;++j)
    {
        CLEAR_SCREEN();
        
        SET_TEXT_COLOR(COLOR_WHITE);
        PRINT(COL_OFFSET,YSize-5, _XL_P _XL_R _XL_E _XL_S _XL_S _XL_SPACE _XL_F _XL_I _XL_R _XL_E);
        
        PRINT(COL_OFFSET, 2,"01234567890");
        PRINT(COL_OFFSET, 4, _XL_a _XL_b _XL_c _XL_d _XL_e _XL_f _XL_g _XL_h _XL_i _XL_j "-");
        PRINT(COL_OFFSET, 5, _XL_k _XL_l _XL_m _XL_n _XL_o _XL_p _XL_q _XL_r _XL_s _XL_t "-");
        PRINT(COL_OFFSET, 6, _XL_u _XL_v _XL_w _XL_x _XL_y _XL_z);
        PRINT(COL_OFFSET, 8, _XL_A _XL_B _XL_C _XL_D _XL_E _XL_F _XL_G _XL_H _XL_I _XL_J "-");
        PRINT(COL_OFFSET, 9, _XL_K _XL_L _XL_M _XL_N _XL_O _XL_P _XL_Q _XL_R _XL_S _XL_T "-");
        PRINT(COL_OFFSET,10, _XL_U _XL_V _XL_W _XL_X _XL_Y _XL_Z);
        
        WAIT_PRESS();
        CLEAR_SCREEN();

        init_snake();
        
        // _XLIB_DRAW(COL_OFFSET,YSize/2, &LEFT_HEAD_IMAGE);
        // _XLIB_DRAW(COL_OFFSET+1,YSize/2,&LEFT_HEAD_JOINT_IMAGE);
        // _XLIB_DRAW(COL_OFFSET+2,YSize/2,&HORIZONTAL_JOINT_IMAGE);
        // _XLIB_DRAW(COL_OFFSET+3,YSize/2,&HORIZONTAL_JOINT_IMAGE);
        // _XLIB_DRAW(COL_OFFSET+4,YSize/2,&HORIZONTAL_JOINT_IMAGE);
        // _XLIB_DRAW(COL_OFFSET+5,YSize/2,&HORIZONTAL_JOINT_IMAGE);
        // _XLIB_DRAW(COL_OFFSET+6,YSize/2,&HORIZONTAL_JOINT_IMAGE);
        // _XLIB_DRAW(COL_OFFSET+7,YSize/2,&HORIZONTAL_JOINT_IMAGE);
        // _XLIB_DRAW(COL_OFFSET+8,YSize/2,&HORIZONTAL_JOINT_IMAGE);
        // _XLIB_DRAW(COL_OFFSET+9,YSize/2,&HORIZONTAL_JOINT_IMAGE);
        // _XLIB_DRAW(COL_OFFSET+10,YSize/2,&HORIZONTAL_JOINT_IMAGE);
        // _XLIB_DRAW(COL_OFFSET+11,YSize/2,&HORIZONTAL_JOINT_IMAGE);
        // _XLIB_DRAW(COL_OFFSET+12,YSize/2,&HORIZONTAL_JOINT_IMAGE);
        // _XLIB_DRAW(COL_OFFSET+13,YSize/2,&HORIZONTAL_JOINT_IMAGE);
        // _XLIB_DRAW(COL_OFFSET+14,YSize/2,&HORIZONTAL_JOINT_IMAGE);
        // _XLIB_DRAW(COL_OFFSET+15,YSize/2,&HORIZONTAL_JOINT_IMAGE);
        // _XLIB_DRAW(COL_OFFSET+16,YSize/2,&LEFT_TAIL_IMAGE);




        
        
        SET_TEXT_COLOR(COLOR_WHITE);
        PRINT(COL_OFFSET,YSize-5, _XL_P _XL_R _XL_E _XL_S _XL_S _XL_SPACE _XL_F _XL_I _XL_R _XL_E);
        WAIT_PRESS();
    }

    PRINT(COL_OFFSET,YSize-5, _XL_E _XL_N _XL_D _XL_SPACE _XL_O _XL_F _XL_SPACE _XL_D _XL_E _XL_M _XL_O);

    while(1){};
    
    return EXIT_SUCCESS;
}

