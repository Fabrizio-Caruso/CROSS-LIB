; ******************************************************
; **       Librería de rutinas para Amstrad CPC       **
; **	   Raúl Simarro, 	  Artaburu 2010           **
; **	   PLAYER programado por  WYZ		          **
; ******************************************************
           
GLOBAL cpc_WyzPlayer

;GLOBAL BUFFER_DEC

GLOBAL CARGA_CANCION_WYZ
GLOBAL INICIA_EFECTO_WYZ
GLOBAL cpc_WyzSetPlayerOn0
GLOBAL cpc_WyzSetPlayerOff0

GLOBAL TABLA_SONG
GLOBAL TABLA_EFECTOS
GLOBAL TABLA_PAUTAS
GLOBAL TABLA_SONIDOS
GLOBAL INTERRUPCION

GLOBAL BUFFER_MUSICA
GLOBAL direcc_tempo


;DEFINE AMSTRAD = 1

;DEFINE BUFFER_DEC  = $100





; CPC PSG proPLAYER - WYZ 2010


.cpc_WyzPlayer



.cpc_WyzSetPlayerOn1
	;El player funcionará por interrupciones.
	DI
	ld a,($0038)
	ld (datos_int),a
	ld (salto_int),a
	ld a,($0039)
	ld (datos_int+1),a
	ld (salto_int+1),a
	ld a,($003a)
	ld (datos_int+2),a
	ld (salto_int+2),a
	
	
;	ld hl,($0038)	
;	ld (datos_int),HL	;guardo el salto original
;	ld a,l
;	ld (salto_int+1),a
;	ld a,h
;	ld (salto_int+2),a
	ld a,$C3
	ld ($0038),a
	ld HL,INICIO
	ld ($0039),HL
	EI
	ret

.cpc_WyzSetPlayerOff1
	
	;apago todos los sonidos poniendo los registros a 0
	call PLAYER_OFF
	DI	
	;Restaura salto original
	ld a,(datos_int)	;guardo el salto original
	ld ($0038),A
	ld a,(datos_int+1)	;guardo el salto original
	ld ($0039),A
	ld a,(datos_int+2)	;guardo el salto original
	ld ($003a),A		
	
	EI
	ret
	
	
	


                


;___________________________________________________________

             ;   DEFB     "PSG PROPLAYER BY WYZ'10"

;___________________________________________________________

	
;___________________________________________________________

.INICIO


	;primero mira si toca tocar :P
	push af
	LD A,(contador)
	DEC A
	LD (contador),A
	cp 0
	JP NZ,termina_int
	.dir_tempo	
	LD A,6
 	LD (contador),A
 	
 	PUSH BC
	PUSH HL
	PUSH DE
	PUSH IX
	PUSH IY	
 	
		CALL    ROUT
		LD	HL,PSG_REG
		LD	DE,PSG_REG_SEC
		LD	BC,14
		LDIR				
        CALL    PLAY	        
		CALL    REPRODUCE_SONIDO         
                
                
		CALL	REPRODUCE_EFECTO_A
		CALL	REPRODUCE_EFECTO_B
		CALL	REPRODUCE_EFECTO_C                
                
	POP IY
	POP IX
	POP DE
	POP HL
	POP BC	

	.termina_int		
		pop af	
		ei
.salto_int
defb 0,0,0
 
 	

.contador defb 0
.datos_int defb 0,0,0		; Se guardan 3 BYTES!!!! (Dedicado a Na_th_an, por los desvelos)



;INICIA EL SONIDO Nº (A)

;INICIA EL SONIDO Nº (A)

.INICIA_EFECTO_WYZ0

;INICIA EL SONIDO Nº (B) EN EL CANAL (C)
		LD	A,C
		CP	0
		JP	Z,INICIA_EFECTO_A
		CP	1
		JP	Z,INICIA_EFECTO_B
		CP	2
		JP	Z,INICIA_EFECTO_C
		JP INICIA_EFECTO_A
		RET
		

;REPRODUCE EFECTOS


			

;REPRODUCE EFECTOS CANAL A


.REPRODUCE_EFECTO_A
                LD      HL,INTERR   
                BIT     3,(HL)          ;ESTA ACTIVADO EL EFECTO?
                RET     Z
                LD      HL,(PUNTERO_EFECTO_A)
                LD      A,(HL)
                CP      $FF
                JR      Z,FIN_EFECTO_A
                CALL 	BLOQUE_COMUN
                LD      (PUNTERO_EFECTO_A),HL
                LD      (IX+0),B                
                LD      (IX+1),C
                LD      (IX+8),A
                RET
.FIN_EFECTO_A
   				LD      HL,INTERR
                RES     3,(HL)
                XOR     A       
                LD      (PSG_REG+0),A
                LD      (PSG_REG+1),A
                LD		(PSG_REG+8),A
                RET    
                
.REPRODUCE_EFECTO_B
                LD      HL,INTERR   
                BIT     5,(HL)          ;ESTA ACTIVADO EL EFECTO?
                RET     Z
                LD      HL,(PUNTERO_EFECTO_B)
                LD      A,(HL)
                CP      $FF
                JR      Z,FIN_EFECTO_B
                CALL 	BLOQUE_COMUN
                LD      (PUNTERO_EFECTO_B),HL
                LD      (IX+2),B                
                LD      (IX+3),C
                LD      (IX+9),A
                RET
.FIN_EFECTO_B
   				LD      HL,INTERR
                RES     5,(HL)
                XOR     A       
                LD      (PSG_REG+2),A
                LD      (PSG_REG+3),A
                LD		(PSG_REG+9),A
                RET                 
                
.REPRODUCE_EFECTO_C
                LD      HL,INTERR   
                BIT     6,(HL)          ;ESTA ACTIVADO EL EFECTO?
                RET     Z
                LD      HL,(PUNTERO_EFECTO_C)
                LD      A,(HL)
                CP      $FF
                JR      Z,FIN_EFECTO_C
                CALL 	BLOQUE_COMUN
                LD      (PUNTERO_EFECTO_C),HL
                LD      (IX+4),B                
                LD      (IX+5),C
                LD      (IX+10),A
                RET
.FIN_EFECTO_C
   				LD      HL,INTERR
                RES     6,(HL)
                XOR     A       
                LD      (PSG_REG+4),A
                LD      (PSG_REG+5),A
                LD		(PSG_REG+10),A
                RET 
                                
.BLOQUE_COMUN                
				LD IX,PSG_REG
                LD B,A
                INC     HL
                LD      A,(HL)
                RRCA
                RRCA
                RRCA
                RRCA
                AND     @00001111
                LD C,A
                LD      A,(HL)
                AND     @00001111
                INC     HL
				RET        
                
.INICIA_EFECTO_A
				LD		A,B
				LD      HL,(TABLA_EFECTOS0)
                CALL    EXT_WORD
                LD      (PUNTERO_EFECTO_A),HL
                LD      HL,INTERR
                SET     3,(HL)
                RET       
                
.INICIA_EFECTO_B
				LD		A,B
				LD      HL,(TABLA_EFECTOS0)
                CALL    EXT_WORD
                LD      (PUNTERO_EFECTO_B),HL
                LD      HL,INTERR
                SET     5,(HL)
                RET       
               
.INICIA_EFECTO_C 
				LD		A,B
				LD      HL,(TABLA_EFECTOS0)
                CALL    EXT_WORD
                LD      (PUNTERO_EFECTO_C),HL
                LD      HL,INTERR
                SET     6,(HL)
                RET       
               
	

.INICIA_SONIDO
				LD       HL,(TABLA_SONIDOS0)
                CALL    EXT_WORD
                LD      (PUNTERO_SONIDO),HL
                LD      HL,INTERR
                SET     2,(HL)
                RET
;PLAYER OFF

.PLAYER_OFF
		LD	HL,INTERR
		RES	1,(HL)
		
		XOR	A
		LD	HL,PSG_REG
		LD	DE,PSG_REG+1
		LD	BC,14
		LD	(HL),A
		LDIR

		LD	HL,PSG_REG_SEC
		LD	DE,PSG_REG_SEC+1
		LD	BC,14
		LD	(HL),A
		LDIR
		
		CALL	ROUT
		CALL	FIN_SONIDO
		RET


		
		
.CARGA_CANCION_WYZ0		
        DI
		CALL	PLAYER_OFF
; MUSICA DATOS INICIALES


 				
 				
				LD		DE,$0010					;  Nº BYTES RESERVADOS POR CANAL
                LD      HL,BUFFER_DEC       	;* RESERVAR MEMORIA PARA BUFFER DE SONIDO!!!!!
                LD      (CANAL_A),HL
                
                ADD     HL,DE       	
                LD      (CANAL_B),HL       	

                ADD     HL,DE       	
                LD      (CANAL_C),HL 

                ADD     HL,DE       	
                LD      (CANAL_P),HL 

                LD      A,0             	;* CANCION Nº 0
                CALL    CARGA_CANCION
                
               	LD A,6
 				LD (contador),A
 				                
;PANTALLA
		EI
		ret		
		
				

;CARGA UNA CANCION
;IN:(A)=Nº DE CANCION

.CARGA_CANCION
				LD      HL,INTERR       ;CARGA CANCION
		
                SET     1,(HL)          ;REPRODUCE CANCION
                LD      HL,SONG
                LD      (HL),A          ;Nº A

                

;DECODIFICAR
;IN-> INTERR 0 ON
;     SONG

;CARGA CANCION SI/NO

.DECODE_SONG
			    LD      A,(SONG)

;LEE CABECERA DE LA CANCION
;BYTE 0=TEMPO

                ;LD      HL,TABLA_SONG
                LD      HL,(TABLA_SONG0)                
                CALL    EXT_WORD
                LD      A,(HL)
                LD      (TEMPO),A
		XOR	A
		LD	(TTEMPO),A
                
;HEADER BYTE 1
;(-|-|-|-|-|-|-|LOOP)

                INC	HL		;LOOP 1=ON/0=OFF?
                LD	A,(HL)
                BIT	0,A
                JR	Z,NPTJP0
                PUSH	HL
                LD	HL,INTERR
                SET	4,(HL)
                POP	HL
             
                
.NPTJP0
		        INC	HL		;2 BYTES RESERVADOS
                INC	HL
                INC	HL

;BUSCA Y GUARDA INICIO DE LOS CANALES EN EL MODULO MUS

		
		LD	(PUNTERO_P_DECA),HL
		LD	E,$3F			;CODIGO INTRUMENTO 0
		LD	B,$FF			;EL MODULO DEBE TENER UNA LONGITUD MENOR DE $FF00 ... o_O!
.BGICMODBC1
		XOR	A			;BUSCA EL BYTE 0
		CPIR
		DEC	HL
		DEC	HL
		LD	A,E			;ES EL INSTRUMENTO 0??
		CP	(HL)
		INC	HL
		INC	HL
		JR	Z,BGICMODBC1

		LD	(PUNTERO_P_DECB),HL

.BGICMODBC2
		XOR	A			;BUSCA EL BYTE 0
		CPIR
		DEC	HL
		DEC	HL
		LD	A,E
		CP	(HL)			;ES EL INSTRUMENTO 0??
		INC	HL
		INC	HL
		JR	Z,BGICMODBC2

		LD	(PUNTERO_P_DECC),HL
		
.BGICMODBC3
		XOR	A			;BUSCA EL BYTE 0
		CPIR
		DEC	HL
		DEC	HL
		LD	A,E
		CP	(HL)			;ES EL INSTRUMENTO 0??
		INC	HL
		INC	HL
		JR	Z,BGICMODBC3
		LD	(PUNTERO_P_DECP),HL
		
                
;LEE DATOS DE LAS NOTAS
;(|)(|||||) LONGITUD\NOTA

.INIT_DECODER
			    LD      DE,(CANAL_A)
                LD      (PUNTERO_A),DE
                LD	HL,(PUNTERO_P_DECA)
                CALL    DECODE_CANAL    ;CANAL A
                LD	(PUNTERO_DECA),HL
                
                LD      DE,(CANAL_B)
                LD      (PUNTERO_B),DE
                LD	HL,(PUNTERO_P_DECB)
                CALL    DECODE_CANAL    ;CANAL B
                LD	(PUNTERO_DECB),HL
                
                LD      DE,(CANAL_C)
                LD      (PUNTERO_C),DE
                LD	HL,(PUNTERO_P_DECC)
                CALL    DECODE_CANAL    ;CANAL C
                LD	(PUNTERO_DECC),HL
                
                LD      DE,(CANAL_P)
                LD      (PUNTERO_P),DE
                LD	HL,(PUNTERO_P_DECP)
                CALL    DECODE_CANAL    ;CANAL P
                LD	(PUNTERO_DECP),HL
               
                RET


;DECODIFICA NOTAS DE UN CANAL
;IN (DE)=DIRECCION DESTINO
;NOTA=0 FIN CANAL
;NOTA=1 SILENCIO
;NOTA=2 PUNTILLO
;NOTA=3 COMANDO I

.DECODE_CANAL
			    LD      A,(HL)
                AND     A               ;FIN DEL CANAL?
                JR      Z,FIN_DEC_CANAL
                CALL    GETLEN

                CP      @00000001       ;ES SILENCIO?
                JR      NZ,NO_SILENCIO
                SET     6,A
                JR      NO_MODIFICA
                
.NO_SILENCIO
			    CP      @00111110        ;ES PUNTILLO?
                JR      NZ,NO_PUNTILLO
                OR      A
                RRC     B
                XOR     A
                JR      NO_MODIFICA

.NO_PUNTILLO
			    CP      @00111111        ;ES COMANDO?
                JR      NZ,NO_MODIFICA
                BIT     0,B             ;COMADO=INSTRUMENTO?
                JR      Z,NO_INSTRUMENTO   
                LD      A,@11000001      ;CODIGO DE INSTRUMENTO      
                LD      (DE),A
                INC     HL
                INC     DE
                LD      A,(HL)          ;Nº DE INSTRUMENTO
                LD      (DE),A
                INC     DE
                INC	HL
                JR      DECODE_CANAL
                
.NO_INSTRUMENTO
				BIT     2,B
                JR      Z,NO_ENVOLVENTE
                LD      A,@11000100      ;CODIGO ENVOLVENTE
                LD      (DE),A
                INC     DE
                INC	HL
                JR      DECODE_CANAL
     
.NO_ENVOLVENTE
				BIT     1,B
                JR      Z,NO_MODIFICA           
                LD      A,@11000010      ;CODIGO EFECTO
                LD      (DE),A                  
                INC     HL                      
                INC     DE                      
                LD      A,(HL)                  
                CALL    GETLEN   
                
.NO_MODIFICA
			    LD      (DE),A
                INC     DE
                XOR     A
                DJNZ    NO_MODIFICA
		SET     7,A
		SET 	0,A
                LD      (DE),A
                INC     DE
                INC	HL
                RET			;** JR      DECODE_CANAL

.FIN_DEC_CANAL
				SET     7,A
                LD      (DE),A
                INC     DE
                RET

.GETLEN
		         LD      B,A
                AND     @00111111
                PUSH    AF
                LD      A,B
                AND     @11000000 
                RLCA
                RLCA
                INC     A
                LD      B,A
                LD      A,@10000000 
.DCBC0
	          RLCA
                DJNZ    DCBC0
                LD      B,A
                POP     AF
                RET
                
          
                

        
                
