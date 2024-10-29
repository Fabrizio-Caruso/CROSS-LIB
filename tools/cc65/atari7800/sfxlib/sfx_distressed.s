	.rodata
	.export	_sfx_distressed
_sfx_distressed:
	.byte  $10,$10,$00 ; version, priority, frames per chunk
	.byte  $00,$06,$02 ; first chunk of freq,channel,volume
	.byte  $00,$06,$01
	.byte  $0f,$04,$09
	.byte  $00,$06,$0a
	.byte  $04,$0c,$0b
	.byte  $0f,$04,$0b
	.byte  $12,$04,$09
	.byte  $0c,$04,$0b
	.byte  $15,$04,$0f
	.byte  $15,$04,$0f
	.byte  $03,$0c,$0e
	.byte  $0c,$04,$0f
	.byte  $0c,$0c,$0f
	.byte  $0d,$04,$0b
	.byte  $04,$0c,$07
	.byte  $00,$06,$05
	.byte  $00,$06,$04
	.byte  $00,$06,$03
	.byte  $00,$06,$02
	.byte  $00,$00,$00
