
/* --------------------------------------------------------------------------------------- */ 
// 
// CROSS CHASE by Fabrizio Caruso
//
// Fabrizio_Caruso@hotmail.com
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from
// the use of this software.

// Permission is granted to anyone to use this software for non-commercial applications, 
// subject to the following restrictions:

// 1. The origin of this software must not be misrepresented; you must not
// claim that you wrote the original software. If you use this software in
// a product, an acknowledgment in the product documentation would be
// appreciated but is not required.

// 2. Altered source versions must be plainly marked as such, and must not
// be misrepresented as being the original software.

// 3. This notice may not be removed or altered from any source distribution.
/* --------------------------------------------------------------------------------------- */ 

#include<conio.h>
// #include <peekpoke.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "display_macros.h"

#define _TILE_4 (0x76-0x20-13-7)

#define _TILE_7 33
#define _TILE_5 34
#define _TILE_17 35

#define _TILE_8 44
#define _TILE_9 45
#define _TILE_11 46
#define _TILE_12 47

#define _TILE_0 '\''
#define _TILE_1 (0x70-0x20-12-7)
#define _TILE_2 (0x74-0x20-13-7)
#define _TILE_3 (0x72-0x20-13-7)
	

#define _TILE_16 ('^'-0x40)
#define _TILE_25 '$'
#define _TILE_14 ('>'-3)
#define _TILE_15 ('<'-2)
#define _TILE_10 (0x73 - 0x20-13-7)

#define _TILE_6 (('^'-0x40)+1)

#define _TILE_13 (0xC3-128-7)

#define _TILE_18 (0x75-0x20-13-7)

#define _TILE_19 42
#define _TILE_20 43
#define _TILE_21 37
#define _TILE_22 38
#define _TILE_23 40
#define _TILE_24 41

#define _TILE_26 0

//130+1...
#define _TILE_27 (_TILE_4+1)
#define _TILE_28 (_TILE_4+2)
#define _TILE_29 (_TILE_4+3)
#define _TILE_30 (_TILE_4+4)
#define _TILE_31 (_TILE_4+5)
#define _TILE_32 (_TILE_4+6)
#define _TILE_33 (_TILE_4+7)
#define _TILE_34 (_TILE_4+8)
#define _TILE_35 (_TILE_4+9)
#define _TILE_36 (_TILE_4+10)
#define _TILE_37 (_TILE_4+11)
#define _TILE_38 (_TILE_4+12)
#define _TILE_39 (_TILE_4+13)
#define _TILE_40 (_TILE_4+14)
#define _TILE_41 (_TILE_4+15)
#define _TILE_42 (_TILE_4+16)
#define _TILE_43 (_TILE_4+17)
#define _TILE_44 (_TILE_4+18)
#define _TILE_45 (_TILE_4+19)
#define _TILE_46 (_TILE_4+20)
#define _TILE_47 (_TILE_4+21)
#define _TILE_48 (_TILE_4+22)
#define _TILE_49 (_TILE_4+23)
#define _TILE_50 (_TILE_4+24)
#define _TILE_51 (_TILE_4+25)
#define _TILE_52 (_TILE_4+26)
#define _TILE_53 (_TILE_4+27)
#define _TILE_54 (_TILE_4+28)
#define _TILE_55 (_TILE_4+29)
#define _TILE_56 (_TILE_4+30)
#define _TILE_57 (_TILE_4+31)
#define _TILE_58 (_TILE_4+32)
#define _TILE_59 (_TILE_4+33)
#define _TILE_60 (_TILE_4+34)
#define _TILE_61 (_TILE_4+35)
#define _TILE_62 (_TILE_4+36)
#define _TILE_63 (_TILE_4+37)
#define _TILE_64 (_TILE_4+38)
#define _TILE_65 (_TILE_4+39)
#define _TILE_66 (_TILE_4+40)
#define _TILE_67 (_TILE_4+41)
#define _TILE_68 (_TILE_4+42)
#define _TILE_69 (_TILE_4+43)
#define _TILE_70 (_TILE_4+44)
#define _TILE_71 (_TILE_4+45)
#define _TILE_72 (_TILE_4+46)
#define _TILE_73 (_TILE_4+47)
#define _TILE_74 (_TILE_4+48)
#define _TILE_75 (_TILE_4+49)
#define _TILE_76 (_TILE_4+50)
#define _TILE_77 (_TILE_4+51)
#define _TILE_78 (_TILE_4+52)
#define _TILE_79 (_TILE_4+53)
#define _TILE_80 (_TILE_4+54)
#define _TILE_81 (_TILE_4+55)
#define _TILE_82 (_TILE_4+56)
#define _TILE_83 (_TILE_4+57)
#define _TILE_84 (_TILE_4+58)
#define _TILE_85 (_TILE_4+59)
#define _TILE_86 (_TILE_4+60)
#define _TILE_87 (_TILE_4+61)
#define _TILE_88 (_TILE_4+62)
#define _TILE_89 (_TILE_4+63)
#define _TILE_90 (_TILE_4+64)
