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

#include "cross_lib.h"

#include "images.h"

#define INITIAL_LEVEL 1

#define X_DINO (XSize/8)
#define Y_DINO ((YSize/2)+2)


#define NUMBER_OF_CACTI 3

uint8_t input;


uint8_t state;

uint8_t x_cactus[NUMBER_OF_CACTI];
uint8_t active_cactus[NUMBER_OF_CACTI];
uint8_t cactus_cooldown[NUMBER_OF_CACTI];
uint8_t number_of_active_cactus;
uint8_t last_active_cactus = 1;

uint8_t x_bird;
uint8_t active_bird;
uint8_t y_bird;
uint8_t bird_cooldown;

uint8_t dead;

uint8_t speed;
uint8_t activate_speed;
// uint8_t disactivate_speed;

uint16_t score;
uint16_t hiscore;

uint8_t level;

uint8_t level_cacti;
uint8_t level_bird;

#define Y_TERRAIN ((Y_DINO)+2)
#define Y_CACTUS ((Y_TERRAIN)-1)

// #define LEFT_END_OF_SCREEN 0
#define RIGHT_END_OF_SCREEN ((XSize)-1)


#define LEVEL_Y ((YSize)/5)
#define LEVEL_X ((XSize)/2-4)


void draw_jump_dino_0(uint8_t height)
{
    // Initial tiles
    _XL_DRAW(X_DINO+1,Y_DINO-height,TOP_DINO_0,_XL_WHITE);
    _XL_DRAW(X_DINO+1,Y_DINO+1-height,BOTTOM_DINO_0,_XL_WHITE);
    _XL_DRAW(X_DINO,Y_DINO+1-height,TAIL_DINO_0,_XL_WHITE);
}    

void draw_dead_dino_0(void)
{
    // Initial tiles
    _XL_DRAW(X_DINO+1,Y_DINO,TOP_DEAD_DINO_0,_XL_WHITE);
    _XL_DRAW(X_DINO+1,Y_DINO+1,BOTTOM_DINO_0,_XL_WHITE);
    _XL_DRAW(X_DINO,Y_DINO+1,TAIL_DINO_0,_XL_WHITE);
}   

void draw_jump_dino_1(uint8_t height)
{
    _XL_DRAW(X_DINO+1,Y_DINO-1-height,TOP_DINO_1,_XL_WHITE);
    _XL_DRAW(X_DINO+1,Y_DINO-height,MIDDLE_DINO_1,_XL_WHITE);    
    _XL_DRAW(X_DINO+1,Y_DINO+1-height,BOTTOM_DINO_1,_XL_WHITE);
    _XL_DRAW(X_DINO,Y_DINO+1-1-height,TAIL_DINO_1,_XL_WHITE);
    _XL_DRAW(X_DINO,Y_DINO+1-height,FOOT_DINO_1,_XL_WHITE);    
}   

void draw_jump_dino_2(uint8_t height)
{
    _XL_DRAW(X_DINO+1,Y_DINO-1-height,TOP_DINO_2,_XL_WHITE);
    _XL_DRAW(X_DINO+1,Y_DINO-height,MIDDLE_DINO_2,_XL_WHITE);    
    _XL_DRAW(X_DINO+1,Y_DINO+1-height,BOTTOM_DINO_2,_XL_WHITE);
    _XL_DRAW(X_DINO,Y_DINO+1-1-height,TAIL_DINO_2,_XL_WHITE);
    _XL_DRAW(X_DINO,Y_DINO+1-height,FOOT_DINO_2,_XL_WHITE);    
}  

void delete_feet(uint8_t height)
{
    _XL_DELETE(X_DINO+1,Y_DINO+1-height);
    _XL_DELETE(X_DINO,Y_DINO+1-height); 
}

void delete_top(uint8_t height)
{
    _XL_DELETE(X_DINO+1,Y_DINO-1-height);
    _XL_DELETE(X_DINO,Y_DINO-height);

}

void draw_dino_feet_0(void)
{
    _XL_DRAW(X_DINO+1,Y_DINO+1,BOTTOM_DINO_0,_XL_WHITE);
    _XL_DRAW(X_DINO,Y_DINO+1,TAIL_RAISED_DINO,_XL_WHITE);
}


