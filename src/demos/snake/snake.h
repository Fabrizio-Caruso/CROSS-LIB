#ifndef _SNAKE_H
#define _SNAKE_H


#include "cross_lib.h"


#define INIT_SNAKE_LENGTH 10
#define MAX_SNAKE_LENGTH 20

struct SnakeBodyStruct
{
    uint8_t x;
    uint8_t y;
};
typedef struct SnakeBodyStruct SnakeBody;

void draw_head(void);

void draw_body_part(uint8_t i);

void draw_snake(void);

void init_snake(void);

#endif // _SNAKE_H

