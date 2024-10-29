	.rodata
	.export	_sfx_jumpman
_sfx_jumpman:
	.byte   $10,$05,$04 ; version, priority, frames per chunk
	.byte   $1E,$04,$08 ; first chunk of freq,channel,volume data 
	.byte   $1B,$04,$08
	.byte   $18,$04,$08
	.byte   $11,$04,$08
	.byte   $16,$04,$08
	.byte   $00,$00,$00 
