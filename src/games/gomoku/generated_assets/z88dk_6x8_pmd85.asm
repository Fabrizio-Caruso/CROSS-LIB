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
defb 192,192,192,192,96,60,12,0

; left missile
defb 0,0,0,0,0,0,0,168

; bullet
defb 12,12,12,12,24,240,192,0

; dollar
defb 0,168,4,128,4,128,84,0

; power-up F
defb 0,140,128,128,132,132,196,0 


; vertical brick
defb 84,0,0,0,0,0,0,0


; skull
defb 124,252,252,248,248,240,192,0
	 

; ghost
defb    0,12,60,124,124,252,252,248
 

; bomb
defb 0,192,240,248,248,252,252,124


; player down
defb 0,12,60,124,124,252,252,252


; horizontal brick
defb 84,0,0,0,0,0,0,0


; power-up S
defb 0,12,60,96,192,192,192,192

defb 16,8,12,252,252,12,8,16

; right missile
defb 0,0,0,0,0,0,0,168


; rocket
defb 0,140,136,136,140,136,204,0

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

defb 0,140,136,136,140,128,192,0 

; power-up H
defb 0,140,136,136,140,136,204,0 


; player up
defb 0,192,240,248,248,252,252,252


; player right
defb 252,252,252,124,124,60,12,0


; player left
defb 252,252,252,248,248,240,192,0


; gun
    defb 0,192,240,24,12,12,12,12


defb 0,196,72,72,136,8,196,0 

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
defb 248,252,252,124,124,60,12,0 

defb 0,196,76,68,132,4,204,0 

defb 0,196,72,64,132,8,204,0 

defb 0,204,64,64,140,0,204,0 

defb 0,200,72,72,140,0,192,0 

_udgs_end:

