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

#define WORD_SIZE 5


#define MAX_HEIGHT ((YSize/2)-1)


#define START_X ((XSize)/2-4)
#define START_Y ((YSize)-3)

#define MIN_PLAYER_X 0
#define MAX_PLAYER_X (1+WORD_SIZE)

#define PLAYER_Y (START_Y+2)


#define END_Y (START_Y+10)

#define VERTICAL_PLAYER_TILE         _TILE_0
#define UP_ARROW_TILE                _TILE_5
#define DOWN_ARROW_TILE              _TILE_7
#define LEFT_ARROW_TILE              _TILE_3 
#define RIGHT_ARROW_TILE             _TILE_4
#define HORIZONTAL_LEFT_PLAYER_TILE  _TILE_2
#define HORIZONTAL_RIGHT_PLAYER_TILE _TILE_8
#define VERTICAL_WALL_TILE           _TILE_12
#define HORIZONTAL_WALL_TILE         _TILE_13
#define EMPTY_SLOT_TILE              _TILE_1
#define HORIZONTAL_BAR_TILE          _TILE_11
#define VERTICAL_BAR_TILE            _TILE_15
#define SCORE_LHS_TILE               _TILE_14
#define SCORE_RHS_TILE               _TILE_16
#define HI_TILE                      _TILE_6
#define LV_TILE                      _TILE_9
#define LEFT_LEFT_TILE               _TILE_10
#define LEFT_RIGHT_TILE              _TILE_17
#define BORDER_TILE                  _TILE_18
#define CROSS_TILE                   _TILE_19
#define RING_TILE                    _TILE_20

#define PLAYER_COLOR _XL_WHITE
#define EMPTY_SLOT_COLOR _XL_WHITE
#define WALL_COLOR _XL_RED

#if YSize<=18
    #define INITIAL_ROWS 2
#else
    #define INITIAL_ROWS 3
#endif

#define INITIAL_DROP ((WORD_SIZE)*INITIAL_ROWS)

#define NO_OF_PRECOMPUTED_WORDS 3

#define SIZE_OF_PRECOMPUTED_WORDS ((NO_OF_PRECOMPUTED_WORDS)*(WORD_SIZE))

#define NO_OF_RANDOM_LETTERS 2
//SIZE_OF_PRECOMPUTED_WORDS

#define NO_OF_PRECOMPUTED_LETTERS ((SIZE_OF_PRECOMPUTED_WORDS)+(NO_OF_RANDOM_LETTERS))


#define LEVEL_X (XSize-2)
#define LEVEL_Y 0

#define REMAINING_WORD_X ((LEVEL_X)-4)
#define REMAINING_WORD_Y 0

#if XSize<=22
    #define HI_X (REMAINING_WORD_X-6)
#else
    #define HI_X (((REMAINING_WORD_X)/2))
#endif

#define HI_Y 0

#define SLOT_SPACING 2

// TODO: Maybe this should depend on the parity of XSize
#define SCORE_X 1

#define BONUS_POINTS 100U

#define INITIAL_LEVEL 1
#define LAST_LEVEL 9

#include "dictionary.h"

uint8_t player_x;
uint8_t alive;
uint8_t slot_index;
uint8_t matrix[WORD_SIZE][MAX_HEIGHT];
uint8_t matrix_height[WORD_SIZE];
uint8_t counter;
uint8_t first_index;
uint8_t last_index;
uint16_t points;
uint16_t record;
uint8_t level;
uint8_t remaining_words;
uint8_t max_level_counter;
uint8_t low_letter_bonus;

uint8_t precomputed_letter[NO_OF_PRECOMPUTED_LETTERS];
uint8_t next_letter_index;

uint8_t aux; // Used to swap values in several functions

// First letter position indices
extern const uint16_t dictionary_index[ALPHABET_SIZE+1];

// 16 most common letters in English 5-letter words
// E A R I O T N S L C U D P M H Y
const uint8_t letter[ALPHABET_SIZE] = {'E', 'A', 'R', 'I', 'O', 'T', 'N', 'S' ,'L', 'C', 'U', 'D', 'P', 'M', 'H', 'Y'};


const uint8_t LETTER_COLOR[ALPHABET_SIZE/4] = {_XL_WHITE, _XL_YELLOW, _XL_CYAN, _XL_GREEN };



void short_pause(void)
{
    _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
}


// TODO: Better compute LETTERS_X
#if XSize>=40
    #define LETTERS_X ((XSize-32)/2)
#else
    #define LETTERS_X 2  
#endif

#if YSize<=18
    #define LETTERS_Y 1
#else
    #define LETTERS_Y 3
#endif

#if XSize>=32
    #define LETTERS_BIT_MASK 7
#elif XSize>=22
    #define LETTERS_BIT_MASK 3
#else
    #define LETTERS_BIT_MASK 1
#endif


void display_letters(void)
{
    uint8_t i;
    // aux: offset
    
    _XL_SET_TEXT_COLOR(_XL_YELLOW);
    for(i=0;i<ALPHABET_SIZE;++i)
    {
        aux = LETTERS_X+(i&LETTERS_BIT_MASK);
        _XL_CHAR(aux,i+LETTERS_Y,letter[i]);
        _XL_CHAR(XSize-aux,i+LETTERS_Y,letter[i]); 
    }
}


uint8_t x_slot(uint8_t x)
{
    return START_X+SLOT_SPACING*x;
}


uint8_t y_slot(uint8_t y)
{
    return START_Y-SLOT_SPACING*y;
}


void draw_letter(uint8_t x, uint8_t y, uint8_t letter_index)
{
    _XL_SET_TEXT_COLOR(LETTER_COLOR[letter_index>>2]);
    _XL_CHAR(x_slot(x),y_slot(y),letter[letter_index]);
}


// TODO: Maybe this could be optimized
#define draw_cross(x) \
do \
{ \
    _XL_DRAW(x_slot(x),START_Y,CROSS_TILE,_XL_RED); \
} while(0)


// TODO: Maybe this could be optimized
void draw_crosses(void)
{
    uint8_t i;
    
    for(i=0;i<WORD_SIZE;++i)
    {
        draw_cross(i);
    }
}


#define draw_empty_slot(x,y) \
do \
{ \
    _XL_DRAW(x_slot(x),y_slot(y),EMPTY_SLOT_TILE,EMPTY_SLOT_COLOR); \
} while(0)


void display_rings(void)
{
    uint8_t i;
    
    for(i=0;i<WORD_SIZE;++i)
    {
        _XL_DRAW(x_slot(i),START_Y,RING_TILE,_XL_WHITE);
        short_pause();
        _XL_PING_SOUND();
    }
}



void display_bottom_row(void)
{
    uint8_t i;
    
    for(i=0;i<WORD_SIZE;++i)
    {
        draw_letter(i,0,matrix[i][0]);
    }
}


void display_column(uint8_t row)
{
    uint8_t i;
    
    for(i=0;i<matrix_height[row];++i)
    {
        draw_letter(row,i,matrix[row][i]);
    }  
    for(;i<MAX_HEIGHT-1;++i)
    {
        draw_empty_slot(row,i);
    }
}


void display_player_column(void)
{
    display_column(player_x-1);
}


void display_matrix(void)
{
    uint8_t i;
    
    for(i=0;i<WORD_SIZE;i++)
    {
        display_column(i);
    }
}


void display_remaining_words(void)
{
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINTD(REMAINING_WORD_X+2,REMAINING_WORD_Y,1,remaining_words);
}



void drop_letter(void)
{
    uint8_t height;
    uint8_t new_letter;
    
    _XL_PING_SOUND();
    
    height = matrix_height[slot_index];
    
    
    // TODO: DEBUGGING why EEEEE (0,0,0,0,0) is seen in the dictionary
    // new_letter = 0;
    
    
    if(next_letter_index<NO_OF_PRECOMPUTED_LETTERS)
    {
        new_letter = precomputed_letter[next_letter_index];
        ++next_letter_index;
    }
    else
    {
        new_letter = _XL_RAND()%ALPHABET_SIZE;
    }

    matrix[slot_index][height]= new_letter;
    draw_letter(slot_index,height,new_letter);
    if(height==MAX_HEIGHT-1)
    {
        alive = 0;
    }
    
    ++matrix_height[slot_index];
                
    slot_index = (slot_index + 1) % WORD_SIZE;
}


uint8_t player_slot_x(void)
{
    return START_X-SLOT_SPACING+SLOT_SPACING*player_x;
}


// Player display routines
void delete_player(void)
{
    if(player_x==MIN_PLAYER_X)
    {
        _XL_DELETE(START_X-SLOT_SPACING+MIN_PLAYER_X, START_Y);
    }
    else if(player_x==MAX_PLAYER_X)
    {
        _XL_DELETE(START_X-SLOT_SPACING+SLOT_SPACING*MAX_PLAYER_X, START_Y);
    }
    else
    {
        _XL_DELETE(player_slot_x(), PLAYER_Y);
    }
}


void display_vertical_player(uint8_t player_tile)
{
    _XL_DRAW(player_slot_x(), PLAYER_Y, player_tile, PLAYER_COLOR);

}


void display_horizontal_left_player(uint8_t player_tile)
{
    _XL_DRAW(START_X-SLOT_SPACING+MIN_PLAYER_X, START_Y, player_tile, PLAYER_COLOR);
}


void display_horizontal_right_player(uint8_t player_tile)
{
    _XL_DRAW(START_X-SLOT_SPACING+SLOT_SPACING*MAX_PLAYER_X, START_Y, player_tile, PLAYER_COLOR); 
}


void display_player(void)
{
    // short_pause();
    
    if(player_x==MIN_PLAYER_X)
    {
        display_horizontal_left_player(HORIZONTAL_LEFT_PLAYER_TILE);
    }
    else if(player_x==MAX_PLAYER_X)
    {
        display_horizontal_right_player(HORIZONTAL_RIGHT_PLAYER_TILE);
    }
    else
    {
        display_vertical_player(VERTICAL_PLAYER_TILE);
    }    
}
    
//

void right_rotate_row(void)
{
    //uint8_t old_first;
    // aux: old_first
    uint8_t i;
    
    
    _XL_TICK_SOUND();

    aux = matrix[0][0];
    
    for(i=0;i<WORD_SIZE-1;++i)
    {
        matrix[i][0] = matrix[i+1][0];
    }
  
    matrix[WORD_SIZE-1][0] = aux;
    
}


void left_rotate_row(void)
{
    // uint8_t old_last;
    // aux: old_last
    uint8_t i;
    
    _XL_TICK_SOUND();
    
    aux = matrix[WORD_SIZE-1][0];
    
    for(i=WORD_SIZE-1;i>0;--i)
    {
        matrix[i][0] = matrix[i-1][0];
    }
  
    matrix[0][0] = aux;
}


void up_rotate_column(void)
{
    // uint8_t old_top;
    // aux: old_top
    uint8_t i;
    
    _XL_TICK_SOUND();    
    
    aux = matrix[player_x-1][matrix_height[player_x-1]-1];
    
    for(i=matrix_height[player_x-1]-1;i>0;--i)
    {
        matrix[player_x-1][i] = matrix[player_x-1][i-1];
    }
  
    matrix[player_x-1][0] = aux;    
}


void down_rotate_column(void)
{
    // uint8_t old_bottom;
    // aux: old_bottom
    uint8_t i;

    _XL_TICK_SOUND();
    
    aux = matrix[player_x-1][0];
    
    for(i=0;i<matrix_height[player_x-1]-1;++i)
    {
        matrix[player_x-1][i] = matrix[player_x-1][i+1];
    }
  
    matrix[player_x-1][matrix_height[player_x-1]-1] = aux;
    
}


// 'E', 'A', 'R', 'I', 
// 'O', 'T', 'N', 'S' ,
// 'L', 'C', 'U', 'D', 
// 'P', 'M', 'H', 'Y'

#if defined(SWITCH_CASE_LETTER_INDEX)
uint8_t letter_index(uint8_t letter)
{
    switch(letter)
    {
        case 'E':
            return 0;
        case 'A':
            return 1;
        case 'R':
            return 2;
        case 'I':
            return 3;
        case 'O':
            return 4;
        case 'T':
            return 5;
        case 'N':
            return 6;
        case 'S':
            return 7;
        case 'L':
            return 8;
        case 'C':
            return 9;
        case 'U':
            return 10;
        case 'D':
            return 11;
        case 'P':
            return 12;
        case 'M':
            return 13;
        case 'H':
            return 14;
    }
    return 15; // 'Y'
}
#else
uint8_t letter_index(uint8_t letter_to_check)
{
    uint8_t i;
    
    while(1) // We exit this loop with return
    {
        if(letter_to_check==letter[i])
        {
            return i;
        }
    }
    return 0; // Never reached
}
#endif

// Score for guessed word (less common letters give more points)
// 'E', 'A', 'R', 'I',  ->   3 point
// 'O', 'T', 'N', 'S' , ->   7 points
// 'L', 'C', 'U', 'D',  ->  11 points
// 'P', 'M', 'H', 'Y'   ->  15 points
uint16_t word_score(void)
{
    uint16_t score = 0;
    uint8_t i;
    
    for(i=0;i<WORD_SIZE;++i)
    {
        // TODO: DEBUG
        // _XL_PRINTD(2,YSize-3,3,score);
        // _XL_SLEEP(1);
        // _XL_WAIT_FOR_INPUT();
        
        score+=3+(((matrix[i][0])>>2)<<1);

    }
    // TODO: DEBUG
    // _XL_PRINTD(2,YSize-3,3,score);
    // _XL_SLEEP(1);
    // _XL_WAIT_FOR_INPUT();    

    //
    
    return score;
}


uint8_t first_letter(uint16_t index)
{
    uint8_t i=1;
    
    while(1) // We know exit with return
    {
        if(index<dictionary_index[i])
        {
            return i-1;
        }
        ++i;
    }
    return 0; // Impossible case
}


// It compresses the last 4 letters of the bottom matrix in 4-bit per letter format
uint16_t compress_bottom_word(void)
{    
    return (uint16_t)matrix[4][0]+(((uint16_t)matrix[3][0])<<4)+(((uint16_t)matrix[2][0])<<8)+(((uint16_t)matrix[1][0])<<12);
}


// Indices are unsigned and the minimum input for last_index is 1
// If last_index becomes 0, then first_index>last_index and it returns 0
uint8_t binary_search(uint16_t search_word, uint16_t first_index, uint16_t last_index)
{
    uint16_t middle_index;
    uint16_t middle_word;
    
    while(last_index>=first_index)
    {    
        middle_index = (first_index+last_index)/2;
        
        middle_word = dictionary[middle_index];
        
        if(dictionary[middle_index]==search_word)
        {
            return 1;
        }
        else if(middle_word<search_word)
        {
            first_index = middle_index + 1;
        }
        else
        {
            last_index = middle_index - 1;
        }
    }

    return 0;
}


uint8_t word_in_dictionary(void)
{
    // uint8_t first_char_index = matrix[0][0];    
    // return binary_search(compress_bottom_word(),dictionary_index[first_char_index], dictionary_index[first_char_index+1]-1);
    return binary_search(compress_bottom_word(),dictionary_index[matrix[0][0]], dictionary_index[matrix[0][0]+1]-1);
}


void display_score(void)
{
    _XL_SET_TEXT_COLOR(_XL_WHITE); 
    _XL_PRINTD(SCORE_X+2,0,4,points);
}


void increase_score(uint8_t value)
{
    points+=value;
    display_score();
}


void remove_bottom_word(void)
{
    uint8_t i;
    uint8_t j;
    // uint8_t height  ;
    
    for(i=0;i<WORD_SIZE;++i)
    {
        // height = matrix_height[i];
        for(j=0;j<matrix_height[i]-1;++j)
        {
            matrix[i][j]=matrix[i][j+1];
        }
        // if(height)
        // {
        --matrix_height[i]; 
        // }
        if(!matrix_height[i])
        {
            low_letter_bonus = 1;
            _XL_SHOOT_SOUND();
        }
    }
    display_matrix();
}


void handle_input(void)
{
    uint8_t input; 
    input = _XL_INPUT();
        
    if(_XL_LEFT(input))
    {
        if(player_x > MIN_PLAYER_X)
        {
            delete_player();
            --player_x;
            display_player();
        }
        else
        {
            display_horizontal_left_player(LEFT_ARROW_TILE);

            right_rotate_row();
            display_bottom_row();
        }
    }
    else if(_XL_RIGHT(input))
    {
        if((player_x < MAX_PLAYER_X))
        {
            delete_player();
            ++player_x;
            display_player();
        }
        else
        {
            display_horizontal_right_player(RIGHT_ARROW_TILE);
    
            left_rotate_row();
            display_bottom_row();
        }
    }

    else if(_XL_UP(input) && player_x>MIN_PLAYER_X && player_x<MAX_PLAYER_X)
    {
        display_vertical_player(UP_ARROW_TILE);

        up_rotate_column();
        display_player_column();
    }
    else if(_XL_DOWN(input) && player_x>MIN_PLAYER_X && player_x<MAX_PLAYER_X)
    {

        display_vertical_player(DOWN_ARROW_TILE);

        down_rotate_column();   
        display_player_column();
    }
    else if(_XL_FIRE(input))
    {
        if(word_in_dictionary())
        {
            _XL_ZAP_SOUND();
            display_rings();
            
            increase_score(word_score());

            remove_bottom_word();
            --remaining_words;
            display_remaining_words();
        }
        else
        {
            draw_crosses();
            _XL_PING_SOUND();
            short_pause();
            display_bottom_row();
            
            drop_letter();
        }
    }
    else
    {
        display_player();
    }
    short_pause();
}



#if defined(_XL_NO_JOYSTICK)
    #define press_fire() \
    do \
    { \
        _XL_PRINT(XSize/2-4, YSize/2+5, "USE IJKL SPACE"); \
    } while(0)
#else
    #define press_fire() \
    do \
    { \
        _XL_PRINT(XSize/2-4, YSize/2+5, "USE STICK"); \
    } while(0)
#endif


#if defined(NO_RANDOMIZE)
#define wait_for_input() \
do \
{ \
    _XL_WAIT_FOR_INPUT(); \
} while(0)
#else
#define wait_for_input() \
do \
{ \
    while(!_XL_KEY_PRESSED()) \
    { \
        points=_XL_RAND(); \
    } \
} while(0)
#endif


void display_borders(void)
{
    uint8_t i;
    
    for(i=2;i<YSize-1;++i)
    {
        _XL_DRAW(1,i,BORDER_TILE, _XL_CYAN);
        _XL_DRAW(XSize-1,i,BORDER_TILE, _XL_CYAN);  
    }        
}


#define title_screen() \
do \
{ \
    _XL_CLEAR_SCREEN(); \
    \
    display_record((XSize/2)-2); \
    \
    _XL_SET_TEXT_COLOR(_XL_CYAN); \
    \
    _XL_PRINT(XSize/2-5,YSize/2-7,"V E R B I X"); \
    \
    _XL_SET_TEXT_COLOR(_XL_WHITE); \
    _XL_PRINT(XSize/2-7,YSize/2-5,"FABRIZIO CARUSO"); \
    \
    _XL_SET_TEXT_COLOR(_XL_RED); \
    _XL_PRINT(XSize/2-4,YSize/2, "WORD GAME"); \
    \
    display_borders(); \
    \
    short_pause(); \
    _XL_SET_TEXT_COLOR(_XL_WHITE); \
    press_fire(); \
    wait_for_input(); \
    _XL_CLEAR_SCREEN(); \
} while(0)


#define initialize_input_output() \
do \
{ \
    _XL_INIT_GRAPHICS(); \
    \
    _XL_INIT_INPUT(); \
    \
    _XL_INIT_SOUND(); \
} while(0)


#define initialize_game() \
do \
{ \
    points = 0; \
    level = INITIAL_LEVEL; \
    alive = 1; \
} while(0)


#define display_level() \
do \
{ \
    _XL_DRAW(LEVEL_X,LEVEL_Y,LV_TILE,_XL_CYAN); \
    _XL_SET_TEXT_COLOR(_XL_WHITE); \
    _XL_PRINTD(LEVEL_X+1,LEVEL_Y,1,level); \
} while(0)


#if defined(DEBUG)
void print_word(uint8_t x, uint8_t y, uint16_t dictionary_index)
{
    uint8_t i;
    
    _XL_CHAR(x,y,letter[first_letter(dictionary_index)]);
    for(i=1;i<WORD_SIZE;++i)
    {
        _XL_CHAR(x+i,y,letter[(dictionary[dictionary_index]>>((4-i)*4))&0x000F]);
    }
}
#endif


// TODO: check whether XOR trick is better
void swap(uint8_t i, uint8_t j)
{
    // aux: swap variable
    
    aux = precomputed_letter[i];
    precomputed_letter[i] = precomputed_letter[j];
    precomputed_letter[j] = aux;
}


// Shuffle with Fisher-Yates algorithm
void shuffle(void)
{
    uint8_t i;
    
    for(i=NO_OF_PRECOMPUTED_LETTERS-1;i>0;--i)
    {
        swap(i,(_XL_RAND())%i);
    }
}


void display_record(uint8_t x)
{
    _XL_DRAW(x,HI_Y,HI_TILE, _XL_RED);
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINTD(x+1,HI_Y,4,record);
}


