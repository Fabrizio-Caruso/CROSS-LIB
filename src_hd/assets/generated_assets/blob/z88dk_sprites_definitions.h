#ifndef _Z88DK_SPRITES_DEFINITIONS
#define _Z88DK_SPRITES_DEFINITIONS


__asm
	._sprites

; 0
	defb    8,8
	defb %"--------"
	defb %"--------"
	defb %"--------"
	defb %"--------"
	defb %"--------"
	defb %"--------"
	defb %"--------"
	defb %"--------"
	

; 1
	defb    8,8
    defb 0xc0,0xe0,0xf0,0x50,0x50,0xf0,0xf0,0xe0


; 2
	defb    8,8
    defb 0xc0,0xe0,0xf0,0x50,0xf0,0x10,0xf0,0xe0


; 3
	defb    8,8	
    defb 0x03,0x07,0x0f,0x0a,0x0a,0x0f,0x0f,0x07

; 4
	defb 8,8
    defb 16,56,46,99,198,116,28,8
;	defb %"--------"
;	defb %"----#---"
;	defb %"--#####-"
;	defb %"--#-#---"
;	defb %"--#####-"
;	defb %"----#-#-"
;	defb %"--#####-"
;	defb %"----#---"

; 5
	defb    8,8
    defb 0xff,0x55,0x22,0x55,0xff,0x00,0x00,0x00

; 6
	defb    8,8	
    defb 0xc0,0xe0,0xf0,0x00,0xb0,0xf0,0xf0,0xe0

; 7
	defb 8,8
    defb 0x3c,0x7e,0xff,0x0f,0xdf,0xff,0xff,0x7f

; 8
	defb 8,8
    defb 0x3c,0x7e,0xff,0xf5,0xf5,0xff,0xff,0xfe

; 9
	defb 8,8
    defb 0x3c,0x7e,0xff,0xaf,0xaf,0xff,0xff,0x7f

; 10
	defb 8,8
    defb 0x00,0x00,0x00,0x01,0x01,0x07,0x1f,0x3f

; 11
	defb 8,8
    defb 0x03,0x07,0x0f,0x00,0x0d,0x0f,0x0f,0x07

; 12
	defb 8,8
    defb 0x3c,0x7e,0xff,0xf5,0xff,0xf1,0xff,0xfe

; 13
	defb 8,8
	defb 0,0,0,24,52,122,52,24

; 14
	defb 8,8
    defb 0x03,0x07,0x0f,0x0a,0x0f,0x08,0x0f,0x07


; 15
	defb 8,8
    defb 0xff,0xaa,0xff,0xaa,0xff,0x00,0x00,0x00

; 16
	defb 8,8
	defb %"--------"
	defb %"--####--"
	defb %"-#---##-"
	defb %"-#--#-#-"
	defb %"-#-#--#-"
	defb %"-##---#-"
	defb %"--####--"
	defb %"--------"

; 17
	defb 8,8
	defb %"--------"
	defb %"---##---"
	defb %"--#-#---"
	defb %"----#---"
	defb %"----#---"
	defb %"----#---"
	defb %"--#####-"
	defb %"--------"

; 18
	defb 8,8
	defb %"--------"
	defb %"--####--"
	defb %"-#----#-"
	defb %"------#-"
	defb %"--####--"
	defb %"-#------"
	defb %"-######-"
	defb %"--------"

; 19
	defb 8,8
	defb %"--------"
	defb %"--####--"
	defb %"-#----#-"
	defb %"----##--"
	defb %"------#-"
	defb %"-#----#-"
	defb %"--####--"
	defb %"--------"

; 20
	defb 8,8
	defb %"--------"
	defb %"----#---"
	defb %"---##---"
	defb %"--#-#---"
	defb %"-#--#---"
	defb %"-######-"
	defb %"----#---"
	defb %"--------"

; 21
	defb 8,8
	defb %"--------"
	defb %"-######-"
	defb %"-#------"
	defb %"-#####--"
	defb %"------#-"
	defb %"-#----#-"
	defb %"--####--"
	defb %"--------"

; 22
	defb 8,8
	defb %"--------"
	defb %"--####--"
	defb %"-#------"
	defb %"-#####--"
	defb %"-#----#-"
	defb %"-#----#-"
	defb %"--####--"
	defb %"--------"

; 23
	defb 8,8
	defb %"--------"
	defb %"-######-"
	defb %"------#-"
	defb %"-----#--"
	defb %"----#---"
	defb %"---#----"
	defb %"---#----"
	defb %"--------"

