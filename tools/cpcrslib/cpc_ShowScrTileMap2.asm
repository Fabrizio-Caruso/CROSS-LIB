; ******************************************************
; **       Librería de rutinas para Amstrad CPC       **
; **	   Raúl Simarro, 	  Artaburu 2007       **
; ******************************************************

GLOBAL cpc_ShowTileMap2	;	para una pantalla de 64x160 bytes. Superbuffer 8192bytes
								;   los tiles son de 2x8 bytes

GLOBAL tiles
GLOBAL pantalla_juego					;datos de la pantalla, cada byte indica un tile
GLOBAL posiciones_super_buffer		
GLOBAL posicion_inicial_area_visible
GLOBAL posicion_inicial_superbuffer
GLOBAL ancho_pantalla_bytes

GLOBAL alto_pantalla_bytes
GLOBAL posiciones_pantalla


GLOBAL tiles_ocultos_ancho0
GLOBAL tiles_ocultos_alto0

GLOBAL posicion_inicio_pantalla_visible 
GLOBAL posicion_inicio_pantalla_visible_sb
GLOBAL posicion_inicio_pantalla_visible2

GLOBAL cpc_PutSp
GLOBAL papa
GLOBAL creascanes

GLOBAL cpc_ShowTileMap



.cpc_ShowTileMap2
	ld b,ancho_pantalla_bytes-4*(tiles_ocultos_ancho0)	;;nuevo ancho
	ld c,alto_pantalla_bytes-16*(tiles_ocultos_alto0)			;;nuevo alto
.posicion_inicio_pantalla_visible	
	ld hl,0000
	
	
.posicion_inicio_pantalla_visible_sb
	ld hl,0000
.papa		; código de Xilen Wars
	di
	ld	(auxsp),sp
	ld	sp,tablascan
	ld	a,alto_pantalla_bytes-16*(tiles_ocultos_alto0)	;16 alto
.ppv0
	pop	de		; va recogiendo de la pila!!
.inicio_salto_ldi	
	ldi
	ldi
	ldi
	ldi
	ldi
	ldi
	ldi
	ldi
	ldi
	ldi
	ldi
	ldi
	ldi
	ldi
	ldi
	ldi
	ldi
	ldi
	ldi
	ldi
	ldi
	ldi
	ldi
	ldi
	ldi
	ldi
	ldi
	ldi
	ldi
	ldi
	ldi
	ldi
	ldi
	ldi
	ldi
	ldi
	ldi
	ldi
	ldi
	ldi
	ldi
	ldi
	ldi
	ldi
	ldi
	ldi
	ldi
	ldi
	ldi
	ldi
	ldi
	ldi
	ldi
	ldi
	ldi
	ldi
	ldi
	ldi
	ldi
	ldi
	ldi
	ldi
	ldi
	ldi
	ldi
	ldi
	ldi
	ldi
	ldi
	ldi
	ldi
	ldi	
.CONT_salto_ldi	
	ld	de,4*tiles_ocultos_ancho0
	add	hl,de
	dec a
	jp nz, ppv0
	ld	sp,(auxsp)
	ei
	ret

.auxsp	defw	0	



.creascanes	
	ld	ix,tablascan
.posicion_inicio_pantalla_visible2
	ld	hl,0000
	ld	b, alto_pantalla_bytes/8-2*tiles_ocultos_alto0 ; 20	; num de filas. 
.cts0	
	push	bc
	push	hl
	ld	b,8
	ld	de,2048
.cts1	
	ld	(ix+0),l
	inc	ix
	ld	(ix+0),h
	inc	ix
	add	hl,de
	djnz	cts1
	pop	hl
	ld	bc,80
	add	hl,bc
	pop	bc
	djnz	cts0
;	jp prepara_salto_ldi
.prepara_salto_ldi		; para el ancho visible de la pantalla
	ld hl,ancho_pantalla_bytes-4*tiles_ocultos_ancho0
	ld de,inicio_salto_ldi 
	add hl,hl
	add hl,de
	ld (hl),$c3
	inc hl
	ld de,CONT_salto_ldi
	ld (hl),e
	inc hl
	ld (hl),d
	ret

.tablascan	defs 20*16  