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

#define _TILE_15 (0x2E - _Z88DK_SPRITE_OFFSET )

#define _TILE_16 (0x2F - _Z88DK_SPRITE_OFFSET)


// 0x50 -> 0
// #if defined(__MO5__)||defined(__TO7__)
    // #define _TILE_19 49
    // #define _TILE_20 50
    // #define _TILE_21 51
    // #define _TILE_22 52
    // #define _TILE_23 53
    // #define _TILE_24 54
    // #define _TILE_26 (_TILE_8+1)
#if defined(__MO5__) || defined(__TO7__) ||  defined(__COCO3__) || defined(__COCO__)||defined(__DRAGON__)
    #define _TILE_19 (_TILE_16+2)
    #define _TILE_20 (_TILE_16+3)
    #define _TILE_21 (_TILE_16+4)
    #define _TILE_22 (_TILE_16+5)
    #define _TILE_23 (_TILE_16+6)
    #define _TILE_24 (_TILE_16+7)
    #define _TILE_26 (_TILE_8+1)
#else
    #define _TILE_19 (0x3A - _Z88DK_SPRITE_OFFSET)
    #define _TILE_20 (0x40 - _Z88DK_SPRITE_OFFSET)
    #define _TILE_21 (0x5C - _Z88DK_SPRITE_OFFSET)
    #define _TILE_22 (0x5D - _Z88DK_SPRITE_OFFSET)
    #define _TILE_23 (0x5E - _Z88DK_SPRITE_OFFSET)
    #define _TILE_24 (0x5F - _Z88DK_SPRITE_OFFSET)
    #define _TILE_26 (_TILE_8+1)
#endif

#define _TILE_27 (_TILE_24 + 1)
#define _TILE_28 (_TILE_24 + 2)
#define _TILE_29 (_TILE_24 + 3)

#define _TILE_30 (_TILE_9 + 1)
#define _TILE_31 (_TILE_9 + 2)
#define _TILE_32 (_TILE_9 + 3)

#define _TILE_33 ('9'+1-13)
#define _TILE_34 ('9'+2-13)
#define _TILE_35 ('9'+3-13)
#define _TILE_36 ('9'+4-13)
#define _TILE_37 ('9'+5-13)
#define _TILE_38 ('9'+6-13)
#define _TILE_39 ('9'+7-13)

#define _TILE_40 ('z'+1+0-13-32)
#define _TILE_41 ('z'+1+1-13-32)
#define _TILE_42 ('z'+1+2-13-32)
#define _TILE_43 ('z'+1+3-13-32)
#define _TILE_44 ('z'+1+4-13-32)
#define _TILE_45 ('z'+1+5-13-32)
#define _TILE_46 ('z'+1+6-13-32)
#define _TILE_47 ('z'+1+7-13-32)
#define _TILE_48 ('z'+1+8-45)
#define _TILE_49 ('z'+1+9-45)
#define _TILE_50 ('z'+1+10-45)
#define _TILE_51 ('z'+1+11-45)
#define _TILE_52 ('z'+1+12-45)
#define _TILE_53 ('z'+1+13-45)
#define _TILE_54 ('z'+1+14-45)
#define _TILE_55 ('z'+1+15-45)
#define _TILE_56 ('z'+1+16-45)
#define _TILE_57 ('z'+1+17-45)
#define _TILE_58 ('z'+1+18-45)
#define _TILE_59 ('z'+1+19-45)
#define _TILE_60 ('z'+1+20-45)
#define _TILE_61 ('z'+1+21-45)
#define _TILE_62 ('z'+1+22-45)
#define _TILE_63 ('z'+1+23-45)
#define _TILE_64 ('z'+1+24-45)
#define _TILE_65 ('z'+1+25-45)
#define _TILE_66 ('z'+1+26-45)
#define _TILE_67 ('z'+1+27-45)
#define _TILE_68 ('z'+1+28-45)
#define _TILE_69 ('z'+1+29-45)
#define _TILE_70 ('z'+1+30-45)
#define _TILE_71 ('z'+1+31-45)
#define _TILE_72 ('z'+1+32-45)
#define _TILE_73 ('z'+1+33-45)
#define _TILE_74 ('z'+1+34-45)
#define _TILE_75 ('z'+1+35-45)
#define _TILE_76 ('z'+1+36-45)
#define _TILE_77 ('z'+1+37-45)
#define _TILE_78 ('z'+1+38-45)
#define _TILE_79 ('z'+1+39-45)
#define _TILE_80 ('z'+1+40-45)
#define _TILE_81 ('z'+1+41-45)
#define _TILE_82 ('z'+1+42-45)
#define _TILE_83 ('z'+1+43-45)
#define _TILE_84 ('z'+1+44-45)
#define _TILE_85 ('z'+1+45-45)
#define _TILE_86 ('z'+1+46-45)
#define _TILE_87 ('z'+1+47-45)
#define _TILE_88 ('z'+1+48-45)
#define _TILE_89 ('z'+1+49-45)
#define _TILE_90 ('z'+1+50-45)

#endif // _UDG_SETTINGS


