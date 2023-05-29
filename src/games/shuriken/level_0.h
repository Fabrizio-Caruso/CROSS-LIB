

// LEVEL 0

#define LV_0_NO_OF_OBJECTS 12

#define LV_0_OBJECTS \
    LV_0_NO_OF_OBJECTS, \
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
    XSize-2,4,1,YSize-1-2-4,DIAMOND,_XL_GREEN, \
    1,4,1,YSize-1-2-4,DIAMOND,_XL_GREEN, \
	\
	XSize/2-2,YSize-2,4,1,DIAMOND,_XL_GREEN, \
	XSize/2-2,2,4,1,DIAMOND,_XL_GREEN

#define LV_0_OBJECTS_SIZE LV_OBJECTS_SIZE(LV_0_NO_OF_OBJECTS)

//

#define LV_0_SHURIKENS \
    12, \
	\
    3,4, \
    3,7, \
    3,10, \
    3,YSize-9, \
    3,YSize-6, \
    3,YSize-3, \
	\
    XSize-3,3, \
    XSize-3,6, \
    XSize-3,9, \
    XSize-3,YSize-10, \
    XSize-3,YSize-7, \
    XSize-3,YSize-4, \
    \
    0, \
    \
    0 


#define LV_0_SHURIKENS_SIZE LV_SHURIKENS_SIZE(12,0,0)


// 

#define LV0_WALLS 0
#define LV0_WALLS_SIZE LC_WALLS_SIZE(0)

