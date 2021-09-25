		.segment "UDCCHAR"
		.export _Charset

_Charset:

; 0xE6
.byte 126,90,126,90,126,90,126,90

; 0xE7
.byte 0,0,0,0,0,170,255,255

; 0xE8
.byte 255,153,255,153,255,153,255,153

; 0xE9
.byte 255,255,255,192,129,255,255,255

; 0xEA
.byte 126,102,126,102,126,102,126,102

; 0xEB
.byte 255,165,255,165,255,165,255,165


; 0xEC player up
.byte 0,0,24, 36, 24,102,153, 24

; 0xED gun
.byte 129,66,24,52,44,24,66,129

; 0xEE pound - invincibility
.byte 24, 36, 24,  0,153,  0, 36,102

; 0xEF powerup
.byte 24, 0, 24,0,153, 0, 36,0

; 0xF0 rocket
.byte 24, 60, 60, 60,126, 90, 66, 66

; 0xF1 left missile
.byte 0,  0,240, 63, 63,240,  0,  0

; 0xF2 space
.byte $00,$00,$00,$00,$00,$00,$00,$00

; 0xF3 bullet
.byte 0,  0,  8, 56, 28, 16,  0,  0

; 0xF4 vertical
.byte 24, 24, 24, 48, 24, 12, 24, 24

; 0xF5 horizontal
.byte  0,  0,  0,255,  0,  0,  0,  0

; 0xF6 ghost
.byte 153, 24, 36,102,0,0,0,0

; 0xF7 skull
.byte 0, 36, 0,102,0, 24, 0,102

; 0xF8 bomb
.byte 0,0,0,0,0,0,24, 36

; 0xF9 player down 
.byte 24, 36, 24,102,153, 24, 36,102

; 0xFA player left
.byte 0,0,0,0,24, 36, 24,102

; 0xFB player right 
.byte 36,102,0,0,0,0,0,0

; 0xFC right missile from the right
.byte 0,  0, 15,252,252, 15,  0,  0


; 0xFD freeze
.byte 0,60,66,207,195,102,36,24


; 0xFE super
.byte 0,60,90,219,195,90,60,24

; 0xFF dead ghost
.byte 24,102,153, 24, 36,102,0,0
