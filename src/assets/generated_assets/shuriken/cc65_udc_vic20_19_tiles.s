		.segment "UDCCHAR"
		.export _Charset

_Charset:


; 0xEC player up
.byte 208,144,16,16,48,32,224,128

; 0xED gun
.byte 48,32,224,128,0,0,0,0

; 0xEE pound - invincibility
.byte 31,112,66,195,159,185,137,143

; 0xEF powerup
.byte 204,68,112,31,0,0,0,0

; 0xF0 rocket
.byte 32,48,63,38,100,252,12,4

; 0xF1 left missile
.byte 1,7,4,12,8,8,9,11

; 0xF2 space
.byte $00,$00,$00,$00,$00,$00,$00,$00

; 0xF3 bullet
.byte 128,224,32,48,16,16,208,144

; 0xF4 vertical
.byte 12,4,7,1,0,0,0,0

; 0xF5 horizontal
.byte  51,18,14,248,0,0,0,0

; 0xF6 ghost
.byte 9,11,8,8,12,4,7,1

; 0xF7 skull
.byte 0,0,0,0,248,14,34,51

; 0xF8 bomb
.byte 241,145,157,249,195,66,14,248

; 0xF9 player down 
.byte 143,137,153,191,195,65,112,31

; 0xFA player left
.byte 0,0,0,0,128,224,32,48

; 0xFB player right 
.byte 0,0,0,0,31,112,72,204

; 0xFC right missile from the right
.byte 248,14,130,195,253,153,145,241


; 0xFD freeze
.byte 39,62,48,16,0,0,0,0


; 0xFE super
.byte 0,0,0,0,8,12,124,228

; 0xFF dead ghost
.byte 0,0,0,0,1,7,4,12