;PLAY __________________________________________________


.PLAY
	          	LD      HL,INTERR       ;PLAY BIT 1 ON?
                BIT     1,(HL)
                RET     Z
;TEMPO          
                LD      HL,TTEMPO       ;CONTADOR TEMPO
                INC     (HL)
                LD      A,(TEMPO)
                CP      (HL)
                JR      NZ,PAUTAS
                LD      (HL),0
                
;INTERPRETA      
                LD      IY,PSG_REG
                LD      IX,PUNTERO_A
                LD      BC,PSG_REG+8
                CALL    LOCALIZA_NOTA
                LD      IY,PSG_REG+2
                LD      IX,PUNTERO_B
                LD      BC,PSG_REG+9
                CALL    LOCALIZA_NOTA
                LD      IY,PSG_REG+4
                LD      IX,PUNTERO_C
                LD      BC,PSG_REG+10
                CALL    LOCALIZA_NOTA
                LD      IX,PUNTERO_P    ;EL CANAL DE EFECTOS ENMASCARA OTRO CANAL
                CALL    LOCALIZA_EFECTO              

;PAUTAS 
                
.PAUTAS
		        LD      IY,PSG_REG+0
                LD      IX,PUNTERO_P_A
                LD      HL,PSG_REG+8
                CALL    PAUTA           ;PAUTA CANAL A
                LD      IY,PSG_REG+2
                LD      IX,PUNTERO_P_B
                LD      HL,PSG_REG+9
                CALL    PAUTA           ;PAUTA CANAL B
                LD      IY,PSG_REG+4
                LD      IX,PUNTERO_P_C
                LD      HL,PSG_REG+10
                CALL    PAUTA           ;PAUTA CANAL C                

                RET
                


;REPRODUCE EFECTOS DE SONIDO 

.REPRODUCE_SONIDO

				LD      HL,INTERR   
                BIT     2,(HL)          ;ESTA ACTIVADO EL EFECTO?
                RET     Z
                LD      HL,(PUNTERO_SONIDO)
                LD      A,(HL)
                CP      $FF
                JR      Z,FIN_SONIDO
                LD      (PSG_REG_SEC+0),A
                INC     HL
                LD      A,(HL)
                RRCA
                RRCA
                RRCA
                RRCA
                AND     @00001111 
                LD      (PSG_REG_SEC+1),A
                LD      A,(HL)
                AND     @00001111 
                LD      (PSG_REG_SEC+8),A
                INC     HL
                LD      A,(HL)
                AND     A
                JR      Z,NO_RUIDO
                LD      (PSG_REG_SEC+6),A
                LD      A,@10110000 
                JR      SI_RUIDO
.NO_RUIDO
		        LD      A,@10111000 
.SI_RUIDO
		        LD      (PSG_REG_SEC+7),A
       
                INC     HL
                LD      (PUNTERO_SONIDO),HL
                RET
.FIN_SONIDO
			    LD      HL,INTERR
                RES     2,(HL)

.FIN_NOPLAYER
				LD      A,@10111000 		;2 BITS ALTOS PARA MSX / AFECTA AL CPC???
       			LD      (PSG_REG+7),A
                RET         
                
;VUELCA BUFFER DE SONIDO AL PSG

;VUELCA BUFFER DE SONIDO AL PSG

.ROUT
       	XOR 	A
		LD 	HL,PSG_REG_SEC
.LOUT
		CALL 	WRITEPSGHL
		INC 	A
		CP 	13
		JR 	NZ,LOUT
		LD	A,(HL)
		AND 	A
		RET 	Z
		LD	A,13
		CALL 	WRITEPSGHL
		XOR	A
		LD      (PSG_REG+13),A
		LD      (PSG_REG_SEC+13),A
		RET

;; A = REGISTER
;; (HL) = VALUE
.WRITEPSGHL
		LD 	B,$F4              
		OUT 	(C),A              
		LD 	BC,$F6C0           
		OUT 	(C),C              
		DEFB 	$ED                
		DEFB 	$71                
		LD 	B,$F5              
		OUTI                       
		LD 	BC,$F680           
		OUT 	(C),C              
		DEFB 	$ED                
		DEFB 	$71                
		RET

;LOCALIZA NOTA CANAL A
;IN (PUNTERO_A)

.LOCALIZA_NOTA
			    LD      L,(IX+0)       		;HL=(PUNTERO_A_C_B)
                LD      H,(IX+1)
                LD      A,(HL)
                AND     @11000000       		;COMANDO?
                CP      @11000000 
                JR      NZ,LNJP0

;BIT(0)=INSTRUMENTO
                
.COMANDOS
		        LD      A,(HL)
                BIT     0,A             	;INSTRUMENTO
                JR      Z,COM_EFECTO

                INC     HL
                LD      A,(HL)          	;Nº DE PAUTA
                INC     HL
                LD      (IX+00),L
                LD      (IX+01),H
                ;LD      HL,TABLA_PAUTAS
                LD      HL,(TABLA_PAUTAS0)
                CALL    EXT_WORD
                LD      (IX+18),L
                LD      (IX+19),H
                LD      (IX+12),L
                LD      (IX+13),H
                LD      L,C
                LD      H,B
                RES     4,(HL)        		;APAGA EFECTO ENVOLVENTE
                XOR     A
                LD      (PSG_REG_SEC+13),A
                LD	(PSG_REG+13),A
                JR      LOCALIZA_NOTA

