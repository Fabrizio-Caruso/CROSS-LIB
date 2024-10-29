	.rodata
	.export	_sfx_drip
_sfx_drip:
	.byte  $10,$10,$00 ; version, priority, frames per chunk
	.byte  $17,$0c,$0f ; first chunk of freq,channel,volume
	.byte  $17,$0c,$0a
	.byte  $0d,$0c,$0a
	.byte  $1e,$06,$07
	.byte  $12,$0c,$01
	.byte  $1e,$06,$00
	.byte  $1e,$06,$02
	.byte  $03,$06,$00
	.byte  $1e,$06,$00
	.byte  $1e,$06,$00
	.byte  $03,$06,$01
	.byte  $03,$06,$02
	.byte  $12,$0c,$01
	.byte  $10,$0c,$00
	.byte  $1e,$06,$00
	.byte  $1e,$06,$01
	.byte  $00,$00,$00
