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
.byte 24,60,24,102,153,24,36,102

; 0xED gun
.byte 0,71,165,165,167,161,65,0

; 0xEE pound - invincibility
.byte 0,66,197,69,69,69,226,0

; 0xEF powerup
.byte 126,129,165,129,129,219,102,231

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
.byte 51,51,204,204,51,51,204,204

; 0xF7 skull
.byte 0,0,8,56,28,16,0,0

; 0xF8 bomb
.byte 85,170,85,170,85,170,85,170

; 0xF9 player down 
.byte 24,36,24,102,153,24,36,102

; 0xFA player left
.byte 24,44,152,110,25,24,40,40

; 0xFB player right 
.byte 24,52,25,118,152,24,20,20

; 0xFC right missile from the right
.byte 0,71,196,68,71,65,231,0


; 0xFD freeze
.byte 0,71,193,65,66,66,226,0


; 0xFE super
.byte 0,71,197,69,71,69,231,0

; 0xFF dead ghost
.byte 126,129,129,141,137,129,129,255