.COM_EFECTO
			    BIT     1,A             	;EFECTO DE SONIDO
                JR      Z,COM_ENVOLVENTE

                INC     HL
                LD      A,(HL)
                INC     HL
                LD      (IX+00),L
                LD      (IX+01),H
                CALL    INICIA_SONIDO
                RET

.COM_ENVOLVENTE
				BIT     2,A
                RET     Z               	;IGNORA - ERROR            
           
                INC     HL
                LD      (IX+00),L
                LD      (IX+01),H
                LD      L,C
                LD      H,B
                LD	(HL),@00010000           ;ENCIENDE EFECTO ENVOLVENTE
                JR      LOCALIZA_NOTA
                
              
.LNJP0
			    LD      A,(HL)
                INC     HL
                BIT     7,A
                JR      Z,NO_FIN_CANAL_A	;
                BIT	0,A
                JR	Z,FIN_CANAL_A

.FIN_NOTA_A
		LD      E,(IX+6)
		LD	D,(IX+7)	;PUNTERO BUFFER AL INICIO
		LD	(IX+0),E
		LD	(IX+1),D
		LD	L,(IX+30)	;CARGA PUNTERO DECODER
		LD	H,(IX+31)
		PUSH	BC
                CALL    DECODE_CANAL    ;DECODIFICA CANAL
                POP	BC
                LD	(IX+30),L	;GUARDA PUNTERO DECODER
                LD	(IX+31),H
                JP      LOCALIZA_NOTA
                
.FIN_CANAL_A
			    LD	HL,INTERR	;LOOP?
                BIT	4,(HL)              
                JR      NZ,FCA_CONT
                CALL	PLAYER_OFF
                RET

.FCA_CONT
		LD	L,(IX+24)	;CARGA PUNTERO INICIAL DECODER
		LD	H,(IX+25)
		LD	(IX+30),L
		LD	(IX+31),H
		JR      FIN_NOTA_A
                
.NO_FIN_CANAL_A
				LD      (IX+0),L        ;(PUNTERO_A_B_C)=HL GUARDA PUNTERO
                LD      (IX+1),H
                AND     A               ;NO REPRODUCE NOTA SI NOTA=0
                JR      Z,FIN_RUTINA
                BIT     6,A             ;SILENCIO?
                JR      Z,NO_SILENCIO_A
                LD	A,(BC)
                AND	@00010000 
                JR	NZ,SILENCIO_ENVOLVENTE
                XOR     A
                LD	(BC),A		;RESET VOLUMEN
                LD	(IY+0),A
                LD	(IY+1),A
				RET
		
.SILENCIO_ENVOLVENTE
				LD	A,$FF
                LD	(PSG_REG+11),A
                LD	(PSG_REG+12),A               
                XOR	A
                LD	(PSG_REG+13),A                               
                LD	(IY+0),A
                LD	(IY+1),A
                RET

.NO_SILENCIO_A
				CALL    NOTA            ;REPRODUCE NOTA
                LD      L,(IX+18)       ;HL=(PUNTERO_P_A0) RESETEA PAUTA 
                LD      H,(IX+19)
                LD      (IX+12),L       ;(PUNTERO_P_A)=HL
                LD      (IX+13),H
.FIN_RUTINA
			    RET


;LOCALIZA EFECTO
;IN HL=(PUNTERO_P)

.LOCALIZA_EFECTO
				LD      L,(IX+0)       ;HL=(PUNTERO_P)
                LD      H,(IX+1)
                LD      A,(HL)
                CP      @11000010 
                JR      NZ,LEJP0

                INC     HL
                LD      A,(HL)
                INC     HL
                LD      (IX+00),L
                LD      (IX+01),H
                CALL    INICIA_SONIDO
                RET
            
              
.LEJP0
	            INC     HL
                BIT     7,A
                JR      Z,NO_FIN_CANAL_P	;
                BIT	0,A
                JR	Z,FIN_CANAL_P
.FIN_NOTA_P
		LD      DE,(CANAL_P)
		LD	(IX+0),E
		LD	(IX+1),D
		LD	HL,(PUNTERO_DECP)	;CARGA PUNTERO DECODER
		PUSH	BC
		CALL    DECODE_CANAL    	;DECODIFICA CANAL
		POP	BC
                LD	(PUNTERO_DECP),HL	;GUARDA PUNTERO DECODER
                JP      LOCALIZA_EFECTO
                
.FIN_CANAL_P
		LD	HL,(PUNTERO_P_DECP)	;CARGA PUNTERO INICIAL DECODER
		LD	(PUNTERO_DECP),HL
		JR      FIN_NOTA_P
                
.NO_FIN_CANAL_P
				LD      (IX+0),L        ;(PUNTERO_A_B_C)=HL GUARDA PUNTERO
                LD      (IX+1),H
                RET
                
; PAUTA DE LOS 3 CANALES
; IN:(IX):PUNTERO DE LA PAUTA
;    (HL):REGISTRO DE VOLUMEN
;    (IY):REGISTROS DE FRECUENCIA

; FORMATO PAUTA	
;	    7    6     5     4   3-0                     3-0  
; BYTE 1 (LOOP|OCT-1|OCT+1|SLIDE|VOL) - BYTE 2 ( | | | |PITCH)

.PAUTA
		        BIT     4,(HL)        ;SI LA ENVOLVENTE ESTA ACTIVADA NO ACTUA PAUTA
                RET     NZ

		LD	A,(IY+0)
		LD	B,(IY+1)
		OR	B
		RET	Z


                PUSH	HL
                ;LD      L,(IX+0)
                ;LD      H,(IX+1)
                		
                ;LD	A,(HL)		;COMPRUEBA SLIDE BIT 4
		;BIT	4,A
		;JR	Z,PCAJP4
		;LD	L,(IY+0)	;FRECUENCIA FINAL
		;LD	H,(IY+1)
		;SBC	HL,DE
		;JR	Z,PCAJP4
		;JR	C,SLIDE_POS
		;EX	DE,HL
		;RRC	D		;/4
		;RR	E
		;RRC	D
		;RR	E


		;ADC	HL,DE
		;LD	(IY+0),L
		;LD	(IY+1),H
.SLIDE_POS		
		;POP	HL
		;RET
                
.PCAJP4
		        LD      L,(IX+0)
                LD      H,(IX+1)         
		LD	A,(HL)
		
		BIT     7,A		;LOOP / EL RESTO DE BITS NO AFECTAN
                JR      Z,PCAJP0
                AND     @00011111        ;LOOP PAUTA (0,32)X2!!!-> PARA ORNAMENTOS
                RLCA			;X2
                LD      D,0
                LD      E,A
                SBC     HL,DE
                LD      A,(HL)

.PCAJP0
		BIT	6,A		;OCTAVA -1
		JR	Z,PCAJP1
		LD	E,(IY+0)
		LD	D,(IY+1)

		AND	A
		RRC	D
		RR	E
		LD	(IY+0),E
		LD	(IY+1),D
		JR	PCAJP2
		
.PCAJP1
		BIT	5,A		;OCTAVA +1
		JR	Z,PCAJP2
		LD	E,(IY+0)
		LD	D,(IY+1)

		AND	A
		RLC	E
		RL	D
		LD	(IY+0),E
		LD	(IY+1),D		


.PCAJP2
		INC     HL
		PUSH	HL
		LD	E,A
		LD	A,(HL)		;PITCH DE FRECUENCIA
		LD	L,A
		AND	A
		LD	A,E
		JR	Z,ORNMJP1

                LD	A,(IY+0)	;SI LA FRECUENCIA ES 0 NO HAY PITCH
                ADD	A,(IY+1)
                AND	A
                LD	A,E
                JR	Z,ORNMJP1
                

		BIT	7,L
		JR	Z,ORNNEG
		LD	H,$FF
		JR	PCAJP3
.ORNNEG
		LD	H,0
		
.PCAJP3
		LD	E,(IY+0)
		LD	D,(IY+1)
		ADC	HL,DE
		LD	(IY+0),L
		LD	(IY+1),H
.ORNMJP1
		POP	HL
		
		INC	HL
                LD      (IX+0),L
                LD      (IX+1),H
.PCAJP5
         POP	HL
                AND	@00001111 	;VOLUMEN FINAL
                LD      (HL),A
                RET



;NOTA : REPRODUCE UNA NOTA
;IN (A)=CODIGO DE LA NOTA
;   (IY)=REGISTROS DE FRECUENCIA


.NOTA
           ;ADD	6		;*************************
		LD      L,C
                LD      H,B
                BIT     4,(HL)
                LD      B,A
                JR	NZ,EVOLVENTES
                LD	A,B
                LD      HL,DATOS_NOTAS
                RLCA                    ;X2
                LD      D,0
                LD      E,A
                ADD     HL,DE
                LD      A,(HL)
                LD      (IY+0),A
                INC     HL
                LD      A,(HL)
                LD      (IY+1),A
                RET

;IN (A)=CODIGO DE LA ENVOLVENTE
;   (IY)=REGISTRO DE FRECUENCIA

.EVOLVENTES     
		PUSH	AF
		CALL	ENV_RUT1
		LD	DE,$0000
		LD      (IY+0),E
                LD      (IY+1),D		
	
		POP	AF	
		ADD	A,48
		CALL	ENV_RUT1
		
	
		LD	A,E
                LD      (PSG_REG+11),A
                LD	A,D
                LD      (PSG_REG+12),A
                LD      A,$0E
                LD      (PSG_REG+13),A
                RET

;IN(A) NOTA
.ENV_RUT1
	LD      HL,DATOS_NOTAS
		RLCA                    ;X2
                LD      D,0
                LD      E,A
                ADD     HL,DE
                LD	E,(HL)
		INC	HL
		LD	D,(HL)
                RET



.EXT_WORD
		        LD      D,0
                SLA     A               ;*2
                LD      E,A
                ADD     HL,DE
                LD      E,(HL)
                INC     HL
                LD      D,(HL)
                EX      DE,HL
                RET

;BANCO DE INSTRUMENTOS 2 BYTES POR INT.

;(0)(RET 2 OFFSET)
;(1)(+-PITCH)


;BANCO DE INSTRUMENTOS 2 BYTES POR INT.

;(0)(RET 2 OFFSET)
;(1)(+-PITCH)

;.TABLA_PAUTAS DEFW 	PAUTA_1,PAUTA_2,PAUTA_3,PAUTA_4,PAUTA_5,PAUTA_6,PAUTA_7;,PAUTA_8,PAUTA_9,PAUTA_10,PAUTA_11,PAUTA_12,PAUTA_13,PAUTA_14,PAUTA_15,PAUTA_16,PAUTA_17,PAUTA_18











;DATOS DE LOS EFECTOS DE SONIDO

;EFECTOS DE SONIDO



;.TABLA_SONIDOS  DEFW    SONIDO1,SONIDO2,SONIDO3,SONIDO4,SONIDO5;,SONIDO6,SONIDO7;,SONIDO8

.TABLA_PAUTAS0 defw 0  

.TABLA_SONIDOS0 defw 0
                

;DATOS MUSICA



;TABLA_SONG:     DEFW    SONG_0;,SONG_1,SONG_2;,SONG_3          ;******** TABLA DE DIRECCIONES DE ARCHIVOS MUS

;DATOS_NOTAS:    .INCBIN "C:/EM/BRMSX/PLAYER/NOTAS.DAT"        ;DATOS DE LAS NOTAS


.DATOS_NOTAS
    DEFW $0000,$0000
;		;DW $077C,$0708
;		DW $06B0,$0640,$05EC,$0594,$0544,$04F8,$04B0,$0470,$042C,$03FD
;		DW $03BE,$0384,$0358,$0320,$02F6,$02CA,$02A2,$027C,$0258,$0238,$0216,$01F8
;		DW $01DF,$01C2,$01AC,$0190,$017B,$0165,$0151,$013E,$012C,$011C,$010A,$00FC
;		DW $00EF,$00E1,$00D6,$00C8,$00BD,$00B2,$00A8,$009F,$0096,$008E,$0085,$007E
;		DW $0077,$0070,$006B,$0064,$005E,$0059,$0054,$004F,$004B,$0047,$0042,$003F
;		DW $003B,$0038,$0035,$0032,$002F,$002C,$002A,$0027,$0025,$0023,$0021,$001F
;		DW $001D,$001C,$001A,$0019,$0017,$0016,$0015,$0013,$0012,$0011,$0010,$000F
		

;TABLA3:	;dw $0CDA,$0C22,$0B73,$0ACF,$0A33,$09A1,$0917,$0894,$0819,$07A4,$0737,$06CF,
;		dw $066D,$0611,$05BA,$0567,$051A,$04D0,$048B,$044A,$040C,$03D2,$039B,$0367
;		dw $0337,$0308,$02DD,$02B4,$028D,$0268,$0246,$0225,$0206,$01E9,$01CE,$01B4
;		dw $019B,$0184,$016E,$015A,$0146,$0134,$0123,$0112,$0103,$00F5,$00E7,$00DA
;		dw $00CE,$00C2,$00B7,$00AD,$00A3,$009A,$0091,$0089,$0082,$007A,$0073,$006D
;		dw $0067,$0061,$005C,$0056,$0052,$004D,$0049,$0045,$0041,$003D,$003A,$0036
;		dw $0033,$0031,$002E,$002B,$0029,$0027,$0024,$0022,$0020,$001F,$001D,$001B
;		dw $001A,$0018,$0017,$0016,$0014,$0013,$0012,$0011,$0010,$000F,$000E,$000D


		;DW	$688,$62A,$5D2,$57E,$52F,$4E5,$49E,$45C,
		DEFW	$41D,$3E2,$3AA,$376,$344,$315,$2E9,$2BF,$297,$272,$24F,$22E,$20E,$1F1,$1D5,$1BB
		DEFW	$1A2,$18A,$174,$15F,$14B,$139,$127,$117,$107,$F8,$EA,$DD
		DEFW	$D1,$C5,$BA,$AF,$A5,$9C,$93,$8B,$83,$7C,$75,$6E
		DEFW	$68,$62,$5D,$57,$52,$4E,$49,$45,$41,$3E,$3A,$37
		DEFW	$34,$31,$2E,$2B,$29,$27,$24,$22,$20,$1F,$1D,$1B
		DEFW	$1A,$18,$17,$15,$14,$13,$12,$11,$10,$F,$E,$D




.SONG_0
		;INCBIN	"WYAZOW.MUS"



; VARIABLES__________________________


.INTERR
         DEFB    00               ;INTERRUPTORES 1=ON 0=OFF
                                        ;BIT 0=CARGA CANCION ON/OFF
                                        ;BIT 1=PLAYER ON/OFF
                                        ;BIT 2=SONIDOS ON/OFF
                                        ;BIT 3=EFECTOS ON/OFF

;MUSICA **** EL ORDEN DE LAS VARIABLES ES FIJO ******

.TABLA_SONG0 defw 0
.TABLA_EFECTOS0 defw 0

DefB      'P','S','G',' ','P','R','O','P','L','A','Y','E','R',' ','B','Y',' ','W','Y','Z','-','1','0'


