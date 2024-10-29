	.rodata
	.export	_sfx_dropmedium
_sfx_dropmedium:
	.byte  $10,$10,$00 ; version, priority, frames per chunk
	.byte  $00,$04,$00 ; first chunk of freq,channel,volume
	.byte  $03,$06,$0c
	.byte  $0d,$0c,$0f
	.byte  $1b,$04,$04
	.byte  $06,$0c,$00
	.byte  $00,$06,$00
	.byte  $07,$06,$00
	.byte  $10,$0c,$00
	.byte  $0d,$0c,$00
	.byte  $10,$0c,$00
	.byte  $03,$06,$00
	.byte  $10,$0c,$00
	.byte  $1b,$04,$00
	.byte  $10,$0c,$00
	.byte  $10,$0c,$00
	.byte  $03,$06,$00
	.byte  $00,$00,$00
