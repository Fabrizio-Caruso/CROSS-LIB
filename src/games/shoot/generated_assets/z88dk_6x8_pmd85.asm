SECTION rodata_font
SECTION rodata_font_6x8

PUBLIC _udgs
PUBLIC _udgs_end

_udgs:

	defb %"------"
	defb %"------"
	defb %"------"
	defb %"------"
	defb %"------"
	defb %"------"
	defb %"------"
	defb %"------"

; invincibility
defb 48,72,48,204,0,48,0,204

; left missile
defb 0,0,224,124,124,224,0,0

; bullet
defb 0,0,16,112,56,32,0,0

; dollar
defb 0,32,124,64,120,8,248,16

; power-up F
defb 0,120,204,220,204,88,80,48 


; vertical brick
defb 252,180,252,180,252,180,252,180


; skull
defb 120,132,204,132,132,72,72,48
	 

; ghost
defb    48,120,252,180,120,48,120,204
 

; bomb
defb 120,132,204,180,180,204,132,120


; player down
defb 48,72,48,204,180,48,72,204


; horizontal brick
defb 0,252,84,252,252,84,252,0


; power-up S
defb 0,120,204,220,204,104,72,48

defb 48,120,180,180,120,48,120,204

; right missile
defb 0,0,28,248,248,28,0,0


; rocket
defb 48,120,120,120,252,180,132,132

	defb %"------"
	defb %"--###-"
	defb %"-#--##"
	defb %"-#-#-#"
	defb %"-#-#-#"
	defb %"-##--#"
	defb %"--###-"
	defb %"------"

	defb %"------"
	defb %"---#--"
	defb %"--##--"
	defb %"---#--"
	defb %"---#--"
	defb %"---#--"
	defb %"--###-"
	defb %"------"

	defb %"------"
	defb %"--###-"
	defb %"-#---#"
	defb %"-----#"
	defb %"--###-"
	defb %"-#----"
	defb %"-#####"
	defb %"------"

	defb %"------"
	defb %"--###-"
	defb %"-#---#"
	defb %"---##-"
	defb %"-----#"
	defb %"-#---#"
	defb %"--###-"
	defb %"------"

	defb %"------"
	defb %"----#-"
	defb %"---##-"
	defb %"--#-#-"
	defb %"-#--#-"
	defb %"-#####"
	defb %"----#-"
	defb %"------"

	defb %"------"
	defb %"-#####"
	defb %"-#----"
	defb %"-####-"
	defb %"-----#"
	defb %"-#---#"
	defb %"--###-"
	defb %"------"

	defb %"------"
	defb %"--###-"
	defb %"-#----"
	defb %"-####-"
	defb %"-#---#"
	defb %"-#---#"
	defb %"--###-"
	defb %"------"

	defb %"------"
	defb %"-#####"
	defb %"-----#"
	defb %"----#-"
	defb %"---#--"
	defb %"---#--"
	defb %"---#--"
	defb %"------"

	defb %"------"
	defb %"--###-"
	defb %"-#---#"
	defb %"--###-"
	defb %"-#---#"
	defb %"-#---#"
	defb %"--###-"
	defb %"------"

	defb %"------"
	defb %"--###-"
	defb %"-#---#"
	defb %"-#---#"
	defb %"--####"
	defb %"-----#"
	defb %"--###-"
	defb %"------"

defb 0,0,0,0,48,72,48,204 

; power-up H
defb 0,120,172,172,140,168,120,48 


; player up
defb 48,120,48,204,180,48,72,204


; player right
defb 48,104,48,124,176,48,40,40


; player left
defb 48,88,48,248,52,48,80,80


; gun
    defb 132,72,48,104,88,48,72,132


