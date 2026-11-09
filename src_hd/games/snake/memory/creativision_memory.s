
.export _snake_x;
_snake_x = $0100

.export _snake_y;
_snake_y = _snake_x + 12

.export _snake_copy_x;
_snake_copy_x = _snake_y + 12

.export _snake_copy_y;
_snake_copy_y = _snake_copy_x + 12

.export _extra_life_achievement;
_extra_life_achievement = _snake_copy_y + 12

.export _magic_wall_achievement;
_magic_wall_achievement = _extra_life_achievement + 9

.export _coin_achievement;
_coin_achievement = _magic_wall_achievement + 9

.export _horizontal_mine_x;
_horizontal_mine_x = _coin_achievement + 9

.export _horizontal_mine_y;
_horizontal_mine_y = _horizontal_mine_x + 4

.export _horizontal_mine_direction;
_horizontal_mine_direction = _horizontal_mine_y + 4

.export _horizontal_mine_transition;
_horizontal_mine_transition = _horizontal_mine_direction + 4

.export _secret_passage;
_secret_passage = _horizontal_mine_transition + 4

.export _extra_count;
_extra_count = _secret_passage + 32

.export _third_coin_achievement;
_third_coin_achievement = _extra_count + 1

.export _fourth_coin_achievement;
_fourth_coin_achievement = _third_coin_achievement + 1

.export _i;
_i = _fourth_coin_achievement + 1

.export _j;
_j = _i + 1



.segment "ZEROPAGE"

.exportzp _snake_head_x;
_snake_head_x = $E9

.exportzp _snake_head_y;
_snake_head_y = $E8

.exportzp _speed_increase_counter;
_speed_increase_counter = 45

.exportzp _spawned_apples;
_spawned_apples = $D5

.exportzp _remaining_apples;
_remaining_apples = $EE

.exportzp _level;
_level = $EF

.exportzp _energy;
_energy = $ED

.exportzp _lives;
_lives = 50

.exportzp _snake_head;
_snake_head = $EC

.exportzp _snake_length;
_snake_length = $EB

.exportzp _snake_direction;
_snake_direction = $EA

.exportzp _apples_on_screen_count;
_apples_on_screen_count = 54

.exportzp _coin_count;
_coin_count = $D6

.exportzp _extra_life_counter;
_extra_life_counter = 56

.exportzp _active_mines;
_active_mines = $E7

.exportzp _horizontal_mines_on_current_level;
_horizontal_mines_on_current_level = $E6

.exportzp _vertical_mines_on_current_level;
_vertical_mines_on_current_level = $E5

.exportzp _transparent_vertical_wall_triggered;
_transparent_vertical_wall_triggered = $D4

.exportzp _transparent_horizontal_wall_triggered;
_transparent_horizontal_wall_triggered = $D3

.exportzp _secret_level_active;
_secret_level_active = 62

.exportzp _secret_level_never_activated;
_secret_level_never_activated = 63

.exportzp _next_level;
_next_level = 64

.exportzp _transparent_vertical_wall_level_flag;
_transparent_vertical_wall_level_flag = 65

.exportzp _transparent_horizontal_wall_level_flag;
_transparent_horizontal_wall_level_flag = 66

.exportzp _head_tile;
_head_tile = 67;

.exportzp _rings;
_rings = $D7

.export _vertical_mine_x;
_vertical_mine_x = $C1

.exportzp _vertical_mine_y;
_vertical_mine_y = $C3

.exportzp _vertical_mine_direction;
_vertical_mine_direction = $C5

.exportzp _vertical_mine_transition;
_vertical_mine_transition = $C7

.exportzp _points;
_points = $D8

.exportzp _slow_down;
_slow_down = $DA

.exportzp _record;
_record = $DC

.exportzp _level_bonus;
_level_bonus = $DE

