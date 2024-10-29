	.rodata
	.export	_sfx_laserrecoil
_sfx_laserrecoil:
	.byte  $10,$10,$00 ; version, priority, frames per chunk
	.byte  $06,$0c,$0f ; first chunk of freq,channel,volume
	.byte  $16,$04,$0f
	.byte  $16,$04,$0f
	.byte  $1e,$04,$0f
	.byte  $0b,$0c,$0f
	.byte  $0b,$0c,$0f
	.byte  $1e,$06,$0f
	.byte  $1e,$06,$0c
	.byte  $16,$04,$09
	.byte  $1e,$04,$09
	.byte  $1e,$04,$0f
	.byte  $0b,$0c,$0f
	.byte  $0e,$0c,$0f
	.byte  $0d,$0c,$04
	.byte  $07,$0c,$05
	.byte  $1b,$04,$03
	.byte  $0b,$0c,$02
	.byte  $10,$0c,$03
	.byte  $03,$06,$02
	.byte  $00,$00,$00
