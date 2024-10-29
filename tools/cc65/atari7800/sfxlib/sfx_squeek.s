	.rodata
	.export	_sfx_squeek
_sfx_squeek:
	.byte  $10,$10,$00 ; version, priority, frames per chunk
	.byte  $06,$0c,$0f ; first chunk of freq,channel,volume
	.byte  $15,$04,$0f
	.byte  $06,$0c,$0f
	.byte  $06,$0c,$0f
	.byte  $0a,$04,$0f
	.byte  $0a,$04,$0f
	.byte  $0c,$04,$0f
	.byte  $0d,$04,$0f
	.byte  $03,$0c,$0f
	.byte  $06,$0c,$0d
	.byte  $03,$0c,$07
	.byte  $0d,$04,$06
	.byte  $06,$0c,$04
	.byte  $15,$04,$08
	.byte  $06,$0c,$03
	.byte  $06,$0c,$05
	.byte  $15,$04,$04
	.byte  $15,$04,$04
	.byte  $06,$0c,$04
	.byte  $06,$0c,$03
	.byte  $03,$0c,$01
	.byte  $15,$04,$02
	.byte  $06,$0c,$03
	.byte  $06,$0c,$03
	.byte  $0c,$04,$00
	.byte  $15,$04,$02
	.byte  $15,$04,$01
	.byte  $00,$00,$00
