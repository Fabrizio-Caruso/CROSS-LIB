; $334-$3FF  (820-1023)
;0334-033B    820-827       ??
;*033C-03FB   828-1019      Cassette buffer

; $200 - $276 (512-630)
;*0200-0258  512-600  Basic input buffer
;*0259-0262  601-610  Logical file table
;*0263-026C  611-620  Device # table
;*026D-0276  621-630  Secondary Address table
.export _snake_x;
_snake_x = $33C

.export _snake_y;
_snake_y = _snake_x + 45 

.export _snake_copy_x;
_snake_copy_x = _snake_y + 45

.export _snake_copy_y;
_snake_copy_y = _snake_copy_x + 45

.export _extra_life_achievement;
_extra_life_achievement = _snake_copy_y + 45

.export _magic_wall_achievement;
_magic_wall_achievement =  $200

.export _super_coin_achievement;
_super_coin_achievement = _magic_wall_achievement + 9

.export _horizontal_mine_x;
_horizontal_mine_x = _super_coin_achievement + 9

.export _horizontal_mine_y;
_horizontal_mine_y = _horizontal_mine_x + 4

.export _horizontal_mine_direction;
_horizontal_mine_direction = _horizontal_mine_y + 4

.export _horizontal_mine_transition;
_horizontal_mine_transition = _horizontal_mine_direction + 4

.export _vertical_mine_x;
_vertical_mine_x = _horizontal_mine_transition + 4

.export _vertical_mine_y;
_vertical_mine_y = _vertical_mine_x + 2

.export _vertical_mine_direction;
_vertical_mine_direction = _vertical_mine_y + 2

.export _vertical_mine_transition;
_vertical_mine_transition = _vertical_mine_direction + 2


.export _points;
_points = _vertical_mine_transition + 2

.export _slow_down;
_slow_down = _points + 2

.export _record;
_record = _slow_down + 2

.export _level_bonus;
_level_bonus = _record + 2


.segment "ZEROPAGE"

.exportzp _snake_head_x;
_snake_head_x = 43

.exportzp _snake_head_y;
_snake_head_y = 44

.exportzp _speed_increase_counter;
_speed_increase_counter = 45

.exportzp _spawned_apples;
_spawned_apples = 46

.exportzp _remaining_apples;
_remaining_apples = 47

.exportzp _level;
_level = 48

.exportzp _energy;
_energy = 49

.exportzp _lives;
_lives = 50

.exportzp _snake_head;
_snake_head = 51

.exportzp _snake_length;
_snake_length = 52

.exportzp _snake_direction;
_snake_direction = 53

.exportzp _apples_on_screen_count;
_apples_on_screen_count = 54

.exportzp _coin_count;
_coin_count = 55

.exportzp _extra_life_counter;
_extra_life_counter = 56

.exportzp _active_mines;
_active_mines = 57

.exportzp _horizontal_mines_on_current_level;
_horizontal_mines_on_current_level = 58

.exportzp _vertical_mines_on_current_level;
_vertical_mines_on_current_level = 59

.exportzp _transparent_vertical_wall_triggered;
_transparent_vertical_wall_triggered = 60

.exportzp _transparent_horizontal_wall_triggered;
_transparent_horizontal_wall_triggered = 61

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

.exportzp _not_many_mines;
_not_many_mines = 67


