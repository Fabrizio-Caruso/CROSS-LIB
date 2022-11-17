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

#define NUMBER_OF_LETTERS 5

#define MAX_HEIGHT 11

#define START_X ((XSize)/2-3)
#define START_Y (((YSize)-(MAX_HEIGHT)/2)-4)

#define MIN_PLAYER_X 0
#define MAX_PLAYER_X (1+NUMBER_OF_LETTERS)

#define PLAYER_Y (START_Y+1)


#define END_Y (START_Y+10)

#define LAST_LEVEL 10

#define PLAYER_TILE _TILE_0
#define PLAYER_COLOR _XL_WHITE
#define EMPTY_SLOT_TILE _TILE_1
#define EMPTY_SLOT_COLOR _XL_WHITE


#define INITIAL_DROP ((NUMBER_OF_LETTERS)*3)

#define ALPHABET_SIZE 16

#define MAX_NUMBER_OF_WORDS 10

#include "dictionary.h"

uint8_t player_x;
uint8_t alive;
uint8_t slot_index;
uint8_t matrix[NUMBER_OF_LETTERS][MAX_HEIGHT];
uint8_t matrix_height[NUMBER_OF_LETTERS];
uint8_t victory;
uint8_t counter;
uint8_t first_index;
uint8_t last_index;
uint8_t generated_letters[MAX_NUMBER_OF_WORDS][NUMBER_OF_LETTERS];
uint8_t number_of_generated_words;
uint16_t points;
uint8_t level;

// 16 most common letters in English 5-letter words
// E A R I O T N S L C U D P M H Y
const uint8_t letters[ALPHABET_SIZE] = {'E', 'A', 'R', 'I', 'O', 'T', 'N', 'S' ,'L', 'C', 'U', 'D', 'P', 'M', 'H', 'Y'};


extern const uint16_t dictionary_index[16+1];


void display_char(uint8_t x, uint8_t y, uint8_t letter)
{
    _XL_CHAR(START_X+x,START_Y-y,letter);
}


void display_bottom_row(void)
{
    uint8_t i;
    
    for(i=0;i<NUMBER_OF_LETTERS;++i)
    {
        display_char(i,0,matrix[i][0]);
    }
}


void display_column(uint8_t row)
{
    uint8_t i;
    
    for(i=0;i<matrix_height[row];++i)
    {
        display_char(row,i,matrix[row][i]);
    }  
    for(;i<MAX_HEIGHT;++i)
    {
        //_XL_DELETE(row,i);
        // TODO: Debug
        // display_char(row,i,'.');
        _XL_DRAW(START_X+row,START_Y-i,EMPTY_SLOT_TILE,EMPTY_SLOT_COLOR);
    }
}


void display_player_column(void)
{
    display_column(player_x-1);
}


void display_matrix(void)
{
    uint8_t i;
    
    for(i=0;i<NUMBER_OF_LETTERS;i++)
    {
        display_column(i);
    }

}


void drop_letter(void)
{
    uint8_t height;
    uint8_t new_letter;
    uint8_t word_index;
    
    height = matrix_height[slot_index];
    
    word_index = _XL_RAND()%number_of_generated_words;
    new_letter = letters[generated_letters[word_index][_XL_RAND()%NUMBER_OF_LETTERS]];

    matrix[slot_index][height]=new_letter;
    display_char(slot_index,height,new_letter);
    if(height==MAX_HEIGHT-1)
    {
        alive = 0;
    }
    
    ++matrix_height[slot_index];
                
    slot_index = (slot_index + 1) % NUMBER_OF_LETTERS;
}


void delete_player(void)
{
    _XL_DELETE(START_X-1+player_x, PLAYER_Y);
}


void display_player(void)
{
    _XL_DRAW(START_X-1+player_x, PLAYER_Y, PLAYER_TILE, PLAYER_COLOR);
}


void right_rotate_letters(void)
{
    uint8_t old_first;
    uint8_t i;
    
    old_first = matrix[0][0];
    
    for(i=0;i<NUMBER_OF_LETTERS-1;++i)
    {
        matrix[i][0] = matrix[i+1][0];
    }
  
    matrix[NUMBER_OF_LETTERS-1][0] = old_first;
}


void left_rotate_letters(void)
{
    uint8_t old_last;
    uint8_t i;
    
    old_last = matrix[NUMBER_OF_LETTERS-1][0];
    
    for(i=NUMBER_OF_LETTERS-1;i>0;--i)
    {
        matrix[i][0] = matrix[i-1][0];
    }
  
    matrix[0][0] = old_last;
}


void up_rotate_letters(void)
{
    uint8_t old_top;
    uint8_t i;
    
    old_top = matrix[player_x-1][matrix_height[player_x-1]-1];
    
    for(i=matrix_height[player_x-1]-1;i>0;--i)
    {
        matrix[player_x-1][i] = matrix[player_x-1][i-1];
    }
  
    matrix[player_x-1][0] = old_top;
}


void down_rotate_letters(void)
{
    uint8_t old_bottom;
    uint8_t i;
    
    old_bottom = matrix[player_x-1][0];
    
    for(i=0;i<matrix_height[player_x-1]-1;++i)
    {
        matrix[player_x-1][i] = matrix[player_x-1][i+1];
    }
  
    matrix[player_x-1][matrix_height[player_x-1]-1] = old_bottom;
}


// 'E', 'A', 'R', 'I', 
// 'O', 'T', 'N', 'S' ,
// 'L', 'C', 'U', 'D', 
// 'P', 'M', 'H', 'Y'


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
        case 'Y':
            return 15;
    }
    return 16;
}


