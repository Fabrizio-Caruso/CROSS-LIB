	.rodata
	.export	_sfx_eek
_sfx_eek:
	.byte  $10,$10,$00 ; version, priority, frames per chunk
	.byte  $1e,$06,$04 ; first chunk of freq,channel,volume
	.byte  $10,$0c,$03
	.byte  $1e,$04,$0b
	.byte  $1c,$04,$0f
	.byte  $0d,$04,$0f
	.byte  $03,$0c,$0f
	.byte  $03,$0c,$0f
	.byte  $03,$0c,$0f
	.byte  $0a,$04,$0f
	.byte  $0a,$04,$0f
	.byte  $0a,$04,$0f
	.byte  $0a,$04,$0f
	.byte  $0a,$04,$0f
	.byte  $0a,$04,$0f
	.byte  $0a,$04,$08
	.byte  $0a,$04,$0f
	.byte  $0a,$04,$07
	.byte  $0a,$04,$0f
	.byte  $0a,$04,$0c
	.byte  $03,$0c,$0f
	.byte  $03,$0c,$0f
	.byte  $0c,$04,$0f
	.byte  $04,$0c,$06
	.byte  $0c,$04,$02
	.byte  $06,$0c,$01
	.byte  $06,$0c,$01
	.byte  $00,$00,$00
