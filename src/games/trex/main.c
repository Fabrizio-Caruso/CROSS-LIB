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
#elif XSize<=28
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

#if YSize>=20

    #define Y_DINO ((YSize/2)+4)
#else
    #define Y_DINO ((YSize/2)+2)
#endif

#define Y_TERRAIN ((Y_DINO)+2)
#define Y_CACTUS ((Y_TERRAIN)-1)


#if YSize>17 && XSize>=20
    #define LEVEL_Y 5
#else
    #define LEVEL_Y 2
#endif

#define LEVEL_X ((XSize)/2-4)

#define NUMBER_OF_CACTI 3

#define MAX_LEVEL 99u
#define MAX_POINTS 9999u

uint8_t input;


uint8_t state;

uint8_t x_cactus[NUMBER_OF_CACTI];
uint8_t active_cactus[NUMBER_OF_CACTI];
uint8_t cactus_cooldown[NUMBER_OF_CACTI];
uint8_t number_of_active_cactus;
uint8_t last_active_cactus = 1;

#if !defined(TINY_GAME)
uint8_t x_bird;
uint8_t active_bird;
uint8_t y_bird;
uint8_t bird_cooldown;

uint8_t hilevel;

#endif

uint8_t dead;

uint16_t slowdown;

uint8_t set_speed;

uint16_t score;
uint16_t hiscore;

uint8_t level;

uint8_t level_cacti;
uint8_t level_bird;

uint16_t slowdown_factor;

uint8_t counter;


void draw_jump_dino_0(uint8_t height)
{

    // Initial tiles
    _XL_DRAW(X_DINO+1,Y_DINO-height,TOP_DINO_0,_XL_WHITE);
    _XL_DRAW(X_DINO+1,Y_DINO+1-height,BOTTOM_DINO_0,_XL_WHITE);
    _XL_DRAW(X_DINO,Y_DINO+1-height,TAIL_DINO_0,_XL_WHITE);

}    

#if !defined(TINY_GAME)
void draw_dead_dino_0(void)
{
    // Initial tiles
    _XL_DRAW(X_DINO+1,Y_DINO,TOP_DEAD_DINO_0,_XL_WHITE);
    _XL_DRAW(X_DINO+1,Y_DINO+1,BOTTOM_DINO_0,_XL_WHITE);
    _XL_DRAW(X_DINO,Y_DINO+1,TAIL_DINO_0,_XL_WHITE);
}   
#else
#define draw_dead_dino_0() 
#endif


#if !defined(TINY_GAME)
void draw_jump_dino_1(uint8_t height)
{
    _XL_DRAW(X_DINO+1,Y_DINO-1-height,TOP_DINO_1,_XL_WHITE);
    _XL_DRAW(X_DINO+1,Y_DINO-height,MIDDLE_DINO_1,_XL_WHITE);    
    _XL_DRAW(X_DINO+1,Y_DINO+1-height,BOTTOM_DINO_1,_XL_WHITE);
    _XL_DRAW(X_DINO,Y_DINO+1-1-height,TAIL_DINO_1,_XL_WHITE);
    _XL_DRAW(X_DINO,Y_DINO+1-height,FOOT_DINO_1,_XL_WHITE);    

}   
#else
    #define draw_jump_dino_1(height)
#endif

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

#if !defined(TINY_GAME)
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
#else
void draw_dino_feet_0(void)
{    
    _XL_DRAW(X_DINO+1,Y_DINO+1,BOTTOM_DINO_0,_XL_WHITE);
    _XL_DRAW(X_DINO,Y_DINO+1,TAIL_DINO_0,_XL_WHITE);
}    

#define draw_dino_feet_1() draw_dino_feet_0()

#endif

// #define RUN_SLOW_DOWN 4


#define JUMP 10

#define END_JUMP 17

