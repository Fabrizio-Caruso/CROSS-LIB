
SECTION rodata_user
PUBLIC _udgs

_udgs:

; Paste in the contents of lower.asm here


	defb	2+4	;256 char encoding, compressed
	defb	128	;Number of tiles

; Character: ? (00)
	defb	@00000000
	defb	@00000000
	defb	@00000000
	defb	@00000000
	defb	@00000000
	defb	@00000000
	defb	@00000000
	defb	@00000000

; Character: ? (01)
        defb    0b00011000      ;    oo
        defb    0b00100100      ;   o  o
        defb    0b01000010      ;  o    o
        defb    0b10000001      ; o      o
        defb    0b11100111      ; ooo  ooo
        defb    0b00100100      ;   o  o
        defb    0b00100100      ;   o  o
        defb    0b00111100      ;   oooo

; Character: ? (02)
        defb    0b00111100      ;   oooo
        defb    0b00100100      ;   o  o
        defb    0b00100100      ;   o  o
        defb    0b11100111      ; ooo  ooo
        defb    0b10000001      ; o      o
        defb    0b01000010      ;  o    o
        defb    0b00100100      ;   o  o
        defb    0b00011000      ;    oo

; Character: ? (03)
        defb    0b00011000      ;    oo
        defb    0b00010100      ;    o o
        defb    0b11110010      ; oooo  o
        defb    0b10000001      ; o      o
        defb    0b10000001      ; o      o
        defb    0b11110010      ; oooo  o
        defb    0b00010100      ;    o o
        defb    0b00011000      ;    oo

; Character: ? (04)
        defb    0b00011000      ;    oo
        defb    0b00101000      ;   o o
        defb    0b01001111      ;  o  oooo
        defb    0b10000001      ; o      o
        defb    0b10000001      ; o      o
        defb    0b01001111      ;  o  oooo
        defb    0b00101000      ;   o o
        defb    0b00011000      ;    oo

; Character: ? (05)
	defb	@11110000
	defb	@11110000
	defb	@11110000
	defb	@11110000
	defb	@00000000
	defb	@00000000
	defb	@00000000
	defb	@00000000

; Character: ? (06)
	defb	@00001111
	defb	@00001111
	defb	@00001111
	defb	@00001111
	defb	@00000000
	defb	@00000000
	defb	@00000000
	defb	@00000000

; Character: ? (07)
	defb	@11111111
	defb	@11111111
	defb	@11111111
	defb	@11111111
	defb	@00000000
	defb	@00000000
	defb	@00000000
	defb	@00000000
; Character: ? (08)
	defb	@00000000
	defb	@00000000
	defb	@00000000
	defb	@00000000
	defb	@11110000
	defb	@11110000
	defb	@11110000
	defb	@11110000
; Character: ? (09)
	defb	@11110000
	defb	@11110000
	defb	@11110000
	defb	@11110000
	defb	@11110000
	defb	@11110000
	defb	@11110000
	defb	@11110000
; Character: ? (0a)
	defb	@00001111
	defb	@00001111
	defb	@00001111
	defb	@00001111
	defb	@11110000
	defb	@11110000
	defb	@11110000
	defb	@11110000
; Character: ? (0b)
	defb	@11111111
	defb	@11111111
	defb	@11111111
	defb	@11111111
	defb	@11110000
	defb	@11110000
	defb	@11110000
	defb	@11110000
; Character: ? (0c)
	defb	@00000000
	defb	@00000000
	defb	@00000000
	defb	@00000000
	defb	@00001111
	defb	@00001111
	defb	@00001111
	defb	@00001111

; Character: ? (0D)
        defb    0b01110000      ;  ooo
        defb    0b11001000      ; oo  o
        defb    0b11011110      ; oo oooo
        defb    0b11011011      ; oo oo oo
        defb    0b11011011      ; oo oo oo
        defb    0b01111110      ;  oooooo
        defb    0b00011011      ;    oo oo
        defb    0b00011011      ;    oo oo

; Character: ? (0E)
        defb    0b00000000      ;
        defb    0b00000000      ;
        defb    0b00000000      ;
        defb    0b11111111      ; oooooooo
        defb    0b11111111      ; oooooooo
        defb    0b11111111      ; oooooooo
        defb    0b00000000      ;
        defb    0b00000000      ;

