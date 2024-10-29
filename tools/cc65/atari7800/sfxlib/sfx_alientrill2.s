	.rodata
	.export	_sfx_alientrill2
_sfx_alientrill2:
	.byte   $10,$05,$01 ; version, priority, frames per chunk
	.byte   $18,$04,$08 ; first chunk of freq,channel,volume data 
	.byte   $1E,$04,$08
	.byte   $18,$04,$08
	.byte   $1E,$04,$08
	.byte   $14,$04,$08
	.byte   $00,$00,$00 
