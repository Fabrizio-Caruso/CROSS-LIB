
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
defb 0,0,0,192,32,80,144,0
	 ; defb    @00011000
	 ; defb    @00100100
	 ; defb    @00000000
	 ; defb    @01100110
	 ; defb    @10011001
	 ; defb    @00000000
	 ; defb    @00100100
	 ; defb    @01100110

; left missile
defb $2A,$2A,$2A,$3A,$2A,$2A,$2A,$00
	 ; defb    @00000000
	 ; defb    @00000000
	 ; defb    @11110000
	 ; defb    @01111111
	 ; defb    @01111111
	 ; defb    @11110000
	 ; defb    @00000000
	 ; defb    @00000000

; bullet
defb 0,0,0,3,4,10,9,0
	 ; defb    @00000000
	 ; defb    @00000000
	 ; defb    @00001000
	 ; defb    @00111000
	 ; defb    @00011100
	 ; defb    @00010000
	 ; defb    @00000000
	 ; defb    @00000000

; dollar
defb $18,$3e,$60,$3c,$06,$7c,$18,$00
	;defb %"--------"
	;defb %"----#---"
	;defb %"--#####-"
	;defb %"--#-#---"
	;defb %"--#####-"
	;defb %"----#-#-"
	;defb %"--#####-"
	;defb %"----#---"

; power-up F
defb 2,7,2,2,2,2,2,5 
	 ; defb    @00000000
	 ; defb    @00111100
	 ; defb    @01000010
	 ; defb    @11001111
	 ; defb    @11000011
	 ; defb    @01100110
	 ; defb    @00100100
	 ; defb    @00011000

; vertical brick
defb 32,112,32,252,34,37,249,80
	 ; defb    @00011000
	 ; defb    @00011000
	 ; defb    @01111000
	 ; defb    @00011000
	 ; defb    @00011000
	 ; defb    @00011110
	 ; defb    @00011000
	 ; defb    @00011000

; skull
defb 24, 36, 24,  0,153,  0, 36,36
	 ; defb    @00111100
	 ; defb    @01000010
	 ; defb    @10100101
	 ; defb    @10000001
	 ; defb    @01011010
	 ; defb    @00100100
	 ; defb    @00100100
	 ; defb    @00111100 	 

; ghost
	 defb    153, 24, 36,36,0,0,0,0
	 ; defb    @10111101
	 ; defb    @01000010
	 ; defb    @10100101
	 ; defb    @10000001
	 ; defb    @10000001
	 ; defb    @10011001
	 ; defb    @10000001
	 ; defb    @01111110 

; bomb
defb 0,0,0,0,0,0,24, 36
	 ; defb    @00111100
	 ; defb    @01000010
	 ; defb    @10100101
	 ; defb    @10011001
	 ; defb    @10011001
	 ; defb    @10100101
	 ; defb    @01000010
	 ; defb    @00111100

; player down
defb 24, 36, 24,102,153, 24, 36,36
	 ; defb    @00011000
	 ; defb    @00100100
	 ; defb    @00011000
	 ; defb    @01100110
	 ; defb    @10011001
	 ; defb    @00011000
	 ; defb    @00100100
	 ; defb    @01100110

; horizontal brick
defb 0,  0,  0,63,64,160,159,0
	 ; defb    @00000000
	 ; defb    @00000000
	 ; defb    @00000000
	 ; defb    @11111111
	 ; defb    @11111111
	 ; defb    @00000000
	 ; defb    @00000000
	 ; defb    @00000000

; power-up S
defb 60,126,231,195,195,231,126,60
	 ; defb    @00000000
	 ; defb    @00111100
	 ; defb    @01100110
	 ; defb    @11011111
	 ; defb    @11100111
	 ; defb    @01111010
	 ; defb    @00100100
	 ; defb    @00011000

	defb %"--------"
	defb %"--------"
	defb %"--------"
	defb %"--------"
	defb %"--#####-"
	defb %"--------"
	defb %"--------"
	defb %"--------"

; right missile
defb 74,74,74,74,74,74,100,0
	 ; defb    @00000000
	 ; defb    @00000000
	 ; defb    @00001111
	 ; defb    @11111110
	 ; defb    @11111110
	 ; defb    @00001111
	 ; defb    @00000000
	 ; defb    @00000000

; rocket
defb 0,0,0,252,2,5,249,0
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

defb 90,36, 24,102,153, 24, 36,36

; power-up H
defb 32,112,32,32,32,32,32,80 
	 ; defb    @00000000
	 ; defb    @00111100
	 ; defb    @01011010
	 ; defb    @11011011
	 ; defb    @11000011
	 ; defb    @01011010
	 ; defb    @00111100
	 ; defb    @00011000

; player up
defb 0,0,24, 36, 24,102,89, 24
	 ; defb    @00011000
	 ; defb    @00111100
	 ; defb    @00011000
	 ; defb    @01100110
	 ; defb    @10011001
	 ; defb    @00011000
	 ; defb    @00100100
	 ; defb    @01100110

; player right
defb 36,36,0,0,0,0,0,0
	 ; defb    @00011000
	 ; defb    @00110100
	 ; defb    @00011001
	 ; defb    @01101111
	 ; defb    @10011000
	 ; defb    @00011000
	 ; defb    @00010100
	 ; defb    @00010100

; player left
defb 0,0,0,0,24, 36, 24,102
	 ; defb    @00011000
	 ; defb    @00101100
	 ; defb    @10011000
	 ; defb    @11101110
	 ; defb    @00011001
	 ; defb    @00011000
	 ; defb    @00101000
	 ; defb    @00101000

; gun
    defb 2,7,2,63,66,162,159,5

	 ; defb    @00000000
	 ; defb    @10000000
	 ; defb    @01111110
	 ; defb    @11001000
	 ; defb    @11111000
	 ; defb    @11000000
	 ; defb    @10000000	 
	 ; defb    @00000000

defb 0,0,90,36, 24,102,89, 24 

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
defb 24,102,154, 24, 36,36,0,0
	 ; defb    @10111101
	 ; defb    @01000010
	 ; defb    @10100101
	 ; defb    @10100101
	 ; defb    @10000001
	 ; defb    @10011001
	 ; defb    @10000001
	 ; defb    @01111110 

defb 0,0,0,0,90,36, 24,102 

defb 0,0,0,0,0,0,90,36 

defb 8,42,28,119,28,42,8,0 

defb 240,240,255,133,255,161,255,0 

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

