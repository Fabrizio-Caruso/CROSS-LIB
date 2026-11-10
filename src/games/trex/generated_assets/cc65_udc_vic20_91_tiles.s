		.segment "UDCCHAR"
		.export _Charset

_Charset:


; 0xE4
.byte 0,128,176,176,176,176,176,176

; 0xE5
.byte 13,15,7,1,1,1,1,1

; 0xE6
.byte 176,224,128,128,128,128,128,128

; 0xE7
.byte 0,32,112,255,31,7,6,4

; 0xE8
.byte 0,0,0,240,192,48,0,0

; 0xE9
.byte 0,2,7,15,1,0,0,0

; 0xEA
.byte 64,96,120,255,252,3,0,0

; 0xEB
.byte 62,87,110,87,112,124,224,224

; 0xEC player up
.byte 0,24,27,27,219,219,219,219

; 0xED gun
.byte 63,8,8,12,0,0,0,0

; 0xEE pound - invincibility
.byte 0,24,216,216,219,219,219,219

; 0xEF powerup
.byte 0,0,0,0,131,207,255,127

; 0xF0 rocket
.byte 192,248,200,192,128,128,192,0

; 0xF1 left missile
.byte 255,255,0,0,32,0,0,0

; 0xF2 space
.byte $00,$00,$00,$00,$00,$00,$00,$00

; 0xF3 bullet
.byte 0,1,13,13,13,13,13,13

; 0xF4 vertical
.byte 0,128,128,128,176,176,176,176

; 0xF5 horizontal
.byte  255,255,0,12,0,0,0,0

; 0xF6 ghost
.byte 219,254,120,24,24,24,24,24

; 0xF7 skull
.byte 128,128,128,192,0,0,0,0

; 0xF8 bomb
.byte 255,255,0,0,0,0,0,0

; 0xF9 player down 
.byte 62,111,127,127,112,124,224,224

; 0xFA player left
.byte 192,248,200,192,128,128,128,192

; 0xFB player right 
.byte 131,207,255,127,63,8,8,12

; 0xFC right missile from the right
.byte 0,0,0,0,62,111,127,127


; 0xFD freeze
.byte 131,207,255,127,63,8,12,0


; 0xFE super
.byte 0,1,1,1,13,13,13,13

; 0xFF dead ghost
.byte 112,124,224,224,192,248,200,192