void draw_dino_feet_1(void)
{
    _XL_DRAW(X_DINO+1,Y_DINO+1,BOTTOM_RAISED_DINO,_XL_WHITE);
    _XL_DRAW(X_DINO,Y_DINO+1,TAIL_DINO_0,_XL_WHITE);
}


// #define RUN_SLOW_DOWN 4


#define JUMP 2


void handle_state_behavior(void)
{
    switch(state)
    {
        case 99: // like case 1
            draw_jump_dino_0(0);
            
            draw_dino_feet_0();
        break;
        case 0:
            // draw_jump_dino_0(0);

            // delete_feet(0);
            
            draw_dino_feet_1();
        break;
        
        case 1:
            // delete_feet(0);
            
            draw_dino_feet_0();
        break;
        
        case JUMP:
                delete_feet(0);
                
                draw_jump_dino_0(0+1);
                
        break;
        
        case JUMP+1:
        
                draw_jump_dino_1(0+1);
                delete_feet(1);
                
                draw_jump_dino_0(1+1);
                
        break;
        
        case JUMP+2:
                draw_jump_dino_1(1+1);

                delete_feet(2);
                
                draw_jump_dino_0(2+1);
                
        break;
        
        case JUMP+3:
                draw_jump_dino_1(2+1);

                delete_feet(3);
                
                draw_jump_dino_0(3+1);
                
        break;
        
        case JUMP+4:
                draw_jump_dino_1(3+1);

                delete_feet(4);
                
                draw_jump_dino_0(4+1);
                
        break;
        
        case JUMP+5:
            delete_feet(4);
            
            draw_jump_dino_2(4+1);
        break;
        
        case JUMP+6:
            delete_top(5);
            draw_jump_dino_0(5);
        break;
        
        case JUMP+7:
            delete_top(4);
            draw_jump_dino_1(4);
        break;
        
        case JUMP+8:
            delete_top(4);
            draw_jump_dino_0(4);
        break;
        
        case JUMP+9:
            delete_top(3);
            draw_jump_dino_1(3);
        break;
        
        case JUMP+10:
            delete_top(3);
            draw_jump_dino_0(3);
        break;
        
        case JUMP+11:
            delete_top(2);
            draw_jump_dino_1(2);
        break;
        
        case JUMP+12:
            delete_top(2);
            draw_jump_dino_0(2);
        break;
        
        case JUMP+13:
            delete_top(1);
            draw_jump_dino_1(1);
        break;
        
        case JUMP+14:
            delete_top(1);
            draw_jump_dino_0(1);
        break;
        
        case JUMP+15:
            delete_top(0);
            draw_jump_dino_1(0);
        break;

        
        case JUMP+16:

            delete_top(0);
            delete_top(1);
            
             draw_jump_dino_0(0);

            delete_feet(0);
            
            draw_dino_feet_0();
        break;

    }
}


void handle_state_transition(void)
{
    switch(state)
    {
        case 99:

            input = _XL_INPUT();
            
            if(_XL_FIRE(input))
            {
                state = JUMP;
            }
            else
            {
                state = 1;
            }
        break;      
        case 0:

            input = _XL_INPUT();
            
            if(_XL_FIRE(input))
            {
                state = JUMP;
            }
            else
            {
                state = 1;
            }
        break;
        
        case 1:
            input = _XL_INPUT();
            
            if(_XL_FIRE(input))
            {
                state = JUMP;
            }
            else
            {
                state = 0;
            }
        break;
        
        case JUMP:
        case JUMP+1:
        case JUMP+2:
        case JUMP+3:
        case JUMP+4:
        case JUMP+5:
        case JUMP+6:
        case JUMP+7:
        case JUMP+8:
        case JUMP+9:
        case JUMP+10:
        case JUMP+11:
        case JUMP+12:
        case JUMP+13:
        case JUMP+14:
        case JUMP+15:
            ++state;
        break;
        
        case JUMP+16:
            // draw_jump_dino_0(0);
            state=99;
        break;
    }
}


void display_score(void)
{
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINTD(0,0,4,score);
}


void display_hiscore(void)
{
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINTD(XSize-1-4,0,4,hiscore);

}



void handle_cactus_half_transition(uint8_t i)
{
    if(active_cactus[i] && x_cactus[i])
    {
        _XL_DRAW(x_cactus[i]-1,Y_CACTUS-1,TOP_LEFT_CACTUS,_XL_WHITE);
        _XL_DRAW(x_cactus[i]-1,Y_CACTUS,BOTTOM_LEFT_CACTUS,_XL_WHITE);
        _XL_DRAW(x_cactus[i],Y_CACTUS-1,TOP_RIGHT_CACTUS,_XL_WHITE);
        _XL_DRAW(x_cactus[i],Y_CACTUS,BOTTOM_RIGHT_CACTUS,_XL_WHITE);
    }
}


void handle_bird_half_transition(void)
{
    if(active_bird)
    {
        _XL_DRAW(x_bird,y_bird,LEFT_BIRD_0,_XL_WHITE);
        _XL_DRAW(x_bird+1,y_bird,LEFT_BIRD_1,_XL_WHITE);

    }
}

#define INITIAL_LOW_COLLISION_THRESHOLD 4
#define FINAL_LOW_COLLISION_THRESHOLD ((JUMP)+15)

void one_point(void)
{
    ++score;
    display_score();
    _XL_PING_SOUND();
}


void update_cactus(uint8_t i)
{
    _XL_DELETE(x_cactus[i],Y_CACTUS-1);
    _XL_DELETE(x_cactus[i],Y_CACTUS);
    
    if(active_cactus[i])
    {
        if(!x_cactus[i])
        {
            one_point();
            cactus_cooldown[i] = _XL_RAND()&15;
            active_cactus[i]=0;
            --number_of_active_cactus;
            return;
        }
        else
        {
            --x_cactus[i];
            _XL_DRAW(x_cactus[i],Y_CACTUS-1,TOP_CACTUS,_XL_WHITE);
            _XL_DRAW(x_cactus[i],Y_CACTUS,BOTTOM_CACTUS,_XL_WHITE);
        }
    }
}


void update_bird(void)
{
    // while(1){};
    _XL_DELETE(x_bird,y_bird);
    _XL_DELETE(x_bird+1,y_bird);
    
    // _XL_PRINTD(0,4,3,x_bird);

    if(active_bird)
    {
        if(!x_bird)
        {
            // while(1){};
            // _XL_DELETE(x_bird,y_bird);
            // _XL_DELETE(x_bird+1,y_bird);
            one_point();
            bird_cooldown = _XL_RAND()&31;
            active_bird = 0;
        // _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
        // _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
        // _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
        // _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
        // _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
            return;
        }
        else
        {
            --x_bird;
            _XL_DRAW(x_bird,y_bird,RIGHT_BIRD_0,_XL_WHITE);
            _XL_DRAW(x_bird+1,y_bird,RIGHT_BIRD_1,_XL_WHITE);
        }
    }
}





void handle_cactus(uint8_t i)
{
    if(cactus_cooldown[i])
    {
        --cactus_cooldown[i];
    }       
    else
    {
        update_cactus(i);
    }
}


void handle_bird(void)
{
    if(bird_cooldown)
    {
        --bird_cooldown;
    }       
    else
    {
        update_bird();
    }
}


uint8_t first_non_active_cactus(void)
{
    uint8_t i;
    
    for(i=0;i<level_cacti;++i)
    {
   // _XL_PRINTD(5*i,YSize-1,2,cactus_cooldown[i]);
   // _XL_PRINTD(10,YSize-1,2,active_cactus[i]);
   

        if((!cactus_cooldown[i])&&(!active_cactus[i]))
        {   
            return i;
        }
    }
    
    return NUMBER_OF_CACTI;
}



void spawn_cacti(void)
{
   uint8_t first_available_cactus = first_non_active_cactus();
   
   // _XL_PRINTD(0,YSize-1,2,first_available_cactus);
   
   if(first_available_cactus<NUMBER_OF_CACTI)
   {
       x_cactus[first_available_cactus] = RIGHT_END_OF_SCREEN;
       active_cactus[first_available_cactus]=1;
       ++number_of_active_cactus;
       last_active_cactus = first_available_cactus;
   }
}


