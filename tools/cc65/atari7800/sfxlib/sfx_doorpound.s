	.rodata
	.export	_sfx_doorpound
_sfx_doorpound:
	.byte  $10,$10,$00 ; version, priority, frames per chunk
	.byte  $0f,$06,$0f ; first chunk of freq,channel,volume
	.byte  $1e,$0c,$0f
	.byte  $0f,$06,$0f
	.byte  $1e,$06,$0f
	.byte  $0a,$06,$0f
	.byte  $07,$06,$0b
	.byte  $0f,$06,$0f
	.byte  $0f,$06,$0c
	.byte  $07,$06,$0a
	.byte  $07,$06,$09
	.byte  $0f,$06,$0b
	.byte  $1e,$06,$07
	.byte  $0a,$06,$06
	.byte  $0a,$06,$08
	.byte  $0f,$06,$06
	.byte  $0a,$06,$05
	.byte  $1e,$06,$03
	.byte  $0f,$06,$02
	.byte  $00,$00,$00
