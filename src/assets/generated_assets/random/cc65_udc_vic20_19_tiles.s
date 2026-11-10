		.segment "UDCCHAR"
		.export _Charset

_Charset:


; 0xEC player up
.byte 0,0,24, 36, 24,102,89, 24

; 0xED gun
.byte 2,7,2,63,66,162,159,5

; 0xEE pound - invincibility
.byte 0,0,0,192,32,80,144,0

; 0xEF powerup
.byte 60,126,231,195,195,231,126,60

; 0xF0 rocket
.byte 0,0,0,252,2,5,249,0

; 0xF1 left missile
.byte $2A,$2A,$2A,$3A,$2A,$2A,$2A,$00

; 0xF2 space
.byte $00,$00,$00,$00,$00,$00,$00,$00

; 0xF3 bullet
.byte 0,0,0,3,4,10,9,0

; 0xF4 vertical
.byte 32,112,32,252,34,37,249,80

; 0xF5 horizontal
.byte  0,  0,  0,63,64,160,159,0

; 0xF6 ghost
.byte 153, 24, 36,36,0,0,0,0

; 0xF7 skull
.byte 24, 36, 24,  0,153,  0, 36,36

; 0xF8 bomb
.byte 0,0,0,0,0,0,24, 36

; 0xF9 player down 
.byte 24, 36, 24,102,153, 24, 36,36

; 0xFA player left
.byte 0,0,0,0,24, 36, 24,102

; 0xFB player right 
.byte 36,36,0,0,0,0,0,0

; 0xFC right missile from the right
.byte 74,74,74,74,74,74,100,0


; 0xFD freeze
.byte 2,7,2,2,2,2,2,5


; 0xFE super
.byte 32,112,32,32,32,32,32,80

; 0xFF dead ghost
.byte 24,102,154, 24, 36,36,0,0
