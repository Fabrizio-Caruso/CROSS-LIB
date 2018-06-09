; ******************************************************
; **       Librer�a de rutinas para Amstrad CPC       **
; **	   Ra�l Simarro, 	  Artaburu 2007       **
; ******************************************************

GLOBAL cpc_PutTile4X16		

GLOBAL cpc_GetScrAddress0

.cpc_PutTile4X16 ;siempre se dibujan en posicion caracter as� que no hay saltos raros en HL, se puede quitar la �ltima parte

	ld ix,2
	add ix,sp


	ld L,(ix+0)
	ld A,(ix+2)	;pantalla
	
	call cpc_GetScrAddress0
		
	ld E,(ix+4)
	ld D,(ix+5)	;DE sprite
	

		;LD A,8

		
   		LD a,16		;ALTO, SE PUEDE TRABAJAR CON HX DIRECTAMENTE
		;ld b,7
		
	.loop_alto_tile_2x8
	.loop_ancho_tile_2x8	
	ex de,hl
		ldi
		ldi
		ldi
		ldi
		
	 
	   dec a
	   jp nz,suma_siguiente_linea0_tile_2x8	
	   ret 
	   
.suma_siguiente_linea0_tile_2x8		;
		ex de,hl
		LD bC,$07fC			;&07f6 			;salto linea menos ancho
		ADD HL,BC
		jp nc,loop_alto_tile_2x8 ;sig_linea_2zz		;si no desborda va a la siguiente linea
		ld bc,$c050
		
		add HL,BC
		;ld b,7			;s�lo se dar�a una de cada 8 veces en un sprite
		jp loop_alto_tile_2x8	

