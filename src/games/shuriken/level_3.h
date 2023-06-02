
// LEVEL 3

#define LV_3_NO_OF_OBJECTS 16

#define LV_3_OBJECTS \
    LV_3_NO_OF_OBJECTS, \
    \
    XSize-2,4,1,YSize-1-2-4,WALL,_XL_RED, \
    1,4,1,YSize-1-2-4,WALL,_XL_RED, \
    \
	5,YSize-2,3,1,WALL,_XL_WHITE, \
	5,2,3,1,WALL,_XL_WHITE, \
	XSize-8,YSize-2,3,1,WALL,_XL_WHITE, \
	XSize-8,2,3,1,WALL,_XL_WHITE, \
	\
	\
	XSize-2,3,1,1,FREEZE,_XL_CYAN, \
	XSize-2,YSize-2,1,1,RING,_XL_WHITE, \
	1,3,1,1,FREEZE,_XL_CYAN, \
	1,YSize-2,1,1,RING,_XL_WHITE, \
	\
    8,4,1,2,BLOCK,_XL_GREEN, \
    XSize-9,4,1,2,BLOCK,_XL_GREEN, \
	\
    XSize-2-1,7,1,YSize-1-2-4-6,DIAMOND,_XL_GREEN, \
    2,7,1,YSize-1-2-4-6,DIAMOND,_XL_GREEN, \
	\
	8,YSize-2,XSize-16,1,DIAMOND,_XL_GREEN, \
	8,2,XSize-16,1,DIAMOND,_XL_GREEN

#define LV_3_OBJECTS_SIZE LV_OBJECTS_SIZE(LV_3_NO_OF_OBJECTS)


//


#define LV3_SHURIKENS \
    3,8,0, \
	\
    3,YSize-6, \
    XSize-4,6, \
	5,YSize-4, \
	\
    \
    3,3, \
    XSize-4,4, \
    7,5, \
    XSize-8,6, \
    5,YSize-3, \
    XSize-6,YSize-4, \
	4,7, \
	XSize-5,YSize-6, \
    \
    \

#define LV_3_SHURIKENS_SIZE LV_SHURIKENS_SIZE(2,6,0)


//

#define LV_3_NO_OF_WALLS 4

#define LV_3_WALLS \
    LV_3_NO_OF_WALLS, \
	\
    6,6, \
    3,1, \
    WALL,_XL_YELLOW, \
    20, \
    \
    6,YSize-6, \
    3,1,\
    WALL,_XL_YELLOW, \
    20, \
    \
    XSize-9,6, \
    3,1, \
    WALL,_XL_YELLOW, \
    20, \
    \
    XSize-9,YSize-6, \
    3,1, \
    WALL,_XL_YELLOW, \
    20

#define LV_3_WALLS_SIZE LC_WALLS_SIZE(LV_3_NO_OF_WALLS)

