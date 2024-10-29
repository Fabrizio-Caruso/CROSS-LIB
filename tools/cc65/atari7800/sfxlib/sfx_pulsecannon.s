	.rodata
	.export	_sfx_pulsecannon
_sfx_pulsecannon:
	.byte  $10,$10,$00 ; version, priority, frames per chunk
	.byte  $1e,$0c,$0a ; first chunk of freq,channel,volume
	.byte  $07,$06,$0f
	.byte  $07,$06,$0f
	.byte  $1e,$06,$0f
	.byte  $17,$0c,$0b
	.byte  $1b,$0c,$0b
	.byte  $1e,$0c,$0f
	.byte  $07,$06,$0f
	.byte  $07,$06,$0f
	.byte  $1e,$06,$08
	.byte  $17,$0c,$06
	.byte  $1b,$0c,$0f
	.byte  $1e,$0c,$0f
	.byte  $07,$06,$0f
	.byte  $07,$06,$0f
	.byte  $0a,$06,$0a
	.byte  $17,$0c,$0a
	.byte  $1e,$0c,$04
	.byte  $1e,$06,$09
	.byte  $1b,$04,$05
	.byte  $07,$06,$0f
	.byte  $0a,$06,$09
	.byte  $17,$0c,$0d
	.byte  $1b,$0c,$09
	.byte  $0a,$06,$05
	.byte  $17,$0c,$03
	.byte  $00,$00,$00
