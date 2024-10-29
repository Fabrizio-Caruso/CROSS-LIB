	.rodata
	.export	_sfx_pewpew
_sfx_pewpew:
	.byte  $10,$10,$00 ; version, priority, frames per chunk
	.byte  $1c,$04,$0f ; first chunk of freq,channel,volume
	.byte  $1c,$04,$0f
	.byte  $09,$04,$0b
	.byte  $03,$0c,$0a
	.byte  $04,$0c,$0e
	.byte  $12,$04,$0c
	.byte  $19,$04,$0f
	.byte  $1c,$04,$0f
	.byte  $07,$04,$05
	.byte  $09,$04,$05
	.byte  $0d,$04,$06
	.byte  $0c,$04,$05
	.byte  $18,$04,$06
	.byte  $1c,$04,$05
	.byte  $1e,$04,$03
	.byte  $07,$04,$03
	.byte  $09,$04,$03
	.byte  $0c,$04,$02
	.byte  $04,$0c,$02
	.byte  $06,$0c,$01
	.byte  $00,$00,$00
