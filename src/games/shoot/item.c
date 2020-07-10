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

#include "settings.h"

#include "character.h"
#include "item.h"
#include "ghost.h"
#include "game_text.h"
#include "level.h"
#include "text_strings.h"
#include "sleep.h"

#include "cross_lib.h"

extern uint16_t points;

extern uint8_t guns;
extern uint8_t lives;

extern uint8_t level;

extern uint8_t freezeActive;
extern uint8_t freeze_count_down;

extern uint8_t bombCount;

extern uint8_t bulletStrength;

extern uint8_t invincibilityActive;
extern uint8_t confuseActive;
extern uint8_t suicideActive;

extern uint8_t invincibility_count_down;
extern uint8_t confuse_count_down;

extern uint8_t destroyed_bases_in_completed_levels;
extern uint8_t all_skulls_killed_in_completed_levels;    

extern uint8_t extraLife_present_on_level;
extern uint8_t suicide_present_on_level;

extern Image DEAD_GHOST_IMAGE;

extern Character ghosts[GHOSTS_NUMBER];
extern Character bombs[BOMBS_NUMBER];
extern Character skull;

extern Character player;

extern Item calmDown;
extern Item fireCharge;
extern Item bombCharge;
extern Item firePower;
extern Item extraPoints;

extern uint8_t ghostsOnScreen;

extern uint8_t isBossLevel;

extern Character *chasedEnemyPtr;

extern Item freeze;

extern Item invincibility;

extern Item super;
extern Item extraLife;

extern Item confuse;
extern Item suicide;

extern Item destroyer;

extern uint8_t destroyed_bases;
extern uint8_t exploded_bombs;
extern uint8_t missileDestroyerActivated;
extern uint8_t bombDestroyerActivated;
extern uint8_t destroyerActive;
extern uint8_t destroyer_count_down;

extern uint8_t firePowerItemSecret;
extern uint8_t firePowerLevelSecret;
extern uint8_t fireChargeSecret;

extern uint8_t discoveredSecrets[];

extern Character skulls[];

extern Image SKULL_IMAGE;
extern Image CONFUSE_IMAGE;

extern uint8_t skullActive;

void itemReached(Character * itemPtr)
{
    ZAP_SOUND();
    deleteItem(itemPtr);
    displayPlayer(&player);
    itemPtr->_status = 0;
    displayScoreStats();
}


void relocateAwayFromWalls(Character * itemPtr)
{        
        do
        {
            relocateNearBy(itemPtr);
        } while(nearInnerVerticalWall(itemPtr)||nearInnerHorizontalWall(itemPtr));        

}    


void _freezeEffect(void)
{
    decreaseGhostLevel();
    freezeActive = 1;    
    points+=FREEZE_BONUS;
    freeze_count_down += FREEZE_COUNT_DOWN;    
}


void _increaseBullets(uint8_t bullets)
{
    uint8_t missing = MAX_GUNS - guns;
    
    if(missing>=bullets)
    {
        guns+=bullets;
    }
    else
    {
        guns=MAX_GUNS;
        fireChargeSecret = 1;
    }
    printGunsStats();
}


void fireChargeEffect(void)
{
    _increaseBullets(BULLET_GUNS);
    points+=FIRE_CHARGE_BONUS;
    fireCharge._coolDown = FIRE_CHARGE_COOL_DOWN;        
}


void bombChargeEffect(void)
{
    uint8_t i;
    
    for(i=0;i<BOMBS_NUMBER;++i)
    {
        deleteBomb(&bombs[i]);
    }

    placeBombs();
    bombCount = 0;
    // TODO: delete Bombs
    bombCharge._coolDown = BOMB_CHARGE_COOL_DOWN;    
}


void calmDownEffect(void)
{
    uint8_t i;
    
    for(i=0;i<CALM_DOWN_EFFECT_FACTOR;++i)
    {
        decreaseGhostLevel();
    }
    points+=CALM_DOWN_BONUS;
    freezeActive = 1;
    freeze_count_down += FREEZE_COUNT_DOWN/4;
    calmDown._coolDown = CALM_DOWN_COOL_DOWN*2;    
}


void _firePowerEffect(void)
{
    if(bulletStrength<9)
    {
        ++bulletStrength;
    }
    if(bulletStrength==FIRE_POWER_ITEM_THRESHOLD)
    {
        firePowerItemSecret = 1;
    }
    if(bulletStrength>=FIRE_POWER_LEVEL_THRESHOLD)
    {
        firePowerLevelSecret = bulletStrength-FIRE_POWER_LEVEL_THRESHOLD+3;
    }
    points+=FIRE_POWER_BONUS;
    printFirePowerStats();
}


void firePowerEffect(void)
{
    _firePowerEffect();
    firePower._coolDown = FIRE_POWER_COOL_DOWN*2;    
}


void extraPointsEffect(void)
{
    
    if(level)
    {
        points+=EXTRA_POINTS+level*EXTRA_POINTS_LEVEL_INCREASE;
        extraPoints._coolDown = SECOND_EXTRA_POINTS_COOL_DOWN;
    }
    else
    {
        points+=SECRET_LEVEL_EXTRA_POINTS;
        extraPoints._coolDown = 4;
    }
    setSecret(EXTRA_POINTS_EFFECT_SECRET_INDEX);
}


void handle_item(register Item *itemPtr)
{
    // Manage item
    if(itemPtr->_character._status)
    {    
        if(areCharctersAtSamePosition(&player, (Character *) itemPtr))
        {
            itemPtr->_effect();
            itemReached((Character *) itemPtr);            
        }
        else
        {
            _blink_draw(itemPtr->_character._x, itemPtr->_character._y, itemPtr->_character._imagePtr, &(itemPtr->_blink));
        }        
    }
    else if (itemPtr->_coolDown == 0)
    {
        itemPtr->_character._status = 1;
        relocateAwayFromWalls((Character *) itemPtr);

        _blink_draw(itemPtr->_character._x, itemPtr->_character._y, itemPtr->_character._imagePtr, &(itemPtr->_blink));
    }
    else
    {
        --itemPtr->_coolDown;
    }
}


