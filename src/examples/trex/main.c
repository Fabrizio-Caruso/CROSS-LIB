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

#if XSize<=21
    #define LEFT_END_OF_TERRAIN 1

    #define SIZE_OF_TERRAIN ((XSize)-2)
#elif XSize<=32
    #define LEFT_END_OF_TERRAIN 1

    #define SIZE_OF_TERRAIN ((XSize)-3)
#else
    #define LEFT_END_OF_TERRAIN ((XSize)/6)

    #define SIZE_OF_TERRAIN (2*((XSize)/3))
    
#endif

#define RIGHT_END_OF_TERRAIN (LEFT_END_OF_TERRAIN+SIZE_OF_TERRAIN)

#if XSize>=22
    #define X_DINO ((XSize/8)+LEFT_END_OF_TERRAIN)
#else
    #define X_DINO (1+LEFT_END_OF_TERRAIN) 
#endif

// TODO: ((YSize/2)+4) crashes on the Vic 20 when a cactus reaches LEFT_END_OF_TERRAIN
#define Y_DINO ((YSize/2)+4)


#define Y_TERRAIN ((Y_DINO)+2)
#define Y_CACTUS ((Y_TERRAIN)-1)


#define LEVEL_Y 5
#define LEVEL_X ((XSize)/2-5)

#define NUMBER_OF_CACTI 3

#define MAX_LEVEL 99

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

uint16_t slowdown;
uint8_t set_speed;
// uint8_t disactivate_speed;

uint16_t score;
uint16_t hiscore;

uint8_t level;
uint8_t hilevel;

uint8_t level_cacti;
uint8_t level_bird;

uint8_t slowdown_factor;
// uint8_t previous_state;


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


#define JUMP 1

#define END_JUMP 16

void handle_state_behavior(void)
{
    switch(state)
    {

        case 0:
            // draw_jump_dino_0(0);

            // delete_feet(0);
            
            draw_dino_feet_1();
        break;
        
        case 1:
            // delete_feet(0);
            
            draw_dino_feet_0();
        break;
        
        // case JUMP:
                // delete_feet(0);
                
                // draw_jump_dino_0(0+1);
                
        // break;
        
        case JUMP+1:
                delete_feet(0);
                // draw_jump_dino_1(0+1);
                delete_feet(1);
                
                draw_jump_dino_0(1+1);
                
        break;
        
        case JUMP+2:
                // draw_jump_dino_1(1+1);

                delete_feet(2);
                
                draw_jump_dino_0(2+1);
                
        break;
        
        case JUMP+3:
                // draw_jump_dino_1(2+1);

                delete_feet(3);
                
                draw_jump_dino_0(3+1);
                
        break;
        
        case JUMP+4:
                // draw_jump_dino_1(3+1);

                delete_feet(4);
                
                draw_jump_dino_0(4+1);
                
        break;

        case JUMP+5:
                // draw_jump_dino_1(4+1);

                delete_feet(5);
                
                draw_jump_dino_0(5+1);
                
        break;
        
        case JUMP+6:
            // delete_feet(5);
            
            draw_jump_dino_2(5+1);
        break;

        case JUMP+7:
            delete_top(6);
            draw_jump_dino_0(6);
        break;
        
        case JUMP+8:
            delete_top(5);
            draw_jump_dino_0(5);
        break;
        
        case JUMP+9:
            // delete_top(4);
            draw_jump_dino_1(4);
        break;
        
        case JUMP+10:
            delete_top(4);
            draw_jump_dino_0(4);
        break;
        
        case JUMP+11:
            // delete_top(3);
            draw_jump_dino_1(3);
        break;
        
        case JUMP+12:
            delete_top(3);
            draw_jump_dino_0(3);
        break;
        
        case JUMP+13:
            // delete_top(2);
            draw_jump_dino_1(2);
        break;
        
        case JUMP+14:
            delete_top(2);
            draw_jump_dino_0(2);
        break;
        
        case JUMP+15:
            // delete_top(1);
            draw_jump_dino_1(1);
        break;
        
        // case JUMP+16:
            // delete_top(1);
            // draw_jump_dino_0(1);
        // break;
        
        // case JUMP+17:
            // delete_top(0);
            // draw_jump_dino_1(0);
        // break;

        
        case JUMP+END_JUMP:

            delete_top(0);
            // _XL_SLEEP(1);
            delete_top(1);
            // _XL_SLEEP(1);
            
            draw_jump_dino_0(0);

            // delete_feet(0);            
            //_XL_SLEEP(1);

            
            // draw_dino_feet_0();            
            //_XL_SLEEP(1);

        break;
        
        case 99: // like case 1
            draw_jump_dino_0(0);
            
            // draw_dino_feet_0();
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
                state = JUMP+1;
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
                state = JUMP+1;
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
                state = JUMP+1;
            }
            else
            {
                state = 0;
            }
        break;
        
        case JUMP+END_JUMP:
            // draw_jump_dino_0(0);
            state=99;
        break;
        
        default:
            ++state;
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
    _XL_PRINT(XSize-1-4-2,0,"HI");
    _XL_PRINTD(XSize-1-4,0,4,hiscore);

}

