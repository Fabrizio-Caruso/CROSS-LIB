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
.byte 44,52,44,52,44,52,44,52

; 0xED gun
.byte 0,1,1,1,0,0,0,1

; 0xEE pound - invincibility
.byte 126,189,219,153,24,126,90,217

; 0xEF powerup
.byte 24,168,72,155,175,83,96,240

; 0xF0 rocket
.byte 0,71,196,68,71,69,231,0

; 0xF1 left missile
.byte 128,128,128,176,240,48,0,0

; 0xF2 space
.byte $00,$00,$00,$00,$00,$00,$00,$00

; 0xF3 bullet
.byte 0,128,128,128,0,0,0,128

; 0xF4 vertical
.byte 1,1,1,13,15,12,0,0

; 0xF5 horizontal
.byte  129,90,36,153,90,165,102,255

; 0xF6 ghost
.byte 36,36,36,165,255,189,36,0

; 0xF7 skull
.byte 24,21,18,217,245,202,6,15

; 0xF8 bomb
.byte 7,27,29,25,1,7,5,29

; 0xF9 player down 
.byte 0,4,0,0,0,0,32,0

; 0xFA player left
.byte 0,24,60,60,189,255,165,36

; 0xFB player right 
.byte 85,170,85,170,85,170,85,170

; 0xFC right missile from the right
.byte 0,71,196,68,71,65,231,0


; 0xFD freeze
.byte 0,71,193,65,66,66,226,0


; 0xFE super
.byte 0,71,197,69,71,69,231,0

; 0xFF dead ghost
.byte 224,216,184,152,128,224,160,152
