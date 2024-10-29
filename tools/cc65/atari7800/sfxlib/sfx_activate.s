	.rodata
	.export	_sfx_activate
_sfx_activate:
	.byte  $10,$10,$00 ; version, priority, frames per chunk
	.byte   $0b, $07, $01  ; first chunk of freq,channel,volume
	.byte   $1f, $07, $06
	.byte   $13, $0c, $07
	.byte   $15, $04, $0d
	.byte   $1c, $04, $0e
	.byte   $16, $04, $0d
	.byte   $0d, $0c, $0d
	.byte   $12, $0c, $0b
	.byte   $14, $04, $0d
	.byte   $1f, $04, $0f
	.byte   $1b, $04, $0d
	.byte   $1b, $04, $0d
	.byte   $1b, $04, $06
	.byte   $15, $0c, $05
	.byte   $1e, $07, $07
	.byte   $0e, $07, $0d
	.byte   $01, $06, $0e
	.byte   $13, $04, $0d
	.byte   $1e, $07, $0d
	.byte   $10, $0c, $0b
	.byte   $14, $04, $0d
	.byte   $0b, $0c, $0e
	.byte   $1b, $04, $0d
	.byte   $1a, $04, $0c
	.byte   $12, $0f, $05
	.byte  $00,$00,$00
