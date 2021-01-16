		.segment "UDCCHAR"
		.export _Charset

_Charset:

; 0xE6
.byte 255,255,0,48,48,0,255,255

; 0xE7
.byte 255,255,0,48,48,0,255,255

; 0xE8
.byte 255,255,0,48,48,0,255,255

; 0xE9
.byte 255,255,0,48,48,0,255,255

; 0xEA
.byte 255,255,0,48,48,0,255,255

; 0xEB
.byte 255,255,0,48,48,0,255,255


; 0xEC player up
.byte 12,6,195,97,127,63,1,3

; 0xED gun
.byte 37,37,37,37,37,37,50,0

; 0xEE pound - invincibility
.byte 66,66,90,126,60,60,60,24

; 0xEF powerup
.byte 255,255,255,192,129,255,255,255

; 0xF0 rocket
.byte $b3,$aa,$aa,$b3,$aa,$aa,$ab,$00

; 0xF1 left missile
.byte $db,$92,$92,$d2,$52,$52,$db,$00

; 0xF2 space
.byte $00,$00,$00,$00,$00,$00,$00,$00

; 0xF3 bullet
.byte 126,102,126,102,126,102,126,102

; 0xF4 vertical
.byte 0,0,0,0,66,66,90,126

; 0xF5 horizontal
.byte  60,60,60,24,0,0,0,0

; 0xF6 ghost
.byte 126,90,126,90,126,90,126,90

; 0xF7 skull
.byte 255,153,255,153,255,153,255,153

; 0xF8 bomb
.byte 126,102,126,102,126,102,126,102

; 0xF9 player down 
.byte 0,0,12,6,7,3,0,0

; 0xFA player left
.byte 192,96,48,24,255,255,24,48

; 0xFB player right 
.byte 0,0,0,128,254,255,128,0

; 0xFC right missile from the right
.byte 255,165,255,165,255,165,255,165


; 0xFD freeze
.byte 0,0,0,0,224,240,0,0


; 0xFE super
.byte 0,0,0,0,0,170,255,255

; 0xFF dead ghost
.byte $54,$54,$54,$74,$54,$54,$54,$00
