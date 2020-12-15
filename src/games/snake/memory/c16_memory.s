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

