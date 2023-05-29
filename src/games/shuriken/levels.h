

#define EMPTY 0
#define RING 1

#define FREEZE 2
#define DIAMOND 3

#define BLOCK 4

#define WALL  5
#define SHURIKEN 6
#define MINI_SHURIKEN 7

#define DEADLY 5


// ------------------------------------------------------------
// Helper macros to compute sizes and offsets
#define LV_OBJECTS_SIZE(no_of_rect) (1+(no_of_rect*6))

#define LV_SHURIKENS_SIZE(horiz,vert,mini) (1+(horiz)*2+ 1+(vert)*2+ 1+(mini))

#define LC_WALLS_SIZE(walls) (1+(walls*8))

#include "level_0.h"
#include "level_1.h"
#include "level_2.h"
#include "level_3.h"


// ----------------------------------------------------------
// Objects: static walls and collectable items
static const uint8_t objects_map[] =
{

	LV_0_OBJECTS,
	
	LV_1_OBJECTS,
    
	LV_2_OBJECTS,
	
	LV_3_OBJECTS

};


static const uint16_t objects_index[] = 
{
	0,
	LV_0_OBJECTS_SIZE,
	LV_0_OBJECTS_SIZE+LV_1_OBJECTS_SIZE,
	LV_0_OBJECTS_SIZE+LV_1_OBJECTS_SIZE+LV_2_OBJECTS_SIZE
    // 0,
    // 1+6*12,
    // 1+6*12+1+6*11,
    // 1+6*12+1+6*11+1+6*12

    // TODO: ....
};


// ----------------------------------------------------------
// Shurikens: horizontal, vertical and mini-shurikens
static const uint8_t shurikens_map[] =
{

	LV_0_SHURIKENS,
	
	// level=1
   	LV_1_SHURIKENS,

    // level=2
   	LV_2_SHURIKENS,
        
    // level=3
	LV3_SHURIKENS
  
};


static const uint8_t shurikens_index[] = 
{
	0,
	LV_0_SHURIKENS_SIZE,
	LV_0_SHURIKENS_SIZE+LV_1_SHURIKENS_SIZE,
	LV_0_SHURIKENS_SIZE+LV_1_SHURIKENS_SIZE+LV_2_SHURIKENS_SIZE
    // 0,
    // 1+2*12+1+1,
    // 1+2*12+1+1+ 1+2*2+1+1+4*1,
    // 1+2*12+1+1+ 1+2*2+1+1+4*1+ 1+2*2+ 1+4*2+ 1,    
    // TODO: ....
};



// ----------------------------------------------------------
// Walls: dynamic walls
static const uint8_t walls_map[] =
{
	LV0_WALLS,
	LV1_WALLS,
	LV2_WALLS,
	LV3_WALLS,
};

static const uint8_t walls_index[] =
{
	LV0_WALLS,
	LV0_WALLS+LV1_WALLS,
	LV0_WALLS+LV1_WALLS+LV2_WALLS,
	
    // 0,
    // 0,
    // 1,
    // 1+1+2*8,
};
