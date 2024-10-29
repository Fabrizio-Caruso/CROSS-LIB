	.rodata
	.export	_sfx_roarcheep
_sfx_roarcheep:
	.byte  $10,$10,$00 ; version, priority, frames per chunk
	.byte  $0a,$06,$00 ; first chunk of freq,channel,volume
	.byte  $0e,$0f,$01
	.byte  $12,$0e,$09
	.byte  $0a,$0e,$07
	.byte  $0a,$0f,$0e
	.byte  $1b,$07,$0f
	.byte  $1b,$06,$0c
	.byte  $13,$0f,$0e
	.byte  $15,$0f,$0d
	.byte  $0f,$0e,$0f
	.byte  $19,$0f,$0f
	.byte  $0d,$0f,$07
	.byte  $0e,$0e,$08
	.byte  $0f,$0f,$08
	.byte  $0a,$01,$09
	.byte  $0d,$0e,$0b
	.byte  $19,$07,$0c
	.byte  $10,$0f,$0f
	.byte  $16,$07,$0f
	.byte  $10,$04,$0f
	.byte  $17,$04,$0f
	.byte  $15,$04,$0e
	.byte  $1b,$07,$0f
	.byte  $1f,$06,$0c
	.byte  $12,$0f,$0b
	.byte  $1a,$06,$04
	.byte  $1f,$01,$04
	.byte  $0a,$0e,$09
	.byte  $0f,$0e,$0b
	.byte  $0f,$0e,$07
	.byte  $0f,$0e,$06
	.byte  $0f,$0e,$05
	.byte  $0f,$0e,$03
	.byte  $0f,$0e,$01
	.byte  $00,$00,$00
