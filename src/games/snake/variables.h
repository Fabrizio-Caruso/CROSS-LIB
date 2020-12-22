#ifndef _VARIABLES_H
#define _VARIABLES_H

#include "settings.h"


extern uint8_t snake_head_x;
#if defined(USE_MEMORY_BUFFERS)
#pragma zpsym("snake_head_x")
#endif

extern uint8_t snake_head_y;
#if defined(USE_MEMORY_BUFFERS)
#pragma zpsym("snake_head_y")
#endif

extern uint8_t speed_increase_counter;
#if defined(USE_MEMORY_BUFFERS)
#pragma zpsym("speed_increase_counter")
#endif

extern uint8_t spawned_apples;
#if defined(USE_MEMORY_BUFFERS)
#pragma zpsym("spawned_apples")
#endif

extern uint8_t remaining_apples;
#if defined(USE_MEMORY_BUFFERS)
#pragma zpsym("remaining_apples")
#endif

extern uint8_t level;
#if defined(USE_MEMORY_BUFFERS)
#pragma zpsym("level")
#endif

extern uint8_t energy;
#if defined(USE_MEMORY_BUFFERS)
#pragma zpsym("energy")
#endif

extern uint8_t lives;
#if defined(USE_MEMORY_BUFFERS)
#pragma zpsym("lives")
#endif

extern uint8_t snake_length;
#if defined(USE_MEMORY_BUFFERS)
#pragma zpsym("snake_length")
#endif

extern uint8_t snake_head;
#if defined(USE_MEMORY_BUFFERS)
#pragma zpsym("snake_head")
#endif

extern uint8_t snake_direction;
#if defined(USE_MEMORY_BUFFERS)
#pragma zpsym("snake_direction")
#endif

extern uint8_t apples_on_screen_count;
#if defined(USE_MEMORY_BUFFERS)
#pragma zpsym("apples_on_screen_count")
#endif

extern uint8_t coin_count;
#if defined(USE_MEMORY_BUFFERS)
#pragma zpsym("coin_count")
#endif

extern uint8_t extra_life_counter;
#if defined(USE_MEMORY_BUFFERS)
#pragma zpsym("extra_life_counter")
#endif

extern uint8_t active_mines;
#if defined(USE_MEMORY_BUFFERS)
#pragma zpsym("active_mines")
#endif

extern uint8_t horizontal_mines_on_current_level;
#if defined(USE_MEMORY_BUFFERS)
#pragma zpsym("horizontal_mines_on_current_level")
#endif

extern uint8_t vertical_mines_on_current_level;
#if defined(USE_MEMORY_BUFFERS)
#pragma zpsym("vertical_mines_on_current_level")
#endif

extern uint8_t transparent_vertical_wall_triggered;
#if defined(USE_MEMORY_BUFFERS)
#pragma zpsym("transparent_vertical_wall_triggered")
#endif

extern uint8_t transparent_horizontal_wall_triggered;
#if defined(USE_MEMORY_BUFFERS)
#pragma zpsym("transparent_horizontal_wall_triggered")
#endif

extern uint8_t secret_level_active;
#if defined(USE_MEMORY_BUFFERS)
#pragma zpsym("secret_level_active")
#endif

extern uint8_t secret_level_never_activated;
#if defined(USE_MEMORY_BUFFERS)
#pragma zpsym("secret_level_never_activated")
#endif

extern uint8_t next_level;
#if defined(USE_MEMORY_BUFFERS)
#pragma zpsym("next_level")
#endif

extern uint8_t transparent_vertical_wall_level_flag;
#if defined(USE_MEMORY_BUFFERS)
#pragma zpsym("transparent_vertical_wall_level_flag")
#endif

extern uint8_t transparent_horizontal_wall_level_flag;
#if defined(USE_MEMORY_BUFFERS)
#pragma zpsym("transparent_horizontal_wall_level_flag")
#endif

extern uint8_t not_many_mines;
#if defined(USE_MEMORY_BUFFERS)
#pragma zpsym("not_many_mines")
#endif

