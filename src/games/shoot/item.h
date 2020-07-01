/* --------------------------------------------------------------------------------------- */ 
// 
// CROSS SHOOT by Fabrizio Caruso
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

#if !defined(_ITEM_HEADER)
#define _ITEM_HEADER

#include "character.h"

#if !defined(TINY_GAME)

    struct ItemStruct
    {
        Character _character;
        void (*_effect)(void);
        uint16_t _coolDown;
        #if !defined(NO_BLINKING)
        uint8_t _blink;
        #endif
    };

    typedef struct ItemStruct Item;

    void handle_count_down(uint8_t * flagPtr, uint16_t * countDownPtr);    

    void reducePowerUpsCoolDowns(void);
    
    void itemReached(Character * powerUpPtr);
    
    void relocatePowerUp(Character * powerUpPtr);
    
    void _commonPowerUpEffect(void);

    void powerUpEffect(void);

    void powerUp2Effect(void);

    #define handle_freeze_count_down() handle_count_down(&freezeActive,&freeze_count_down)

    void _gunEffect(void);

    void gunEffect(void);

    void extraPointsEffect(void);

    void handle_item(register Item *itemPtr);


#endif // !defined(TINY_GAME)
    
#if defined(FULL_GAME)

    void _freezeEffect(void);

    void freezeEffect(void);
    
    void extraLifeEffect(void);

    void _invincibilityEffect(void);
    
    void invincibilityEffect(void);
    
    void superEffect(void);

    void confuseEffect(void);
    
    void zombieEffect(void);
    
    void chaseEffect(void);
#endif // defined(FULL_GAME)
    
    #if defined(FULL_GAME)
        #define handle_invincibility_count_down() handle_count_down(&invincibilityActive, &invincibility_count_down)    
        #define handle_confuse_count_down() handle_count_down(&confuseActive, &confuse_count_down)
        #define handle_zombie_count_down() handle_count_down(&zombieActive,&zombie_count_down)
    #endif    
    
    #define handle_gun_item() handle_item(&gun);
    #define handle_powerup_item() handle_item(&powerUp);
    #define handle_powerup2_item() handle_item(&powerUp2);
    #define handle_extraPoints_item() handle_item(&extraPoints);
    
    #define handle_freeze_count_down() handle_count_down(&freezeActive,&freeze_count_down)

    #if defined(FULL_GAME)
        #define handle_invincibility_count_down() handle_count_down(&invincibilityActive, &invincibility_count_down)    
        #define handle_confuse_count_down() handle_count_down(&confuseActive, &confuse_count_down)
        #define handle_zombie_count_down() handle_count_down(&zombieActive,&zombie_count_down)

        #define handle_freeze_item() handle_item(&freeze);    
        #define handle_invincibility_item() handle_item(&invincibility)
        
        #define handle_chase_item() handle_item(&chase);
        
        #define handle_super_item() handle_item(&super)
        #define handle_extraLife_item() handle_item(&extraLife)
        
        #define handle_confuse_item() handle_item(&confuse);
        #define handle_zombie_item() handle_item(&zombie);
        
    #endif

    
    #define handle_gun_item() handle_item(&gun);
    #define handle_powerup_item() handle_item(&powerUp);
    #define handle_powerup2_item() handle_item(&powerUp2);
    #define handle_extraPoints_item() handle_item(&extraPoints);

#endif // _item.h

