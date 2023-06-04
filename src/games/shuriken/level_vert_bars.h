


// LEVEL 2

#if XSize<=20
    #define LV_VERT_BARS_WIDTH 2
#else
    #define LV_VERT_BARS_WIDTH (XSize/7-1)
#endif

#define LV_VERT_BARS_NO_OF_OBJECTS 14

#define LV_VERT_BARS_OBJECTS \
    LV_VERT_BARS_NO_OF_OBJECTS, \
	\
    5+LV_VERT_BARS_WIDTH,4,1,YSize-7,WALL,_XL_YELLOW, \
    XSize-6-LV_VERT_BARS_WIDTH,4,1,YSize-7,WALL,_XL_YELLOW, \
    \
    3,4,1,YSize-7,WALL,_XL_RED, \
    XSize-4,4,1,YSize-7,WALL,_XL_RED, \
	\
	XSize-3,4,1,1,RING,_XL_WHITE, \
	XSize-3,YSize-4,1,1,FREEZE,_XL_CYAN, \
	2,4,1,1,RING,_XL_WHITE, \
	2,YSize-4,1,1,FREEZE,_XL_CYAN, \
    \
    2,5,1,YSize-9,DIAMOND,_XL_GREEN, \
    4,5,1,YSize-9,DIAMOND,_XL_GREEN, \
    6+LV_VERT_BARS_WIDTH,7,1,YSize-7-6,DIAMOND,_XL_GREEN, \
    XSize-7-LV_VERT_BARS_WIDTH,7,1,YSize-7-6,DIAMOND,_XL_GREEN, \
    XSize-5,5,1,YSize-9,DIAMOND,_XL_GREEN,  \
    XSize-3,5,1,YSize-9,DIAMOND,_XL_GREEN

#define LV_VERT_BARS_OBJECTS_SIZE LV_OBJECTS_SIZE(LV_VERT_BARS_NO_OF_OBJECTS)


//

#define LV_VERT_BARS_SHURIKENS \
    2, 4, 0, \
	\
    3,3, \
    3,YSize-3, \
    \
    \
	\
    6,3, \
    1,3, \
    XSize-2,6, \
    XSize-2-5,6 \
	\
	\

#define LV_VERT_BARS_SHURIKENS_SIZE LV_SHURIKENS_SIZE(2,4,0)



#define LV_VERT_BARS_NO_OF_WALLS 2


#define LV_VERT_BARS_WALLS \
    LV_VERT_BARS_NO_OF_WALLS, \
	\
    6+LV_VERT_BARS_WIDTH,5, \
    XSize-12-2*LV_VERT_BARS_WIDTH, 1, \
    SHURIKEN, _XL_CYAN, \
    30, \
    \
    6+LV_VERT_BARS_WIDTH,YSize-5, \
    XSize-12-2*LV_VERT_BARS_WIDTH,1, \
    SHURIKEN,_XL_CYAN, \
    30

#define LV_VERT_BARS_WALLS_SIZE LC_WALLS_SIZE(LV_VERT_BARS_NO_OF_WALLS)