extern const Image *head_image_ptr;
#if defined(USE_MEMORY_BUFFERS)
#pragma zpsym("head_image_ptr")
#endif

extern char strings[NUMBER_OF_STRINGS][MAX_STRING_SIZE];

extern uint8_t snake_x[MAX_SNAKE_LENGTH];
extern uint8_t snake_y[MAX_SNAKE_LENGTH];
extern uint8_t snake_copy_x[MAX_SNAKE_LENGTH];
extern uint8_t snake_copy_y[MAX_SNAKE_LENGTH];

extern uint16_t points;
#if defined(USE_MEMORY_BUFFERS)
#pragma zpsym("points")
#endif

extern uint16_t slow_down;
#if defined(USE_MEMORY_BUFFERS)
#pragma zpsym("slow_down")
#endif

extern uint16_t record;
#if defined(USE_MEMORY_BUFFERS)
#pragma zpsym("record")
#endif

extern uint16_t level_bonus;
#if defined(USE_MEMORY_BUFFERS)
#pragma zpsym("level_bonus")
#endif


extern uint8_t horizontal_mine_x[MAX_NUMBER_OF_HORIZONTAL_MINES];
extern uint8_t horizontal_mine_y[MAX_NUMBER_OF_HORIZONTAL_MINES];
extern uint8_t horizontal_mine_direction[MAX_NUMBER_OF_HORIZONTAL_MINES];
extern uint8_t horizontal_mine_transition[MAX_NUMBER_OF_HORIZONTAL_MINES];


extern uint8_t vertical_mine_x[MAX_NUMBER_OF_VERTICAL_MINES];
#if defined(USE_MEMORY_BUFFERS)
#pragma zpsym("vertical_mine_x")
#endif

extern uint8_t vertical_mine_y[MAX_NUMBER_OF_VERTICAL_MINES];
#if defined(USE_MEMORY_BUFFERS)
#pragma zpsym("vertical_mine_y")
#endif

extern uint8_t vertical_mine_direction[MAX_NUMBER_OF_VERTICAL_MINES];
#if defined(USE_MEMORY_BUFFERS)
#pragma zpsym("vertical_mine_direction")
#endif

extern uint8_t vertical_mine_transition[MAX_NUMBER_OF_VERTICAL_MINES];
#if defined(USE_MEMORY_BUFFERS)
#pragma zpsym("vertical_mine_transition")
#endif

// It records if one extra life was taken in bonus/secret levels
extern uint8_t extra_life_achievement[9];

// It records if the magic wall was created in bonus/secret levels
extern uint8_t magic_wall_achievement[9];

// It records if the super coin was spawned in bonus/secret/levels
extern uint8_t super_coin_achievement[9];


extern uint8_t map[XSize][YSize];


extern const Image VERTICAL_HEAD_IMAGE;
extern const Image HORIZONTAL_HEAD_IMAGE;
extern const Image LEFT_MINE_IMAGE;
extern const Image RIGHT_MINE_IMAGE;
extern const Image BODY_IMAGE;
extern const Image APPLE_IMAGE;
extern const Image HI_TEXT_IMAGE;
extern const Image CENTRAL_BRICK_IMAGE;
extern const Image COIN_IMAGE;
extern const Image LV_TEXT_IMAGE;
extern const Image TRANSPARENT_BRICK_IMAGE;
extern const Image UP_MINE_IMAGE;
extern const Image VERTICAL_BRICK_IMAGE;
extern const Image HORIZONTAL_BRICK_IMAGE;
extern const Image SCORE_TEXT_LEFT_IMAGE;
extern const Image DOWN_MINE_IMAGE;
extern const Image SCORE_TEXT_RIGHT_IMAGE;
extern const Image MINE_IMAGE;
extern const Image SUPER_COIN_IMAGE;
extern const Image EXTRA_IMAGE;

extern uint8_t secret_passage[2*NUMBER_OF_MAPS];
#endif // _VARIABLES_H

