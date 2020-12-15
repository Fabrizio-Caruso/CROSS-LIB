#ifndef _VARIABLES_H
#define _VARIABLES_H

#include "settings.h"

#if defined(USE_MEMORY_BUFFERS)
    #define _EXTERN extern
#else
    #define _EXTERN
#endif

_EXTERN uint8_t snake_x[MAX_SNAKE_LENGTH];
_EXTERN uint8_t snake_y[MAX_SNAKE_LENGTH];

_EXTERN uint8_t snake_copy_x[MAX_SNAKE_LENGTH];
_EXTERN uint8_t snake_copy_y[MAX_SNAKE_LENGTH];

uint8_t snake_head_x;
uint8_t snake_head_y;

uint16_t points;

uint8_t speed_increase_counter;

uint16_t slow_down;

uint8_t spawned_apples;

uint8_t lives;

uint16_t record;

uint8_t remaining_apples;

uint8_t level;

uint8_t energy;

uint8_t coin_count;

uint8_t extra_life_counter;

uint8_t active_mines;

uint16_t level_bonus;

uint8_t transparent_vertical_wall_triggered;
uint8_t transparent_horizontal_wall_triggered;

uint8_t secret_level_active;
uint8_t secret_level_never_activated;
uint8_t next_level;


uint8_t horizontal_mine_x[MAX_NUMBER_OF_HORIZONTAL_MINES];
uint8_t horizontal_mine_y[MAX_NUMBER_OF_HORIZONTAL_MINES];
uint8_t horizontal_mine_direction[MAX_NUMBER_OF_HORIZONTAL_MINES];
uint8_t horizontal_mines_on_current_level;
uint8_t horizontal_mine_transition[MAX_NUMBER_OF_HORIZONTAL_MINES];

uint8_t vertical_mine_x[MAX_NUMBER_OF_VERTICAL_MINES];
uint8_t vertical_mine_y[MAX_NUMBER_OF_VERTICAL_MINES];
uint8_t vertical_mine_direction[MAX_NUMBER_OF_VERTICAL_MINES];
uint8_t vertical_mines_on_current_level;
uint8_t vertical_mine_transition[MAX_NUMBER_OF_VERTICAL_MINES];

#endif // _VARIABLES_H

