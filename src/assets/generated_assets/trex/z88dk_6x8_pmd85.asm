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
defb 0,48,176,176,180,180,180,180

; left missile
defb 252,252,0,0,8,0,0,0

; bullet
defb 0,4,20,20,20,20,20,20

; dollar
defb 128,192,240,252,248,4,0,0

; power-up F
defb 4,156,252,252,124,16,24,0 


; vertical brick
defb 0,128,128,128,160,160,160,160


; skull
defb 0,0,0,128,0,0,0,0
	 

; ghost
defb    180,252,240,48,48,48,48,48
 

; bomb
defb 252,252,0,0,0,0,0,0


; player down
defb 124,220,252,252,224,248,192,192


; horizontal brick
defb 252,252,0,48,0,0,0,0


; power-up S
defb 0,0,0,0,4,156,252,252

defb 124,172,220,172,224,248,192,192

; right missile
defb 0,0,0,0,124,220,252,252


; rocket
defb 128,240,144,128,0,0,128,0

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

defb 0,128,160,160,160,160,160,160 

; power-up H
defb 0,4,4,4,20,20,20,20 


; player up
defb 0,48,52,52,180,180,180,180


; player right
defb 4,156,252,252,124,16,16,24


; player left
defb 128,240,144,128,0,0,0,128


; gun
    defb 124,16,16,24,0,0,0,0


defb 24,28,12,0,0,0,0,0 

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
defb 224,248,192,192,128,240,144,128 

defb 96,192,0,0,0,0,0,0 

defb 0,64,224,252,60,12,12,8 

defb 0,0,0,224,128,96,0,0 

defb 0,4,12,28,0,0,0,0 

_udgs_end:

