	.rodata
	.export	_sfx_ouch
_sfx_ouch:
	.byte  $10,$10,$00 ; version, priority, frames per chunk
	.byte  $07,$0c,$0f ; first chunk of freq,channel,volume
	.byte  $07,$0c,$0f
	.byte  $07,$0c,$0f
	.byte  $18,$04,$07
	.byte  $19,$04,$04
	.byte  $07,$0c,$09
	.byte  $19,$04,$0f
	.byte  $19,$04,$0d
	.byte  $19,$04,$0f
	.byte  $19,$04,$0f
	.byte  $1b,$04,$0f
	.byte  $1b,$04,$0f
	.byte  $1b,$04,$0f
	.byte  $1b,$04,$0f
	.byte  $1b,$04,$09
	.byte  $1b,$04,$05
	.byte  $1b,$04,$03
	.byte  $1b,$04,$02
	.byte  $1c,$04,$01
	.byte  $1c,$04,$01
	.byte  $1c,$04,$01
	.byte  $1b,$04,$02
	.byte  $19,$04,$00
	.byte  $1b,$04,$00
	.byte  $19,$04,$01
	.byte  $00,$00,$00
