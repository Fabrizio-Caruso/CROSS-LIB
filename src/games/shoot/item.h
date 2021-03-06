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

void handle_count_down(uint8_t * flagPtr, uint8_t * countDownPtr);    

void reduceItemCoolDowns(void);

void itemReached(Character * itemPtr);

void relocateAwayFromWalls(Character * itemPtr);

void _freezeEffect(void);

void fireChargeEffect(void);

void bombChargeEffect(void);

void _firePowerEffect(void);

void firePowerEffect(void);

void calmDownEffect(void);

void extraPointsEffect(void);

void handle_item(register Item *itemPtr);

void _freezeEffect(void);

void freezeEffect(void);

void extraLifeEffect(void);

void _invincibilityEffect(void);

void invincibilityEffect(void);

void superEffect(void);

void confuseEffect(void);

void suicideEffect(void);

void destroyerEffect(void);

#define handle_freeze_count_down() handle_count_down(&freezeActive,&freeze_count_down)
#define handle_invincibility_count_down() handle_count_down(&invincibilityActive, &invincibility_count_down)    
#define handle_confuse_count_down() handle_count_down(&confuseActive, &confuse_count_down)
#define handle_destroyer_count_down() handle_count_down(&destroyerActive,&destroyer_count_down)

// Standard items
#define handle_calmDown_item() handle_item(&calmDown);
#define handle_freeze_item() handle_item(&freeze);    
#define handle_firePower_item() handle_item(&firePower);
#define handle_fireCharge_item() handle_item(&fireCharge);
#define handle_bombCharge_item() handle_item(&bombCharge);

// To be Triggered 
#define handle_invincibility_item() handle_item(&invincibility)
#define handle_extraPoints_item() handle_item(&extraPoints);

// Secret
#define handle_super_item() handle_item(&super)
#define handle_confuse_item() handle_item(&confuse)
#define handle_suicide_item() handle_item(&suicide)
#define handle_extraLife_item() handle_item(&extraLife)
#define handle_destroyer_item() handle_item(&destroyer)

void handle_destroyer_triggers(void);

void setSecret(uint8_t secretIndex);

#define SECRETS_NUMBER 16

#define EXTRA_POINTS_EFFECT_SECRET_INDEX 0
#define INVINCIBILITY_EFFECT_SECRET_INDEX 1 
#define CONFUSE_EFFECT_SECRET_INDEX 2
#define SUICIDE_EFFECT_SECRET_INDEX 3
#define SUPER_EFFECT_SECRET_INDEX 4
#define EXTRA_LIFE_EFFECT_SECRET_INDEX 5
#define FIRE_CHARGE_AT_START_SECRET_INDEX 6
#define FIRE_POWER_AT_START_SECRET_INDEX 7
#define CALM_DOWN_AT_START_SECRET_INDEX 8
#define EXTRA_POINTS_AT_START_SECRET_INDEX 9
#define FREEZE_AT_START_SECRET_INDEX 10
#define MISSILE_DESTROYER_SECRET_INDEX 11
#define BOMB_DESTROYER_SECRET_INDEX 12
#define FIRE_POWER_LEVEL_SECRET_INDEX 13
#define SKULLS_ACTIVATED_SECRET_INDEX 14
#define ZERO_LEVEL_SECRET_INDEX 15

#endif // _item.h

