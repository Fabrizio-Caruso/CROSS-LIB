
GLOBAL cpc_ScrollRight0





GLOBAL tiles
GLOBAL ancho_pantalla_bytes 
GLOBAL rightColumnScr
GLOBAL alto_pantalla_bytes
GLOBAL pantalla_juego
GLOBAL posiciones_pantalla
GLOBAL posicion_inicial_superbuffer

.cpc_ScrollRight0		;;scrollea el area de pantalla de tiles
ld hl,pantalla_juego+alto_pantalla_bytes*ancho_pantalla_bytes/16-1
ld de,pantalla_juego+alto_pantalla_bytes*ancho_pantalla_bytes/16
ld bc,alto_pantalla_bytes*ancho_pantalla_bytes/16 -1 ;-1
LDDR

;;scrollea el superbuffer
ld hl,posicion_inicial_superbuffer+alto_pantalla_bytes*ancho_pantalla_bytes-2 ; pantalla_juego+alto_pantalla_bytes*ancho_pantalla_bytes/16-1
ld de,posicion_inicial_superbuffer+alto_pantalla_bytes*ancho_pantalla_bytes ;pantalla_juego+alto_pantalla_bytes*ancho_pantalla_bytes/16
ld bc,alto_pantalla_bytes*ancho_pantalla_bytes-1 ;-1
LDDR
RET