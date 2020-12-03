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

#include "snake.h"
#include "move_snake.h"

#include "control_player.h"

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

extern Image HORIZONTAL_JOINT_IMAGE; 

extern Image EXTRA_POINTS_IMAGE; 

extern Image LEFT_TAIL_IMAGE; 

extern Image RIGHT_TAIL_IMAGE; 

extern Image VERTICAL_JOINT_IMAGE; 

extern Image TOP_HEAD_IMAGE; 
extern Image TOP_HEAD_JOINT_IMAGE; 
extern Image BOTTOM_HEAD_JOINT_IMAGE; 
extern Image LEFT_HEAD_JOINT_IMAGE; 
extern Image RIGHT_HEAD_JOINT_IMAGE; 
extern Image TOP_TAIL_IMAGE; 


extern SnakeBody snake[MAX_SNAKE_LENGTH];

extern uint8_t snake_length;

extern uint8_t snake_head;

extern Image *head_image_ptr;


#define COL_OFFSET ((XSize-16)/2-1)
#define ROW_OFFSET 3




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
        
        while(1)
        {
            MOVE_PLAYER();
            WAIT_PRESS();
        }
        // WAIT_PRESS();
        // move_snake(SNAKE_RIGHT);
        // WAIT_PRESS();
        // move_snake(SNAKE_RIGHT);
        // WAIT_PRESS();
        // move_snake(SNAKE_UP);
        // WAIT_PRESS();
        // move_snake(SNAKE_UP);
        // WAIT_PRESS();
        // move_snake(SNAKE_LEFT);
        // WAIT_PRESS();
        // move_snake(SNAKE_LEFT);
        // WAIT_PRESS();
        // move_snake(SNAKE_DOWN);
        // WAIT_PRESS();
        // move_snake(SNAKE_DOWN);





        
        
        SET_TEXT_COLOR(COLOR_WHITE);
        PRINT(COL_OFFSET,YSize-5, _XL_P _XL_R _XL_E _XL_S _XL_S _XL_SPACE _XL_F _XL_I _XL_R _XL_E);
        WAIT_PRESS();
    }

    PRINT(COL_OFFSET,YSize-5, _XL_E _XL_N _XL_D _XL_SPACE _XL_O _XL_F _XL_SPACE _XL_D _XL_E _XL_M _XL_O);

    while(1){};
    
    return EXIT_SUCCESS;
}

