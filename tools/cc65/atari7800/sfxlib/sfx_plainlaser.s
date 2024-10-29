	.rodata
	.export	_sfx_plainlaser
_sfx_plainlaser:
	.byte  $10,$10,$00 ; version, priority, frames per chunk
	.byte  $10,$04,$06 ; first chunk of freq,channel,volume
	.byte  $13,$04,$08
	.byte  $16,$04,$08
	.byte  $16,$04,$07
	.byte  $1c,$04,$09
	.byte  $0b,$0c,$0f
	.byte  $0d,$0c,$0f
	.byte  $0e,$0c,$0f
	.byte  $0e,$0c,$0f
	.byte  $12,$0c,$0f
	.byte  $03,$06,$0d
	.byte  $1e,$0c,$0a
	.byte  $1e,$0c,$0c
	.byte  $0a,$06,$04
	.byte  $00,$00,$00
