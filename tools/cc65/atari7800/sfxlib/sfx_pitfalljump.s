	.rodata
	.export	_sfx_pitfalljump
_sfx_pitfalljump:
	.byte   $10,$05,$03 ; version, priority, frames per chunk
	.byte   $06,$01,$04 ; first chunk of freq,channel,volume data 
	.byte   $04,$01,$04 ; first chunk of freq,channel,volume data 
	.byte   $03,$01,$04 ; first chunk of freq,channel,volume data 
	.byte   $02,$01,$04 ; first chunk of freq,channel,volume data 
	.byte   $04,$01,$04 ; first chunk of freq,channel,volume data 
	.byte   $00,$00,$00
