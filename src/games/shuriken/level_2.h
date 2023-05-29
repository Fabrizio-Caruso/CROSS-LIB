


// LEVEL 2

#define LV_2_NO_OF_OBJECTS 12

#define LV_2_OBJECTS \
    LV_2_NO_OF_OBJECTS, \
	\
    7,4,1,YSize-7,WALL,_XL_YELLOW, \
    XSize-8,4,1,YSize-7,WALL,_XL_YELLOW, \
    3,4,1,YSize-7,WALL,_XL_RED, \
    XSize-4,4,1,YSize-7,WALL,_XL_RED, \
	\
	XSize-3,4,1,1,RING,_XL_WHITE, \
	XSize-3,YSize-4,1,1,FREEZE,_XL_CYAN, \
	2,4,1,1,RING,_XL_WHITE, \
	2,YSize-4,1,1,FREEZE,_XL_CYAN, \
    \
    2,5,1,YSize-9,DIAMOND,_XL_GREEN, \
    8,7,1,YSize-7-6,DIAMOND,_XL_GREEN, \
    XSize-9,7,1,YSize-7-6,DIAMOND,_XL_GREEN, \
    XSize-3,5,1,YSize-9,DIAMOND,_XL_GREEN

#define LV_2_OBJECTS_SIZE LV_OBJECTS_SIZE(LV_2_NO_OF_OBJECTS)


//

#define LV_2_SHURIKENS \
    2, \
	\
    3,3, \
    3,YSize-3, \
    \
    4, \
	\
    6,3, \
    1,3, \
    XSize-2,6, \
    XSize-2-5,6, \
    \
    0

#define LV_2_SHURIKENS_SIZE LV_SHURIKENS_SIZE(2,4,0)


// 

#if XSize<29
	#define LV2_SHURIKEN_X 9
	#define LV2_SHURIKEN_WIDTH XSize+1-19
	#define LV2_SHURIKEN_HEIGHT 2
#else
	#define LV2_SHURIKEN_X 12
	#define LV2_SHURIKEN_WIDTH XSize+1-25
	#define LV2_SHURIKEN_HEIGHT 1
#endif


//

#define LV2_NO_OF_WALLS 2


#define LV2_WALLS \
    LV2_NO_OF_WALLS, \
	\
    LV2_SHURIKEN_X,4, \
    LV2_SHURIKEN_WIDTH, LV2_SHURIKEN_HEIGHT, \
    SHURIKEN, _XL_CYAN, \
    0, \
    30, \
    \
    LV2_SHURIKEN_X,YSize-5, \
    LV2_SHURIKEN_WIDTH,LV2_SHURIKEN_HEIGHT, \
    SHURIKEN,_XL_CYAN, \
    0, \
    30

#define LV2_WALLS_SIZE LC_WALLS_SIZE(LV2_NO_OF_WALLS)


