	.rodata
	.export	_sfx_snore
_sfx_snore:
	.byte  $10,$10,$00 ; version, priority, frames per chunk
	.byte  $0c,$0c,$01 ; first chunk of freq,channel,volume
	.byte  $17,$0c,$03
	.byte  $03,$06,$06
	.byte  $0a,$06,$04
	.byte  $0a,$06,$04
	.byte  $07,$06,$03
	.byte  $1e,$06,$0c
	.byte  $0f,$06,$0f
	.byte  $0f,$06,$0f
	.byte  $0a,$06,$0f
	.byte  $0f,$06,$0f
	.byte  $0d,$04,$0f
	.byte  $0f,$06,$0f
	.byte  $0f,$06,$0f
	.byte  $0a,$06,$0e
	.byte  $00,$06,$0c
	.byte  $0d,$04,$0d
	.byte  $0a,$06,$04
	.byte  $0f,$06,$03
	.byte  $0a,$06,$04
	.byte  $1e,$06,$01
	.byte  $0a,$06,$01
	.byte  $00,$00,$00
