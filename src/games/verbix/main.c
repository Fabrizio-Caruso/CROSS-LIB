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


#if YSize<12
    #define MAX_HEIGHT (YSize-2)
#else
    #define MAX_HEIGHT ((YSize/2)-1)
#endif

#define START_X ((XSize)/2-4)
#define START_Y ((YSize)-3)

#define MIN_PLAYER_X 0
#define MAX_PLAYER_X (1+WORD_SIZE)

#define PLAYER_Y (START_Y+2)

#define END_Y (START_Y+10)

#define VERTICAL_PLAYER_TILE         _TILE_0
#define EMPTY_SLOT_TILE              _TILE_1
#define HORIZONTAL_LEFT_PLAYER_TILE  _TILE_2
#define LEFT_ARROW_TILE              _TILE_3 
#define RIGHT_ARROW_TILE             _TILE_4
#define UP_ARROW_TILE                _TILE_5
#define HI_TILE                      _TILE_6
#define DOWN_ARROW_TILE              _TILE_7
#define HORIZONTAL_RIGHT_PLAYER_TILE _TILE_8
#define LV_TILE                      _TILE_9
#define LEFT_LHS_TILE                _TILE_10
#define HORIZONTAL_BAR_TILE          _TILE_11
#define VERTICAL_WALL_TILE           _TILE_12
#define HORIZONTAL_WALL_TILE         _TILE_13
#define SCORE_LHS_TILE               _TILE_14
#define VERTICAL_BAR_TILE            _TILE_15
#define SCORE_RHS_TILE               _TILE_16
#define LEFT_RHS_TILE                _TILE_17
#define BORDER_TILE1                 _TILE_18
#define CROSS_TILE                   _TILE_19
#define RING_TILE                    _TILE_20
#define BONUS_LINE_TILE              _TILE_21
#define VERTICAL_BONUS_WALL_TILE     _TILE_22
#define HORIZONTAL_BONUS_WALL_TILE   _TILE_23
#define BORDER_TILE2                 _TILE_24
#define BORDER_TILE0                 _TILE_25
#define BORDER_TILE3                 _TILE_26


#define PLAYER_COLOR _XL_WHITE
#define EMPTY_SLOT_COLOR _XL_WHITE
#define WALL_COLOR _XL_RED

#if YSize<12
    #define INITIAL_ROWS 1
#elif YSize<=18
    #define INITIAL_ROWS 2
#else
    #define INITIAL_ROWS 3
#endif


#if YSize<20
    #define SMALL_SCREEN_REDUCTION 1
#else
    #define SMALL_SCREEN_REDUCTION 0
#endif

#define MIN_INITIAL_DROP ((((WORD_SIZE)*INITIAL_ROWS))+SMALL_SCREEN_REDUCTION)

#if !defined(NO_OF_PRECOMPUTED_WORDS)
    #define NO_OF_PRECOMPUTED_WORDS 8U
#endif

#define SIZE_OF_PRECOMPUTED_WORDS ((NO_OF_PRECOMPUTED_WORDS)*(WORD_SIZE))

#if !defined(NO_OF_RANDOM_LETTERS)
    #define NO_OF_RANDOM_LETTERS ((NO_OF_PRECOMPUTED_WORDS/2))
#endif
//SIZE_OF_PRECOMPUTED_WORDS

#define NO_OF_PRECOMPUTED_LETTERS ((SIZE_OF_PRECOMPUTED_WORDS)+(NO_OF_RANDOM_LETTERS))


#define LEVEL_X ((XSize)-2)
#define LEVEL_Y 0U

    #define REMAINING_WORD_Y 0U

#if XSize>=17
    #define REMAINING_WORD_X ((LEVEL_X)-4)
#else
    #define REMAINING_WORD_X ((LEVEL_X)-3)
#endif

#if XSize<17
    #define HI_X 6
#elif XSize<22
    #define HI_X (REMAINING_WORD_X-6)
#else
    #define HI_X (((REMAINING_WORD_X)/2))
#endif

#define HI_Y 0U

#define SLOT_X_SPACING 2U

