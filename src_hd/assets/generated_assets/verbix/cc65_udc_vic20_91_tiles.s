		.segment "UDCCHAR"
		.export _Charset

_Charset:


; 0xE4
.byte 129,66,36,24,24,36,66,129

; 0xE5
.byte 60,126,231,195,195,231,126,60

; 0xE6
.byte 170,85,0,0,0,0,0,0

; 0xE7
.byte 74,126,82,126,74,126,82,126

; 0xE8
.byte 0,255,170,187,238,170,255,0

; 0xE9
.byte 238,206,190,206,230,250,230,238

; 0xEA
.byte 238,142,190,142,226,250,226,238

; 0xEB
.byte 238,222,238,246,238,222,238,246

; 0xEC player up
.byte 0,0,42,0,42,0,42,0

; 0xED gun
.byte 74,74,74,74,74,74,100,0

; 0xEE pound - invincibility
.byte 0,142,136,136,140,136,238,0

; 0xEF powerup
.byte 128,64,32,255,255,32,64,128

; 0xF0 rocket
.byte 179,170,170,179,170,170,171,0

; 0xF1 left missile
.byte 219,146,146,210,82,82,219,0

; 0xF2 space
.byte $00,$00,$00,$00,$00,$00,$00,$00

; 0xF3 bullet
.byte 0,255,255,187,238,255,255,0

; 0xF4 vertical
.byte 90,126,90,126,90,126,90,126

; 0xF5 horizontal
.byte  0,255,170,255,255,170,255,0

; 0xF6 ghost
.byte 8,4,6,255,255,6,4,8

; 0xF7 skull
.byte 24,24,24,24,153,126,60,24

; 0xF8 bomb
.byte 24,60,126,153,24,24,24,24

; 0xF9 player down 
.byte 153,90,60,24,24,24,24,24

; 0xFA player left
.byte 16,32,96,255,255,96,32,16

; 0xFB player right 
.byte 1,2,4,255,255,4,2,1

; 0xFC right missile from the right
.byte 126,110,126,118,126,110,126,118


; 0xFD freeze
.byte 0,238,132,196,132,132,132,0


; 0xFE super
.byte 190,222,238,246,250,246,238,222

; 0xFF dead ghost
.byte 84,84,84,116,84,84,84,0

