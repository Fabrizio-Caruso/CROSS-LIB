
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
#include <peekpoke.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "display_macros.h"

// #define _TILE_0 1
// #define _TILE_1 4
// #define _TILE_2 7
// #define _TILE_3 43
// #define _TILE_4 153
// #define _TILE_5 16
// #define _TILE_6 19
// #define _TILE_7 22
// #define _TILE_8 25
// #define _TILE_9 28
// #define _TILE_10 33
// #define _TILE_11 37
// #define _TILE_12 40
// #define _TILE_13 58
// #define _TILE_14 61
// #define _TILE_15 91
// #define _TILE_16 94
// #define _TILE_17 123
// #define _TILE_18 126
// #define _TILE_19 129
// #define _TILE_20 132
// #define _TILE_21 135
// #define _TILE_22 138
// #define _TILE_23 141
// #define _TILE_24 144
// #define _TILE_25 147
// #define _TILE_26 150 

// #define _NES_RED 0
// #define _NES_CYAN 1
// KO tile 4 cyan -> 4+1 = 5
// KO tile 6  red -> 6+0 = 6

    
#define _TILE_0    1
#define _TILE_1    3
#define _TILE_2    5
#define _TILE_3    7

#define _TILE_4  223

#define _TILE_5   11

#define _TILE_6  225

#define _TILE_7   15
#define _TILE_8   17
#define _TILE_9   19
#define _TILE_10  21
#define _TILE_11  23
#define _TILE_12  25
#define _TILE_13  27
#define _TILE_14  29

#define _TILE_15  33
#define _TILE_16  35
#define _TILE_17  37
#define _TILE_18  39
#define _TILE_19  41
#define _TILE_20  43
#define _TILE_21  45

#define _TILE_22  58
#define _TILE_23  60
#define _TILE_24  62

#define _TILE_25  91
#define _TILE_26  93
#define _TILE_27  95
#define _TILE_28  97
#define _TILE_29  99
#define _TILE_30 101
#define _TILE_31 103
#define _TILE_32 105
#define _TILE_33 107
#define _TILE_34 109
#define _TILE_35 111
#define _TILE_36 113
#define _TILE_37 115
#define _TILE_38 117
#define _TILE_39 119
#define _TILE_40 121
#define _TILE_41 123
#define _TILE_42 125
#define _TILE_43 127
#define _TILE_44 129
#define _TILE_45 131
#define _TILE_46 133
#define _TILE_47 135
#define _TILE_48 137
#define _TILE_49 139
#define _TILE_50 141
#define _TILE_51 143
#define _TILE_52 145
#define _TILE_53 147
#define _TILE_54 149
#define _TILE_55 151
#define _TILE_56 153
#define _TILE_57 155
#define _TILE_58 157
#define _TILE_59 159
#define _TILE_60 161
#define _TILE_61 163
#define _TILE_62 165
#define _TILE_63 167
#define _TILE_64 169
#define _TILE_65 171
#define _TILE_66 173
#define _TILE_67 175
#define _TILE_68 177
#define _TILE_69 179
#define _TILE_70 181
#define _TILE_71 183
#define _TILE_72 185
#define _TILE_73 187
#define _TILE_74 189
#define _TILE_75 191
#define _TILE_76 193
#define _TILE_77 195
#define _TILE_78 197
#define _TILE_79 199
#define _TILE_80 201
#define _TILE_81 203
#define _TILE_82 205
#define _TILE_83 207
#define _TILE_84 209
#define _TILE_85 211
#define _TILE_86 213
#define _TILE_87 215
#define _TILE_88 217
#define _TILE_89 219
#define _TILE_90 221


