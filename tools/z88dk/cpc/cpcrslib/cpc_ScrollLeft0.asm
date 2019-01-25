; ******************************************************
; **       Librería de rutinas para Amstrad CPC       **
; **	   Raúl Simarro, 	  Artaburu 2009       **
; ******************************************************

GLOBAL cpc_ScrollLeft0





GLOBAL tiles
GLOBAL ancho_pantalla_bytes 
GLOBAL rightColumnScr
GLOBAL alto_pantalla_bytes
GLOBAL pantalla_juego
GLOBAL posiciones_pantalla
GLOBAL posicion_inicial_superbuffer


.cpc_ScrollLeft0

ld hl,pantalla_juego+1
ld de,pantalla_juego
ld bc,alto_pantalla_bytes*ancho_pantalla_bytes/16 -1
LDIR

ld hl,posicion_inicial_superbuffer+2
ld de,posicion_inicial_superbuffer
ld bc,alto_pantalla_bytes*ancho_pantalla_bytes -1
LDIR

RET