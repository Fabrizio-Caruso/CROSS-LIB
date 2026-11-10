		.segment "UDCCHAR"
		.export _Charset

_Charset:


; right missile
.byte $03,$07,$0f,$0a,$0f,$08,$0f,$07


; 0xFD freeze
; formerly 17
.byte 99,46,56,16,0,0,0,0

; 0xFE super
.byte 0,0,0,0,16,56,16,0

.byte 0,1,7,12,6,2,3,1

.byte 128,192,64,96,48,224,128,0

.byte 0,0,0,0,8,28,116,198

; formerly 24
.byte $ff,$55,$22,$55,$ff,$00,$00,$00

; A-Z
.byte $38,$6c,$c6,$c6,$fe,$c6,$c6,$00 ; a
.byte $fc,$c6,$c6,$fc,$c6,$c6,$fc,$00,$3c,$66,$c0,$c0,$c0,$66,$3c,$00
.byte $f8,$cc,$c6,$c6,$c6,$cc,$f8,$00,$fc,$c0,$c0,$f8,$c0,$c0,$fe,$00
.byte $fc,$c0,$c0,$f8,$c0,$c0,$c0,$00,$3e,$60,$c0,$ce,$c6,$66,$3e,$00
.byte $c6,$c6,$c6,$fe,$c6,$c6,$c6,$00,$fc,$30,$30,$30,$30,$30,$fc,$00
.byte $06,$06,$06,$06,$06,$c6,$7c,$00,$c6,$cc,$d8,$f0,$f8,$dc,$ce,$00
.byte $c0,$c0,$c0,$c0,$c0,$c0,$fe,$00,$c6,$ee,$fe,$d6,$c6,$c6,$c6,$00
.byte $c6,$e6,$f6,$de,$ce,$c6,$c6,$00,$7c,$c6,$c6,$c6,$c6,$c6,$7c,$00
.byte $fc,$c6,$c6,$c6,$fc,$c0,$c0,$00,$7c,$c6,$c6,$c6,$de,$cc,$7a,$00
.byte $fc,$c6,$c6,$cc,$f8,$dc,$ce,$00,$78,$cc,$c0,$7c,$06,$c6,$7c,$00
.byte $fc,$30,$30,$30,$30,$30,$30,$00,$c6,$c6,$c6,$c6,$c6,$c6,$7c,$00
.byte $c6,$c6,$c6,$c6,$6c,$38,$10,$00,$c6,$c6,$d6,$fe,$fe,$ee,$c6,$00
.byte $c6,$ee,$7c,$38,$7c,$ee,$c6,$00,$cc,$cc,$cc,$78,$30,$30,$30,$00 
.byte $fe,$0e,$1c,$38,$70,$e0,$fe,$00 ; z

; gun
.byte $3c,$7e,$ff,$af,$ff,$8f,$ff,$7f

; pound - invincibility
.byte $c0,$e0,$f0,$50,$50,$f0,$f0,$e0

; powerup
.byte $3c,$7e,$ff,$f5,$ff,$f1,$ff,$fe

; rocket
.byte $ff,$aa,$ff,$aa,$ff,$00,$00,$00

; cbm left arrow - left missile
.byte $c0,$e0,$f0,$50,$f0,$10,$f0,$e0

; space
.byte $00,$00,$00,$00,$00,$00,$00,$00

; bullet
.byte $03,$07,$0f,$0a,$0a,$0f,$0f,$07

; vertical brick
.byte $c0,$e0,$f0,$00,$b0,$f0,$f0,$e0

; horizontal brick
.byte  $03,$07,$0f,$00,$0d,$0f,$0f,$07

; ghost
.byte $3c,$7e,$ff,$f5,$f5,$ff,$ff,$fe

; skull
.byte $3c,$7e,$ff,$0f,$df,$ff,$ff,$7f

; bomb
.byte $3c,$7e,$ff,$af,$af,$ff,$ff,$7f

; player down 
.byte $00,$00,$00,$01,$01,$07,$1f,$3f

; player left
.byte $80,$e0,$f0,$f8,$f8,$fc,$fe,$ff

; player right 
.byte $01,$07,$0f,$1f,$1f,$3f,$7f,$ff

; extraPoints
;.byte $18,$3e,$60,$3c,$06,$7c,$18,$00
.byte 16,56,46,99,198,116,28,8

; player up
.byte $00,$00,$00,$80,$80,$e0,$f8,$fc

; comma
.byte $3c,$7e,$ff,$f0,$fb,$ff,$ff,$fe

; dash
.byte 0,0,0,24,52,122,52,24

; period
;.byte $00,$00,$00,$00,$00,$18,$18,$00
.byte 16,40,68,146,68,40,16,16

; slash
;.byte 0,0,0,0,0,0,0,0
.byte 60,126,231,195,195,231,126,60

.byte $38,$4c,$c6,$c6,$c6,$64,$38,$00 ; 0
.byte $30,$70,$30,$30,$30,$30,$fc,$00,$7c,$c6,$0e,$3c,$60,$c0,$fe,$00
.byte $7e,$0c,$18,$3c,$06,$c6,$7c,$00,$1c,$3c,$6c,$cc,$fe,$0c,$0c,$00
.byte $fc,$c0,$fc,$06,$06,$c6,$7c,$00,$3c,$60,$c0,$fc,$c6,$c6,$7c,$00
.byte $fe,$c6,$0c,$18,$30,$30,$30,$00,$78,$c4,$e4,$78,$9e,$86,$7c,$00
.byte $7c,$c6,$c6,$7e,$06,$0c,$78,$00 ; 9



; ------------------------------------------------------------------------
; Unused - Conflict?



