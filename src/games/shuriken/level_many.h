

// LEVEL 0

#define LV_MANY_NO_OF_OBJECTS 16

#define LV_MANY_OBJECTS \
    LV_MANY_NO_OF_OBJECTS, \
	\
	XSize/2-6,YSize-2,4,1,WALL,_XL_RED, \
	XSize/2-6,2,4,1,WALL,_XL_RED, \
	\
	XSize/2+2,YSize-2,4,1,WALL,_XL_RED, \
	XSize/2+2,2,4,1,WALL,_XL_RED, \
	\
	XSize-2,2,1,1,FREEZE,_XL_CYAN, \
	XSize-2,YSize-2,1,1,RING,_XL_WHITE, \
    \
	1,2,1,1,RING,_XL_WHITE, \
	1,YSize-2,1,1,FREEZE,_XL_CYAN, \
	\
    XSize/2-5,YSize/2,1,1,FREEZE,_XL_CYAN, \
    XSize/2+5,YSize/2,1,1,FREEZE,_XL_CYAN, \
    \
    XSize/2-4,YSize/2,3,1,BLOCK,_XL_GREEN, \
    XSize/2+2,YSize/2,3,1,BLOCK,_XL_GREEN, \
    \
    XSize-2,4,1,YSize-1-2-4,DIAMOND,_XL_GREEN, \
    1,4,1,YSize-1-2-4,DIAMOND,_XL_GREEN, \
	\
	XSize/2-2,YSize-2,4,1,DIAMOND,_XL_GREEN, \
	XSize/2-2,2,4,1,DIAMOND,_XL_GREEN

#define LV_MANY_OBJECTS_SIZE LV_OBJECTS_SIZE(LV_MANY_NO_OF_OBJECTS)

//

#define LV_MANY_SHURIKENS \
    12, 4, 6, \
	\
    3,4, \
    3,7, \
    3,10, \
    3,YSize-9, \
    3,YSize-6, \
    3,YSize-3, \
	\
    XSize-4,3, \
    XSize-4,6, \
    XSize-4,9, \
    XSize-4,YSize-10, \
    XSize-4,YSize-7, \
    XSize-4,YSize-4, \
    \
    2,2, \
    XSize-1-2,3, \
    4,2, \
    XSize-1-4,5, \
    \
    3, \
    XSize-1-2, \
    4, \
    XSize-1-3, \
    5, \
    XSize-1-4


#define LV_MANY_SHURIKENS_SIZE LV_SHURIKENS_SIZE(12,4,6)


// 

#define LV_MANY_WALLS 0
#define LV_MANY_WALLS_SIZE LC_WALLS_SIZE(LV_MANY_WALLS)

