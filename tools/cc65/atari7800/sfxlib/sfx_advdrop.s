	.rodata
	.export	_sfx_advdrop
_sfx_advdrop:
	.byte   $10,$04,$02 ; version, priority, frames per chunk
	.byte   $00,$06,$08 ; first chunk of freq,channel,volume data 
	.byte   $01,$06,$08 
	.byte   $02,$06,$08 
	.byte   $03,$06,$08 
	.byte   $00,$00,$00 