void spawn_bird(void)
{
    uint8_t height = 2*(_XL_RAND()&1);

    if((!bird_cooldown)&&(!active_bird) && (x_cactus[last_active_cactus]<XSize/2))
    {
        y_bird = Y_DINO-height;

        active_bird = 1;
        x_bird = RIGHT_END_OF_SCREEN-1;
    }
}

#if defined(_XL_NO_JOYSTICK)
    #define PRESS_TO_START "PRESS A KEY"
#else
    #define PRESS_TO_START "PRESS START"
#endif
#define DELETE_PRESS       "           "

#define INITIAL_CACTUS_COOLDOWN 10

uint16_t counter;

void handle_enemy_spawn(void)
{
    if(!activate_speed) // && !disactivate_speed)
    {
        if((number_of_active_cactus<level_cacti) && !(counter&3) && (x_bird<XSize/2))
        {
            spawn_cacti();
        }
        else if(level_bird && ((counter&3)==2))
        {
            spawn_bird();
        }
    }
}

// TODO: Optimize for space
void handle_enemies(void)
{
    uint8_t i;
    
    if(speed)
    {
        if(counter&1)
        {
            for(i=0;i<level_cacti;++i)
            {
                handle_cactus(i);
            }
            if (level_bird)
            {
                handle_bird();
            }
        }
        else
        {
            for(i=0;i<level_cacti;++i)
            {
                handle_cactus_half_transition(i);
            }
            if (level_bird)
            {
                handle_bird_half_transition();
            }
        }
    }
    else
    {
        if(!(counter&3))
        {
            for(i=0;i<level_cacti;++i)
            {
                handle_cactus(i);
            }
            if (level_bird)
            {
                handle_bird();
            }
        }
        else if((counter&3)==2)
        {
            for(i=0;i<level_cacti;++i)
            {
                handle_cactus_half_transition(i);
            }
            if (level_bird)
            {
                handle_bird_half_transition();
            }
        }
    }
}


void handle_collisions(void)
{
    uint8_t i;
    
    if ((state<INITIAL_LOW_COLLISION_THRESHOLD)||(state>FINAL_LOW_COLLISION_THRESHOLD))
    {
        i=0;
        while((i<NUMBER_OF_CACTI)&&(!dead))
        {
            dead = x_cactus[i++]==X_DINO;
        }
        
        dead |= (x_bird==X_DINO) && ((y_bird==Y_DINO)||(y_bird==Y_DINO-1));
    }
    else
    {
        dead = (x_bird==X_DINO) && ((y_bird==Y_DINO-2)||(y_bird==Y_DINO-3));
    }
}


void draw_terrain(void)
{
    uint8_t i;
    
    for(i=0;i<XSize;++i)
    {
        _XL_DRAW(i,Y_TERRAIN,TERRAIN,_XL_WHITE);
    }       
}


void initialize_enemies(void)
{
    uint8_t i;
    
    for(i=0;i<NUMBER_OF_CACTI;++i)
    {
        x_cactus[i] = 0;
        active_cactus[i] = 0;
        cactus_cooldown[i] = INITIAL_CACTUS_COOLDOWN+(_XL_RAND()&16);
    }
    number_of_active_cactus = 0;
    
    x_bird = 0;
    active_bird = 0;
    bird_cooldown = INITIAL_CACTUS_COOLDOWN+_XL_RAND()&63;
}


