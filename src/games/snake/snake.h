#ifndef _SNAKE_H
#define _SNAKE_H


#include "cross_lib.h"


#define INIT_SNAKE_LENGTH 4
#define MAX_SNAKE_LENGTH 240

#define EMPTY 0
#define SNAKE 1
#define BONUS 2

struct SnakeBodyStruct
{
    uint8_t x;
    uint8_t y;
};
typedef struct SnakeBodyStruct SnakeBody;

void draw_head(void);

void draw_body_part(uint8_t i);

void delete_body_part(uint8_t i);

void draw_snake(void);

void init_snake(void);

void init_map(void);

void snake_grows(void);

#endif // _SNAKE_H

