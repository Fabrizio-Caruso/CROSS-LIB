
// LEVEL 3

#define LV_3_NO_OF_OBJECTS 20

#define LV_3_OBJECTS \
    LV_3_NO_OF_OBJECTS, \
    \
    XSize-2,4,1,YSize-1-2-4,WALL,_XL_RED, \
    1,4,1,YSize-1-2-4,WALL,_XL_RED, \
    \
	5,YSize-2,4,1,WALL,_XL_WHITE, \
	5,2,3,1,WALL,_XL_WHITE, \
	XSize-8,YSize-2,3,1,WALL,_XL_WHITE, \
	XSize-8,2,3,1,WALL,_XL_WHITE, \
	\
	8,YSize-2,1,1,FREEZE,_XL_CYAN, \
	8,2,1,1,FREEZE,_XL_CYAN, \
	XSize-9,YSize-2,1,1,FREEZE,_XL_CYAN, \
	XSize-9,2,1,1,FREEZE,_XL_CYAN, \
	\
	XSize-2,3,1,1,RING,_XL_WHITE, \
	XSize-2,YSize-2,1,1,RING,_XL_WHITE, \
	1,3,1,1,RING,_XL_WHITE, \
	1,YSize-2,1,1,RING,_XL_WHITE, \
	\
    8,4,1,2,BLOCK,_XL_GREEN, \
    XSize-9,4,1,2,BLOCK,_XL_GREEN, \
	\
    XSize-2-1,7,1,YSize-1-2-4-6,DIAMOND,_XL_GREEN, \
    2,7,1,YSize-1-2-4-6,DIAMOND,_XL_GREEN, \
	\
	9,YSize-2,XSize-18,1,DIAMOND,_XL_GREEN, \
	9,2,XSize-18,1,DIAMOND,_XL_GREEN

#define LV_3_OBJECTS_SIZE LV_OBJECTS_SIZE(LV_3_NO_OF_OBJECTS)


//


#define LV3_SHURIKENS \
    2,6,0, \
	\
    3,YSize-1-5, \
    XSize-4,6, \
	\
    \
    3,3, \
    XSize-4,4, \
    7,5, \
    XSize-8,6, \
    5,YSize-3, \
    XSize-6,YSize-4 \
    \
    \

#define LV_3_SHURIKENS_SIZE LV_SHURIKENS_SIZE(2,6,0)


//

#define LV3_NO_OF_WALLS 4

#define LV3_WALLS \
    LV3_NO_OF_WALLS, \
	\
    6,6, \
    3,1, \
    WALL,_XL_YELLOW, \
    0, \
    20, \
    \
    6,YSize-6, \
    3,1,\
    WALL,_XL_YELLOW, \
    0, \
    20, \
    \
    XSize-9,6, \
    3,1, \
    WALL,_XL_YELLOW, \
    0,\
    20, \
    \
    XSize-9,YSize-6, \
    3,1, \
    WALL,_XL_YELLOW, \
    0, \
    20

#define LV3_WALLS_SIZE LC_WALLS_SIZE(LV3_NO_OF_WALLS)