void handle_count_down(uint8_t * flagPtr, uint8_t * countDownPtr)
{
    if(*flagPtr)
    {
        if(*countDownPtr==0)
        {
            *flagPtr=0;
        }
        else
        {
            --(*countDownPtr);
        }
    }
}    


void reduceItemCoolDowns(void)
{
    extraPoints._coolDown-=extraPoints._coolDown/4;
    invincibility._coolDown-=invincibility._coolDown/8;
    freeze._coolDown-=freeze._coolDown/16;
    fireCharge._coolDown-=fireCharge._coolDown/32;
    TICK_SOUND();        
}


void freezeEffect(void)
{
    _freezeEffect();
    freeze._coolDown = ((uint16_t) (FREEZE_COOL_DOWN));    
}


void extraLifeEffect(void)
{
    ++lives;
    all_skulls_killed_in_completed_levels=1;
    // destroyed_bases_in_completed_levels = 0;
    extraLife_present_on_level = 0;
    // extraLife._coolDown = EXTRA_LIFE_COOL_DOWN*10; // second time must be impossible
    printLivesStats();        
    setSecret(EXTRA_LIFE_EFFECT_SECRET_INDEX);
}


void _invincibilityEffect(void)
{
    invincibilityActive = 1;
    if(invincibility_count_down<INVINCIBILITY_COUNT_DOWN)
    {
        invincibility_count_down = INVINCIBILITY_COUNT_DOWN;
    }
}


void invincibilityEffect(void)
{
    _invincibilityEffect();
    if(level)
    {
        invincibility._coolDown = ((uint16_t) (INVINCIBILITY_COOL_DOWN)*4);
    }
    else
    {
        invincibility._coolDown = SECRET_LEVEL_INVINCIBILITY_COOL_DOWN;
    }
    setSecret(INVINCIBILITY_EFFECT_SECRET_INDEX);
}


void superEffect(void)
{
    _freezeEffect();
    _firePowerEffect();
    _invincibilityEffect();
    super._coolDown = ((uint16_t) (SUPER_COOL_DOWN)*8);
    setSecret(SUPER_EFFECT_SECRET_INDEX);
}


void confuseEffect(void)
{
    uint8_t i;
    uint8_t j;
    
    if(skullActive)
    {
        for(j=0;j<8;++j)
        {
            for(i=0;i<SKULLS_NUMBER;++i)
            {
                if(skulls[i]._status && (skulls[i]._imagePtr==&SKULL_IMAGE))
                {
                    skulls[i]._imagePtr=&CONFUSE_IMAGE;
                    displaySkull(&skulls[i]);
                    SHORT_SLEEP(2);
                    skulls[i]._imagePtr=&SKULL_IMAGE;
                    displaySkull(&skulls[i]);
                    SHORT_SLEEP(1);
                }
            }
        }
    }
    confuseActive = 1;
    confuse._coolDown = SECOND_CONFUSE_COOL_DOWN; //20000UL;//(CONFUSE_COOL_DOWN<<4);
    confuse_count_down = CONFUSE_COUNT_DOWN;

    setSecret(CONFUSE_EFFECT_SECRET_INDEX);
}


void suicideEffect(void)
{
    uint8_t i;
    
    destroyed_bases_in_completed_levels = 1;
    suicide._coolDown = SECOND_SUICIDE_COOL_DOWN; 
    setSecret(SUICIDE_EFFECT_SECRET_INDEX);
    for(i=0;i<ghostsOnScreen;++i)
    {
        if(ghosts[i]._status)
        {
            ghostDiesAndSpawns(&ghosts[i]);
            points+=GHOST_VS_BOMBS_BONUS;
        }
    }
}


void destroyerEffect(void)
{
    destroyerActive = 1;
    invincibilityActive = 1;
    invincibility_count_down= DESTROYER_COUNT_DOWN+INVINCIBILITY_COUNT_DOWN/4;
    destroyer_count_down = DESTROYER_COUNT_DOWN;
    destroyer._coolDown = DESTROYER_COOL_DOWN;
}


void handle_destroyer_triggers(void)
{
    if((destroyed_bases>=MISSILE_DESTROYER_TRIGGER)&& !missileDestroyerActivated)
    {
        missileDestroyerActivated = 1;
        destroyer._coolDown = 2;
        setSecret(MISSILE_DESTROYER_SECRET_INDEX);
    }
    
    if((exploded_bombs>=BOMB_DESTROYER_TRIGGER)&& !bombDestroyerActivated)
    {
        bombDestroyerActivated = 1;
        destroyer._coolDown = 2;
        setSecret(BOMB_DESTROYER_SECRET_INDEX);
    }
}


void setSecret(uint8_t secretIndex)
{   if(!discoveredSecrets[secretIndex])
    {        
        TICK_SOUND();
        _draw_stat(PLAYER_IMAGE_X, PLAYER_IMAGE_Y, &INVINCIBILITY_IMAGE);
        printCenteredMessageWithCol(COLOR_YELLOW, SECRET_FOUND_STRING);
        SLEEP(1);
        _draw_stat(PLAYER_IMAGE_X, PLAYER_IMAGE_Y, &PLAYER_IMAGE);
        printCenteredMessageWithCol(COLOR_YELLOW, EMPTY_STRING);
        discoveredSecrets[secretIndex] = 1;
    }
}

