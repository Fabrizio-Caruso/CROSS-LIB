#ifndef _SNAKE_H
#define _SNAKE_H


#include "cross_lib.h"


#define INITIAL_SNAKE_LENGTH 3

#define MAX_APPLES 24

// This is to accout for extra length gained by eating extra points ($)
#define EXTRA_LENGTH 10

#define MAX_SNAKE_LENGTH ((XSize/4)+((NUMBER_OF_LEVELS-1)/4)+(MAX_APPLES)+((MAX_APPLES)/8)+EXTRA_LENGTH)
//254

#define EMPTY 0
#define DEADLY 1
#define SUPER_COIN 2
#define COIN 3
#define EXTRA 4
#define APPLE 5
#define WALL  6
#define HORIZONTAL_WALL 7
#define VERTICAL_WALL 8
#define TRANSPARENT 9

#define NUMBER_OF_LEVELS 16


void draw_head(void);

void draw_body_part(uint8_t i);

void delete_body_part(uint8_t i);

void draw_snake(void);

void init_snake(void);

void init_map(void);

void snake_grows(void);

#endif // _SNAKE_H