; Character: ? (0F)
        defb    0b00011100      ;    ooo
        defb    0b00011100      ;    ooo
        defb    0b00011100      ;    ooo
        defb    0b00011100      ;    ooo
        defb    0b00011100      ;    ooo
        defb    0b00011100      ;    ooo
        defb    0b00011100      ;    ooo
        defb    0b00011100      ;    ooo

; Character: ? (10)
	defb	@11110000
	defb	@11110000
	defb	@11110000
	defb	@11110000
	defb	@00001111
	defb	@00001111
	defb	@00001111
	defb	@00001111
; Character: ? (11)
	defb	@00001111
	defb	@00001111
	defb	@00001111
	defb	@00001111
	defb	@00001111
	defb	@00001111
	defb	@00001111
	defb	@00001111
; Character: ? (12)
	defb	@11111111
	defb	@11111111
	defb	@11111111
	defb	@11111111
	defb	@00001111
	defb	@00001111
	defb	@00001111
	defb	@00001111
; Character: ? (13)
	defb	@00000000
	defb	@00000000
	defb	@00000000
	defb	@00000000
	defb	@11111111
	defb	@11111111
	defb	@11111111
	defb	@11111111
; Character: ? (14)
	defb	@11110000
	defb	@11110000
	defb	@11110000
	defb	@11110000
	defb	@11111111
	defb	@11111111
	defb	@11111111
	defb	@11111111
; Character: ? (15)
	defb	@00001111
	defb	@00001111
	defb	@00001111
	defb	@00001111
	defb	@11111111
	defb	@11111111
	defb	@11111111
	defb	@11111111
; Character: ? (16)
	defb	@11111111
	defb	@11111111
	defb	@11111111
	defb	@11111111
	defb	@11111111
	defb	@11111111
	defb	@11111111
	defb	@11111111
; Character: ? (17)
	defb	@10101010
	defb	@01010101
	defb	@10101010
	defb	@01010101
	defb	@10101010
	defb	@01010101
	defb	@10101010
	defb	@01010101
; Character: ? (18)
	defb	@10101010
	defb	@01010101
	defb	@10101010
	defb	@01010101
	defb	@00000000
	defb	@00000000
	defb	@00000000
	defb	@00000000
; Character: ? (19)
	defb	@00000000
	defb	@00000000
	defb	@00000000
	defb	@00000000
	defb	@10101010
	defb	@01010101
	defb	@10101010
	defb	@01010101
; Character: ? (1A)
	defb	@00001010
	defb	@00000101
	defb	@00001010
	defb	@00000101
	defb	@00001010
	defb	@00000101
	defb	@00001010
	defb	@00000101
; Character: ? (1B)
	defb	@10100000
	defb	@01010000
	defb	@10100000
	defb	@01010000
	defb	@10100000
	defb	@01010000
	defb	@10100000
	defb	@01010000

; Character: ? (1C)
        defb    0b00000000      ;
        defb    0b00000000      ;
        defb    0b00000000      ;
        defb    0b00011111      ;    ooooo
        defb    0b00011111      ;    ooooo
        defb    0b00011111      ;    ooooo
        defb    0b00011100      ;    ooo
        defb    0b00011100      ;    ooo

; Character: ? (1D)
        defb    0b00000000      ;
        defb    0b00000000      ;
        defb    0b00000000      ;
        defb    0b11111100      ; oooooo
        defb    0b11111100      ; oooooo
        defb    0b11111100      ; oooooo
        defb    0b00011100      ;    ooo
        defb    0b00011100      ;    ooo


; Character: ? (1E)
        defb    0b00011100      ;    ooo
        defb    0b00011100      ;    ooo
        defb    0b00011100      ;    ooo
        defb    0b00011111      ;    ooooo
        defb    0b00011111      ;    ooooo
        defb    0b00011111      ;    ooooo
        defb    0b00000000      ;
        defb    0b00000000      ;

; Character: ? (1F)
        defb    0b00011100      ;    ooo
        defb    0b00011100      ;    ooo
        defb    0b00011100      ;    ooo
        defb    0b11111100      ; oooooo
        defb    0b11111100      ; oooooo
        defb    0b11111100      ; oooooo
        defb    0b00000000      ;
        defb    0b00000000      ;



