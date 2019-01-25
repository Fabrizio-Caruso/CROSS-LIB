GLOBAL cpc_UpdateTileMap

GLOBAL cpc_PutSpTileMap	
GLOBAL cpc_PutMaskSpriteTileMap2b
GLOBAL cpc_UpdScr
GLOBAL cpc_ShowTouchedTiles
GLOBAL cpc_PutMaskSpTileMap2b
GLOBAL cpc_PutORSpTileMap2b
GLOBAL cpc_PutSpTileMap2b
GLOBAL cpc_PutTrSpTileMap2b
GLOBAL cpc_PutTrSpriteTileMap2b


   	;struct sprite 
	;   	int datos_fondo;		//2 bytes 	01	
	;   	int datos_sprite;		//2 bytes	23
	;   	int coord_pantalla;		//2 bytes	45  pos pantalla actual
	;   	int ocoord_pantalla;	//2 bytes	67	pos pantalla anterior
	;   	char cx, cy;			//2 bytes 	coordenadas nuevas
	;   	char ox, oy;			//2 bytes   coordenadas anteriores
	;   	char dir1;				//1 byte
	   		
   	
   	

.cpc_UpdateTileMap
;HL puntero a la lista de sprites
push hl
push hl
pop iy


;Se envía el sprite en HL
.bucle_recorrido_lista_f1
	ld e,(iy+0)
	LD a,e
	cp $ff
	jp z,cont_fase2
	ld d,(iy+1)
	;push ix
	
	;push hl	;11
	;pop ix		;14
	;25
	

	LD IXH,d	;9
    LD IXL,e	;9   
    ;18
    
	bit 0,(ix+12)
    ;ret z			;If the sprite doesn't have to draw
    
	call nz, cpc_PutSpTileMap+5	;OJO, cambiado para dar algo más de velocidad.
	;pop ix
	
	inc iy 
	inc iy
	jp bucle_recorrido_lista_f1

.cont_fase2	

call cpc_UpdScr

pop iy	
.bucle_recorrido_lista_f2
	ld e,(iy+0)
	LD a,e
	cp $ff
	jp z,cont_fase3
	ld d,(iy+1)
	;push iy
	
	;push hl
	;pop ix

	LD IXH,d	;9
    LD IXL,e	;9 
    	
	ld a,(ix+12)

	
	and @00001110
	
	cp @00000010	;2
	jp z,op1
	;ld a,b
	;and @00000100
	cp @00000100
	jp z,op2
	;ld a,b
	;and @00000110
	cp @00000110
	jp z,op3
	;ld a,b
	;and @00001000
	cp @00001000
	jp z,op4		
	cp @00001010
	jp z,op5			
	cp @00001100
	jp z,op6	
.cont_buc	
	;pop iy
	inc iy 
	inc iy
	jp bucle_recorrido_lista_f2	
.cont_fase3	
jp cpc_ShowTouchedTiles
;ret
.op1
call cpc_PutMaskSpriteTileMap2b+5
jp cont_buc
.op2
call cpc_PutMaskSpTileMap2b+5
jp cont_buc
.op3
call cpc_PutORSpTileMap2b+5
jp cont_buc
.op4
call cpc_PutSpTileMap2b+5
jp cont_buc
.op5
call cpc_PutTrSpTileMap2b+5
jp cont_buc
.op6
call cpc_PutTrSpriteTileMap2b+5
jp cont_buc