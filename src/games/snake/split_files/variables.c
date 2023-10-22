#include "cross_lib.h"
#include "settings.h"

#if !defined(USE_MEMORY_BUFFERS)
uint8_t snake_head_x;
uint8_t snake_head_y;
uint8_t speed_increase_counter;
uint8_t spawned_apples;
uint8_t remaining_apples;
uint8_t level;

uint8_t snake_x[MAX_SNAKE_LENGTH];
uint8_t snake_y[MAX_SNAKE_LENGTH];
uint8_t snake_copy_x[MAX_SNAKE_LENGTH];
uint8_t snake_copy_y[MAX_SNAKE_LENGTH];

uint16_t points;
uint16_t slow_down;
uint16_t record;
uint16_t level_bonus;


uint8_t horizontal_mine_x[MAX_NUMBER_OF_HORIZONTAL_MINES];
uint8_t horizontal_mine_y[MAX_NUMBER_OF_HORIZONTAL_MINES];
uint8_t horizontal_mine_direction[MAX_NUMBER_OF_HORIZONTAL_MINES];
uint8_t horizontal_mine_transition[MAX_NUMBER_OF_HORIZONTAL_MINES];

uint8_t vertical_mine_x[MAX_NUMBER_OF_VERTICAL_MINES];
uint8_t vertical_mine_y[MAX_NUMBER_OF_VERTICAL_MINES];
uint8_t vertical_mine_direction[MAX_NUMBER_OF_VERTICAL_MINES];
uint8_t vertical_mine_transition[MAX_NUMBER_OF_VERTICAL_MINES];


// It records if one extra life was taken in bonus/secret levels
uint8_t extra_life_achievement[9];

// It records if the magic wall was created in bonus/secret levels
uint8_t magic_wall_achievement[9];

// It records if the super coin was spawned in bonus/secret/levels
uint8_t coin_achievement[9];

uint8_t energy;

uint8_t lives;

uint8_t snake_length;
uint8_t snake_head;
uint8_t snake_direction;

uint8_t apples_on_screen_count;
uint8_t coin_count;
uint8_t extra_life_counter;
uint8_t active_mines;


uint8_t horizontal_mines_on_current_level;
uint8_t vertical_mines_on_current_level;
uint8_t transparent_vertical_wall_triggered;
uint8_t transparent_horizontal_wall_triggered;
uint8_t secret_level_active;
uint8_t secret_level_never_activated;
uint8_t next_level;
uint8_t transparent_vertical_wall_level_flag;
uint8_t transparent_horizontal_wall_level_flag;
uint8_t head_tile;
uint8_t secret_passage[2*NUMBER_OF_MAPS];

uint8_t rings;

uint8_t extra_count;
uint8_t third_coin_achievement;
uint8_t fourth_coin_achievement;
uint8_t i;
uint8_t j;

#endif



uint8_t map[XSize][YSize];