void handle_state_behavior(void)
{

    switch(state)
    {

        case 0:  
            draw_dino_feet_1();
        break;
        
        case 1:
            draw_dino_feet_0();
        break;
        
        case JUMP+1:
            delete_feet(0);
            delete_feet(1);
            draw_jump_dino_0(1+1);
                
        break;
        
        case JUMP+2:
            delete_feet(2);
            draw_jump_dino_0(2+1);
                
        break;

        case JUMP+3:
            delete_feet(3);        
            draw_jump_dino_0(3+1);
                
        break;
        
        case JUMP+4:
            delete_feet(4);   
            draw_jump_dino_0(4+1);
        break;

        case JUMP+5:
            draw_jump_dino_1(4+1);
        break;

        case JUMP+6:
            delete_feet(5);
            draw_jump_dino_0(5+1);
        break;
        
        case JUMP+7:
            delete_feet(5);
            draw_jump_dino_1(5+1);
        break;

        case JUMP+8:

        break;
        
        case JUMP+9:
        break;

        case JUMP+10: // 7
            draw_jump_dino_1(5+1);
        break;
        
        case JUMP+11: // 6
            delete_top(6);
            draw_jump_dino_0(5+1);
        break;
        
        case JUMP+12: // 5
            draw_jump_dino_1(4+1);
        break;
        
        case JUMP+13: // 4
            delete_top(5);
            draw_jump_dino_0(4+1);
        break;
        
        case JUMP+14: // 3
            delete_top(4);
            draw_jump_dino_0(3+1);
        break;
        
        case JUMP+15: // 2
            delete_top(3);
            draw_jump_dino_0(2+1);
        break;
        
        case JUMP+16:
            delete_top(2);
            draw_jump_dino_0(1+1);
        break;

        case JUMP+END_JUMP:

            delete_top(0);
            delete_top(1);            
            draw_jump_dino_0(0);
        break;
        
        case 99: // like case 1
            draw_jump_dino_0(0);
            
        break;
    }
}

void handle_fire(void)
{
    input = _XL_INPUT();
    
    if(_XL_FIRE(input))
    {
        state = JUMP+1;
    }
}

void handle_state_transition(void)
{
    switch(state)
    {     
        case 0:

            state = 90;
            handle_fire();
        break;
        
        case 1:
            state = 91;
            handle_fire();
        break;
        
        case JUMP+END_JUMP:
            // draw_jump_dino_0(0);
            state=1;
        break;

        case 90:
            #if !defined(SLOWER_FEET)
            if(counter&1)
            #else
            if(!(counter&7))
            #endif
            {
                state = 1;
            }
            handle_fire();

        break;
        
        case 91:           
            #if !defined(SLOWER_FEET)
            if(counter&1)
            #else
            if(!(counter&7))
            #endif
            {
                state = 0;
            }
            handle_fire();
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
    _XL_PRINT(XSize-4-2,0,"HI");
    _XL_PRINTD(XSize-4,0,4,hiscore);

}


#if YSize >= 19
    #define HILEVEL_Y YSize-3
#else
    #define HILEVEL_Y YSize-2
#endif

#if !defined(TINY_GAME)
void display_hilevel(void)
{
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(XSize/2-8,HILEVEL_Y,"RECORD LEVEL");
    _XL_PRINTD(XSize/2-8+13,HILEVEL_Y,2,hilevel);

}
#else
#define display_hilevel()
#endif


#if !defined(NO_CACTUS_TRANSITION)
void handle_cactus_half_transition(uint8_t i)
{
    if(active_cactus[i])// && x_cactus[i])
    {
        if(i&1)
        {
            _XL_DRAW(x_cactus[i]-1,Y_CACTUS-1,TOP_LEFT_CACTUS,_XL_WHITE);
            _XL_DRAW(x_cactus[i],Y_CACTUS-1,TOP_RIGHT_CACTUS,_XL_WHITE);
            _XL_DRAW(x_cactus[i]-1,Y_CACTUS,BOTTOM_LEFT_CACTUS,_XL_WHITE);
            _XL_DRAW(x_cactus[i],Y_CACTUS,BOTTOM_RIGHT_CACTUS,_XL_WHITE);
        }
        else
        {
            _XL_DRAW(x_cactus[i]-1,Y_CACTUS-1,TOP_LEFT_CACTUS_2,_XL_WHITE);
            _XL_DRAW(x_cactus[i],Y_CACTUS-1,TOP_RIGHT_CACTUS_2,_XL_WHITE);
            _XL_DRAW(x_cactus[i]-1,Y_CACTUS,BOTTOM_LEFT_CACTUS_2,_XL_WHITE);
            _XL_DRAW(x_cactus[i],Y_CACTUS,BOTTOM_RIGHT_CACTUS_2,_XL_WHITE);
        }
    }
}
#endif

#if !defined(TINY_GAME)
    void handle_bird_half_transition(void)
    {
        if(active_bird)
        {
            _XL_DRAW(x_bird,y_bird,LEFT_BIRD_0,_XL_WHITE);
            _XL_DRAW(x_bird+1,y_bird,LEFT_BIRD_1,_XL_WHITE);
        }
    }
#else
    #define handle_bird_half_transition() 
#endif

#define INITIAL_LOW_COLLISION_THRESHOLD (JUMP+3)
#define FINAL_LOW_COLLISION_THRESHOLD ((JUMP)+END_JUMP-2)

void one_point(void)
{
    if(score<=MAX_POINTS)
    {
        ++score;
        display_score();
    }
    _XL_PING_SOUND();
}

#define BIRD_COOLDOWN_MASK 63U
#define CACTUS_COOLDOWN_MASK 31U

void draw_cactus_1(uint8_t x)
{
    _XL_DRAW(x,Y_CACTUS-1,TOP_CACTUS,_XL_WHITE);
    _XL_DRAW(x,Y_CACTUS,BOTTOM_CACTUS,_XL_WHITE);
}

#if !defined(TINY_GAME)
void draw_cactus_2(uint8_t x)
{
    _XL_DRAW(x,Y_CACTUS-1,TOP_CACTUS_2,_XL_WHITE);
    _XL_DRAW(x,Y_CACTUS,BOTTOM_CACTUS_2,_XL_WHITE);
}
#else
    #define draw_cactus_2(x) draw_cactus_1(x)
#endif

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

            if(i&1)
            {
                draw_cactus_1(x_cactus[i]);
            }
            else
            {
                draw_cactus_2(x_cactus[i]);
            }
        }
    }
}


#if !defined(TINY_GAME)
    void update_bird(void)
    {

        if(active_bird)
        {
            _XL_DELETE(x_bird,y_bird);
            _XL_DELETE(x_bird+1,y_bird);
            if(x_bird == LEFT_END_OF_TERRAIN)
            {
                x_bird = 0;

                one_point();
                bird_cooldown = (uint8_t) (_XL_RAND()&BIRD_COOLDOWN_MASK);
                active_bird = 0;
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
#else
    #define update_bird()
#endif



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

#if !defined(TINY_GAME)
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
#else
    #define handle_bird()
#endif

uint8_t first_non_active_cactus(void)
{
    uint8_t i;
    
    for(i=0;i<level_cacti;++i)
    {
        if((!cactus_cooldown[i])&&(!active_cactus[i]))
        {   
            return i;
        }
    }
    
    return NUMBER_OF_CACTI;
}

#if XSize>=22
    #define CACTUS_SAFE_DISTANCE ((XSize/2)+1)
    #define BIRD_SAFE_DISTACE (XSize/2)
#else
    #define CACTUS_SAFE_DISTANCE 10
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

#if !defined(TINY_GAME)
    void spawn_bird(void)
    {
        if((!bird_cooldown)&&(!active_bird) && (x_cactus[last_active_cactus]<BIRD_SAFE_DISTACE))
        {
            y_bird = (uint8_t) (Y_DINO-2*(_XL_RAND()&1));

            active_bird = 1;
            x_bird = RIGHT_END_OF_TERRAIN-1;
        }  
    }
#else
    #define spawn_bird()
#endif


#if defined(_XL_NO_JOYSTICK)
    #define PRESS_TO_FIRE "PRESS SPACE"
#else
    #define PRESS_TO_FIRE "PRESS FIRE"
#endif
#define DELETE_PRESS       "           "

#define INITIAL_CACTUS_COOLDOWN 3

#if !defined(TINY_GAME)
void handle_enemy_spawn(void)
{
    if(!(counter&3) && (number_of_active_cactus<level_cacti) && (x_bird<XSize/2) &&(!set_speed) )
    {
        spawn_cacti();
    }
    else if(level_bird && !set_speed)
    {
        spawn_bird();
    }
}
#else
void handle_enemy_spawn(void)
{
    if(!(counter&3) && (number_of_active_cactus<level_cacti))
    {
        spawn_cacti();
    }
}    
#endif

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
        #if !defined(TINY_GAME)
        if (level_bird)
        {
            handle_bird();
        }
        #endif
    }
    else
    {
        #if !defined(NO_CACTUS_TRANSITION)
        for(i=0;i<level_cacti;++i)
        {
            handle_cactus_half_transition(i);
        }
        #endif
        if (level_bird)
        {
            handle_bird_half_transition();
        }
    }
}


// TODO: y_bird has only 2 values
#if !defined(TINY_GAME)
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
#else
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
    }
}  
#endif

#if !defined(TINY_GAME)
void draw_terrain(void)
{
    uint8_t i;
    uint8_t r;
    
    for(i=LEFT_END_OF_TERRAIN;i<RIGHT_END_OF_TERRAIN;++i)
    {
        r = (uint8_t)(_XL_RAND()&3);
        
        if(!r)
        {
            _XL_DRAW(i,Y_TERRAIN,TERRAIN_2,_XL_WHITE);
        }
        else if(r==1)
        {
            _XL_DRAW(i,Y_TERRAIN,TERRAIN_3,_XL_WHITE);
        }
        else if(!(i&1))
        {
            _XL_DRAW(i,Y_TERRAIN,TERRAIN_3,_XL_WHITE);
        }
        else
        {
            _XL_DRAW(i,Y_TERRAIN,TERRAIN,_XL_WHITE);
        }
    }       
}
#else
    
void draw_terrain(void)
{
    uint8_t i;
    
    for(i=LEFT_END_OF_TERRAIN;i<RIGHT_END_OF_TERRAIN;++i)
    {
        _XL_DRAW(i,Y_TERRAIN,TERRAIN,_XL_WHITE);
    }       
}
#endif

#if !defined(TINY_GAME)
void draw_cacti(void)
{
    draw_cactus_1(RIGHT_END_OF_TERRAIN-2);
    draw_cactus_2(RIGHT_END_OF_TERRAIN-4);
    draw_cactus_1(RIGHT_END_OF_TERRAIN-6);
}

void delete_cacti(void)
{
    _XL_DELETE(RIGHT_END_OF_TERRAIN-2,Y_CACTUS-1);
    _XL_DELETE(RIGHT_END_OF_TERRAIN-4,Y_CACTUS-1);
    _XL_DELETE(RIGHT_END_OF_TERRAIN-6,Y_CACTUS-1);
    
    _XL_DELETE(RIGHT_END_OF_TERRAIN-2,Y_CACTUS);
    _XL_DELETE(RIGHT_END_OF_TERRAIN-4,Y_CACTUS);
    _XL_DELETE(RIGHT_END_OF_TERRAIN-6,Y_CACTUS);
}
#else
    #define draw_cacti() 

    #define delete_cacti()
#endif

void initialize_enemies(void)
{
    uint8_t i;
    
    for(i=0;i<NUMBER_OF_CACTI;++i)
    {
        x_cactus[i] = 0;
        active_cactus[i] = 0;
        cactus_cooldown[i] = (uint8_t) (INITIAL_CACTUS_COOLDOWN);
    }
    number_of_active_cactus = 0;
    
    #if !defined(TINY_GAME)
    x_bird = 0;
    active_bird = 0;
    bird_cooldown = (uint8_t) (INITIAL_CACTUS_COOLDOWN);
    #endif
}


    #if XSize>16
        #if !defined(TINY_GAME)
            #define GAME_OVER_Y XSize/2-9
        
        #else
            #define GAME_OVER_Y XSize/2-4
        #endif
    #else
        #define GAME_OVER_Y 0
    #endif 


void handle_game_over(void)
{
    uint8_t i;
    uint8_t j;
    
    // _XL_DELETE(X_DINO,Y_DINO);

    for(i=1;i<8;++i)
    {
        for(j=1;j<3;++j)
        {
            _XL_DELETE(X_DINO-1+j,Y_DINO-i);
        }
    }

    #if !defined(TINY_GAME)
    draw_dead_dino_0();
    _XL_REFRESH();
    _XL_SHOOT_SOUND();
    _XL_SLEEP(1);
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(GAME_OVER_Y, 7, "G A M E  O V E R");
    _XL_REFRESH();
    _XL_SLEEP(1);
    _XL_WAIT_FOR_INPUT();
    #else
    _XL_SHOOT_SOUND();
    _XL_PRINT(GAME_OVER_Y, 7, "GAME OVER");
    _XL_REFRESH();
    _XL_WAIT_FOR_INPUT();
    #endif
    if(score>hiscore)
    {
        hiscore = score;
    }
    #if !defined(TINY_GAME)
    if(level>hilevel)
    {
        hilevel = level;
    }
    #endif
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

#if !defined(TINY_GAME)
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
    
    draw_cacti();
    
    
    _XL_SLOW_DOWN(10*_XL_SLOW_DOWN_FACTOR);
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(LEVEL_X+1,  0,     "TREX");
    _XL_PRINT(LEVEL_X+1+1,1,      "BY");
    _XL_PRINT(LEVEL_X+1-4,2, "FABRIZIO CARUSO");

    display_hilevel();
    
    _XL_SLOW_DOWN(10*_XL_SLOW_DOWN_FACTOR);
    _XL_PRINT(XSize/2-6, YSize/2-3, PRESS_TO_FIRE);
    _XL_SLOW_DOWN(10*_XL_SLOW_DOWN_FACTOR);
    draw_jump_dino_0(0);
    _XL_REFRESH();
    x_bird = RIGHT_END_OF_TERRAIN;
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
        }
        else
        {  
            _XL_DELETE(x_bird,y_bird);
            _XL_DELETE(x_bird+1,y_bird);

            x_bird = RIGHT_END_OF_TERRAIN;
            y_bird = (uint8_t) (Y_DINO-1-2*((_XL_RAND())&1));
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
    
    delete_cacti();
    
    _XL_PRINT(LEVEL_X+1+1,1,      "  ");

    _XL_PRINT(LEVEL_X+1-4,2, "               ");

    _XL_PRINT(XSize/2-6, YSize/2-3, DELETE_PRESS);
    _XL_PRINT(LEVEL_X,LEVEL_Y, "LEVEL");
    display_level();
    _XL_REFRESH();
}
#else
    void handle_game_start(void)
    {
        _XL_CLEAR_SCREEN();
        _XL_REFRESH();
        counter = 0;

        display_score();
        display_hiscore();
        draw_terrain();
        
        // draw_cacti();
        
        
        _XL_SLOW_DOWN(10*_XL_SLOW_DOWN_FACTOR);
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINT(LEVEL_X+1,  0,     "TREX");
        // _XL_PRINT(LEVEL_X+1+1,1,      "BY");
        _XL_PRINT(LEVEL_X+1-4,2, "FABRIZIO CARUSO");
        
        _XL_PRINT(LEVEL_X,LEVEL_Y, "LEVEL");
        display_level();
        _XL_WAIT_FOR_INPUT();
        _XL_PRINT(LEVEL_X+1+1,1,      "  ");

        _XL_PRINT(LEVEL_X+1-4,2, "               ");
        _XL_REFRESH();
    }
#endif

#define LEVEL_SIZE  256U

#if !defined(TINY_GAME)
void birdAndTwoCacti(void)
{
    level_bird = 1;
    level_cacti = 2;
}

void birdAndThreeCacti(void)
{
    level_bird = 1;
    level_cacti = 3;
}
#endif

#if !defined(TINY_GAME)
void activate_level(void)
{
    set_speed = 1;
    switch(level)
    {
        case 1:
            level_bird = 0;
            level_cacti = 1;
            slowdown_factor = 20;
            break;
        case 2:
            level_bird = 1;
            level_cacti = 1;
            slowdown_factor = 19;
            break;
        case 3:
            birdAndTwoCacti();
            slowdown_factor = 18; 
            break;
        case 4:
            birdAndTwoCacti();
            slowdown_factor = 16;
            break;
        case 5:
            birdAndTwoCacti();
            slowdown_factor = 15;
            break;
        case 6:
            birdAndTwoCacti();
            slowdown_factor = 14;
            break;
        case 7:
            birdAndThreeCacti();
            slowdown_factor = 14;
            break;
        case 8:
            birdAndThreeCacti();
            slowdown_factor = 13;
            break;
        case 9:
            birdAndThreeCacti();
            slowdown_factor = 12;
            break;
        case 10:
            birdAndThreeCacti();
            slowdown_factor = 11;
            break;
        case 11:
        case 12:
            birdAndThreeCacti();
            slowdown_factor = 10;
            break;
        case 13:
        case 14:
        case 15:
            birdAndThreeCacti();
            slowdown_factor = 9;
            break;
        case 16: 
        case 17:
        case 18:
        case 19:
            birdAndThreeCacti();
            slowdown_factor = 8;
            break;
        case 20:
        case 21:
        case 22:
        case 23:
        case 24:
            birdAndThreeCacti();
            slowdown_factor = 7;
            break;
        case 25:
        case 26:
        case 27:
        case 28:
        case 29:
            birdAndThreeCacti();
            slowdown_factor = 6;
            break;
        default:
            birdAndThreeCacti();
            slowdown_factor = 5;

            // counter = 0;
    }
}
#else
    void activate_level(void)
    {
        set_speed = 1;
        if(level<3)
        {
            level_cacti = 2;
            slowdown_factor = 16;
        }
        else if(level<=11)
        {
            level_cacti = 3;
            slowdown_factor = 17-level; 
        }
        else
        {
            level_cacti=3;
            slowdown_factor = 5;
        }
        // slowdown_factor = 20;
        
    }
#endif


void handle_level(void)
{
    if(!counter) 
    {
        if(level<MAX_LEVEL)
        {
            ++level;
        
            display_level();
            
            activate_level();
        }
    }
}


#if !defined(TINY_GAME)
void handle_speed(void)
{
    if(set_speed)
    {
        if(!active_bird && !number_of_active_cactus)
        {
            slowdown = ((uint16_t) slowdown_factor)*((uint16_t)_XL_SLOW_DOWN_FACTOR/2);
            set_speed = 0;
        }            
    }
}
#else
void handle_speed(void)
{
    if(set_speed)
    {
        if(!number_of_active_cactus)
        {
            slowdown = ((uint16_t) slowdown_factor)*((uint16_t)_XL_SLOW_DOWN_FACTOR/2);
            set_speed = 0;
        }            
    }
}   
#endif


int main(void)
{            
    _XL_INIT_GRAPHICS();
    
    _XL_INIT_SOUND();

    _XL_INIT_INPUT();

    hiscore = 0;
    #if !defined(TINY_GAME)
    hilevel = 1;
    #endif
    
    while(1)
    {

        level = INITIAL_LEVEL;
        
        initialize_player();

        handle_game_start();

        initialize_enemies();


        activate_level();
        while(!dead)
        {
            // _XL_PRINTD(0,4,2,counter);
            
            handle_state_behavior();

            handle_state_transition();
            
            ++counter;
      
            handle_enemy_spawn();
            
            handle_enemies();

            _XL_SLOW_DOWN(slowdown);
                        
            handle_collisions();
            
            handle_level();
            
            handle_speed();
            
            _XL_WAIT_VSYNC();
            _XL_REFRESH();
            
        }
        _XL_REFRESH();
        handle_game_over();
    }

    return EXIT_SUCCESS;
}
