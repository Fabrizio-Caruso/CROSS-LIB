
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

// #define _TILE_4 (0x76-0x20-13-7)

#define _TILE_7 33
#define _TILE_5 34
#define _TILE_17 35

#define _TILE_8 44
#define _TILE_9 45
#define _TILE_11 46
#define _TILE_12 47

#define _TILE_0 '\''
#define _TILE_1 (0x70-0x20-12-7)
// #define _TILE_2 (0x74-0x20-13-7)
#define _TILE_3 (0x72-0x20-13-7)
	

#define _TILE_16 ('^'-0x40)
#define _TILE_25 '$'
#define _TILE_14 ('>'-3)
#define _TILE_15 ('<'-2)
#define _TILE_10 (0x73 - 0x20-13-7)

#define _TILE_6 (('^'-0x40)+1)

#define _TILE_13 (0xC3-128-7)

// #define _TILE_18 (0x75-0x20-13-7)

#define _TILE_19 42
#define _TILE_20 43
#define _TILE_21 37
#define _TILE_22 38
#define _TILE_23 40
#define _TILE_24 41

#define _TILE_26 0

#define _TILE_2 ('Z'+1)
#define _TILE_4 ('Z'+2)
#define _TILE_18 ('Z'+3)

#define _TILE_27 ('Z'+4)
#define _TILE_28 ('Z'+5)
#define _TILE_29 ('Z'+6)
#define _TILE_30 ('Z'+7)
#define _TILE_31 ('Z'+8)
#define _TILE_32 ('Z'+9)
#define _TILE_33 ('Z'+10)
#define _TILE_34 ('Z'+11)
#define _TILE_35 ('Z'+12)
#define _TILE_36 ('Z'+13)
#define _TILE_37 ('Z'+14)
#define _TILE_38 ('Z'+15)
#define _TILE_39 ('Z'+16)
#define _TILE_40 ('Z'+17)
#define _TILE_41 ('Z'+18)
#define _TILE_42 ('Z'+19)
#define _TILE_43 ('Z'+20)
#define _TILE_44 ('Z'+21)
#define _TILE_45 ('Z'+22)
#define _TILE_46 ('Z'+23)
#define _TILE_47 ('Z'+24)
#define _TILE_48 ('Z'+25)
#define _TILE_49 ('Z'+26)
#define _TILE_50 ('Z'+27)
#define _TILE_51 ('Z'+28)
#define _TILE_52 ('Z'+29)
#define _TILE_53 ('Z'+30)
#define _TILE_54 ('Z'+31)
#define _TILE_55 ('Z'+32)
#define _TILE_56 ('Z'+33)
#define _TILE_57 ('Z'+34)
#define _TILE_58 ('Z'+35)
#define _TILE_59 ('Z'+36)
#define _TILE_60 ('Z'+37)
#define _TILE_61 ('Z'+38)
#define _TILE_62 ('Z'+39)
#define _TILE_63 ('Z'+40)
#define _TILE_64 ('Z'+41)
#define _TILE_65 ('Z'+42)
#define _TILE_66 ('Z'+43)
#define _TILE_67 ('Z'+44)
#define _TILE_68 ('Z'+45)
#define _TILE_69 ('Z'+46)
#define _TILE_70 ('Z'+47)
#define _TILE_71 ('Z'+48)
#define _TILE_72 ('Z'+49)
#define _TILE_73 ('Z'+50)
#define _TILE_74 ('Z'+51)
#define _TILE_75 ('Z'+52)
#define _TILE_76 ('Z'+53)
#define _TILE_77 ('Z'+54)
#define _TILE_78 ('Z'+55)
#define _TILE_79 ('Z'+56)
#define _TILE_80 ('Z'+57)
#define _TILE_81 ('Z'+58)
#define _TILE_82 ('Z'+59)
#define _TILE_83 ('Z'+60)
#define _TILE_84 ('Z'+61)
#define _TILE_85 ('Z'+62)
#define _TILE_86 ('Z'+63)
#define _TILE_87 ('Z'+64)
#define _TILE_88 ('Z'+65)
#define _TILE_89 ('Z'+66)
#define _TILE_90 ('Z'+67)
