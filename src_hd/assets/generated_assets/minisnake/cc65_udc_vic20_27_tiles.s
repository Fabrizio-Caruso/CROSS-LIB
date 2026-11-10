		.segment "UDCCHAR"
		.export _Charset

_Charset:


; 0xE4
.byte 0,71,197,69,71,65,225,0

; 0xE5
.byte 0,98,165,37,69,133,226,0

; 0xE6
.byte 0,98,166,34,66,130,231,0

; 0xE7
.byte 32,4,32,4,32,4,32,4

; 0xE8
.byte 0,103,161,33,71,129,231,0

; 0xE9
.byte 0,0,170,0,0,85,0,0

; 0xEA
.byte 0,24,52,122,253,122,52,24

; 0xEB
.byte 170,85,170,85,170,85,170,85

; 0xEC player up
.byte 60,102,231,255,153,255,126,60

; 0xED gun
.byte 60,126,255,255,255,126,60,24

; 0xEE pound - invincibility
.byte 28,62,127,255,255,127,62,28

; 0xEF powerup
.byte 56,124,254,255,255,254,124,56

; 0xF0 rocket
.byte 0,71,196,68,71,69,231,0

; 0xF1 left missile
.byte 251,251,251,251,246,254,252,248

; 0xF2 space
.byte $00,$00,$00,$00,$00,$00,$00,$00

; 0xF3 bullet
.byte 15,7,0,0,63,127,255,255

; 0xF4 vertical
.byte 0,0,192,192,252,254,255,255

; 0xF5 horizontal
.byte  255,255,255,255,127,127,63,31

; 0xF6 ghost
.byte 60,110,239,249,249,239,110,60

; 0xF7 skull
.byte 24,60,126,255,255,255,126,60

; 0xF8 bomb
.byte 60,126,255,153,255,231,102,60

; 0xF9 player down 
.byte 0,66,165,165,165,165,66,0

; 0xFA player left
.byte 48,8,126,255,253,253,255,126

; 0xFB player right 
.byte 60,126,255,255,255,255,126,60

; 0xFC right missile from the right
.byte 0,71,196,68,71,65,231,0


; 0xFD freeze
.byte 0,71,193,65,66,66,226,0


; 0xFE super
.byte 0,71,197,69,71,69,231,0

; 0xFF dead ghost
.byte 60,118,247,159,159,247,118,60

