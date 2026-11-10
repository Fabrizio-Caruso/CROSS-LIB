		.segment "UDCCHAR"
		.export _Charset

_Charset:

; 0xE6
.byte 0,24,60,90,24,36,0,0

; 0xE7
.byte 0,0,0,24,24,0,0,0

; 0xE8
.byte 16,0,2,72,2,21,75,21

; 0xE9
.byte 130,16,64,162,72,160,90,224

; 0xEA
.byte 7,42,133,18,5,32,4,0

; 0xEB
.byte 168,210,168,64,18,64,8,32


; 0xEC player up
.byte 170,85,170,85,170,85,170,85

; 0xED gun
.byte 255,252,252,248,248,240,248,188

; 0xEE pound - invincibility
.byte 0,0,84,124,108,56,56,189

; 0xEF powerup
.byte 13,13,25,27,59,63,31,7

; 0xF0 rocket
.byte 0,0,16,8,4,2,1,0

; 0xF1 left missile
.byte 0,0,0,0,0,0,255,0

; 0xF2 space
.byte $00,$00,$00,$00,$00,$00,$00,$00

; 0xF3 bullet
.byte 21,31,27,31,14,14,14,94

; 0xF4 vertical
.byte 255,255,255,255,170,255,255,0

; 0xF5 horizontal
.byte  254,254,254,254,170,254,254,0

; 0xF6 ghost
.byte 255,63,63,31,31,15,31,61

; 0xF7 skull
.byte 0,28,54,127,240,240,248,248

; 0xF8 bomb
.byte 176,176,152,216,220,252,248,224

; 0xF9 player down 
.byte 60,66,165,129,153,165,66,60

; 0xFA player left
.byte 0,0,0,0,1,6,12,152

; 0xFB player right 
.byte 0,56,108,254,15,15,31,31

; 0xFC right missile from the right
.byte 0,0,0,0,1,2,4,16


; 0xFD freeze
.byte 85,42,21,10,21,10,21,40


; 0xFE super
.byte 24,126,90,219,24,36,66,195

; 0xFF dead ghost
.byte 0,0,0,0,128,96,48,25
