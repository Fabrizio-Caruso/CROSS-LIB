
#ifndef _LETTERS_H
#define _LETTERS_H

    #if defined(ONLY_SMALL_LETTERS) 
        #define _XL_A "a"
        #define _XL_B "b"
        #define _XL_C "c"
        #define _XL_D "d"
        #define _XL_E "e"
        #define _XL_F "f"
        #define _XL_G "g"
        #define _XL_H "h"
        #define _XL_I "i"
        #define _XL_J "j"
        #define _XL_K "k"
        #define _XL_L "l"
        #define _XL_M "m"
        #define _XL_N "n"
        #define _XL_O "o"
        #define _XL_P "p"
        #define _XL_Q "q"
        #define _XL_R "r"
        #define _XL_S "s"
        #define _XL_T "t"
        #define _XL_U "u"
        #define _XL_V "v"
        #define _XL_W "w"
        #define _XL_X "x"
        #define _XL_Y "y"
        #define _XL_Z "z"
        

    #elif defined(ONLY_CAPITAL_LETTERS)
    
        #define _XL_a "A"
        #define _XL_b "B"
        #define _XL_c "C"
        #define _XL_d "D"
        #define _XL_e "E"
        #define _XL_f "F"
        #define _XL_g "G"
        #define _XL_h "H"
        #define _XL_i "I"
        #define _XL_j "J"
        #define _XL_k "K"
        #define _XL_l "L"
        #define _XL_m "M"
        #define _XL_n "N"
        #define _XL_o "O"
        #define _XL_p "P"
        #define _XL_q "Q"
        #define _XL_r "R"
        #define _XL_s "S"
        #define _XL_t "T"
        #define _XL_u "U"
        #define _XL_v "V"
        #define _XL_w "W"
        #define _XL_x "X"
        #define _XL_y "Y"
        #define _XL_z "Z"
        
    #endif
    
    #if !defined(ONLY_SMALL_LETTERS)
        #define _XL_A "A"
        #define _XL_B "B"
        #define _XL_C "C"
        #define _XL_D "D"
        #define _XL_E "E"
        #define _XL_F "F"
        #define _XL_G "G"
        #define _XL_H "H"
        #define _XL_I "I"
        #define _XL_J "J"
        #define _XL_K "K"
        #define _XL_L "L"
        #define _XL_M "M"
        #define _XL_N "N"
        #define _XL_O "O"
        #define _XL_P "P"
        #define _XL_Q "Q"
        #define _XL_R "R"
        #define _XL_S "S"
        #define _XL_T "T"
        #define _XL_U "U"
        #define _XL_V "V"
        #define _XL_W "W"
        #define _XL_X "X"
        #define _XL_Y "Y"
        #define _XL_Z "Z"
    #endif
    
    #if !defined(ONLY_CAPITAL_LETTERS)
        #define _XL_a "a"
        #define _XL_b "b"
        #define _XL_c "c"
        #define _XL_d "d"
        #define _XL_e "e"
        #define _XL_f "f"
        #define _XL_g "g"
        #define _XL_h "h"
        #define _XL_i "i"
        #define _XL_j "j"
        #define _XL_k "k"
        #define _XL_l "l"
        #define _XL_m "m"
        #define _XL_n "n"
        #define _XL_o "o"
        #define _XL_p "p"
        #define _XL_q "q"
        #define _XL_r "r"
        #define _XL_s "s"
        #define _XL_t "t"
        #define _XL_u "u"
        #define _XL_v "v"
        #define _XL_w "w"
        #define _XL_x "x"
        #define _XL_y "y"
        #define _XL_z "z"
    #endif

    #if !(defined(_API_VERSION) && _API_VERSION>=2)
        #if defined(NO_SPACE_TEXT)
            #if !defined(__ATARI__)
                #define _XL_SPACE "\xA0"
            #else
                #define _XL_SPACE "\x40"
            #endif
        #endif
    #endif
    

    #if !defined(_XL_SPACE)
        #define _XL_SPACE " "
    #endif


#endif // _LETTERS_H

