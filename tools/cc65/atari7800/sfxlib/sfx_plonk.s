	.rodata
	.export	_sfx_plonk
_sfx_plonk:
	.byte  $10,$10,$00 ; version, priority, frames per chunk
	.byte  $13, $01, $0d ; first chunk of freq,channel,volume
	.byte   $12, $01, $0f
	.byte   $0d, $04, $0d
	.byte   $12, $01, $0b
	.byte   $12, $01, $05
	.byte   $06, $07, $03
	.byte   $0a, $0f, $02
	.byte   $09, $07, $01
	.byte   $10, $0f, $00
	.byte   $1b, $0c, $00
	.byte   $0b, $0e, $00
	.byte   $09, $06, $00
	.byte   $12, $01, $01
	.byte   $0a, $07, $01
	.byte  $00,$00,$00
