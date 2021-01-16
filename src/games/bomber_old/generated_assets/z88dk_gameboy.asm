
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
defb 66,66,90,126,60,60,60,24
	 ; defb    @00011000
	 ; defb    @00100100
	 ; defb    @00000000
	 ; defb    @01100110
	 ; defb    @10011001
	 ; defb    @00000000
	 ; defb    @00100100
	 ; defb    @01100110

; left missile
defb $db,$92,$92,$d2,$52,$52,$db,$00
	 ; defb    @00000000
	 ; defb    @00000000
	 ; defb    @11110000
	 ; defb    @01111111
	 ; defb    @01111111
	 ; defb    @11110000
	 ; defb    @00000000
	 ; defb    @00000000

; bullet
defb 126,102,126,102,126,102,126,102
	 ; defb    @00000000
	 ; defb    @00000000
	 ; defb    @00001000
	 ; defb    @00111000
	 ; defb    @00011100
	 ; defb    @00010000
	 ; defb    @00000000
	 ; defb    @00000000

	defb %"--------"
	defb %"----#---"
	defb %"--#####-"
	defb %"--#-#---"
	defb %"--#####-"
	defb %"----#-#-"
	defb %"--#####-"
	defb %"----#---"

; power-up F
defb 0,0,0,0,224,240,0,0 
	 ; defb    @00000000
	 ; defb    @00111100
	 ; defb    @01000010
	 ; defb    @11001111
	 ; defb    @11000011
	 ; defb    @01100110
	 ; defb    @00100100
	 ; defb    @00011000

; vertical brick
defb 0,0,0,0,66,66,90,126
	 ; defb    @00011000
	 ; defb    @00011000
	 ; defb    @01111000
	 ; defb    @00011000
	 ; defb    @00011000
	 ; defb    @00011110
	 ; defb    @00011000
	 ; defb    @00011000

; skull
defb 255,153,255,153,255,153,255,153
	 ; defb    @00111100
	 ; defb    @01000010
	 ; defb    @10100101
	 ; defb    @10000001
	 ; defb    @01011010
	 ; defb    @00100100
	 ; defb    @00100100
	 ; defb    @00111100 	 

; ghost
	 defb    126,90,126,90,126,90,126,90
	 ; defb    @10111101
	 ; defb    @01000010
	 ; defb    @10100101
	 ; defb    @10000001
	 ; defb    @10000001
	 ; defb    @10011001
	 ; defb    @10000001
	 ; defb    @01111110 

; bomb
defb 126,102,126,102,126,102,126,102
	 ; defb    @00111100
	 ; defb    @01000010
	 ; defb    @10100101
	 ; defb    @10011001
	 ; defb    @10011001
	 ; defb    @10100101
	 ; defb    @01000010
	 ; defb    @00111100

; player down
defb 0,0,12,6,7,3,0,0
	 ; defb    @00011000
	 ; defb    @00100100
	 ; defb    @00011000
	 ; defb    @01100110
	 ; defb    @10011001
	 ; defb    @00011000
	 ; defb    @00100100
	 ; defb    @01100110

; horizontal brick
defb 60,60,60,24,0,0,0,0
	 ; defb    @00000000
	 ; defb    @00000000
	 ; defb    @00000000
	 ; defb    @11111111
	 ; defb    @11111111
	 ; defb    @00000000
	 ; defb    @00000000
	 ; defb    @00000000

; power-up S
defb 255,255,255,192,129,255,255,255
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
defb 255,165,255,165,255,165,255,165
	 ; defb    @00000000
	 ; defb    @00000000
	 ; defb    @00001111
	 ; defb    @11111110
	 ; defb    @11111110
	 ; defb    @00001111
	 ; defb    @00000000
	 ; defb    @00000000

; rocket
defb $b3,$aa,$aa,$b3,$aa,$aa,$ab,$00
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

defb 255,255,0,48,48,0,255,255

; power-up H
defb 0,0,0,0,0,170,255,255 
	 ; defb    @00000000
	 ; defb    @00111100
	 ; defb    @01011010
	 ; defb    @11011011
	 ; defb    @11000011
	 ; defb    @01011010
	 ; defb    @00111100
	 ; defb    @00011000

; player up
defb 12,6,195,97,127,63,1,3
	 ; defb    @00011000
	 ; defb    @00111100
	 ; defb    @00011000
	 ; defb    @01100110
	 ; defb    @10011001
	 ; defb    @00011000
	 ; defb    @00100100
	 ; defb    @01100110

; player right
defb 0,0,0,128,254,255,128,0
	 ; defb    @00011000
	 ; defb    @00110100
	 ; defb    @00011001
	 ; defb    @01101111
	 ; defb    @10011000
	 ; defb    @00011000
	 ; defb    @00010100
	 ; defb    @00010100

; player left
defb 192,96,48,24,255,255,24,48
	 ; defb    @00011000
	 ; defb    @00101100
	 ; defb    @10011000
	 ; defb    @11101110
	 ; defb    @00011001
	 ; defb    @00011000
	 ; defb    @00101000
	 ; defb    @00101000

; gun
    defb 37,37,37,37,37,37,50,0

	 ; defb    @00000000
	 ; defb    @10000000
	 ; defb    @01111110
	 ; defb    @11001000
	 ; defb    @11111000
	 ; defb    @11000000
	 ; defb    @10000000	 
	 ; defb    @00000000

defb 255,255,0,48,48,0,255,255 

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
defb $54,$54,$54,$74,$54,$54,$54,$00
	 ; defb    @10111101
	 ; defb    @01000010
	 ; defb    @10100101
	 ; defb    @10100101
	 ; defb    @10000001
	 ; defb    @10011001
	 ; defb    @10000001
	 ; defb    @01111110 

defb 255,255,0,48,48,0,255,255 

defb 255,255,0,48,48,0,255,255 

defb 255,255,0,48,48,0,255,255 

defb 255,255,0,48,48,0,255,255 

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