.SONG           DEFB    00               ;DBNº DE CANCION
.TEMPO          DEFB    00               ;DEFBTEMPO
.TTEMPO         DEFB    00               ;DEFBCONTADOR TEMPO
.PUNTERO_A      DEFW   00               ;DW PUNTERO DEL CANAL A
.PUNTERO_B      DEFW   00               ;DW PUNTERO DEL CANAL B
.PUNTERO_C      DEFW   00               ;DW PUNTERO DEL CANAL C

.BUFFER_MUSICA
.CANAL_A        DEFW   BUFFER_DEC       ;DW DIRECION DE INICIO DE LA MUSICA A
.CANAL_B        DEFW   00               ;DW DIRECION DE INICIO DE LA MUSICA B
.CANAL_C        DEFW   00               ;DW DIRECION DE INICIO DE LA MUSICA C

.PUNTERO_P_A    DEFW   00               ;DW PUNTERO PAUTA CANAL A
.PUNTERO_P_B    DEFW   00               ;DW PUNTERO PAUTA CANAL B
.PUNTERO_P_C    DEFW   00               ;DW PUNTERO PAUTA CANAL C

.PUNTERO_P_A0   DEFW   00               ;DW INI PUNTERO PAUTA CANAL A
.PUNTERO_P_B0   DEFW   00               ;DW INI PUNTERO PAUTA CANAL B
.PUNTERO_P_C0   DEFW   00               ;DW INI PUNTERO PAUTA CANAL C


.PUNTERO_P_DECA	DEFW   00		;DW PUNTERO DE INICIO DEL DECODER CANAL A
.PUNTERO_P_DECB	DEFW   00		;DW PUNTERO DE INICIO DEL DECODER CANAL B
.PUNTERO_P_DECC	DEFW   00		;DW PUNTERO DE INICIO DEL DECODER CANAL C

.PUNTERO_DECA	DEFW   00		;DW PUNTERO DECODER CANAL A
.PUNTERO_DECB	DEFW   00		;DW PUNTERO DECODER CANAL B
.PUNTERO_DECC	DEFW   00		;DW PUNTERO DECODER CANAL C       


;CANAL DE EFECTOS - ENMASCARA OTRO CANAL

.PUNTERO_P       DEFW   00           	;DW PUNTERO DEL CANAL EFECTOS
.CANAL_P         DEFW   00           	;DW DIRECION DE INICIO DE LOS EFECTOS
.PUNTERO_P_DECP 	DEFW   00		;DW PUNTERO DE INICIO DEL DECODER CANAL P
.PUNTERO_DECP 	DEFW   00		;DW PUNTERO DECODER CANAL P

.PSG_REG         DEFB     00,00,00,00,00,00,00,@10111000 ,00,00,00,00,00,00,00    ;DEFB(11) BUFFER DE REGISTROS DEL PSG
.PSG_REG_SEC     DEFB     00,00,00,00,00,00,00,@10111000 ,00,00,00,00,00,00,00    ;DEFB(11) BUFFER SECUNDARIO DE REGISTROS DEL PSG



;ENVOLVENTE_A    EQU     $D033           ;DB
;ENVOLVENTE_B    EQU     $D034           ;DB
;ENVOLVENTE_C    EQU     $D035           ;DB




;EFECTOS DE SONIDO

.N_SONIDO       defb      0               ;defb  NUMERO DE SONIDO
.PUNTERO_SONIDO defw      0               ;defw  PUNTERO DEL SONIDO QUE SE REPRODUCE

;EFECTOS

.N_EFECTO       defb      0               ;defb  NUMERO DE SONIDO
;PUNTERO_EFECTO defw      0               ;defw  PUNTERO DEL SONIDO QUE SE REPRODUCE
				    
.PUNTERO_EFECTO_A 	defw      0             ;DW : PUNTERO DEL SONIDO QUE SE REPRODUCE
.PUNTERO_EFECTO_B 	defw      0             ;DW : PUNTERO DEL SONIDO QUE SE REPRODUCE
.PUNTERO_EFECTO_C 	defw      0             ;DW : PUNTERO DEL SONIDO QUE SE REPRODUCE











.BUFFER_DEC   defs $40
;   DEFB     $00		;************************* mucha atencion!!!!
;.BUFFER_DEC defs 2048		;space dinamically asigned in source code compilation!!
					; aqui se decodifica la cancion hay que dejar suficiente espacio libre.
					;*************************
                
DEFC CARGA_CANCION_WYZ = CARGA_CANCION_WYZ0
DEFC INICIA_EFECTO_WYZ = INICIA_EFECTO_WYZ0
DEFC cpc_WyzSetPlayerOn0 = cpc_WyzSetPlayerOn1
DEFC cpc_WyzSetPlayerOff0 = cpc_WyzSetPlayerOff1
DEFC TABLA_SONG = TABLA_SONG0
DEFC TABLA_EFECTOS = TABLA_EFECTOS0
DEFC TABLA_PAUTAS = TABLA_PAUTAS0
DEFC TABLA_SONIDOS = TABLA_SONIDOS0
DEFC INTERRUPCION = INTERR                
DEFC direcc_tempo = dir_tempo