; 24
	defb 8,8
	defb %"--------"
	defb %"--####--"
	defb %"-#----#-"
	defb %"--####--"
	defb %"-#----#-"
	defb %"-#----#-"
	defb %"--####--"
	defb %"--------"

; 25
	defb 8,8
	defb %"--------"
	defb %"--####--"
	defb %"-#----#-"
	defb %"-#----#-"
	defb %"--#####-"
	defb %"------#-"
	defb %"--####--"
	defb %"--------"

; 26
	defb 8,8
    defb 16,40,68,146,68,40,16,16

; 27
	defb 8,8
    defb 0,0,0,0,16,56,16,0

; 28
	defb 8,8
    defb 0x00,0x00,0x00,0x80,0x80,0xe0,0xf8,0xfc

; 29
	defb 8,8
    defb 0x01,0x07,0x0f,0x1f,0x1f,0x3f,0x7f,0xff

; 30
	defb 8,8
    defb 0x80,0xe0,0xf0,0xf8,0xf8,0xfc,0xfe,0xff

; 31
	defb 8,8
    defb 0x3c,0x7e,0xff,0xaf,0xff,0x8f,0xff,0x7f

; 32
	defb    8,8
    defb 60,126,231,195,195,231,126,60

; 33
	defb 8,8
	defb %"--------"
	defb %"--####--"
	defb %"-#----#-"
	defb %"-#----#-"
	defb %"-######-"
	defb %"-#----#-"
	defb %"-#----#-"
	defb %"--------"
    
; 34
	defb 8,8
	defb %"--------"
	defb %"-#####--"
	defb %"-#----#-"
	defb %"-#####--"
	defb %"-#----#-"
	defb %"-#----#-"
	defb %"-#####--"
	defb %"--------"

; 35
	defb 8,8
	defb %"--------"
	defb %"--####--"
	defb %"-#----#-"
	defb %"-#------"
	defb %"-#------"
	defb %"-#----#-"
	defb %"--####--"
	defb %"--------"

; 36
	defb 8,8
	defb %"--------"
	defb %"-####---"
	defb %"-#---#--"
	defb %"-#----#-"
	defb %"-#----#-"
	defb %"-#---#--"
	defb %"-####---"
	defb %"--------"

; 37
	defb 8,8
	defb %"--------"
	defb %"-######-"
	defb %"-#------"
	defb %"-#####--"
	defb %"-#------"
	defb %"-#------"
	defb %"-######-"
	defb %"--------"

; 38
	defb 8,8
	defb %"--------"
	defb %"-######-"
	defb %"-#------"
	defb %"-#####--"
	defb %"-#------"
	defb %"-#------"
	defb %"-#------"
	defb %"--------"

; 39
	defb 8,8
	defb %"--------"
	defb %"--####--"
	defb %"-#----#-"
	defb %"-#------"
	defb %"-#--###-"
	defb %"-#----#-"
	defb %"--####--"
	defb %"--------"

; 40
	defb 8,8
	defb %"--------"
	defb %"-#----#-"
	defb %"-#----#-"
	defb %"-######-"
	defb %"-#----#-"
	defb %"-#----#-"
	defb %"-#----#-"
	defb %"--------"

; 41
	defb 8,8
	defb %"--------"
	defb %"--#####-"
	defb %"----#---"
	defb %"----#---"
	defb %"----#---"
	defb %"----#---"
	defb %"--#####-"
	defb %"--------"

; 42
	defb 8,8
	defb %"--------"
	defb %"------#-"
	defb %"------#-"
	defb %"------#-"
	defb %"-#----#-"
	defb %"-#----#-"
	defb %"--####--"
	defb %"--------"

; 43
	defb 8,8
	defb %"--------"
	defb %"-#---#--"
	defb %"-#--#---"
	defb %"-###----"
	defb %"-#--#---"
	defb %"-#---#--"
	defb %"-#----#-"
	defb %"--------"

; 44
	defb 8,8
	defb %"--------"
	defb %"-#------"
	defb %"-#------"
	defb %"-#------"
	defb %"-#------"
	defb %"-#------"
	defb %"-######-"
	defb %"--------"

; 45
	defb 8,8
	defb %"--------"
	defb %"-#----#-"
	defb %"-##--##-"
	defb %"-#-##-#-"
	defb %"-#----#-"
	defb %"-#----#-"
	defb %"-#----#-"
	defb %"--------"

; 46
	defb 8,8
	defb %"--------"
	defb %"-#----#-"
	defb %"-##---#-"
	defb %"-#-#--#-"
	defb %"-#--#-#-"
	defb %"-#---##-"
	defb %"-#----#-"
	defb %"--------"

