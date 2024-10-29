	.rodata
	.export	_sfx_echobang
_sfx_echobang:
	.byte  $10,$10,$00 ; version, priority, frames per chunk
	.byte  $19,$07,$0f ; first chunk of freq,channel,volume
	.byte  $13,$07,$0f
	.byte  $1a,$06,$0f
	.byte  $1a,$01,$0f
	.byte  $17,$07,$0f
	.byte  $10,$0c,$0f
	.byte  $14,$07,$0f
	.byte  $04,$07,$0f
	.byte  $0e,$07,$0f
	.byte  $1b,$01,$0f
	.byte  $1b,$01,$0f
	.byte  $1e,$07,$0f
	.byte  $12,$07,$0f
	.byte  $09,$06,$0f
	.byte  $17,$0f,$0f
	.byte  $11,$06,$0f
	.byte  $09,$07,$0b
	.byte  $09,$07,$0e
	.byte  $0b,$0f,$0b
	.byte  $0e,$0f,$0c
	.byte  $1a,$07,$09
	.byte  $13,$0f,$09
	.byte  $14,$0f,$0b
	.byte  $0c,$0e,$07
	.byte  $18,$07,$07
	.byte  $19,$07,$07
	.byte  $0b,$0f,$06
	.byte  $1a,$07,$07
	.byte  $1a,$07,$03
	.byte  $1f,$07,$03
	.byte  $18,$06,$06
	.byte  $17,$06,$04
	.byte  $14,$06,$04
	.byte  $15,$07,$01
	.byte  $00,$00,$00
