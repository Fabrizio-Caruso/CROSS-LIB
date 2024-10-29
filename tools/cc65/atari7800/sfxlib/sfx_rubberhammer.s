	.rodata
	.export	_sfx_rubberhammer
_sfx_rubberhammer:
	.byte  $10,$10,$00 ; version, priority, frames per chunk
	.byte  $00,$06,$08 ; first chunk of freq,channel,volume
	.byte  $1c,$04,$0f
	.byte  $1e,$06,$0f
	.byte  $1b,$04,$0f
	.byte  $1e,$04,$0f
	.byte  $1e,$04,$0f
	.byte  $0c,$0c,$0f
	.byte  $0c,$0c,$0f
	.byte  $1e,$06,$0b
	.byte  $0a,$0c,$08
	.byte  $0a,$0c,$05
	.byte  $1e,$04,$06
	.byte  $1c,$04,$05
	.byte  $1c,$04,$03
	.byte  $0e,$0c,$01
	.byte  $1b,$04,$01
	.byte  $00,$00,$00