#if YSize>=12
    #define SLOT_Y_SPACING 2U
#else
    #define SLOT_Y_SPACING 1U
#endif

#define BONUS_HEIGHT 2U

// TODO: Maybe this should depend on the parity of XSize
#define SCORE_X 1U

#define BONUS_POINTS 100U

// remaining = 9 -> max_level_count = INITIAL_MAX_LEVEL_COUNT/level + (remaining x 8)
// level 1 with 3 remaining words: (240-72)/1 + (3x8) = 168 + 27 = 195
// level 1 with 1 remaining word : (240-72)/1 + (1x8) = 168 +  9 = 177
//
// level 2 with 4 remaining words: (240-72)/2 + (4x8) =  84 + 32 = 116
// level 2 with 1 remaining word : (240-72)/2 + (1x8) =  84 +  9 =  93
//
// level 3 with 9 remaining words: (240-72)/3 + (5x8) =  56 + 40 = 106
// level 3 with 1 remaining word : (240-72)/3 + (1x8) =  56 +  9 =  65
// .....
// level 9 with 9 remaining words: (240-72)/9 + (9x8) =  18 + 72 =  90
// level 9 with 1 remaining word : (240-72)/9 + (1x8) =  18 +  9 =  27
#define INITIAL_MAX_LEVEL_COUNT (240U-72U)

#define INITIAL_LEVEL 1U
#define LAST_LEVEL 9U

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
uint8_t player_display_refresh;


uint8_t precomputed_letter[NO_OF_PRECOMPUTED_LETTERS];
uint8_t next_letter_index;

uint8_t aux; // Used to swap values in several functions

// First letter position indices
extern const uint16_t dictionary_index[ALPHABET_SIZE+1];


const uint8_t border_tile[4] = {BORDER_TILE0, BORDER_TILE1, BORDER_TILE2, BORDER_TILE3};

// 16 most common letters in English 5-letter words
// E A R I O T N S L C U D P M H Y
const char *letter = "EARIOTNSLCUDPMHY";

const uint8_t LETTER_COLOR[ALPHABET_SIZE/4] = {_XL_WHITE, _XL_YELLOW, _XL_CYAN, _XL_GREEN };


// TODO: Better compute LETTERS_X
// #if XSize>=40
    // #define LETTERS_X 2
// #else
    // #define LETTERS_X 2  
// #endif
#if XSize>17
    #define LETTERS_X XSize/2-7
#else
    #define LETTERS_X 0
#endif


#if YSize<=18
    #define LETTERS_Y 1
#else
    #define LETTERS_Y 3
#endif

#if XSize>40
    #define LETTERS_BIT_MASK 7
#elif XSize>=24
    #define LETTERS_BIT_MASK 3
#else
    #define LETTERS_BIT_MASK 1
#endif


#define player_slot() \
    player_x-1

void short_pause(void)
{
    _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
}


void tiny_pause(void)
{
    _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR/8);
}


void one_second_pause(void)
{
    _XL_SLEEP(1);
}


#if XSize>17 && YSize>16
    #define DISPLAY_LETTERS_X 2

    void display_letters(void)
    {
        uint8_t i;
        // aux: offset
        
        _XL_SET_TEXT_COLOR(_XL_YELLOW);
        for(i=0;i<ALPHABET_SIZE;++i)
        {
            aux = DISPLAY_LETTERS_X+(i&LETTERS_BIT_MASK);
            _XL_CHAR(aux,i+LETTERS_Y,letter[i]);
            _XL_CHAR(XSize-aux,i+LETTERS_Y,letter[i]); 
        }
    }

#else
    
    #define display_letters()
#endif

uint8_t x_slot(uint8_t x)
{
    return START_X+SLOT_X_SPACING*x;
}

#if YSize>=12
    #define LOWER_SPACING 0
#else
    #define LOWER_SPACING 1
#endif

//  
uint8_t y_slot(uint8_t y)
{
    return START_Y-SLOT_Y_SPACING*y+LOWER_SPACING;
}


