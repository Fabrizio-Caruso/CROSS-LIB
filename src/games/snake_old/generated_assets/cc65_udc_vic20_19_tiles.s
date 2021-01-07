		.segment "UDCCHAR"
		.export _Charset

_Charset:


; 0xEC player up
.byte 60,102,231,255,255,231,102,60

; 0xED gun
.byte 74,74,74,74,74,74,100,0

; 0xEE pound - invincibility
.byte 170,0,170,0,170,0,170,0

; 0xEF powerup
.byte 60,126,231,195,195,231,126,60

; 0xF0 rocket
.byte $b3,$aa,$aa,$b3,$aa,$aa,$ab,$00

; 0xF1 left missile
.byte $db,$92,$92,$d2,$52,$52,$db,$00

; 0xF2 space
.byte $00,$00,$00,$00,$00,$00,$00,$00

; 0xF3 bullet
.byte 0,0,0,0,8,28,116,198

; 0xF4 vertical
.byte 90,126,90,126,90,126,90,126

; 0xF5 horizontal
.byte  0,255,170,255,255,170,255,0

; 0xF6 ghost
.byte 60,126,255,255,255,255,126,60

; 0xF7 skull
.byte 170,255,170,255,170,255,170,255

; 0xF8 bomb
.byte 48,8,126,255,253,253,255,126

; 0xF9 player down 
.byte 60,126,255,153,153,255,126,60

; 0xFA player left
.byte 128,192,64,96,48,224,128,0

; 0xFB player right 
.byte 0,1,7,12,6,2,3,1

; 0xFC right missile from the right
.byte 99,46,56,16,0,0,0,0


; 0xFD freeze
.byte 16,56,46,99,198,116,28,8


; 0xFE super
.byte 60,126,231,219,219,231,126,60

; 0xFF dead ghost
.byte $54,$54,$54,$74,$54,$54,$54,$00