; Include your usual font file here


	defb %"--------"
	defb %"--------"
	defb %"--------"
	defb %"--------"
	defb %"--------"
	defb %"--------"
	defb %"--------"
	defb %"--------"

; invincibility
defb 0,0,84,124,108,56,56,189
	 ; defb    @00011000
	 ; defb    @00100100
	 ; defb    @00000000
	 ; defb    @01100110
	 ; defb    @10011001
	 ; defb    @00000000
	 ; defb    @00100100
	 ; defb    @01100110

; left missile
defb 0,0,0,0,0,0,255,0
	 ; defb    @00000000
	 ; defb    @00000000
	 ; defb    @11110000
	 ; defb    @01111111
	 ; defb    @01111111
	 ; defb    @11110000
	 ; defb    @00000000
	 ; defb    @00000000

; bullet
defb 21,31,27,31,14,14,14,94
	 ; defb    @00000000
	 ; defb    @00000000
	 ; defb    @00001000
	 ; defb    @00111000
	 ; defb    @00011100
	 ; defb    @00010000
	 ; defb    @00000000
	 ; defb    @00000000

; dollar
defb 0,40,68,82,5,10,21,10
	;defb %"--------"
	;defb %"----#---"
	;defb %"--#####-"
	;defb %"--#-#---"
	;defb %"--#####-"
	;defb %"----#-#-"
	;defb %"--#####-"
	;defb %"----#---"

; power-up F
defb 85,42,21,10,21,10,21,40 
	 ; defb    @00000000
	 ; defb    @00111100
	 ; defb    @01000010
	 ; defb    @11001111
	 ; defb    @11000011
	 ; defb    @01100110
	 ; defb    @00100100
	 ; defb    @00011000

; vertical brick
defb 255,255,255,255,170,255,255,0
	 ; defb    @00011000
	 ; defb    @00011000
	 ; defb    @01111000
	 ; defb    @00011000
	 ; defb    @00011000
	 ; defb    @00011110
	 ; defb    @00011000
	 ; defb    @00011000

; skull
defb 0,28,54,127,240,240,248,248
	 ; defb    @00111100
	 ; defb    @01000010
	 ; defb    @10100101
	 ; defb    @10000001
	 ; defb    @01011010
	 ; defb    @00100100
	 ; defb    @00100100
	 ; defb    @00111100 	 

; ghost
	 defb    255,63,63,31,31,15,31,61
	 ; defb    @10111101
	 ; defb    @01000010
	 ; defb    @10100101
	 ; defb    @10000001
	 ; defb    @10000001
	 ; defb    @10011001
	 ; defb    @10000001
	 ; defb    @01111110 

; bomb
defb 176,176,152,216,220,252,248,224
	 ; defb    @00111100
	 ; defb    @01000010
	 ; defb    @10100101
	 ; defb    @10011001
	 ; defb    @10011001
	 ; defb    @10100101
	 ; defb    @01000010
	 ; defb    @00111100

; player down
defb 60,66,165,129,153,165,66,60
	 ; defb    @00011000
	 ; defb    @00100100
	 ; defb    @00011000
	 ; defb    @01100110
	 ; defb    @10011001
	 ; defb    @00011000
	 ; defb    @00100100
	 ; defb    @01100110

; horizontal brick
defb 254,254,254,254,170,254,254,0
	 ; defb    @00000000
	 ; defb    @00000000
	 ; defb    @00000000
	 ; defb    @11111111
	 ; defb    @11111111
	 ; defb    @00000000
	 ; defb    @00000000
	 ; defb    @00000000

; power-up S
defb 13,13,25,27,59,63,31,7
	 ; defb    @00000000
	 ; defb    @00111100
	 ; defb    @01100110
	 ; defb    @11011111
	 ; defb    @11100111
	 ; defb    @01111010
	 ; defb    @00100100
	 ; defb    @00011000

defb 128,144,0,144,72,164,80,160
	; defb %"--------"
	; defb %"--------"
	; defb %"--------"
	; defb %"--------"
	; defb %"--#####-"
	; defb %"--------"
	; defb %"--------"
	; defb %"--------"

