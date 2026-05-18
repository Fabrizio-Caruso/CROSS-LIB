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

#ifndef _UDG_SETTINGS
#define _UDG_SETTINGS

#define _TILE_0  (0x2A - _Z88DK_SPRITE_OFFSET)
#define _TILE_1  (0x3C - _Z88DK_SPRITE_OFFSET)
#define _TILE_2  (0x3D - _Z88DK_SPRITE_OFFSET) 
#define _TILE_3  (0x3E - _Z88DK_SPRITE_OFFSET)


#define _TILE_9  (0x3F-_Z88DK_SPRITE_OFFSET)

// YELLOW
#define _TILE_7  (0x27 - _Z88DK_SPRITE_OFFSET)
#define _TILE_12 (0x26 -_Z88DK_SPRITE_OFFSET)
#define _TILE_13 (0x2B -_Z88DK_SPRITE_OFFSET)
#define _TILE_25 (0x24 - _Z88DK_SPRITE_OFFSET)

// GREEN
#define _TILE_8  (0x2C - _Z88DK_SPRITE_OFFSET)

#define _TILE_17 (0x25 - _Z88DK_SPRITE_OFFSET)

#define _TILE_18 (0x3B - _Z88DK_SPRITE_OFFSET)

// RED
#define _TILE_5  (0x29 - _Z88DK_SPRITE_OFFSET)

// CYAN
#define _TILE_10 (0x21 - _Z88DK_SPRITE_OFFSET)
#define _TILE_11 (0x23 - _Z88DK_SPRITE_OFFSET)

// WHITE
#define _TILE_4  (0x28 - _Z88DK_SPRITE_OFFSET)

#if defined(__MO5__)||defined(__TO7__)|| defined(__COCO3__) || defined(__COCO__)||defined(__DRAGON__)
	#define _TILE_6 (0x3A - _Z88DK_SPRITE_OFFSET)
#else
	#define _TILE_6 (0x5B - _Z88DK_SPRITE_OFFSET)
#endif

#define _TILE_14 (0x22 - _Z88DK_SPRITE_OFFSET)

#define _TILE_15 (0x0E)

#define _TILE_16 (0x2F - _Z88DK_SPRITE_OFFSET)



#define _TILE_19 0x1A
#define _TILE_20 0x20
#define _TILE_21 0x3C
#define _TILE_22 0x3D
#define _TILE_23 0x3E
#define _TILE_24 0x3F
#define _TILE_26 0x0D

#define _TILE_27 (0x3E+2)
#define _TILE_28 (0x3E+3)
#define _TILE_29 (0x3E+4)
#define _TILE_30 (0x3E+5)
#define _TILE_31 (0x3E+6)
#define _TILE_32 (0x3E+7)
#define _TILE_33 (0x3E+8)
#define _TILE_34 (0x3E+9)
#define _TILE_35 (0x3E+10)
#define _TILE_36 (0x3E+11)
#define _TILE_37 (0x3E+12)
#define _TILE_38 (0x3E+13)
#define _TILE_39 (0x3E+14)
#define _TILE_40 (0x3E+15)
#define _TILE_41 (0x3E+16)
#define _TILE_42 (0x3E+17)
#define _TILE_43 (0x3E+18)
#define _TILE_44 (0x3E+19)
#define _TILE_45 (0x3E+20)
#define _TILE_46 (0x3E+21)
#define _TILE_47 (0x3E+22)
#define _TILE_48 (0x3E+23)
#define _TILE_49 (0x3E+24)
#define _TILE_50 (0x3E+25)
#define _TILE_51 (0x3E+26)
#define _TILE_52 (0x3E+27)
#define _TILE_53 (0x3E+28)
#define _TILE_54 (0x3E+29)
#define _TILE_55 (0x3E+30)
#define _TILE_56 (0x3E+31)
#define _TILE_57 (0x3E+32)
#define _TILE_58 (0x3E+33)

#define _TILE_59 (0x5E+3)
#define _TILE_60 (0x5E+4)
#define _TILE_61 (0x5E+5)
#define _TILE_62 (0x5E+6)
#define _TILE_63 (0x5E+7)
#define _TILE_64 (0x5E+8)
#define _TILE_65 (0x5E+9)
#define _TILE_66 (0x5E+10)
#define _TILE_67 (0x5E+11)
#define _TILE_68 (0x5E+12)
#define _TILE_69 (0x5E+13)
#define _TILE_70 (0x5E+14)
#define _TILE_71 (0x5E+15)
#define _TILE_72 (0x5E+16)
#define _TILE_73 (0x5E+17)
#define _TILE_74 (0x5E+18)
#define _TILE_75 (0x5E+19)
#define _TILE_76 (0x5E+20)
#define _TILE_77 (0x5E+21)
#define _TILE_78 (0x5E+22)
#define _TILE_79 (0x5E+23)
#define _TILE_80 (0x5E+24)
#define _TILE_81 (0x5E+25)
#define _TILE_82 (0x5E+26)
#define _TILE_83 (0x5E+27)
#define _TILE_84 (0x5E+28)
#define _TILE_85 (0x5E+29)
#define _TILE_86 (0x5E+30)
#define _TILE_87 (0x5E+31)
#define _TILE_88 (0x5E+32)
#define _TILE_89 (0x5E+33)
#define _TILE_90 (0x5E+34)


#endif // _UDG_SETTINGS