// Score for guessed word (less common letters give more points)
// 'E', 'A', 'R', 'I',  -> 1 point
// 'O', 'T', 'N', 'S' , -> 2 points
// 'L', 'C', 'U', 'D',  -> 3 points
// 'P', 'M', 'H', 'Y'   -> 4 points
uint16_t word_score(void)
{
    uint16_t score = 0;
    uint8_t i;
    
    for(i=0;i<NUMBER_OF_LETTERS;++i)
    {
        score+=1+((letter_index(matrix[i][0])>>2));
        _XL_PRINTD(i*4,YSize-2,2,1+((letter_index(matrix[i][0])>>2)));
    }
    return score;
}


uint8_t first_letter(uint16_t index)
{
    uint8_t i;
    
    i = 1;
    
    while(i<15)
    {
        if(index<dictionary_index[i])
        {
            return i-1;
        }
        ++i;
    }
    return 15;
}


// It compresses the last 4 letters of the bottom matrix in 4-bit per letter format
uint16_t compress_bottom_word(void)
{    
    return (uint16_t)letter_index(matrix[4][0])+(((uint16_t)letter_index(matrix[3][0]))<<4)+(((uint16_t)letter_index(matrix[2][0]))<<8)+(((uint16_t)letter_index(matrix[1][0]))<<12);
}


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
    uint8_t first_char_index = letter_index(matrix[0][0]);
    
    return binary_search(compress_bottom_word(),dictionary_index[first_char_index], dictionary_index[first_char_index+1]-1  );
}


void remove_bottom_word(void)
{
    uint8_t i;
    uint8_t j;
    
    for(i=0;i<NUMBER_OF_LETTERS;++i)
    {
        
        for(j=0;j<matrix_height[i]-1;++j)
        {
            matrix[i][j]=matrix[i][j+1];
        }
        if(matrix_height[i])
        {
           --matrix_height[i]; 
        }
        if(!matrix_height[i])
        {
            victory = 1;
        }
    }
    display_matrix();
}


void display_score(void)
{
    _XL_PRINTD(0,0,4,points);
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
        }
        else
        {
            right_rotate_letters();
            display_bottom_row();
        }
    }
    else if(_XL_RIGHT(input))
    {
        if((player_x < MAX_PLAYER_X))
        {
            delete_player();
            ++player_x;
        }
        else
        {
            left_rotate_letters();
            display_bottom_row();
        }
    }
    else if(_XL_UP(input) && player_x>MIN_PLAYER_X && player_x<MAX_PLAYER_X)
    {
        up_rotate_letters();
        display_player_column();
    }
    else if(_XL_DOWN(input) && player_x>MIN_PLAYER_X && player_x<MAX_PLAYER_X)
    {
        down_rotate_letters();
        display_player_column();
    }
    else if(_XL_FIRE(input))
    {
        // TODO: DEBUG
        if(word_in_dictionary())
        {
            
            _XL_PRINT(XSize/2-3,YSize-3, "WORD FOUND");
            
            points += word_score();
            display_score();
            _XL_SLEEP(1);
            _XL_PRINT(XSize/2-3,YSize-1, "          ");
            remove_bottom_word();
        }
        else
        {
            drop_letter();
        }
    }
    display_player();
}


void title_screen(void)
{
    _XL_CLEAR_SCREEN();

    _XL_SET_TEXT_COLOR(_XL_WHITE);

    _XL_PRINT_CENTERED("WORDIZ");
    
    _XL_WAIT_FOR_INPUT();
}


void initialize_input_output(void)
{
    _XL_INIT_GRAPHICS();

    _XL_INIT_INPUT();
    
    _XL_INIT_SOUND();
}


void initialize_game(void)
{
    
    points = 0;
    level = 1; 
    alive = 1;
    
}


void display_level(void)
{
    _XL_PRINTD(XSize-3,0,2,level);
}


void initialize_level(void)
{
    uint8_t i;
    uint16_t compressed_word;
    uint16_t random_index;
    
    alive = 1;
    victory = 0;
    slot_index = 0;
    player_x = 2;
    counter = 1;
    
    display_player();
    
    
    for(i=0;i<NUMBER_OF_LETTERS;++i)
    {
        matrix_height[i]=0;
    }
    
    number_of_generated_words = MAX_NUMBER_OF_WORDS;
    
    for(i=0;i<number_of_generated_words;++i)
    {
        random_index = _XL_RAND()%DICTIONARY_SIZE;
        compressed_word = dictionary[random_index];

        generated_letters[i][0]=first_letter(random_index);
        
        generated_letters[i][1]=compressed_word>>12;
        
        generated_letters[i][2]=(compressed_word&0x0FFF)>>8;

        generated_letters[i][3]=(compressed_word&0x00FF)>>4;

        generated_letters[i][4]=compressed_word&0x000F;

    }
    
    display_matrix();
    for(i=0;i<INITIAL_DROP;++i)
    {
        drop_letter();
        _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
    }     
    
    display_level();
}

void re_start_game(void)
{
    title_screen();
    
    _XL_CLEAR_SCREEN();

    initialize_game();
    
    display_score();
}


void end_game(void)
{
    _XL_PRINT(XSize/2-4, YSize-3, "THE END");
    _XL_SLEEP(1);
    _XL_WAIT_FOR_INPUT();
}


void handle_drop(void)
{
    if(!(counter&63))
    {
        drop_letter();
    }
}


int main(void)
{        
    
    initialize_input_output();
    
    // main loop
    while(1)
    {
        re_start_game();

        // game main loop
        while((level<LAST_LEVEL+1) && alive)
        {
            initialize_level();

            // level main loop
            while(alive && !victory)
            {    
                handle_drop();
                
                handle_input();
                
                _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
                
                ++counter;
                
            }
            _XL_CLEAR_SCREEN();
        }
        end_game();
    }

    return EXIT_SUCCESS;
}