void draw_letter(uint8_t x, uint8_t y)
{
    uint8_t letter_index = matrix[x][y];
    
    _XL_SET_TEXT_COLOR(LETTER_COLOR[letter_index>>2]);
    _XL_CHAR(x_slot(x),y_slot(y),letter[letter_index]);
}


// TODO: Maybe this could be optimized
#define draw_cross(x) \
do \
{ \
    _XL_DRAW(x_slot(x),START_Y+LOWER_SPACING,CROSS_TILE,_XL_RED); \
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
        draw_letter(i,0);
    }
    player_display_refresh = 1;
}


void display_column(uint8_t row)
{
    uint8_t i;
    
    for(i=0;i<matrix_height[row];++i)
    {
        draw_letter(row,i);
    }  
    for(;i<MAX_HEIGHT-1;++i)
    {
        draw_empty_slot(row,i);
    }
}


void display_player_column(void)
{
    display_column(player_slot());
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

    max_level_counter = ((INITIAL_MAX_LEVEL_COUNT)/level+(remaining_words>>3));
    
    height = matrix_height[slot_index];
    
    if(next_letter_index<NO_OF_PRECOMPUTED_LETTERS)
    {
        new_letter = precomputed_letter[next_letter_index];
        ++next_letter_index;
    }
    else
    {
        new_letter = (uint8_t) _XL_RAND()%ALPHABET_SIZE;
    }

    matrix[slot_index][height]= new_letter;
    draw_letter(slot_index,height);
    if(height==MAX_HEIGHT-1)
    {
        alive = 0;
    }
    
    ++matrix_height[slot_index];
                
    slot_index = (slot_index + 1U) % WORD_SIZE;
}


uint8_t player_slot_x(void)
{
    return START_X-SLOT_X_SPACING+SLOT_X_SPACING*player_x;
}


// Player display routines
void delete_player(void)
{
    if(player_x==MIN_PLAYER_X)
    {
        _XL_DELETE(START_X-SLOT_X_SPACING+MIN_PLAYER_X, START_Y+LOWER_SPACING);
    }
    else if(player_x==MAX_PLAYER_X)
    {
        _XL_DELETE(START_X-SLOT_X_SPACING+SLOT_X_SPACING*MAX_PLAYER_X, START_Y+LOWER_SPACING);
    }
    else
    {
        _XL_DELETE(player_slot_x(), PLAYER_Y);
    }
}


void display_vertical_player(uint8_t player_tile)
{
    player_display_refresh = 1;
    _XL_DRAW(player_slot_x(), PLAYER_Y, player_tile, PLAYER_COLOR);

}


void display_horizontal_left_player(uint8_t player_tile)
{
    _XL_DRAW(START_X-SLOT_X_SPACING+MIN_PLAYER_X, START_Y+LOWER_SPACING, player_tile, PLAYER_COLOR);
}


void display_horizontal_right_player(uint8_t player_tile)
{
    _XL_DRAW(START_X-SLOT_X_SPACING+SLOT_X_SPACING*MAX_PLAYER_X, START_Y+LOWER_SPACING, player_tile, PLAYER_COLOR); 
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
    player_display_refresh = 0;    
}
    
//

void right_rotate_row(void)
{
    //uint8_t old_first;
    // aux: old_first
    uint8_t i;
    
    
    _XL_TICK_SOUND();

	++counter;
	
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
    
	++counter;
	
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
    
	counter+=4;	
	
    aux = matrix[player_slot()][matrix_height[player_slot()]-1];
    
    for(i=matrix_height[player_slot()]-1;i>0;--i)
    {
        matrix[player_slot()][i] = matrix[player_slot()][i-1];
    }
  
    matrix[player_slot()][0] = aux;    
}


void down_rotate_column(void)
{
    // uint8_t old_bottom;
    // aux: old_bottom
    uint8_t i;

    _XL_TICK_SOUND();
	
	counter+=4;
    
    aux = matrix[player_slot()][0];
    
    for(i=0;i<matrix_height[player_slot()]-1;++i)
    {
        matrix[player_slot()][i] = matrix[player_slot()][i+1];
    }
  
    matrix[player_slot()][matrix_height[player_slot()]-1] = aux;
    
}


