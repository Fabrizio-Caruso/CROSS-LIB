; ******************************************************
; **       Librería de rutinas para Amstrad CPC       **
; **	   Raúl Simarro, 	  Artaburu 2009       **
; ******************************************************

GLOBAL cpc_ScanKeyboard
GLOBAL cpc_KeysData
GLOBAL  keymap


.cpc_ScanKeyboard

        di              ;1 $$%%$$ C P C   VERSION $$%%$$   FROM CPCWIKI
        ld hl,cpc_KeysData+keymap    ;3
        ld bc,$f782     ;3
        out (c),c       ;4
        ld bc,$f40e     ;3
        ld e,b          ;1
        out (c),c       ;4
        ld bc,$f6c0     ;3
        ld d,b          ;1
        out (c),c       ;4
        ld c,0          ;2
        out (c),c       ;4
        ld bc,$f792     ;3
        out (c),c       ;4
        ld a,$40        ;2
        ld c,$4a        ;2 44
.loop    ld b,d          ;1
        out (c),a       ;4 select line
        ld b,e          ;1
        ini             ;5 read bits and write into KEYMAP
        inc a           ;1
        cp c            ;1
        jr c,loop       ;2/3 9*16+1*15=159
        ld bc,$f782     ;3
        out (c),c       ;4
        ei              ;1 8 =211 microseconds
        ret

        
        




		
			   		

	   

	   
