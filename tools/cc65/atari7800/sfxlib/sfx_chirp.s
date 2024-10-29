	.rodata
	.export	_sfx_chirp
_sfx_chirp:
	.byte  $10,$10,$00 ; version, priority, frames per chunk
	.byte  $07, $04, $0e ; first chunk of freq,channel,volume
	.byte  $0a, $04, $0e
	.byte  $0c, $04, $0e
	.byte  $09, $04, $0f
	.byte  $0a, $04, $0e
	.byte  $07, $04, $01
	.byte  $09, $04, $00
	.byte  $00,$00,$00
