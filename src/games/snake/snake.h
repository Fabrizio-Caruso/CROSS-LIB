#ifndef _SNAKE_H
#define _SNAKE_H


#include "cross_lib.h"
#include "settings.h"


void draw_head(void);

void draw_body_part(uint8_t i);

void delete_body_part(uint8_t i);

void draw_snake(void);

void init_snake(void);

void init_map(void);

void snake_grows(void);

#endif // _SNAKE_H