; 47
	defb 8,8
	defb %"--------"
	defb %"--####--"
	defb %"-#----#-"
	defb %"-#----#-"
	defb %"-#----#-"
	defb %"-#----#-"
	defb %"--####--"
	defb %"--------"

; 48
	defb 8,8
	defb %"--------"
	defb %"-#####--"
	defb %"-#----#-"
	defb %"-#----#-"
	defb %"-#####--"
	defb %"-#------"
	defb %"-#------"
	defb %"--------"

; 49
	defb 8,8
	defb %"--------"
	defb %"--####--"
	defb %"-#----#-"
	defb %"-#----#-"
	defb %"-#-#--#-"
	defb %"-#--#-#-"
	defb %"--####--"
	defb %"--------"

; 50
	defb 8,8
	defb %"--------"
	defb %"-#####--"
	defb %"-#----#-"
	defb %"-#----#-"
	defb %"-#####--"
	defb %"-#---#--"
	defb %"-#----#-"
	defb %"--------"

; 51
	defb 8,8
	defb %"--------"
	defb %"--####--"
	defb %"-#------"
	defb %"--####--"
	defb %"------#-"
	defb %"-#----#-"
	defb %"--####--"
	defb %"--------"

; 52
	defb 8,8
	defb %"--------"
	defb %"#######-"
	defb %"---#----"
	defb %"---#----"
	defb %"---#----"
	defb %"---#----"
	defb %"---#----"
	defb %"--------"

; 53
	defb 8,8
	defb %"--------"
	defb %"-#----#-"
	defb %"-#----#-"
	defb %"-#----#-"
	defb %"-#----#-"
	defb %"-#----#-"
	defb %"--####--"
	defb %"--------"

; 54
	defb 8,8
	defb %"--------"
	defb %"-#----#-"
	defb %"-#----#-"
	defb %"-#----#-"
	defb %"-#----#-"
	defb %"--#--#--"
	defb %"---##---"
	defb %"--------"

; 55
	defb 8,8
	defb %"--------"
	defb %"-#----#-"
	defb %"-#----#-"
	defb %"-#----#-"
	defb %"-#----#-"
	defb %"-#-##-#-"
	defb %"--#--#--"
	defb %"--------"

; 56
	defb 8,8
	defb %"--------"
	defb %"-#----#-"
	defb %"--#--#--"
	defb %"---##---"
	defb %"---##---"
	defb %"--#--#--"
	defb %"-#----#-"
	defb %"--------"

; 57
	defb 8,8
	defb %"--------"
	defb %"#-----#-"
	defb %"-#---#--"
	defb %"--#-#---"
	defb %"---#----"
	defb %"---#----"
	defb %"---#----"
	defb %"--------"

; 58
	defb 8,8
	defb %"--------"
	defb %"-######-"
	defb %"-----#--"
	defb %"----#---"
	defb %"---#----"
	defb %"--#-----"
	defb %"-######-"
	defb %"--------"

; 59
	defb 8,8
    defb 0x3c,0x7e,0xff,0xf0,0xfb,0xff,0xff,0xfe

; 60
	defb 8,8
    defb 0,1,7,12,6,2,3,1

; 61
	defb 8,8
    defb 128,192,64,96,48,224,128,0

; 62
	defb 8,8
    defb 0,0,0,0,8,28,116,198

; 63
	defb 8,8
    defb 99,46,56,16,0,0,0,0