; right missile
defb 0,0,0,0,1,2,4,16
	 ; defb    @00000000
	 ; defb    @00000000
	 ; defb    @00001111
	 ; defb    @11111110
	 ; defb    @11111110
	 ; defb    @00001111
	 ; defb    @00000000
	 ; defb    @00000000

; rocket
defb 0,0,16,8,4,2,1,0
	 ; defb    @00011000
	 ; defb    @00111100
	 ; defb    @00111100
	 ; defb    @00111100
	 ; defb    @01111110
	 ; defb    @01111110
	 ; defb    @01011010
	 ; defb    @01000010

	defb %"--------"
	defb %"--####--"
	defb %"-#---##-"
	defb %"-#--#-#-"
	defb %"-#-#--#-"
	defb %"-##---#-"
	defb %"--####--"
	defb %"--------"

	defb %"--------"
	defb %"---##---"
	defb %"--#-#---"
	defb %"----#---"
	defb %"----#---"
	defb %"----#---"
	defb %"--#####-"
	defb %"--------"

	defb %"--------"
	defb %"--####--"
	defb %"-#----#-"
	defb %"------#-"
	defb %"--####--"
	defb %"-#------"
	defb %"-######-"
	defb %"--------"

	defb %"--------"
	defb %"--####--"
	defb %"-#----#-"
	defb %"----##--"
	defb %"------#-"
	defb %"-#----#-"
	defb %"--####--"
	defb %"--------"

	defb %"--------"
	defb %"----#---"
	defb %"---##---"
	defb %"--#-#---"
	defb %"-#--#---"
	defb %"-######-"
	defb %"----#---"
	defb %"--------"

	defb %"--------"
	defb %"-######-"
	defb %"-#------"
	defb %"-#####--"
	defb %"------#-"
	defb %"-#----#-"
	defb %"--####--"
	defb %"--------"

	defb %"--------"
	defb %"--####--"
	defb %"-#------"
	defb %"-#####--"
	defb %"-#----#-"
	defb %"-#----#-"
	defb %"--####--"
	defb %"--------"

	defb %"--------"
	defb %"-######-"
	defb %"------#-"
	defb %"-----#--"
	defb %"----#---"
	defb %"---#----"
	defb %"---#----"
	defb %"--------"

	defb %"--------"
	defb %"--####--"
	defb %"-#----#-"
	defb %"--####--"
	defb %"-#----#-"
	defb %"-#----#-"
	defb %"--####--"
	defb %"--------"

	defb %"--------"
	defb %"--####--"
	defb %"-#----#-"
	defb %"-#----#-"
	defb %"--#####-"
	defb %"------#-"
	defb %"--####--"
	defb %"--------"

defb 0,24,60,90,24,36,0,0

; power-up H
defb 24,126,90,219,24,36,66,195 
	 ; defb    @00000000
	 ; defb    @00111100
	 ; defb    @01011010
	 ; defb    @11011011
	 ; defb    @11000011
	 ; defb    @01011010
	 ; defb    @00111100
	 ; defb    @00011000

; player up
defb 170,85,170,85,170,85,170,85
	 ; defb    @00011000
	 ; defb    @00111100
	 ; defb    @00011000
	 ; defb    @01100110
	 ; defb    @10011001
	 ; defb    @00011000
	 ; defb    @00100100
	 ; defb    @01100110

; player right
defb 0,56,108,254,15,15,31,31
	 ; defb    @00011000
	 ; defb    @00110100
	 ; defb    @00011001
	 ; defb    @01101111
	 ; defb    @10011000
	 ; defb    @00011000
	 ; defb    @00010100
	 ; defb    @00010100

; player left
defb 0,0,0,0,1,6,12,152
	 ; defb    @00011000
	 ; defb    @00101100
	 ; defb    @10011000
	 ; defb    @11101110
	 ; defb    @00011001
	 ; defb    @00011000
	 ; defb    @00101000
	 ; defb    @00101000

; gun
    defb 255,252,252,248,248,240,248,188

	 ; defb    @00000000
	 ; defb    @10000000
	 ; defb    @01111110
	 ; defb    @11001000
	 ; defb    @11111000
	 ; defb    @11000000
	 ; defb    @10000000	 
	 ; defb    @00000000

