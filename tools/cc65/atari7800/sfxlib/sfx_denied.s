	.rodata
	.export	_sfx_denied
_sfx_denied:
	.byte  $10,$10,$00 ; version, priority, frames per chunk
	.byte  $0e,$0c,$0d ; first chunk of freq,channel,volume
	.byte  $10,$0c,$0a
	.byte  $10,$0c,$0c
	.byte  $1e,$06,$05
	.byte  $1e,$06,$09
	.byte  $12,$0c,$07
	.byte  $12,$0c,$0b
	.byte  $12,$0c,$0f
	.byte  $12,$0c,$0b
	.byte  $12,$0c,$07
	.byte  $03,$06,$05
	.byte  $03,$06,$04
	.byte  $03,$06,$09
	.byte  $1e,$06,$03
	.byte  $12,$0c,$07
	.byte  $12,$0c,$07
	.byte  $12,$0c,$05
	.byte  $1e,$06,$05
	.byte  $1f,$0c,$03
	.byte  $1f,$0c,$02
	.byte  $1f,$0c,$01
	.byte  $00,$00,$00
