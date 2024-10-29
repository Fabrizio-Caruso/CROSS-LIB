	.rodata
	.export	_sfx_jumpo
_sfx_jumpo:
	.byte  $10,$10,$00 ; version, priority, frames per chunk
	.byte  $1e,$0c,$03 ; first chunk of freq,channel,volume
	.byte  $1e,$0c,$01
	.byte  $1e,$0c,$06
	.byte  $1e,$0c,$06
	.byte  $1e,$0c,$04
	.byte  $1e,$06,$03
	.byte  $1e,$0c,$05
	.byte  $1e,$0c,$0a
	.byte  $1e,$0c,$06
	.byte  $1e,$06,$06
	.byte  $1e,$0c,$06
	.byte  $1b,$0c,$04
	.byte  $1b,$0c,$02
	.byte  $1b,$0c,$08
	.byte  $1b,$0c,$0b
	.byte  $1b,$0c,$0c
	.byte  $1b,$0c,$0a
	.byte  $1b,$0c,$04
	.byte  $1b,$0c,$05
	.byte  $1e,$0c,$07
	.byte  $1e,$0c,$07
	.byte  $10,$0c,$02
	.byte  $1e,$0c,$04
	.byte  $1e,$0c,$03
	.byte  $10,$0c,$01
	.byte  $1e,$0c,$06
	.byte  $1e,$0c,$06
	.byte  $07,$06,$04
	.byte  $07,$06,$05
	.byte  $07,$06,$0b
	.byte  $07,$06,$0e
	.byte  $07,$06,$0e
	.byte  $07,$06,$0a
	.byte  $17,$0c,$05
	.byte  $00,$00,$00