defb 0,0,0,24,24,0,0,0 

	defb %"--------"
	defb %"--####--"
	defb %"-#----#-"
	defb %"-#----#-"
	defb %"-######-"
	defb %"-#----#-"
	defb %"-#----#-"
	defb %"--------"

	defb %"--------"
	defb %"-#####--"
	defb %"-#----#-"
	defb %"-#####--"
	defb %"-#----#-"
	defb %"-#----#-"
	defb %"-#####--"
	defb %"--------"

	defb %"--------"
	defb %"--####--"
	defb %"-#----#-"
	defb %"-#------"
	defb %"-#------"
	defb %"-#----#-"
	defb %"--####--"
	defb %"--------"

	defb %"--------"
	defb %"-####---"
	defb %"-#---#--"
	defb %"-#----#-"
	defb %"-#----#-"
	defb %"-#---#--"
	defb %"-####---"
	defb %"--------"

	defb %"--------"
	defb %"-######-"
	defb %"-#------"
	defb %"-#####--"
	defb %"-#------"
	defb %"-#------"
	defb %"-######-"
	defb %"--------"

	defb %"--------"
	defb %"-######-"
	defb %"-#------"
	defb %"-#####--"
	defb %"-#------"
	defb %"-#------"
	defb %"-#------"
	defb %"--------"

	defb %"--------"
	defb %"--####--"
	defb %"-#----#-"
	defb %"-#------"
	defb %"-#--###-"
	defb %"-#----#-"
	defb %"--####--"
	defb %"--------"

	defb %"--------"
	defb %"-#----#-"
	defb %"-#----#-"
	defb %"-######-"
	defb %"-#----#-"
	defb %"-#----#-"
	defb %"-#----#-"
	defb %"--------"

	defb %"--------"
	defb %"--#####-"
	defb %"----#---"
	defb %"----#---"
	defb %"----#---"
	defb %"----#---"
	defb %"--#####-"
	defb %"--------"

	defb %"--------"
	defb %"------#-"
	defb %"------#-"
	defb %"------#-"
	defb %"-#----#-"
	defb %"-#----#-"
	defb %"--####--"
	defb %"--------"

	defb %"--------"
	defb %"-#---#--"
	defb %"-#--#---"
	defb %"-###----"
	defb %"-#--#---"
	defb %"-#---#--"
	defb %"-#----#-"
	defb %"--------"

	defb %"--------"
	defb %"-#------"
	defb %"-#------"
	defb %"-#------"
	defb %"-#------"
	defb %"-#------"
	defb %"-######-"
	defb %"--------"

	defb %"--------"
	defb %"-#----#-"
	defb %"-##--##-"
	defb %"-#-##-#-"
	defb %"-#----#-"
	defb %"-#----#-"
	defb %"-#----#-"
	defb %"--------"

	defb %"--------"
	defb %"-#----#-"
	defb %"-##---#-"
	defb %"-#-#--#-"
	defb %"-#--#-#-"
	defb %"-#---##-"
	defb %"-#----#-"
	defb %"--------"

	defb %"--------"
	defb %"--####--"
	defb %"-#----#-"
	defb %"-#----#-"
	defb %"-#----#-"
	defb %"-#----#-"
	defb %"--####--"
	defb %"--------"

	defb %"--------"
	defb %"-#####--"
	defb %"-#----#-"
	defb %"-#----#-"
	defb %"-#####--"
	defb %"-#------"
	defb %"-#------"
	defb %"--------"

	defb %"--------"
	defb %"--####--"
	defb %"-#----#-"
	defb %"-#----#-"
	defb %"-#-#--#-"
	defb %"-#--#-#-"
	defb %"--####--"
	defb %"--------"

	defb %"--------"
	defb %"-#####--"
	defb %"-#----#-"
	defb %"-#----#-"
	defb %"-#####--"
	defb %"-#---#--"
	defb %"-#----#-"
	defb %"--------"

	defb %"--------"
	defb %"--####--"
	defb %"-#------"
	defb %"--####--"
	defb %"------#-"
	defb %"-#----#-"
	defb %"--####--"
	defb %"--------"

	defb %"--------"
	defb %"#######-"
	defb %"---#----"
	defb %"---#----"
	defb %"---#----"
	defb %"---#----"
	defb %"---#----"
	defb %"--------"

	defb %"--------"
	defb %"-#----#-"
	defb %"-#----#-"
	defb %"-#----#-"
	defb %"-#----#-"
	defb %"-#----#-"
	defb %"--####--"
	defb %"--------"

	defb %"--------"
	defb %"-#----#-"
	defb %"-#----#-"
	defb %"-#----#-"
	defb %"-#----#-"
	defb %"--#--#--"
	defb %"---##---"
	defb %"--------"

	defb %"--------"
	defb %"-#----#-"
	defb %"-#----#-"
	defb %"-#----#-"
	defb %"-#----#-"
	defb %"-#-##-#-"
	defb %"--#--#--"
	defb %"--------"

	defb %"--------"
	defb %"-#----#-"
	defb %"--#--#--"
	defb %"---##---"
	defb %"---##---"
	defb %"--#--#--"
	defb %"-#----#-"
	defb %"--------"

	defb %"--------"
	defb %"#-----#-"
	defb %"-#---#--"
	defb %"--#-#---"
	defb %"---#----"
	defb %"---#----"
	defb %"---#----"
	defb %"--------"

	defb %"--------"
	defb %"-######-"
	defb %"-----#--"
	defb %"----#---"
	defb %"---#----"
	defb %"--#-----"
	defb %"-######-"
	defb %"--------"