// Score for guessed word (less common letters give more points)
// 'E', 'A', 'R', 'I',  ->  1 point
// 'O', 'T', 'N', 'S' , ->  4 points
// 'L', 'C', 'U', 'D',  ->  7 points
// 'P', 'M', 'H', 'Y'   -> 10 points

/*
#define INSTRUCTIONS_START_Y YSize/4

#if XSize>=32
    #define INSTR_X_SPACING 1
#else
    #define INSTR_X_SPACING 1
#endif


#if defined(NO_LETTER_VALUES)
    #define display_letter_values()
#else
void display_letter_values(void)
{
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    
    _XL_PRINTD(5*INSTR_X_SPACING,INSTRUCTIONS_START_Y,  2, 1);
    _XL_PRINTD(5*INSTR_X_SPACING,INSTRUCTIONS_START_Y+2,2, 4);
    _XL_PRINTD(5*INSTR_X_SPACING,INSTRUCTIONS_START_Y+4,2, 7);
    _XL_PRINTD(5*INSTR_X_SPACING,INSTRUCTIONS_START_Y+6,2,10);
    
    #if INSTR_X_SPACING==2
        _XL_PRINT(1,INSTRUCTIONS_START_Y,   "E A R I");
        
        _XL_SET_TEXT_COLOR(_XL_YELLOW);
        _XL_PRINT(1,INSTRUCTIONS_START_Y+2, "O T N S");
        
        _XL_SET_TEXT_COLOR(_XL_CYAN);
        _XL_PRINT(1,INSTRUCTIONS_START_Y+4, "L C U D");

        _XL_SET_TEXT_COLOR(_XL_GREEN);
        _XL_PRINT(1,INSTRUCTIONS_START_Y+6, "P M H Y");
    #else
        _XL_PRINT(0,INSTRUCTIONS_START_Y,   "EARI");
        
        _XL_SET_TEXT_COLOR(_XL_YELLOW);
        _XL_PRINT(0,INSTRUCTIONS_START_Y+2, "OTNS");
        
        _XL_SET_TEXT_COLOR(_XL_CYAN);
        _XL_PRINT(0,INSTRUCTIONS_START_Y+4, "LCUD");

        _XL_SET_TEXT_COLOR(_XL_GREEN);
        _XL_PRINT(0,INSTRUCTIONS_START_Y+6, "PMHY");        
    #endif
    _XL_WAIT_FOR_INPUT();
    _XL_CLEAR_SCREEN();    
}
#endif
*/

#define display_score_glyphs() \
do \
{ \
    _XL_DRAW(SCORE_X,0,SCORE_LHS_TILE,_XL_GREEN); \
    _XL_DRAW(SCORE_X+1,0,SCORE_RHS_TILE,_XL_GREEN); \
    _XL_DRAW(REMAINING_WORD_X,REMAINING_WORD_Y,LEFT_LEFT_TILE,_XL_YELLOW); \
    _XL_DRAW(REMAINING_WORD_X+1,REMAINING_WORD_Y,LEFT_RIGHT_TILE,_XL_YELLOW); \
} while(0)


void display_walls(void)
{
    uint8_t i;
    uint8_t j;
    uint8_t horizontal_wall_tile;
    uint8_t vertical_wall_tile;
    uint8_t wall_color;
    
    for(i=0;i<MAX_HEIGHT*2;++i)
    {
        if(i<=2)
        {
            horizontal_wall_tile = HORIZONTAL_BAR_TILE;
            vertical_wall_tile = VERTICAL_BAR_TILE;
            wall_color = _XL_RED;
        }
        else
        {
            horizontal_wall_tile = HORIZONTAL_WALL_TILE;
            vertical_wall_tile = VERTICAL_WALL_TILE;
            wall_color = _XL_GREEN;
        }
        _XL_DRAW(START_X-1,START_Y-i+1,vertical_wall_tile, wall_color);
        _XL_DRAW(START_X-1+WORD_SIZE*2,START_Y-i+1,vertical_wall_tile, wall_color);
             
        
        for(j=0;j<WORD_SIZE*2-1;++j)
        {
            if(!(i&1))
            {
                _XL_DRAW(START_X+j,START_Y-i+1,horizontal_wall_tile, wall_color);
            }

        }
        if(i&1)
        {
            for(j=0;j<WORD_SIZE*2-2;j+=2)
            {  
                _XL_DRAW(START_X+j+1,START_Y-i+1,vertical_wall_tile, wall_color);
            }
        }
    }
}


#define INITIAL_MAX_LEVEL_COUNT 220

// TODO: nearly all display elements do not need to be redisplayed
void initialize_level(void)
{
    uint8_t i;
    uint8_t j;
    
    uint16_t random_dictionary_index;
    uint16_t compressed_code;
    
    alive = 1;
    slot_index = 0;
    player_x = 3;
    counter = 1;
    next_letter_index = 0;
    if(level<=6)
    {
        remaining_words = 2+level;
    }
    else
    {
        remaining_words = 9;
    }
    // remaining_words = level;
    max_level_counter = INITIAL_MAX_LEVEL_COUNT/level;
    low_letter_bonus = 0;
    
    _XL_CLEAR_SCREEN();
    
    display_player();
    
    for(i=0;i<WORD_SIZE;++i)
    {
        matrix_height[i]=0;
    }

    for(i=0;i<NO_OF_PRECOMPUTED_WORDS;++i)
    {
        random_dictionary_index = _XL_RAND()%DICTIONARY_SIZE;
        
        compressed_code = dictionary[random_dictionary_index];
        
        precomputed_letter[WORD_SIZE*i] = first_letter(random_dictionary_index);
        for(j=1;j<WORD_SIZE;++j)
        {
            precomputed_letter[WORD_SIZE*i+j]=(compressed_code>>((4-j)*4))&0x000F;
        }
    }

    for(i=SIZE_OF_PRECOMPUTED_WORDS;i<NO_OF_PRECOMPUTED_LETTERS;++i)
    {
        precomputed_letter[i]=_XL_RAND()%ALPHABET_SIZE;
    }

    // Fisher-Yates shuffle on precomputed words
    shuffle();

    display_level();
    
    display_score();

    display_borders();

    display_walls();

    display_letters();

    display_matrix();
        
    display_score_glyphs();        
        
    display_record(HI_X);
    
    display_remaining_words();
}


void restart_game(void)
{
    title_screen();
    
    initialize_game();    
}


void end_game(void)
{
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(START_X, START_Y+2, "GAME OVER");
    _XL_SLEEP(1);
    _XL_WAIT_FOR_INPUT();
}


// #define MAX_LEVEL_COUNT_INCREASE 10

void handle_drop(void)
{
    // if(!(counter&63))
    if(counter==max_level_counter)
    {
        drop_letter();
        counter=0;
        // max_level_counter+=MAX_LEVEL_COUNT_INCREASE;
    }
}


#define handle_record_update() \
do \
{ \
    if(points>record) \
    { \
        record = points; \
    } \
} while(0)


void initial_letter_drop(void)
{
    uint8_t i;
        
    for(i=0;i<INITIAL_DROP;++i)
    {
        drop_letter();
        short_pause();
    }     
    
}


#define handle_level_end() \
do \
{ \
    if(alive) \
    { \
        increase_score(BONUS_POINTS*remaining_words); \
        ++level; \
        _XL_SET_TEXT_COLOR(_XL_YELLOW); \
        _XL_PRINT(START_X, START_Y+2, "LEVEL  UP"); \
        _XL_SLEEP(1); \
        _XL_WAIT_FOR_INPUT(); \
    } \
    else \
    { \
       _XL_EXPLOSION_SOUND(); \
    } \
} while(0)


#define victory_message() \
do \
{ \
    _XL_SET_TEXT_COLOR(_XL_YELLOW); \
    _XL_PRINT(START_X,YSize/2," THE END "); \
    _XL_SLEEP(2); \
    _XL_WAIT_FOR_INPUT(); \
} while(0)


//
int main(void)
{        
    initialize_input_output();
    
    record = 0;
    
    // main loop
    while(1)
    {
        restart_game();

        // game main loop
        while((level<LAST_LEVEL+1) && alive)
        {
            initialize_level();

            initial_letter_drop();

            // level main loop
            while(alive && remaining_words && !low_letter_bonus)
            {    
                handle_drop();
                
                handle_input();
                                
                ++counter;                
            }
            handle_level_end();
        }
        if(alive) // game completed alive
        {
            victory_message();
        }
        end_game();
        handle_record_update();
    }

    return EXIT_SUCCESS;
}