// 'E', 'A', 'R', 'I', 
// 'O', 'T', 'N', 'S' ,
// 'L', 'C', 'U', 'D', 
// 'P', 'M', 'H', 'Y'


uint8_t letter_index(uint8_t letter_to_check)
{
    uint8_t i = 0;
    
    while(1) // We exit this loop with return
    {
        if(letter_to_check==letter[i])
        {
            return i;
        }
    }
    return 0U; // Never reached
}


// Score for guessed word (less common letters give more points)
// 'E', 'A', 'R', 'I',  ->   3 point
// 'O', 'T', 'N', 'S' , ->   7 points
// 'L', 'C', 'U', 'D',  ->  11 points
// 'P', 'M', 'H', 'Y'   ->  15 points
uint8_t word_score(void)
{
    uint8_t score = 0;
    uint8_t i;
    
    for(i=0;i<WORD_SIZE;++i)
    {       
        score+=3+(((matrix[i][0])>>2)<<1);

    }
    
    return score;
}


uint8_t first_letter(uint16_t index)
{
    uint8_t i=0;
    
    while(1) // We know that we will exit with return
    {
        if(index<dictionary_index[i+1])
        {
            return i;
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
            first_index = middle_index + 1U;
        }
        else
        {
            last_index = middle_index - 1U;
        }
    }

    return 0;
}


uint8_t word_in_dictionary(void)
{
    // uint8_t first_char_index = matrix[0][0];    
    // return binary_search(compress_bottom_word(),dictionary_index[first_char_index], dictionary_index[first_char_index+1]-1);
    return binary_search(compress_bottom_word(),dictionary_index[matrix[0][0]], dictionary_index[matrix[0][0]+1]-1U);
}

#if XSize>17
    #define SCORE_X_OFFSET 2
#else
    #define SCORE_X_OFFSET 0
#endif

void display_score(void)
{
    _XL_SET_TEXT_COLOR(_XL_WHITE); 
    _XL_PRINTD(SCORE_X+SCORE_X_OFFSET,0,4,points);
}


void increase_score(uint8_t value)
{
    _XL_ZAP_SOUND();
    points+=value;
    display_score();
}