defb 180,48,72,204,0,0,0,0 

	defb %"------"
	defb %"--###-"
	defb %"-#---#"
	defb %"-#---#"
	defb %"-#####"
	defb %"-#---#"
	defb %"-#---#"
	defb %"------"

	defb %"------"
	defb %"-####-"
	defb %"-#---#"
	defb %"-####-"
	defb %"-#---#"
	defb %"-#---#"
	defb %"-####-"
	defb %"------"

	defb %"------"
	defb %"--###-"
	defb %"-#---#"
	defb %"-#----"
	defb %"-#----"
	defb %"-#---#"
	defb %"--###-"
	defb %"------"

	defb %"------"
	defb %"-###--"
	defb %"-#--#-"
	defb %"-#---#"
	defb %"-#---#"
	defb %"-#--#-"
	defb %"-###--"
	defb %"------"

	defb %"------"
	defb %"-#####"
	defb %"-#----"
	defb %"-####-"
	defb %"-#----"
	defb %"-#----"
	defb %"-#####"
	defb %"------"

	defb %"------"
	defb %"-#####"
	defb %"-#----"
	defb %"-####-"
	defb %"-#----"
	defb %"-#----"
	defb %"-#----"
	defb %"------"

	defb %"------"
	defb %"--###-"
	defb %"-#---#"
	defb %"-#----"
	defb %"-#-###"
	defb %"-#---#"
	defb %"--###-"
	defb %"------"

	defb %"------"
	defb %"-#---#"
	defb %"-#---#"
	defb %"-#####"
	defb %"-#---#"
	defb %"-#---#"
	defb %"-#---#"
	defb %"------"

	defb %"------"
	defb %"--###-"
	defb %"---#--"
	defb %"---#--"
	defb %"---#--"
	defb %"---#--"
	defb %"--###-"
	defb %"------"

	defb %"------"
	defb %"-----#"
	defb %"-----#"
	defb %"-----#"
	defb %"-#---#"
	defb %"-#---#"
	defb %"--###-"
	defb %"------"

	defb %"------"
	defb %"-#--#-"
	defb %"-#-#--"
	defb %"-##---"
	defb %"-#-#--"
	defb %"-#--#-"
	defb %"-#---#"
	defb %"------"

	defb %"------"
	defb %"-#----"
	defb %"-#----"
	defb %"-#----"
	defb %"-#----"
	defb %"-#----"
	defb %"-#####"
	defb %"------"

	defb %"------"
	defb %"-#---#"
	defb %"-##-##"
	defb %"-#-#-#"
	defb %"-#---#"
	defb %"-#---#"
	defb %"-#---#"
	defb %"------"

	defb %"------"
	defb %"-#---#"
	defb %"-#---#"
	defb %"-##--#"
	defb %"-#-#-#"
	defb %"-#--##"
	defb %"-#---#"
	defb %"------"

	defb %"------"
	defb %"--###-"
	defb %"-#---#"
	defb %"-#---#"
	defb %"-#---#"
	defb %"-#---#"
	defb %"--###-"
	defb %"------"

	defb %"------"
	defb %"-####-"
	defb %"-#---#"
	defb %"-#---#"
	defb %"-####-"
	defb %"-#----"
	defb %"-#----"
	defb %"------"

	defb %"------"
	defb %"--###-"
	defb %"-#---#"
	defb %"-#---#"
	defb %"-#-#-#"
	defb %"-#--##"
	defb %"--###-"
	defb %"------"

	defb %"------"
	defb %"-####-"
	defb %"-#---#"
	defb %"-#---#"
	defb %"-####-"
	defb %"-#--#-"
	defb %"-#---#"
	defb %"------"

	defb %"------"
	defb %"--###-"
	defb %"-#----"
	defb %"--###-"
	defb %"-----#"
	defb %"-#---#"
	defb %"--###-"
	defb %"------"

	defb %"------"
	defb %"-#####"
	defb %"---#--"
	defb %"---#--"
	defb %"---#--"
	defb %"---#--"
	defb %"---#--"
	defb %"------"

	defb %"------"
	defb %"-#---#"
	defb %"-#---#"
	defb %"-#---#"
	defb %"-#---#"
	defb %"-#---#"
	defb %"--###-"
	defb %"------"

	defb %"------"
	defb %"-#---#"
	defb %"-#---#"
	defb %"-#---#"
	defb %"-#---#"
	defb %"--#-#-"
	defb %"---#--"
	defb %"------"

	defb %"------"
	defb %"-#---#"
	defb %"-#---#"
	defb %"-#---#"
	defb %"-#---#"
	defb %"-#-#-#"
	defb %"--#-#-"
	defb %"------"

	defb %"------"
	defb %"-#---#"
	defb %"--#-#-"
	defb %"---#--"
	defb %"---#--"
	defb %"--#-#-"
	defb %"-#---#"
	defb %"------"

	defb %"------"
	defb %"-#---#"
	defb %"-#---#"
	defb %"--#-#-"
	defb %"---#--"
	defb %"---#--"
	defb %"---#--"
	defb %"------"

	defb %"------"
	defb %"-#####"
	defb %"----#-"
	defb %"---#--"
	defb %"--#---"
	defb %"-#----"
	defb %"-#####"
	defb %"------"

; dead ghost
defb 180,120,252,180,120,48,120,204 

defb 0,0,0,0,48,120,48,204 

defb 4,12,4,28,4,4,8,8 

defb 128,192,128,224,128,128,64,64 

defb 180,72,204,180,180,204,72,180 

_udgs_end:

