	.rodata
	.export	_sfx_buzzbomb
_sfx_buzzbomb:
	.byte  $10,$10,$00 ; version, priority, frames per chunk
	.byte  $03,$06,$05 ; first chunk of freq,channel,volume
	.byte  $0f,$06,$0f
	.byte  $0f,$06,$0f
	.byte  $07,$06,$0b
	.byte  $07,$06,$0f
	.byte  $0f,$06,$0f
	.byte  $07,$06,$0e
	.byte  $0f,$06,$0f
	.byte  $0f,$06,$0a
	.byte  $0f,$06,$0f
	.byte  $1b,$0c,$0a
	.byte  $0f,$06,$08
	.byte  $03,$06,$07
	.byte  $07,$06,$06
	.byte  $03,$06,$07
	.byte  $0f,$06,$05
	.byte  $07,$06,$05
	.byte  $0f,$06,$04
	.byte  $07,$06,$04
	.byte  $07,$06,$02
	.byte  $0f,$06,$02
	.byte  $1b,$0c,$01
	.byte  $00,$00,$00