void remove_bottom_word(void)
{
    uint8_t i;
    uint8_t j;
    // uint8_t height  ;
    
    low_letter_bonus = remaining_words;
    for(i=0;i<WORD_SIZE;++i)
    {
        // height = matrix_height[i];
        for(j=0;j<matrix_height[i]-1;++j)
        {
            matrix[i][j]=matrix[i][j+1];
        }
        
        // _XL_PRINTD(0,YSize-3,1,i);

        
        // _XL_PRINTD(0,YSize-1,2,matrix_height[i]);
        // _XL_WAIT_FOR_INPUT();
        
        --matrix_height[i]; 
        
        // One single letter above BONUS_HEIGHT prevents disables the bonus
        if(matrix_height[i]>BONUS_HEIGHT) 
        {
            low_letter_bonus = 0;
        }
        // _XL_PRINTD(XSize-3,YSize-1,2,matrix_height[i]);
        // _XL_WAIT_FOR_INPUT();
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
            tiny_pause();
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
            tiny_pause();
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
            // _XL_ZAP_SOUND();
            display_rings();
            
            increase_score(word_score());

            --remaining_words;
            remove_bottom_word();
            display_remaining_words();
            max_level_counter = INITIAL_MAX_LEVEL_COUNT; // slow down next drop
            counter = 0;                                 // re-start counter to slow down next drop
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
    else if(player_display_refresh)
    {
        display_player();
    }
    short_pause();
}



#if defined(_XL_NO_JOYSTICK)
    #define control_instructions() \
    do \
    { \
        _XL_PRINT(XSize/2-6, YSize/2+6, "USE IJKL SPACE"); \
    } while(0)
#else
    #define control_instructions() \
    do \
    { \
        _XL_PRINT(XSize/2-5, YSize/2+6, "USE JOYSTICK"); \
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


void display_borders(uint8_t offset, uint8_t tile)
{
    uint8_t i;
    
    for(i=2;i<YSize-1;++i)
    {
        _XL_DRAW(1+offset,i,tile, _XL_CYAN);
        _XL_DRAW(XSize-1-offset,i,tile, _XL_CYAN);  
    }        
}

#if XSize<=20
    #define BORDER_OFFSET 0
#elif XSize<=22
    #define BORDER_OFFSET 0
#elif XSize<=32
    #define BORDER_OFFSET 3
#elif XSize<=40
    #define BORDER_OFFSET 5
#else
    #define BORDER_OFFSET ((XSize)/10)
#endif


#if XSize>17
    #define title_borders() display_borders(BORDER_OFFSET, BORDER_TILE1)
#else
    #define title_borders()
#endif


#define title_screen() \
do \
{ \
    _XL_CLEAR_SCREEN(); \
    \
    display_record((XSize/2)-2); \
    \
    _XL_SET_TEXT_COLOR(_XL_YELLOW); \
    \
    _XL_PRINT(XSize/2-5,YSize/2-7,"V E R B I X"); \
    \
    _XL_SET_TEXT_COLOR(_XL_WHITE); \
    _XL_PRINT(XSize/2-7,YSize/2-5,"FABRIZIO CARUSO"); \
    \
    _XL_SET_TEXT_COLOR(_XL_RED); \
    _XL_PRINT(XSize/2-7,YSize/2-1, "FIND WORDS WITH"); \
    _XL_SET_TEXT_COLOR(_XL_CYAN); \
    _XL_PRINT(LETTERS_X,YSize/2+2,letter); \
    \
    title_borders(); \
    \
    _XL_SET_TEXT_COLOR(_XL_WHITE); \
    control_instructions(); \
    wait_for_input(); \
    _XL_CLEAR_SCREEN(); \
    display_letter_values(); \
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


// #if defined(DEBUG)
// void print_word(uint8_t x, uint8_t y, uint16_t dictionary_index)
// {
    // uint8_t i;
    
    // _XL_CHAR(x,y,letter[first_letter(dictionary_index)]);
    // for(i=1;i<WORD_SIZE;++i)
    // {
        // _XL_CHAR(x+i,y,letter[(dictionary[dictionary_index]>>((4-i)*4))&0x000F]);
    // }
// }
// #endif


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
        swap(i,(uint8_t) (_XL_RAND())%i);
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


#define INSTRUCTIONS_START_Y ((YSize)/2-3)

#define INSTRUCTIONS_START_X ((XSize)/2-3)


#if XSize>17
    #define letter_values_borders() display_borders(BORDER_OFFSET+4,BORDER_TILE3);
#else
    #define letter_values_borders() display_borders(2,BORDER_TILE3);
#endif

#if defined(NO_LETTER_VALUES)
    #define display_letter_values()
#else
void display_letter_values(void)
{
    
    letter_values_borders();
    
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    
    _XL_PRINTD(INSTRUCTIONS_START_X+6,INSTRUCTIONS_START_Y,  1, 3U);
    _XL_PRINTD(INSTRUCTIONS_START_X+6,INSTRUCTIONS_START_Y+2,1, 7U);
    _XL_PRINTD(INSTRUCTIONS_START_X+5,INSTRUCTIONS_START_Y+4,2,11U);
    _XL_PRINTD(INSTRUCTIONS_START_X+5,INSTRUCTIONS_START_Y+6,2,15U);
    
    
    _XL_PRINT(INSTRUCTIONS_START_X,INSTRUCTIONS_START_Y,   "EARI");
    
    _XL_SET_TEXT_COLOR(_XL_YELLOW);
    _XL_PRINT(INSTRUCTIONS_START_X,INSTRUCTIONS_START_Y+2, "OTNS");
    
    _XL_SET_TEXT_COLOR(_XL_CYAN);
        
    _XL_PRINT(INSTRUCTIONS_START_X,INSTRUCTIONS_START_Y+4, "LCUD");

    _XL_SET_TEXT_COLOR(_XL_GREEN);
    _XL_PRINT(INSTRUCTIONS_START_X,INSTRUCTIONS_START_Y+6, "PMHY");        
    
    _XL_PRINT(INSTRUCTIONS_START_X, INSTRUCTIONS_START_Y-2, "POINTS");
    
    _XL_WAIT_FOR_INPUT();
    _XL_CLEAR_SCREEN();    
}
#endif


#if XSize>17
    #define display_score_text() \
    do \
    { \
        _XL_DRAW(SCORE_X,0,SCORE_LHS_TILE,_XL_GREEN); \
        _XL_DRAW(SCORE_X+1,0,SCORE_RHS_TILE,_XL_GREEN); \
    } while(0)
#else
    #define display_score_text()
#endif

#define display_score_glyphs() \
do \
{ \
    display_score_text(); \
    _XL_DRAW(REMAINING_WORD_X,REMAINING_WORD_Y,LEFT_LHS_TILE,_XL_YELLOW); \
    _XL_DRAW(REMAINING_WORD_X+1,REMAINING_WORD_Y,LEFT_RHS_TILE,_XL_YELLOW); \
} while(0)


#if XSize>40
    #define BONUS_LINE_SIZE 6
#elif XSize>=40
    #define BONUS_LINE_SIZE 5
#elif XSize>=32
    #define BONUS_LINE_SIZE 4
#elif XSize>20
    #define BONUS_LINE_SIZE 3
#elif XSize>17
    #define BONUS_LINE_SIZE 2
#else
    #define BONUS_LINE_SIZE 1 
#endif


void display_walls(void)
{
    uint8_t i;
    #if YSize>=12
    uint8_t j;

    uint8_t horizontal_wall_tile;
    #endif
    uint8_t vertical_wall_tile;
	
	#if !defined(_XL_NO_COLOR)
    uint8_t wall_color;
	#endif
    
    #if YSize>=12
        #define HEIGHT_FACTOR 2
    #else
        #define HEIGHT_FACTOR 1
    #endif
    
    for(i=0;i<MAX_HEIGHT*HEIGHT_FACTOR;++i)
    {
        
        if(i>4)
        {
            #if YSize>=12
            horizontal_wall_tile = HORIZONTAL_WALL_TILE;
            #endif
            vertical_wall_tile = VERTICAL_WALL_TILE;
			#if !defined(_XL_NO_COLOR)
            wall_color = _XL_GREEN;
			#endif
        }
        else if(i>2)
        {
            #if YSize>=12
            horizontal_wall_tile = HORIZONTAL_BONUS_WALL_TILE;
            #endif
            vertical_wall_tile = VERTICAL_BONUS_WALL_TILE;
			#if !defined(_XL_NO_COLOR)
            wall_color = _XL_YELLOW;
			#endif
        }
        else
        {
            #if YSize>=12
            horizontal_wall_tile = HORIZONTAL_BAR_TILE;
            #endif
            vertical_wall_tile = VERTICAL_BAR_TILE;
			#if !defined(_XL_NO_COLOR)
            wall_color = _XL_RED;  
			#endif
        }
        _XL_DRAW(START_X-1,START_Y-i+1,vertical_wall_tile, wall_color);
        _XL_DRAW(START_X-1+WORD_SIZE*2,START_Y-i+1,vertical_wall_tile, wall_color);
             
        
        #if YSize>=12
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
        #endif
    }
    
    // Draw bonus limit
    for(i=0;i<BONUS_LINE_SIZE;++i)
    {
        _XL_DRAW(START_X-2-i,START_Y-SLOT_Y_SPACING*BONUS_HEIGHT+1,BONUS_LINE_TILE,_XL_WHITE);
        _XL_DRAW(START_X-1+WORD_SIZE*2+1+i,START_Y-SLOT_Y_SPACING*BONUS_HEIGHT+1,BONUS_LINE_TILE,_XL_WHITE);
    }
}


// TODO: nearly all display elements do not need to be redisplayed
void initialize_level(void)
{
    uint8_t i;
    uint8_t j;
    
    uint16_t random_dictionary_index;
    uint16_t compressed_code;
    
    alive = 1U;
    slot_index = 0U;
    player_x = 3U;
    counter = 1U;
    next_letter_index = 0U;
    
    if(level<=6)
    {
        remaining_words = 2+level;
    }
    else
    {
        remaining_words = 9;
    }
    // remaining_words = level;
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
            // Extract the nibble from compressed_code corresponding to the letters starting from second (j=1) till fifth position (j=4)
            precomputed_letter[WORD_SIZE*i+j]=(uint8_t) ((compressed_code>>((4U-j)*4U))&0x000F);
        }
    }

    for(i=SIZE_OF_PRECOMPUTED_WORDS;i<NO_OF_PRECOMPUTED_LETTERS;++i)
    {
        // No need to apply cast to the full operation as ALPHABET_SIZE is 16, which is a divisor of 256
        precomputed_letter[i]=(uint8_t) _XL_RAND()%ALPHABET_SIZE; 
    }

    // Fisher-Yates shuffle on precomputed words
    shuffle();

    display_level();
    
    display_score();

    display_borders(0,border_tile[level&3]);

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


// TODO: Verify offset
#if defined(NO_LEVEL_END)
    #define level_end() 
#else

    void level_end(void)
    {
        for(aux=1;aux<XSize/2;++aux)
        {
            display_borders(aux, BORDER_TILE3);
            short_pause();
        }
    }
#endif


#define end_game() \
do \
{ \
    _XL_SET_TEXT_COLOR(_XL_WHITE); \
    _XL_PRINT(START_X, START_Y+2, "GAME OVER"); \
    one_second_pause(); \
    _XL_WAIT_FOR_INPUT(); \
    level_end(); \
} while(0)


// #define MAX_LEVEL_COUNT_INCREASE 10

#define handle_drop() \
do \
{ \
    if(counter>=max_level_counter) \
    { \
        drop_letter(); \
        counter=0; \
    } \
} while(0)


#define handle_record_update() \
do \
{ \
    if(points>record) \
    { \
        record = points; \
    } \
} while(0)


// CC65 produces broken code with an infinite loop if we use (level>>SMALL_SCREEN_REDUCTION) 
#define initial_letter_drop() \
do \
{ \
    uint8_t i; \
    \
    for(i=0;i<(uint8_t) MIN_INITIAL_DROP + (level/(1+SMALL_SCREEN_REDUCTION));++i) \
    { \
        drop_letter(); \
        short_pause(); \
    } \
} while(0)



#define handle_level_end() \
do \
{ \
    \
    if(alive) \
    { \
        if(low_letter_bonus) \
        { \
            _XL_EXPLOSION_SOUND(); \
            _XL_SET_TEXT_COLOR(_XL_WHITE); \
            _XL_PRINT(START_X-1, START_Y+2, "EXTRA BONUS"); \
            one_second_pause(); \
            _XL_WAIT_FOR_INPUT(); \
        } \
        \
        for(aux=0;aux<low_letter_bonus;++aux) \
        { \
            --remaining_words; \
            display_remaining_words(); \
            increase_score(BONUS_POINTS); \
            _XL_PING_SOUND(); \
            short_pause(); \
            short_pause(); \
        } \
        ++level; \
        _XL_SET_TEXT_COLOR(_XL_YELLOW); \
        _XL_PRINT(START_X-1, START_Y+2, " LEVEL  UP "); \
        one_second_pause(); \
        _XL_WAIT_FOR_INPUT(); \
        level_end(); \
    } \
} while(0)


// TODO: Use _XL_DRAW
// TODO: Implement some animation
#define victory_message() \
do \
{ \
    \
    _XL_SET_TEXT_COLOR(_XL_YELLOW); \
    _XL_PRINT(START_X,YSize/2," THE END "); \
    one_second_pause(); \
    _XL_WAIT_FOR_INPUT(); \
    level_end(); \
} while(0)


//
int main(void)
{        
    initialize_input_output();
    
    // record is already set to zero because it is a global variable
    
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
				// _XL_PRINTD(0,YSize-1,4,counter);
				
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