void handle_game_over(void)
{
    _XL_DELETE(X_DINO,Y_DINO);

    _XL_DELETE(X_DINO,Y_DINO-1);
    _XL_DELETE(X_DINO,Y_DINO-2);
    _XL_DELETE(X_DINO,Y_DINO-3);
    _XL_DELETE(X_DINO,Y_DINO-4);
    _XL_DELETE(X_DINO,Y_DINO-5);

    _XL_DELETE(X_DINO+1,Y_DINO-1);
    _XL_DELETE(X_DINO+1,Y_DINO-2);
    _XL_DELETE(X_DINO+1,Y_DINO-3);
    _XL_DELETE(X_DINO+1,Y_DINO-4);
    _XL_DELETE(X_DINO+1,Y_DINO-5);
    
    draw_dead_dino_0();
    _XL_SHOOT_SOUND();
    _XL_SLEEP(1);
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(XSize/2-7, YSize/2-5, "G A M E  O V E R");
    _XL_SLEEP(1);
    _XL_WAIT_FOR_INPUT();
    if(score>hiscore)
    {
        hiscore = score;
    }
}


void initialize_player(void)
{
    state = 0;
    dead = 0;
    score = 0;
}


void handle_game_start(void)
{
    _XL_CLEAR_SCREEN();

    display_score();
    display_hiscore();
    draw_terrain();
    
    _XL_SLEEP(1);
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(XSize/2-5, YSize/2-3, PRESS_TO_START);
    _XL_SLOW_DOWN(10*_XL_SLOW_DOWN_FACTOR);
    draw_jump_dino_0(0);
    _XL_WAIT_FOR_INPUT();
    _XL_PRINT(XSize/2-5, YSize/2-3, DELETE_PRESS);
    _XL_PRINT(LEVEL_X,LEVEL_Y, "LEVEL 01");

}


void display_level(void)
{
    _XL_PRINTD(LEVEL_X+6, LEVEL_Y,2,level);
}


void activate_level(void)
{

    switch(level)
    {
        case 1:
            level_bird = 0;
            level_cacti = 1;
            // counter = 2048U-1024U;
            // level_bird = 1;
            // level_cacti = NUMBER_OF_CACTI;
            counter = 384U;
            break;
        case 2:
            level_bird = 1;
            level_cacti = 1;
            counter = 384U;
            break;
        case 3:
            level_bird = 1;
            level_cacti = 2;
            counter = 384U;
            break;
        case 4:
            level_bird = 1;
            level_cacti = 2;
            activate_speed = 1;
            // counter = 0;
            break;
        case 5:
            level_bird = 1;
            level_cacti = 3;
            // counter = 0;
            // disactivate_speed = 1;
            break;
        // case 6:
            // level_bird = 1;
            // level_cacti = 3;
            // break;
        default:
            level_bird = 1;
            level_cacti = NUMBER_OF_CACTI;
            // counter = 0;
    }
}


void handle_level(void)
{
    // _XL_PRINTD(0,YSize-1,3,counter);
    if(!(counter&1023))
    {
        ++level;
        display_level();
        
        activate_level();
    }
}


// #define INITIAL_LEVEL_COUNTER 200

void handle_speed(void)
{
    if(activate_speed)
    {
        if(!active_bird && !number_of_active_cactus)
        {
            speed = 1;
            activate_speed = 0;
        }            
    }
    // if(disactivate_speed)
    // {
        // if(!active_bird && !number_of_active_cactus)
        // {
            // speed = 0;
            // disactivate_speed = 0;
        // }  
    // }
}


int main(void)
{            
    _XL_INIT_GRAPHICS();
    
    _XL_INIT_SOUND();

    _XL_INIT_INPUT();

    hiscore = 0;
    
    while(1)
    {

        // counter = INITIAL_LEVEL_COUNTER;
        level = INITIAL_LEVEL;
        activate_speed = 0;
        // disactivate_speed = 0;
        speed = 0;
        
        activate_level();
        
        initialize_player();

        initialize_enemies();

        handle_game_start();

        while(!dead)
        {
            
            // _XL_PRINTD(0,3,2, number_of_active_cactus);
            // _XL_PRINTD(5,3,2, level_cacti);
            
            // _XL_PRINTD(10,3,1, active_bird);
            // _XL_PRINTD(15,3,1, level_bird);
            
            handle_state_behavior();
            handle_state_transition();
            
            ++counter;
            handle_enemies();
            
            handle_enemy_spawn();
            
            _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
            
            handle_collisions();
            
            handle_level();
            
            handle_speed();
            
        }
        handle_game_over();
    }

    return EXIT_SUCCESS;
}
