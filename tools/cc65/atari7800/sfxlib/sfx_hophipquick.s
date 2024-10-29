	.rodata
	.export	_sfx_hophipquick
_sfx_hophipquick:
	.byte  $10,$10,$00 ; version, priority, frames per chunk
	.byte  $1e,$04,$0f ; first chunk of freq,channel,volume
	.byte  $12,$04,$0f
	.byte  $05,$0c,$0c
	.byte  $05,$0c,$0f
	.byte  $05,$0c,$0f
	.byte  $05,$0c,$07
	.byte  $05,$0c,$0f
	.byte  $05,$0c,$03
	.byte  $10,$04,$0c
	.byte  $10,$04,$09
	.byte  $10,$04,$0b
	.byte  $10,$04,$02
	.byte  $10,$04,$07
	.byte  $10,$04,$03
	.byte  $10,$04,$02
	.byte  $00,$00,$00
