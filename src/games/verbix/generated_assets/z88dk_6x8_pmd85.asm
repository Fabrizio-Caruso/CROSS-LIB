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
defb 0,156,144,144,152,144,220,0

; left missile
defb 0,204,164,196,132,132,132,0

; bullet
defb 0,252,252,116,220,252,252,0

; dollar
defb 216,24,120,24,200,232,200,216

; power-up F
defb 0,220,136,200,136,136,136,0 


; vertical brick
defb 168,248,168,248,168,248,168,248


; skull
defb 48,48,48,48,180,120,48,0
	 

; ghost
defb    0,16,8,252,252,8,16,0
 

; bomb
defb 48,120,180,48,48,48,48,0


; player down
defb 180,120,48,48,48,48,48,0


; horizontal brick
defb 0,252,84,252,252,84,252,0


; power-up S
defb 0,128,64,252,252,64,128,0

defb 216,184,216,232,216,184,216,232

; right missile
defb 248,216,248,232,248,216,248,232


; rocket
defb 0,184,32,16,8,8,56,0

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

defb 0,132,72,48,48,72,132,0 

; power-up H
defb 120,184,216,200,232,248,216,184 


; player up
defb 0,0,84,0,84,0,84,0


; player right
defb 0,4,8,252,252,8,4,0


; player left
defb 0,32,64,252,252,64,32,0


; gun
    defb 0,168,168,168,168,168,208,0


defb 120,252,204,132,132,204,252,120 

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
defb 0,168,168,232,168,168,168,0 

defb 168,84,0,0,0,0,0,0 

defb 168,248,136,248,168,248,136,248 

defb 0,252,84,116,220,84,252,0 

defb 216,152,120,152,216,232,216,216 

_udgs_end:

