		.segment "UDCCHAR"
		.export _Charset

_Charset:


; 0xEC player up
.byte $00,$00,$00,$80,$80,$e0,$f8,$fc

; 0xED gun
.byte $3c,$7e,$ff,$af,$ff,$8f,$ff,$7f

; 0xEE pound - invincibility
.byte $c0,$e0,$f0,$50,$50,$f0,$f0,$e0

; 0xEF powerup
.byte $3c,$7e,$ff,$f5,$ff,$f1,$ff,$fe

; 0xF0 rocket
.byte $ff,$aa,$ff,$aa,$ff,$00,$00,$00

; 0xF1 left missile
.byte $c0,$e0,$f0,$50,$f0,$10,$f0,$e0

; 0xF2 space
.byte $00,$00,$00,$00,$00,$00,$00,$00

; 0xF3 bullet
.byte $03,$07,$0f,$0a,$0a,$0f,$0f,$07

; 0xF4 vertical
.byte $c0,$e0,$f0,$00,$b0,$f0,$f0,$e0

; 0xF5 horizontal
.byte  $03,$07,$0f,$00,$0d,$0f,$0f,$07

; 0xF6 ghost
.byte $3c,$7e,$ff,$f5,$f5,$ff,$ff,$fe

; 0xF7 skull
.byte $3c,$7e,$ff,$0f,$df,$ff,$ff,$7f

; 0xF8 bomb
.byte $3c,$7e,$ff,$af,$af,$ff,$ff,$7f

; 0xF9 player down 
.byte $00,$00,$00,$01,$01,$07,$1f,$3f

; 0xFA player left
.byte $80,$e0,$f0,$f8,$f8,$fc,$fe,$ff

; 0xFB player right 
.byte $01,$07,$0f,$1f,$1f,$3f,$7f,$ff

; 0xFC right missile from the right
.byte $03,$07,$0f,$0a,$0f,$08,$0f,$07


; 0xFD freeze
.byte $ff,$55,$22,$55,$ff,$00,$00,$00


; 0xFE super
.byte 0,0,0,0,16,56,16,0

; 0xFF dead ghost
.byte $3c,$7e,$ff,$f0,$fb,$ff,$ff,$fe
