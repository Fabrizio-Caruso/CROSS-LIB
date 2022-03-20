org &4000

txt_output equ &bb5a
scr_set_mode equ &bc0e
km_wait_char equ &bb06

start:
ld a,1
call scr_set_mode
ld hl,message
call display_msg
call km_wait_char
rst 0

message:
defb "Welcome to the test",0

display_msg:
ld a,(hl)
inc hl
or a
ret z
call txt_output
jr display_msg

jp start