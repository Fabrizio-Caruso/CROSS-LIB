		.segment "UDCCHAR"
		.export _Charset

_Charset:


; 0xE4
.byte 0,39,101,37,39,33,113,0

; 0xE5
.byte 0,50,85,21,37,69,114,0

; 0xE6
.byte 0,50,86,18,34,66,119,0

; 0xE7
.byte 0,51,85,17,34,68,119,0

; 0xE8
.byte 0,55,81,17,39,65,119,0

; 0xE9
.byte 0,53,85,21,39,65,113,0

; 0xEA
.byte 0,55,84,20,39,65,119,0

; 0xEB
.byte 0,55,84,20,39,69,119,0

; 0xEC player up
.byte 0,34,86,82,82,82,39,0

; 0xED gun
.byte 0,39,85,85,87,81,33,0

; 0xEE pound - invincibility
.byte 0,34,101,37,37,37,114,0

; 0xEF powerup
.byte 0,39,85,85,87,85,39,0

; 0xF0 rocket
.byte 0,39,100,36,39,37,119,0

; 0xF1 left missile
.byte 0,37,101,37,39,33,113,0

; 0xF2 space
.byte $00,$00,$00,$00,$00,$00,$00,$00

; 0xF3 bullet
.byte 0,34,102,34,34,34,119,0

; 0xF4 vertical
.byte 0,35,101,33,34,36,119,0

; 0xF5 horizontal
.byte  0,39,97,33,39,33,119,0

; 0xF6 ghost
.byte 0,37,85,85,87,81,33,0

; 0xF7 skull
.byte 0,39,81,81,82,82,34,0

; 0xF8 bomb
.byte 0,39,84,84,87,81,39,0

; 0xF9 player down 
.byte 0,34,85,85,85,85,34,0

; 0xFA player left
.byte 0,39,81,81,87,81,39,0

; 0xFB player right 
.byte 0,35,85,81,82,84,39,0

; 0xFC right missile from the right
.byte 0,39,100,36,39,33,119,0


; 0xFD freeze
.byte 0,39,97,33,34,34,114,0


; 0xFE super
.byte 0,39,101,37,39,37,119,0

; 0xFF dead ghost
.byte 0,39,84,84,87,85,39,0

