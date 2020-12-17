; $334-$3FF  (820-1023)
;0334-033B    820-827       ??
;*033C-03FB   828-1019      Cassette buffer

.export _snake_x;
_snake_x = $334

.export _snake_y;
_snake_y = _snake_x + 45 

.export _snake_copy_x;
_snake_copy_x = _snake_y + 45

.export _snake_copy_y;
_snake_copy_y = _snake_copy_x + 45

.export _extra_life_achievement;
_extra_life_achievement = _snake_copy_y + 45

.export _magic_wall_achievement;
_magic_wall_achievement = _extra_life_achievement + 9

.export _super_coin_achievement;
_super_coin_achievement = $200

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

