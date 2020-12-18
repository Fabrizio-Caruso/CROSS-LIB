; TAPBUF    $0333-03F2       819-1010   Cassette tape buffer
; WRLEN     $03F3-03F4      1011-1012   Length of data to be written to tape
; RDCNT     $03F5-03F6      1013-1014   Length of data to be read from tape
; INPQUE    $03F7-0436      1015-1078   RS-232 input queue
; ESTARL    $0437-0454      1079-1108
; ESTAKH    $0455-0472      1109-1138

.export _snake_x;
_snake_x = $333

.export _snake_y;
_snake_y = _snake_x + 50 

.export _snake_copy_x;
_snake_copy_x = _snake_y + 50

.export _snake_copy_y;
_snake_copy_y = _snake_copy_x + 50

.export _extra_life_achievement;
_extra_life_achievement = _snake_copy_y + 45

.export _magic_wall_achievement;
_magic_wall_achievement = _extra_life_achievement + 9

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
