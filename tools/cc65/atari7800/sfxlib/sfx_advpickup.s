	.rodata
	.export	_sfx_advpickup
_sfx_advpickup:
	.byte   $10,$04,$02 ; version, priority, frames per chunk
	.byte   $03,$06,$08 ; first chunk of freq,channel,volume data 
	.byte   $02,$06,$08 
	.byte   $01,$06,$08 
	.byte   $00,$06,$08 
	.byte   $00,$00,$00 
