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
 
#ifndef _MISSILE_HEADER
#define _MISSILE_HEADER

#if !defined(TINY_GAME)

	#include "character.h"
	#include "settings.h"
	#include "definitions.h"

	void handle_bullet(void);

	void checkBullet(Character *bulletPtr);

	void checkBulletVsGhost(Character * bulletPtr,
							 Character * ghostPtr);
		
	void checkBulletVsGhosts(Character * bulletPtr);

	void checkBulletVsSkull(register Character *bulletPtr);
				
	void _moveBullet(register Character * bulletPtr);
		
	void moveBullet(register Character * bulletPtr);


	void reducePowerUpsCoolDowns(void);
		
	#if defined(FULL_GAME)
		void handle_chasing_bullet(void);
	#endif
		
#endif	// !defined(TINY_GAME)

#endif //_MISSILE

