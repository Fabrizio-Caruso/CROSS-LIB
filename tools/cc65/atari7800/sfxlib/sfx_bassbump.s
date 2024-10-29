	.rodata
	.export	_sfx_bassbump
_sfx_bassbump:
	.byte  $10,$10,$00 ; version, priority, frames per chunk
	.byte  $1b,$0c,$0f ; first chunk of freq,channel,volume
	.byte  $0f,$06,$0c
	.byte  $07,$06,$0e
	.byte  $0f,$06,$0f
	.byte  $0f,$06,$0f
	.byte  $0f,$06,$0f
	.byte  $07,$06,$06
	.byte  $0f,$06,$0f
	.byte  $1b,$0c,$04
	.byte  $0f,$06,$05
	.byte  $0f,$06,$06
	.byte  $07,$06,$01
	.byte  $0f,$06,$03
	.byte  $00,$00,$00
