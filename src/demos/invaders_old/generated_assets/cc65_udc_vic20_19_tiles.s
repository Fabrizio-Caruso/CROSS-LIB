		.segment "UDCCHAR"
		.export _Charset

_Charset:


; 0xEC player up
.byte 0,128,128,240,248,248,248,248

; 0xED gun
.byte 37,37,37,37,37,37,50,0

; 0xEE pound - invincibility
.byte 66,66,90,126,60,60,60,24

; 0xEF powerup
.byte 255,255,255,192,129,255,255,255

; 0xF0 rocket
.byte 8,18,250,238,254,252,8,4

; 0xF1 left missile
.byte 128,0,128,192,224,160,160,0

; 0xF2 space
.byte $00,$00,$00,$00,$00,$00,$00,$00

; 0xF3 bullet
.byte 32,17,63,110,255,191,160,27

; 0xF4 vertical
.byte 15,127,255,230,255,25,54,192

; 0xF5 horizontal
.byte  0,224,240,112,240,128,192,48

; 0xF6 ghost
.byte 64,224,224,252,254,254,254,254

; 0xF7 skull
.byte 0,7,15,14,15,3,6,3

; 0xF8 bomb
.byte 1,3,7,13,15,2,5,10

; 0xF9 player down 
.byte 1,3,3,31,63,63,63,63

; 0xFA player left
.byte 64,224,224,252,254,254,254,254

; 0xFB player right 
.byte 0,0,0,7,15,15,15,15

; 0xFC right missile from the right
.byte 2,9,11,14,15,7,2,4


; 0xFD freeze
.byte 0,0,0,0,224,240,0,0


; 0xFE super
.byte 24,60,126,219,255,90,129,66

; 0xFF dead ghost
.byte 128,192,224,176,240,64,160,80
