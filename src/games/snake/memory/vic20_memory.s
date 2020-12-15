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

E_IMAGE + $2

