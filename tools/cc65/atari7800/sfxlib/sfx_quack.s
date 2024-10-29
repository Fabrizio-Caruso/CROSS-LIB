	.rodata
	.export	_sfx_quack
_sfx_quack:
	.byte  $10,$10,$00 ; version, priority, frames per chunk
	.byte   $15, $06, $08 ; first chunk of freq,channel,volume
	.byte   $15, $06, $09
	.byte   $15, $06, $0A
	.byte   $14, $06, $0B
	.byte   $14, $06, $0C
	.byte   $14, $06, $0D
	.byte   $14, $06, $0E
	.byte   $13, $06, $0F
	.byte   $13, $06, $0F
	.byte   $13, $06, $0F
	.byte   $13, $06, $0F
	.byte   $13, $06, $0F
	.byte   $00, $00, $00
