	.rodata
	.export	_sfx_dropped
_sfx_dropped:
	.byte  $10,$10,$00 ; version, priority, frames per chunk
	.byte  $0a,$06,$0f ; first chunk of freq,channel,volume
	.byte  $0a,$06,$0f
	.byte  $0a,$06,$06
	.byte  $0c,$04,$02
	.byte  $1b,$0c,$01
	.byte  $07,$06,$0f
	.byte  $0a,$06,$0f
	.byte  $0a,$06,$0f
	.byte  $0a,$06,$07
	.byte  $07,$04,$06
	.byte  $07,$04,$04
	.byte  $07,$06,$0f
	.byte  $07,$06,$0d
	.byte  $07,$04,$0e
	.byte  $07,$04,$06
	.byte  $07,$04,$03
	.byte  $0a,$06,$09
	.byte  $0a,$06,$0f
	.byte  $07,$04,$0f
	.byte  $07,$04,$05
	.byte  $07,$04,$05
	.byte  $1b,$0c,$08
	.byte  $07,$04,$0d
	.byte  $07,$04,$07
	.byte  $07,$04,$07
	.byte  $07,$04,$03
	.byte  $07,$04,$07
	.byte  $07,$04,$05
	.byte  $07,$04,$03
	.byte  $07,$04,$01
	.byte  $00,$00,$00
