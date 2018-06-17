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

// REMARK: The Assembly routine is an adapted version of an Assembly routine provided
// by Simon Jonassen


#include "../cross_lib/wincmoc/wincmoc_input.h"

#include <conio.h>

unsigned char GET_CHAR(void)
{
	unsigned char res;
	unsigned char machine;
	
	asm {
		ldd $8000
		cmpd #$7EBB
		beq dragon
		lda #253
		sta machine
		bra pia
dragon	lda #247
		sta machine
pia		lda #253		
		sta $FF02
		ldb #73
test    lda $ff00
		cmpa machine
		beq out
		incb
		rol $ff02
		inc $ff02
		cmpb #77
		bne test
		clrb 
out		stb res
	}
	
	return res;
}