; 32 tiles here (27 thru 27+31=58)
    defb 8,8
    defb 0, 0, 0, 0, 0, 0, 0, 0


    defb 8,8
    defb 0, 0, 0, 0, 0, 0, 0, 0


    defb 8,8
    defb 0, 0, 0, 0, 0, 0, 0, 0


    defb 8,8
    defb 0, 0, 0, 0, 0, 0, 0, 0


    defb 8,8
    defb 0, 0, 0, 0, 0, 0, 0, 0


    defb 8,8
    defb 0, 0, 0, 0, 0, 0, 0, 0


    defb 8,8
    defb 0, 0, 0, 0, 0, 0, 0, 0


    defb 8,8
    defb 0, 0, 0, 0, 0, 0, 0, 0


    defb 8,8
    defb 0, 0, 0, 0, 0, 0, 0, 0


    defb 8,8
    defb 0, 0, 0, 0, 0, 0, 0, 0


    defb 8,8
    defb 0, 0, 0, 0, 0, 0, 0, 0


    defb 8,8
    defb 0, 0, 0, 0, 0, 0, 0, 0


    defb 8,8
    defb 0, 0, 0, 0, 0, 0, 0, 0


    defb 8,8
    defb 0, 0, 0, 0, 0, 0, 0, 0


    defb 8,8
    defb 0, 0, 0, 0, 0, 0, 0, 0


    defb 8,8
    defb 0, 0, 0, 0, 0, 0, 0, 0


    defb 8,8
    defb 0, 0, 0, 0, 0, 0, 0, 0


    defb 8,8
    defb 0, 0, 0, 0, 0, 0, 0, 0


    defb 8,8
    defb 0, 0, 0, 0, 0, 0, 0, 0


    defb 8,8
    defb 0, 0, 0, 0, 0, 0, 0, 0


    defb 8,8
    defb 0, 0, 0, 0, 0, 0, 0, 0


    defb 8,8
    defb 0, 0, 0, 0, 0, 0, 0, 0


    defb 8,8
    defb 0, 0, 0, 0, 0, 0, 0, 0


    defb 8,8
    defb 0, 0, 0, 0, 0, 0, 0, 0


    defb 8,8
    defb 0, 0, 0, 0, 0, 0, 0, 0


    defb 8,8
    defb 0, 0, 0, 0, 0, 0, 0, 0


    defb 8,8
    defb 0, 0, 0, 0, 0, 0, 0, 0


    defb 8,8
    defb 0, 0, 0, 0, 0, 0, 0, 0


    defb 8,8
    defb 0, 0, 0, 0, 0, 0, 0, 0


    defb 8,8
    defb 0, 0, 0, 0, 0, 0, 0, 0


    defb 8,8
    defb 0, 0, 0, 0, 0, 0, 0, 0


    defb 8,8
    defb 0, 0, 0, 0, 0, 0, 0, 0

	defb    8,8
	defb %"########"
	defb %"########"
	defb %"########"
	defb %"########"
	defb %"########"
	defb %"########"
	defb %"########"
	defb %"########"

    defb 8,8
    defb 0, 0, 0, 0, 0, 0, 0, 0

    defb 8,8
    defb 0, 0, 0, 0, 0, 0, 0, 0

    defb 8,8
    defb 0, 0, 0, 0, 0, 0, 0, 0

    defb 8,8
    defb 0, 0, 0, 0, 0, 0, 0, 0

    defb 8,8
    defb 0, 0, 0, 0, 0, 0, 0, 0

    defb 8,8
    defb 0, 0, 0, 0, 0, 0, 0, 0

    defb 8,8
    defb 0, 0, 0, 0, 0, 0, 0, 0

    defb 8,8
    defb 0, 0, 0, 0, 0, 0, 0, 0

    defb 8,8
    defb 0, 0, 0, 0, 0, 0, 0, 0

    defb 8,8
    defb 0, 0, 0, 0, 0, 0, 0, 0

    defb 8,8
    defb 0, 0, 0, 0, 0, 0, 0, 0

    defb 8,8
    defb 0, 0, 0, 0, 0, 0, 0, 0

    defb 8,8
    defb 0, 0, 0, 0, 0, 0, 0, 0

    defb 8,8
    defb 0, 0, 0, 0, 0, 0, 0, 0

    defb 8,8
    defb 0, 0, 0, 0, 0, 0, 0, 0

    defb 8,8
    defb 0, 0, 0, 0, 0, 0, 0, 0

    defb 8,8
    defb 0, 0, 0, 0, 0, 0, 0, 0

    defb 8,8
    defb 0, 0, 0, 0, 0, 0, 0, 0

    defb 8,8
    defb 0, 0, 0, 0, 0, 0, 0, 0

    defb 8,8
    defb 0, 0, 0, 0, 0, 0, 0, 0

    defb 8,8
    defb 0, 0, 0, 0, 0, 0, 0, 0

    defb 8,8
    defb 0, 0, 0, 0, 0, 0, 0, 0

    defb 8,8
    defb 0, 0, 0, 0, 0, 0, 0, 0

    defb 8,8
    defb 0, 0, 0, 0, 0, 0, 0, 0

    defb 8,8
    defb 0, 0, 0, 0, 0, 0, 0, 0

    defb 8,8
    defb 0, 0, 0, 0, 0, 0, 0, 0

    defb 8,8
    defb 0, 0, 0, 0, 0, 0, 0, 0

    defb 8,8
    defb 0, 0, 0, 0, 0, 0, 0, 0

    defb 8,8
    defb 0, 0, 0, 0, 0, 0, 0, 0

    defb 8,8
    defb 0, 0, 0, 0, 0, 0, 0, 0

    defb 8,8
    defb 0, 0, 0, 0, 0, 0, 0, 0

    defb 8,8
    defb 0, 0, 0, 0, 0, 0, 0, 0

__endasm;

#endif // _Z88DK_SPRITES_DEFINITIONS

