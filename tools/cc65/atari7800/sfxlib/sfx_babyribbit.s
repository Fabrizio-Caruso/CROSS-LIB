	.rodata
	.export	_sfx_babyribbit
_sfx_babyribbit:
	.byte  $10,$10,$00 ; version, priority, frames per chunk
	.byte  $1e,$04,$0f ; first chunk of freq,channel,volume
	.byte  $1c,$04,$0f
	.byte  $1c,$04,$0e
	.byte  $0c,$04,$0e
	.byte  $1c,$04,$0e
	.byte  $0a,$0c,$06
	.byte  $0e,$0c,$00
	.byte  $1e,$06,$00
	.byte  $18,$04,$06
	.byte  $18,$04,$0b
	.byte  $16,$04,$0b
	.byte  $08,$04,$07
	.byte  $04,$04,$04
	.byte  $00,$04,$02
	.byte  $00,$04,$01
	.byte  $00,$00,$00
