		.segment "UDCCHAR"
		.export _Charset

_Charset:

; 0xE6
.byte 0,71,197,69,71,65,225,0

; 0xE7
.byte 0,98,165,37,69,133,226,0

; 0xE8
.byte 0,98,166,34,66,130,231,0

; 0xE9
.byte 0,99,165,33,66,132,231,0

; 0xEA
.byte 0,103,161,33,71,129,231,0

; 0xEB
.byte 0,101,165,37,71,129,225,0


; 0xEC player up
.byte 170,85,170,85,170,85,170,85

; 0xED gun
.byte 0,71,165,165,167,161,65,0

; 0xEE pound - invincibility
.byte 0,66,197,69,69,69,226,0

; 0xEF powerup
.byte 170,85,170,85,170,85,170,85

; 0xF0 rocket
.byte 0,71,196,68,71,69,231,0

; 0xF1 left missile
.byte 0,69,197,69,71,65,225,0

; 0xF2 space
.byte $00,$00,$00,$00,$00,$00,$00,$00

; 0xF3 bullet
.byte 0,66,198,66,66,66,231,0

; 0xF4 vertical
.byte 0,67,197,65,66,68,231,0

; 0xF5 horizontal
.byte  0,71,193,65,71,65,231,0

; 0xF6 ghost
.byte 60,126,255,255,189,255,255,170

; 0xF7 skull
.byte 0,0,24,60,60,24,0,0

; 0xF8 bomb
.byte 60,126,255,255,189,255,255,85

; 0xF9 player down 
.byte 30,126,126,248,248,126,126,30

; 0xFA player left
.byte 0,0,24,60,60,24,0,0

; 0xFB player right 
.byte 0,231,231,255,255,126,126,24

; 0xFC right missile from the right
.byte 0,71,196,68,71,65,231,0


; 0xFD freeze
.byte 0,71,193,65,66,66,226,0


; 0xFE super
.byte 0,71,197,69,71,69,231,0

; 0xFF dead ghost
.byte 0,0,0,24,24,0,0,0
