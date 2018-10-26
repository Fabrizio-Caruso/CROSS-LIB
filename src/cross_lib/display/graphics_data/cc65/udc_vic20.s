		.segment "UDCCHAR"
		.export _Charset

_Charset:


; 0xEC player up
.byte $18,$3c,$18,$66,$99,$18,$24,$66

; 0xED gun
.byte 0,128,126,200,248,192,128,  0

; 0xEE pound - invincibility
.byte 24, 36, 24,  0,153,  0, 36,102

; 0xEF powerup
.byte 0, 60, 54,223,231,122, 36, 24

; 0xF0 rocket
.byte $18,$3c,$3c,$3c,$7e,$7e,$5a,$42

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
.byte $bd,$42,$a5,$81,$81,$99,$81,$7e

; 0xF7 skull
.byte 60, 66,165,129, 90, 36, 36, 60

; 0xF8 bomb
.byte $3c,$42,$a5,$99,$99,$a5,$42,$3c

; 0xF9 player down 
.byte $18,$24,$18,$66,$99,$18,$24,$66

; 0xFA player left
.byte $18,$ac,$d8,$2e,$19,$18,$28,$28

; 0xFB player right 
.byte $18,$35,$1b,$74,$98,$18,$14,$14

; 0xFC right missile from the right
.byte $00,$00,$0f,$fe,$fe,$0f,$00,$00


; 0xFD dead ghost
.byte $ff,$ff,$ff,$ff,$ff,$ff,$ff,$ff

; 0xFE super
.byte $ff,$ff,$ff,$ff,$ff,$ff,$ff,$ff


; 0xFF freeze
.byte $ff,$ff,$ff,$ff,$ff,$ff,$ff,$ff