; dead ghost
defb 0,0,0,0,128,96,48,25
	 ; defb    @10111101
	 ; defb    @01000010
	 ; defb    @10100101
	 ; defb    @10100101
	 ; defb    @10000001
	 ; defb    @10011001
	 ; defb    @10000001
	 ; defb    @01111110 

defb 16,0,2,72,2,21,75,21 

defb 130,16,64,162,72,160,90,224 

defb 7,42,133,18,5,32,4,0 

defb 168,210,168,64,18,64,8,32 

	defb %"--------"
	defb %"---###--"
	defb %"--#---#-"
	defb %"-####---"
	defb %"--#-----"
	defb %"--#-----"
	defb %"-######-"
	defb %"--------"

	defb %"--------"
	defb %"--------"
	defb %"--###---"
	defb %"-----#--"
	defb %"--####--"
	defb %"-#---#--"
	defb %"--####--"
	defb %"--------"

	defb %"--------"
	defb %"--#-----"
	defb %"--#-----"
	defb %"--####--"
	defb %"--#---#-"
	defb %"--#---#-"
	defb %"--####--"
	defb %"--------"

	defb %"--------"
	defb %"--------"
	defb %"---###--"
	defb %"--#-----"
	defb %"--#-----"
	defb %"--#-----"
	defb %"---###--"
	defb %"--------"

	defb %"--------"
	defb %"-----#--"
	defb %"-----#--"
	defb %"--####--"
	defb %"-#---#--"
	defb %"-#---#--"
	defb %"--####--"
	defb %"--------"

	defb %"--------"
	defb %"--------"
	defb %"--###---"
	defb %"-#---#--"
	defb %"-####---"
	defb %"-#------"
	defb %"--####--"
	defb %"--------"

	defb %"--------"
	defb %"----##--"
	defb %"---#----"
	defb %"---##---"
	defb %"---#----"
	defb %"---#----"
	defb %"---#----"
	defb %"--------"

	defb %"--------"
	defb %"--------"
	defb %"--####--"
	defb %"-#---#--"
	defb %"-#---#--"
	defb %"--####--"
	defb %"-----#--"
	defb %"--###---"

	defb %"--------"
	defb %"-#------"
	defb %"-#------"
	defb %"-####---"
	defb %"-#---#--"
	defb %"-#---#--"
	defb %"-#---#--"
	defb %"--------"

	defb %"--------"
	defb %"---#----"
	defb %"--------"
	defb %"--##----"
	defb %"---#----"
	defb %"---#----"
	defb %"--###---"
	defb %"--------"

	defb %"--------"
	defb %"-----#--"
	defb %"--------"
	defb %"-----#--"
	defb %"-----#--"
	defb %"-----#--"
	defb %"--#--#--"
	defb %"---##---"

	defb %"--------"
	defb %"--#-----"
	defb %"--#-#---"
	defb %"--##----"
	defb %"--##----"
	defb %"--#-#---"
	defb %"--#--#--"
	defb %"--------"

	defb %"--------"
	defb %"---#----"
	defb %"---#----"
	defb %"---#----"
	defb %"---#----"
	defb %"---#----"
	defb %"----##--"
	defb %"--------"

	defb %"--------"
	defb %"--------"
	defb %"-##-#---"
	defb %"-#-#-#--"
	defb %"-#-#-#--"
	defb %"-#-#-#--"
	defb %"-#-#-#--"
	defb %"--------"

	defb %"--------"
	defb %"--------"
	defb %"-####---"
	defb %"-#---#--"
	defb %"-#---#--"
	defb %"-#---#--"
	defb %"-#---#--"
	defb %"--------"

	defb %"--------"
	defb %"--------"
	defb %"--###---"
	defb %"-#---#--"
	defb %"-#---#--"
	defb %"-#---#--"
	defb %"--###---"
	defb %"--------"

	defb %"--------"
	defb %"--------"
	defb %"-####---"
	defb %"-#---#--"
	defb %"-#---#--"
	defb %"-####---"
	defb %"-#------"
	defb %"-#------"

	defb %"--------"
	defb %"--------"
	defb %"--####--"
	defb %"-#---#--"
	defb %"-#---#--"
	defb %"--####--"
	defb %"-----#--"
	defb %"-----##-"

	defb %"--------"
	defb %"--------"
	defb %"---###--"
	defb %"--#-----"
	defb %"--#-----"
	defb %"--#-----"
	defb %"--#-----"
	defb %"--------"

	defb %"--------"
	defb %"--------"
	defb %"--###---"
	defb %"-#------"
	defb %"--###---"
	defb %"-----#--"
	defb %"-####---"
	defb %"--------"

	defb %"--------"
	defb %"---#----"
	defb %"--###---"
	defb %"---#----"
	defb %"---#----"
	defb %"---#----"
	defb %"----##--"
	defb %"--------"

	defb %"--------"
	defb %"--------"
	defb %"-#---#--"
	defb %"-#---#--"
	defb %"-#---#--"
	defb %"-#---#--"
	defb %"--###---"
	defb %"--------"

	defb %"--------"
	defb %"--------"
	defb %"-#---#--"
	defb %"-#---#--"
	defb %"--#-#---"
	defb %"--#-#---"
	defb %"---#----"
	defb %"--------"

	defb %"--------"
	defb %"--------"
	defb %"-#---#--"
	defb %"-#-#-#--"
	defb %"-#-#-#--"
	defb %"-#-#-#--"
	defb %"--#-#---"
	defb %"--------"

	defb %"--------"
	defb %"--------"
	defb %"-#---#--"
	defb %"--#-#---"
	defb %"---#----"
	defb %"--#-#---"
	defb %"-#---#--"
	defb %"--------"

	defb %"--------"
	defb %"--------"
	defb %"-#---#--"
	defb %"-#---#--"
	defb %"-#---#--"
	defb %"--####--"
	defb %"-----#--"
	defb %"--###---"

	defb %"--------"
	defb %"--------"
	defb %"-#####--"
	defb %"----#---"
	defb %"---#----"
	defb %"--#-----"
	defb %"-#####--"
	defb %"--------"

	defb %"--------"
	defb %"----###-"
	defb %"----#---"
	defb %"--##----"
	defb %"----#---"
	defb %"----#---"
	defb %"----###-"
	defb %"--------"

	defb %"--------"
	defb %"----#---"
	defb %"----#---"
	defb %"----#---"
	defb %"----#---"
	defb %"----#---"
	defb %"----#---"
	defb %"--------"

	defb %"--------"
	defb %"-###----"
	defb %"---#----"
	defb %"----##--"
	defb %"---#----"
	defb %"---#----"
	defb %"-###----"
	defb %"--------"

	defb %"--------"
	defb %"---#-#--"
	defb %"--#-#---"
	defb %"--------"
	defb %"--------"
	defb %"--------"
	defb %"--------"
	defb %"--------"

	defb %"--####--"
	defb %"-#----#-"
	defb %"#--##--#"
	defb %"#-#----#"
	defb %"#-#----#"
	defb %"#--##--#"
	defb %"-#----#-"
	defb %"--####--"

_udgs_end:

