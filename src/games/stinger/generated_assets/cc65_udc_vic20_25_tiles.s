		.segment "UDCCHAR"
		.export _Charset

_Charset:

; 0xE6
.byte 198,186,254,170,238,186,214,16

; 0xE7
.byte 254,170,238,186,214,16,0,0

; 0xE8
.byte 238,186,214,16,0,0,0,0

; 0xE9
.byte 214,16,0,0,0,0,0,0

; 0xEA
.byte 0,24,60,122,253,122,60,24

; 0xEB
.byte 120,252,255,133,255,161,255,0


; 0xEC player up
.byte 0,0,198,186,254,170,238,186

; 0xED gun
.byte 4,14,14,4,63,68,228,64

; 0xEE pound - invincibility
.byte 0,0,0,0,128,160,64,160

; 0xEF powerup
.byte 60,126,231,195,195,231,126,60

; 0xF0 rocket
.byte 0,0,0,0,248,4,14,4

; 0xF1 left missile
.byte 198,186,68,84,186,214,16,16

; 0xF2 space
.byte $00,$00,$00,$00,$00,$00,$00,$00

; 0xF3 bullet
.byte 0,0,0,0,3,10,4,10

; 0xF4 vertical
.byte 64,224,224,64,248,68,78,4

; 0xF5 horizontal
.byte  0,0,0,0,63,64,224,64

; 0xF6 ghost
.byte 238,186,170,16,0,0,0,0

; 0xF7 skull
.byte 198,186,0,170,0,186,214,16

; 0xF8 bomb
.byte 0,0,0,0,0,0,198,186

; 0xF9 player down 
.byte 198,186,254,170,238,186,170,16

; 0xFA player left
.byte 0,0,0,0,198,186,254,170

; 0xFB player right 
.byte 170,16,0,0,0,0,0,0

; 0xFC right missile from the right
.byte 0,0,0,16,130,16,84,16


; 0xFD freeze
.byte 4,14,14,4,4,4,0,0


; 0xFE super
.byte 64,224,224,64,64,64,0,0

; 0xFF dead ghost
.byte 254,170,238,186,170,16,0,0
