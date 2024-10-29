	.rodata
	.export	_sfx_advbite
_sfx_advbite:
	.byte   $10,$10,$02 ; version, priority, frames per chunk
	.byte   $1F,$03,$0F ; first chunk of freq,channel,volume data 
	.byte   $1F,$08,$0E 
	.byte   $1F,$03,$0D 
	.byte   $1F,$08,$0C 
	.byte   $1F,$03,$0B 
	.byte   $1F,$08,$0A 
	.byte   $1F,$03,$09 
	.byte   $1F,$08,$08 
	.byte   $1F,$03,$07 
	.byte   $1F,$08,$06 
	.byte   $1F,$03,$05 
	.byte   $1F,$08,$04 
	.byte   $1F,$03,$03 
	.byte   $1F,$08,$02 
	.byte   $1F,$03,$01 
	.byte   $00,$00,$00 
