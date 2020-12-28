; TAPBUF    $0333-03F2       819-1010   Cassette tape buffer
; WRLEN     $03F3-03F4      1011-1012   Length of data to be written to tape
; RDCNT     $03F5-03F6      1013-1014   Length of data to be read from tape
; INPQUE    $03F7-0436      1015-1078   RS-232 input queue
; ESTARL    $0437-0454      1079-1108
; ESTAKH    $0455-0472      1109-1138

; ZP candidates: $D0-$E8 

.export _snake_x;
_snake_x = $333

.export _snake_y;
_snake_y = _snake_x + 54 

.export _snake_copy_x;
_snake_copy_x = _snake_y + 54

.export _snake_copy_y;
_snake_copy_y = _snake_copy_x + 54

.export _extra_life_achievement;
_extra_life_achievement = _snake_copy_y + 54

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


.exportzp _head_image_ptr;
_head_image_ptr = 67;

.exportzp _rings;
_rings = 69

.export _vertical_mine_x;
_vertical_mine_x = $D0

.exportzp _vertical_mine_y;
_vertical_mine_y = $D2

.exportzp _vertical_mine_direction;
_vertical_mine_direction = $D4

.exportzp _vertical_mine_transition;
_vertical_mine_transition = $D6

.exportzp _points;
_points = $D8

.exportzp _slow_down;
_slow_down = $DA

.exportzp _record;
_record = $DC

.exportzp _level_bonus;
_level_bonus = $DE


