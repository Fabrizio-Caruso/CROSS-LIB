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
defb 128,192,96,48,252,252,48,96

; left missile
defb 0,236,164,164,228,132,0,0

; bullet
defb 124,76,124,76,124,100,124,100

; dollar
defb 0,32,124,64,120,8,248,16

; power-up F
defb 0,0,0,0,192,224,0,0 


; vertical brick
defb 0,0,0,0,0,132,132,180


; skull
defb 252,180,252,180,252,180,252,180
	 

; ghost
defb    124,84,124,84,124,84,124,84
 

; bomb
defb 0,0,0,0,0,72,252,252


; player down
defb 252,252,144,36,252,252,0,0


; horizontal brick
defb 252,120,120,120,48,0,0,0


; power-up S
defb 4,0,24,12,12,4,0,0

defb 0,0,0,0,0,0,0,0

; right missile
defb 252,164,252,148,252,164,252,148


; rocket
defb 0,184,32,16,8,56,0,0

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

defb 252,252,0,96,96,0,252,252 

; power-up H
defb 252,196,252,140,252,196,252,140 


; player up
defb 48,24,204,100,124,60,4,12


; player right
defb 0,0,0,128,248,252,128,0


; player left
defb 132,132,180,252,120,120,120,48


; gun
    defb 0,148,148,148,148,200,0,0


defb 252,252,0,96,96,0,252,252 

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
defb 0,84,84,116,84,84,0,0 

defb 252,252,0,96,96,0,252,252 

defb 252,252,0,96,96,0,252,252 

defb 252,252,0,96,96,0,252,252 

defb 252,252,0,96,96,0,252,252 

_udgs_end:

