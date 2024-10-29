	.rodata
	.export	_sfx_echo2
_sfx_echo2:
	.byte   $10,$05,$04 ; version, priority, frames per chunk
	.byte   $1F,$04,$0A ; first chunk of freq,channel,volume data 
	.byte   $01,$00,$00
	.byte   $1F,$04,$05
	.byte   $01,$00,$00
	.byte   $1F,$04,$02
	.byte   $00,$00,$00 
