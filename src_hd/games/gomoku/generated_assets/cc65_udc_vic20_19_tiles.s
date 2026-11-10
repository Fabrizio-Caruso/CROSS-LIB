		.segment "UDCCHAR"
		.export _Charset

_Charset:


; 0xEC player up
.byte 224,248,252,254,254,255,255,255

; 0xED gun
.byte 224,248,12,6,6,3,3,3

; 0xEE pound - invincibility
.byte 192,192,192,96,96,48,31,7

; 0xEF powerup
.byte 7,31,48,96,96,192,192,192

; 0xF0 rocket
.byte 0,71,196,68,71,69,231,0

; 0xF1 left missile
.byte 128,0,128,0,128,0,128,85

; 0xF2 space
.byte $00,$00,$00,$00,$00,$00,$00,$00

; 0xF3 bullet
.byte 3,3,3,6,6,12,248,224

; 0xF4 vertical
.byte 170,0,128,0,128,0,128,0

; 0xF5 horizontal
.byte  170,1,0,1,0,1,0,1

; 0xF6 ghost
.byte 7,31,63,127,127,255,255,254

; 0xF7 skull
.byte 127,255,255,254,254,252,248,224

; 0xF8 bomb
.byte 224,248,252,254,254,255,255,127

; 0xF9 player down 
.byte 7,31,63,127,127,255,255,255

; 0xFA player left
.byte 255,255,255,254,254,252,248,224

; 0xFB player right 
.byte 255,255,255,127,127,63,31,7

; 0xFC right missile from the right
.byte 0,1,0,1,0,1,0,85


; 0xFD freeze
.byte 0,71,193,65,66,66,226,0


; 0xFE super
.byte 0,71,197,69,71,69,231,0

; 0xFF dead ghost
.byte 254,255,255,127,127,63,31,7
