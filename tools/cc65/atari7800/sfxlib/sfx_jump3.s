	.rodata
	.export	_sfx_jump3
_sfx_jump3:
	.byte  $10,$10,$01 ; version, priority, frames per chunk
	.byte  $17,$0c,$07 ; first chunk of freq,channel,volume
	.byte  $1b,$0c,$0f
	.byte  $1b,$0c,$09
	.byte  $17,$0c,$0d
	.byte  $17,$0c,$0f
	.byte  $0d,$0c,$0b
	.byte  $0c,$0c,$0d
	.byte  $0a,$0c,$0a
	.byte  $1c,$04,$09
	.byte  $1b,$04,$0f
	.byte  $1b,$04,$07
	.byte  $18,$04,$04
	.byte  $1b,$04,$0c
	.byte  $0a,$0c,$04
	.byte  $0b,$0c,$03
	.byte  $0c,$0c,$03
	.byte  $0d,$0c,$01
	.byte  $00,$00,$00
