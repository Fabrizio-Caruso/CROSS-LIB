	.rodata
	.export	_sfx_simplebuzz
_sfx_simplebuzz:
	.byte  $10,$10,$00 ; version, priority, frames per chunk
	.byte  $1e,$06,$00 ; first chunk of freq,channel,volume
	.byte  $0a,$06,$01
	.byte  $1e,$06,$02
	.byte  $1e,$06,$04
	.byte  $1e,$06,$05
	.byte  $0f,$06,$0f
	.byte  $0f,$06,$0f
	.byte  $0f,$06,$0f
	.byte  $0f,$06,$0d
	.byte  $0f,$06,$0f
	.byte  $0f,$06,$0f
	.byte  $0f,$06,$0f
	.byte  $0f,$06,$0f
	.byte  $07,$06,$08
	.byte  $07,$06,$06
	.byte  $0f,$06,$06
	.byte  $0f,$06,$04
	.byte  $0f,$06,$02
	.byte  $00,$00,$00