void display_hilevel(void)
{
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(XSize/2-8,YSize-3,"RECORD LEVEL");
    _XL_PRINTD(XSize/2-8+13,YSize-3,2,hilevel);

}



void handle_cactus_half_transition(uint8_t i)
{
    if(active_cactus[i])// && x_cactus[i])
    {
        _XL_DRAW(x_cactus[i]-1,Y_CACTUS-1,TOP_LEFT_CACTUS,_XL_WHITE);
        _XL_DRAW(x_cactus[i],Y_CACTUS-1,TOP_RIGHT_CACTUS,_XL_WHITE);
        _XL_DRAW(x_cactus[i]-1,Y_CACTUS,BOTTOM_LEFT_CACTUS,_XL_WHITE);
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

#define INITIAL_LOW_COLLISION_THRESHOLD 5
#define FINAL_LOW_COLLISION_THRESHOLD ((JUMP)+END_JUMP-2)

void one_point(void)
{
    ++score;
    display_score();
    _XL_PING_SOUND();
}

#define BIRD_COOLDOWN_MASK 63U
#define CACTUS_COOLDOWN_MASK 31U

void update_cactus(uint8_t i)
{
    if(active_cactus[i])
    {
        _XL_DELETE(x_cactus[i],Y_CACTUS-1);
        _XL_DELETE(x_cactus[i],Y_CACTUS);
        if(x_cactus[i]==LEFT_END_OF_TERRAIN)
        {
            _XL_DELETE(LEFT_END_OF_TERRAIN-1,Y_CACTUS-1);            
            _XL_DELETE(LEFT_END_OF_TERRAIN-1,Y_CACTUS);

            x_cactus[i]=0;

            // while(1){};
            one_point();
            if(!(_XL_RAND()&3))
            {
                cactus_cooldown[i] = SIZE_OF_TERRAIN;
            }
            else
            {
                cactus_cooldown[i] = (uint8_t) (_XL_RAND()&CACTUS_COOLDOWN_MASK);
            }
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
    // if(!y_bird)
        // return;

    
    // _XL_PRINTD(0,4,3,x_bird);

    if(active_bird)
    {
        _XL_DELETE(x_bird,y_bird);
        _XL_DELETE(x_bird+1,y_bird);
        if(x_bird == LEFT_END_OF_TERRAIN)
        {
            x_bird = 0;
            // while(1){};
            // _XL_DELETE(x_bird,y_bird);
            // _XL_DELETE(x_bird+1,y_bird);
            one_point();
            bird_cooldown = (uint8_t) (_XL_RAND()&BIRD_COOLDOWN_MASK);
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

#if XSize>=22
    #define CACTUS_SAFE_DISTANCE ((XSize/2)+2)
    #define BIRD_SAFE_DISTACE (XSize/2)
#else
    #define CACTUS_SAFE_DISTANCE 11
    #define BIRD_SAFE_DISTACE 9
#endif

#define last_is_far() (x_cactus[last_active_cactus]<CACTUS_SAFE_DISTANCE)
#define last_is_very_close()(x_cactus[last_active_cactus]>RIGHT_END_OF_TERRAIN-3)

#define is_cactus_available() (first_available_cactus<NUMBER_OF_CACTI)

void spawn_cacti(void)
{

   if(last_is_far()||last_is_very_close())
   {
        uint8_t first_available_cactus = first_non_active_cactus();

        if(is_cactus_available())
        {
            x_cactus[first_available_cactus] = RIGHT_END_OF_TERRAIN;
            active_cactus[first_available_cactus]=1;
            ++number_of_active_cactus;
            last_active_cactus = first_available_cactus;
        }
   }
}


void spawn_bird(void)
{
    if((!bird_cooldown)&&(!active_bird) && (x_cactus[last_active_cactus]<BIRD_SAFE_DISTACE))
    {
        y_bird = (uint8_t) (Y_DINO-2*(_XL_RAND()&1));

        active_bird = 1;
        x_bird = RIGHT_END_OF_TERRAIN-1;
    }
    
}

#if defined(_XL_NO_JOYSTICK)
    #define PRESS_TO_START "PRESS A KEY"
#else
    #define PRESS_TO_START "PRESS START"
#endif
#define DELETE_PRESS       "           "

#define INITIAL_CACTUS_COOLDOWN 5

uint8_t counter;

void handle_enemy_spawn(void)
{
    // if(!set_speed) 
    // {
        if(!(counter&3) && (number_of_active_cactus<level_cacti) && (x_bird<XSize/2) &&(!set_speed) )
        {
            spawn_cacti();
        }
        else if(level_bird && !set_speed)// && ((counter&3)==2))
        {
            spawn_bird();
        }
    // }
}

// void handle_enemy_spawn(void)
// {
    // if(!set_speed) // && !disactivate_speed)
    // {
        // if((number_of_active_cactus<level_cacti) && !(counter&3) && (x_bird<XSize/2))
        // {
            // spawn_cacti();
        // }
        // else if(level_bird && ((counter&3)==2))
        // {
            // spawn_bird();
        // }
    // }
// }

// TODO: Optimize for space
void handle_enemies(void)
{
    uint8_t i;
    
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


// TODO: y_bird has only 2 values
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
        
        dead |= (x_bird==X_DINO) && ((y_bird==Y_DINO));
    }
    else
    {
        dead = (x_bird==X_DINO) && ((y_bird==Y_DINO-2));
    }
}


void draw_terrain(void)
{
    uint8_t i;
    
    for(i=LEFT_END_OF_TERRAIN;i<RIGHT_END_OF_TERRAIN;++i)
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
        cactus_cooldown[i] = (uint8_t) (INITIAL_CACTUS_COOLDOWN+(_XL_RAND()&16));
    }
    number_of_active_cactus = 0;
    
    x_bird = 0;
    active_bird = 0;
    bird_cooldown = (uint8_t) (INITIAL_CACTUS_COOLDOWN+(_XL_RAND()&63));
}


void handle_game_over(void)
{
    _XL_DELETE(X_DINO,Y_DINO);

    _XL_DELETE(X_DINO,Y_DINO-1);
    _XL_DELETE(X_DINO,Y_DINO-2);
    _XL_DELETE(X_DINO,Y_DINO-3);
    _XL_DELETE(X_DINO,Y_DINO-4);
    _XL_DELETE(X_DINO,Y_DINO-5);
    _XL_DELETE(X_DINO,Y_DINO-6);
    _XL_DELETE(X_DINO,Y_DINO-7);
    
    _XL_DELETE(X_DINO+1,Y_DINO-1);
    _XL_DELETE(X_DINO+1,Y_DINO-2);
    _XL_DELETE(X_DINO+1,Y_DINO-3);
    _XL_DELETE(X_DINO+1,Y_DINO-4);
    _XL_DELETE(X_DINO+1,Y_DINO-5);
    _XL_DELETE(X_DINO+1,Y_DINO-6);
    _XL_DELETE(X_DINO+1,Y_DINO-7);
    
    draw_dead_dino_0();
    _XL_SHOOT_SOUND();
    _XL_SLEEP(1);
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(XSize/2-9, 7, "G A M E  O V E R");
    _XL_SLEEP(1);
    _XL_WAIT_FOR_INPUT();
    if(score>hiscore)
    {
        hiscore = score;
    }
    if(level>hilevel)
    {
        hilevel = level;
    }
}


void initialize_player(void)
{
    // last_state = 0;
    state = 0;
    dead = 0;
    score = 0;
}


void display_level(void)
{
    _XL_PRINTD(LEVEL_X+6, LEVEL_Y,2,level);
}


void handle_game_start(void)
{
    uint8_t start;
    
    start = 0;
    counter = 0;
    _XL_CLEAR_SCREEN();
    _XL_REFRESH();
    
    display_score();
    display_hiscore();
    draw_terrain();
    
    _XL_SLOW_DOWN(10*_XL_SLOW_DOWN_FACTOR);
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(LEVEL_X+1,  0,     "TREX");
    _XL_PRINT(LEVEL_X+1+1,1,      "BY");
    _XL_PRINT(LEVEL_X+1-4,2, "FABRIZIO CARUSO");
    _XL_PRINT(LEVEL_X+1-3,YSize-1, "BETA VERSION");

    display_hilevel();
    
    _XL_SLOW_DOWN(10*_XL_SLOW_DOWN_FACTOR);
    _XL_PRINT(XSize/2-6, YSize/2-3, PRESS_TO_START);
    _XL_SLOW_DOWN(10*_XL_SLOW_DOWN_FACTOR);
    draw_jump_dino_0(0);
    _XL_REFRESH();
    x_bird = 0;
    y_bird = Y_DINO-1;
    while(!start || x_bird>LEFT_END_OF_TERRAIN)
    {
        ++counter;
        if(_XL_FIRE(_XL_INPUT()))
        {
            start = 1;
        }
        if(x_bird>LEFT_END_OF_TERRAIN)
        {
            if(counter&1)
            {
                --x_bird;
            }
        }
        else
        {  
            _XL_DELETE(x_bird,y_bird);
            _XL_DELETE(x_bird+1,y_bird);

            x_bird = RIGHT_END_OF_TERRAIN;
            y_bird = (uint8_t) (Y_DINO-1-2*((_XL_RAND())&1));
        }
        if(counter&1)
        {
            _XL_DELETE(x_bird+2,y_bird);

            _XL_DRAW(x_bird,y_bird,RIGHT_BIRD_0,_XL_WHITE);
            _XL_DRAW(x_bird+1,y_bird,RIGHT_BIRD_1,_XL_WHITE);
        }
        else
        {
            _XL_DRAW(x_bird,y_bird,LEFT_BIRD_0,_XL_WHITE);
            _XL_DRAW(x_bird+1,y_bird,LEFT_BIRD_1,_XL_WHITE);
        }
        
        if(start)
        {
            _XL_SLOW_DOWN(2*_XL_SLOW_DOWN_FACTOR);
        }
        else
        {
            _XL_SLOW_DOWN(6*_XL_SLOW_DOWN_FACTOR);
        }
        _XL_WAIT_VSYNC();
        _XL_REFRESH();
    }
    _XL_DELETE(x_bird,y_bird);
    _XL_DELETE(x_bird+1,y_bird);
    
    _XL_PRINT(LEVEL_X+1+1,1,      "  ");

    _XL_PRINT(LEVEL_X+1-4,2, "               ");

    _XL_PRINT(XSize/2-6, YSize/2-3, DELETE_PRESS);
    _XL_PRINT(LEVEL_X,LEVEL_Y, "LEVEL");
    display_level();
    _XL_REFRESH();
}


#define LEVEL_SIZE  256U


void activate_level(void)
{
    set_speed = 1;
    switch(level)
    {
        case 1:
            level_bird = 0;
            level_cacti = 1;
            // counter = 2048U-1024U;
            // level_bird = 1;
            // level_cacti = NUMBER_OF_CACTI;
            // counter = 1;
            slowdown_factor = 10;
            // speed = 10;

            break;
        case 2:
            level_bird = 1;
            level_cacti = 1;
            // counter = LEVEL_SIZE/2;

            slowdown_factor = 10;
            break;
        case 3:
            level_bird = 1;
            level_cacti = 2;
            slowdown_factor = 9; 
            // counter = LEVEL_SIZE/2;
            break;
        case 4:
            level_bird = 1;
            level_cacti = 2;
            slowdown_factor = 8;
            // counter = LEVEL_SIZE/2;
            // disactivate_speed = 1;
            break;
        case 5:
            level_bird = 1;
            level_cacti = 2;
            slowdown_factor = 7;
            // counter = LEVEL_SIZE/2;
            break;
        case 6:
            level_bird = 1;
            level_cacti = 2;
            slowdown_factor = 6;
            // counter = LEVEL_SIZE/4;
            break;
        case 7:
            level_bird = 1;
            level_cacti = 2;
            slowdown_factor = 5;
            // counter = LEVEL_SIZE/4;
            break;
        case 8:
            level_bird = 1;
            level_cacti = 2;
            slowdown_factor = 4;
            break;
        case 9:
            level_bird = 1;
            level_cacti = 3;
            slowdown_factor = 4;
            break;
        default:
            level_bird = 1;
            level_cacti = 3;
            slowdown_factor = 3;

            // counter = 0;
    }
}


void handle_level(void)
{
    // _XL_PRINTD(0,YSize-1,3,counter);
    if(!counter) //!(counter&(LEVEL_SIZE-1)))
    {
        if(level<MAX_LEVEL)
        {
            ++level;
        
            display_level();
            
            activate_level();
        }
        // _XL_SLEEP(3);
    }
}


// #define LEVEL_SIZE 200

void handle_speed(void)
{
    if(set_speed)
    {
        if(!active_bird && !number_of_active_cactus)
        {
            // ++slowdown;
            slowdown = slowdown_factor*_XL_SLOW_DOWN_FACTOR;
            set_speed = 0;
        }            
    }

}


int main(void)
{            
    _XL_INIT_GRAPHICS();
    
    _XL_INIT_SOUND();

    _XL_INIT_INPUT();

    hiscore = 0;
    hilevel = 1;
    
    while(1)
    {

        // counter = LEVEL_SIZE;
        level = INITIAL_LEVEL;
        
        #if INITIAL_LEVEL>=3
            // activate_speed = 1;
            // speed = 1;
        #else
            // activate_speed = 0;
            // speed = 0;
        #endif
        
        // activate_speed = 0;

        // disactivate_speed = 0;
        // slowdown = 3*_XL_SLOW_DOWN_FACTOR;
        
        
        initialize_player();

        initialize_enemies();

        handle_game_start();
            // _XL_PRINTD(0,YSize-1,5, slowdown_factor);

        activate_level();
        while(!dead)
        {
            
            // _XL_PRINTD(0,3,2, counter);
            // _XL_PRINTD(5,3,2, level_cacti);
            
            // _XL_PRINTD(10,3,1, speed);

            // _XL_PRINTD(15,3,1, y_bird);
            
            handle_state_behavior();
            // _XL_WAIT_VSYNC();

            handle_state_transition();
            
            ++counter;
            // _XL_WAIT_VSYNC();
      
            handle_enemy_spawn();
            
            handle_enemies();

            _XL_SLOW_DOWN(slowdown);
                        
            handle_collisions();
            
            handle_level();
            
            handle_speed();
            
            _XL_WAIT_VSYNC();
            _XL_REFRESH();
            
        }
        handle_game_over();
    }

    return EXIT_SUCCESS;
}
