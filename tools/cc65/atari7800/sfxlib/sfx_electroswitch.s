	.rodata
	.export	_sfx_electroswitch
_sfx_electroswitch:
	.byte   $10,$01,$02 ; version, priority, frames per chunk
	.byte   $06,$04,$0F ; first chunk of freq,channel,volume data
	.byte   $0C,$04,$08 ; first chunk of freq,channel,volume data
	.byte   $18,$04,$04 ; first chunk of freq,channel,volume data
	.byte   $31,$04,$02 ; first chunk of freq,channel,volume data
	.byte   $00,$00,$00
