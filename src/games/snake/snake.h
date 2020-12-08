#ifndef _SNAKE_H
#define _SNAKE_H


#include "cross_lib.h"


#define INITIAL_SNAKE_LENGTH 3

#define MAX_APPLES 50

#define MAX_SNAKE_LENGTH ((XSize)+(MAX_APPLES)+((MAX_APPLES)/8)+10)
//254

#define EMPTY 0
#define DEADLY 1
#define BONUS 2
#define APPLE 3
#define WALL  4
#define HORIZONTAL_WALL 5
#define VERTICAL_WALL 6
#define TRANSPARENT 7

#define NUMBER_OF_LEVELS 16


void draw_head(void);

void draw_body_part(uint8_t i);

void delete_body_part(uint8_t i);

void draw_snake(void);

void init_snake(void);

void init_map(void);

void snake_grows(void);

#endif // _SNAKE_H

