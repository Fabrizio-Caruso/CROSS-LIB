

// LEVEL 2

#if XSize>40
    #define LV_CROSS_WIDTH XSize/7
#else
    #define LV_CROSS_WIDTH XSize/5
#endif

#if YSize>=22
    #define LV_CROSS_HEIGHT YSize/4
#else
    #define LV_CROSS_HEIGHT YSize/5
#endif

#define LV_CROSS_NO_OF_OBJECTS 30

// XSize-2*(XSize/3)-4

#define LV_CROSS_OBJECTS \
    LV_CROSS_NO_OF_OBJECTS, \
	\
    XSize/3,4,1,LV_CROSS_HEIGHT,WALL, \
    XSize-1-XSize/3,4,1,LV_CROSS_HEIGHT,WALL, \
    \
    XSize/3,YSize-4-LV_CROSS_HEIGHT,1,LV_CROSS_HEIGHT+1,WALL, \
    XSize-1-XSize/3,YSize-4-LV_CROSS_HEIGHT+1,1,LV_CROSS_HEIGHT,WALL, \
    \
    XSize/3-LV_CROSS_WIDTH+1,4+LV_CROSS_HEIGHT,LV_CROSS_WIDTH,1,WALL, \
    XSize-1-XSize/3,4+LV_CROSS_HEIGHT,LV_CROSS_WIDTH,1,WALL, \
    \
    XSize/3-LV_CROSS_WIDTH+1,YSize-4-LV_CROSS_HEIGHT,LV_CROSS_WIDTH,1,WALL, \
    XSize-1-XSize/3,YSize-4-LV_CROSS_HEIGHT,LV_CROSS_WIDTH,1,WALL, \
    \
    \
    XSize/3+1+1,YSize-4-LV_CROSS_HEIGHT+1+2,2,1,BLOCK, \
    XSize/3+1+1,4+LV_CROSS_HEIGHT-1-2   ,2,1,BLOCK, \
    XSize/3+1+1+XSize-2*(XSize/3)-6,YSize-4-LV_CROSS_HEIGHT+1+2,2,1,BLOCK, \
    XSize/3+1+1+XSize-2*(XSize/3)-6,4+LV_CROSS_HEIGHT-1-2   ,2,1,BLOCK, \
    \
    \
    XSize/3+1+1+2,YSize-4-LV_CROSS_HEIGHT+1+2,XSize-2*(XSize/3)-8,1,WALL, \
    XSize/3+1+1+2,4+LV_CROSS_HEIGHT-1-2   ,XSize-2*(XSize/3)-8,1,WALL, \
	\
    XSize/3-1,4,1,LV_CROSS_HEIGHT,DIAMOND, \
    XSize-1-XSize/3+1,4,1,LV_CROSS_HEIGHT,DIAMOND, \
    \
    XSize/3-1,YSize-4-LV_CROSS_HEIGHT+1,1,LV_CROSS_HEIGHT,DIAMOND, \
    XSize-1-XSize/3+1,YSize-4-LV_CROSS_HEIGHT+1,1,LV_CROSS_HEIGHT,DIAMOND, \
	\
	XSize-2,2,1,1,FREEZE, \
	XSize-2,YSize-2,1,1,RING, \
    \
	1,2,1,1,RING, \
	1,YSize-2,1,1,FREEZE, \
	\
    XSize/3-LV_CROSS_WIDTH+1,4+LV_CROSS_HEIGHT-1,LV_CROSS_WIDTH-2,1,DIAMOND, \
    XSize-1-XSize/3+2,4+LV_CROSS_HEIGHT-1,LV_CROSS_WIDTH-2,1,DIAMOND, \
    \
    XSize/3-LV_CROSS_WIDTH+1,YSize-4-LV_CROSS_HEIGHT+1,LV_CROSS_WIDTH-2,1,DIAMOND, \
    XSize-1-XSize/3+2,YSize-4-LV_CROSS_HEIGHT+1,LV_CROSS_WIDTH-2,1,DIAMOND, \
    \
    XSize/3+1,4,1,LV_CROSS_HEIGHT+1,DIAMOND, \
    XSize-1-XSize/3-1,4,1,LV_CROSS_HEIGHT+1,DIAMOND, \
    \
    XSize/3+1,YSize-4-LV_CROSS_HEIGHT,1,LV_CROSS_HEIGHT+1,DIAMOND, \
    XSize-1-XSize/3-1,YSize-4-LV_CROSS_HEIGHT,1,LV_CROSS_HEIGHT+1,DIAMOND \
    \



#define LV_CROSS_OBJECTS_SIZE LV_OBJECTS_SIZE(LV_CROSS_NO_OF_OBJECTS)


//

#define LV_CROSS_SHURIKENS \
    4,4,2, \
    2,2,\
    XSize-3,3, \
    2,YSize-2, \
    XSize-3,YSize-3, \
    \
    XSize/3-3,2, \
    XSize-1-XSize/3+3,2, \
    XSize/3-3,YSize-2, \
    XSize-1-XSize/3+3,YSize-2, \
    \
    2, \
    XSize-3
    

#define LV_CROSS_SHURIKENS_SIZE LV_SHURIKENS_SIZE(4,4,2)



#define LV_CROSS_NO_OF_WALLS 4

#define LV_CROSS_WALLS \
    LV_CROSS_NO_OF_WALLS, \
    XSize/3+1+1,YSize-4-YSize/4+1, \
    XSize-2*(XSize/3)-4,1,\
    \
    XSize/3+1+1,4+YSize/4-1,\
    XSize-2*(XSize/3)-4,1,\
    \
    XSize/3-LV_CROSS_WIDTH+1,4+YSize/4+1,\
    1,YSize-9-2*(YSize/4),\
    \
    XSize-1-XSize/3+LV_CROSS_WIDTH-1,4+YSize/4+1,\
    1,YSize-9-2*(YSize/4)

#define LV_CROSS_WALLS_SIZE LC_WALLS_SIZE(LV_CROSS_NO_OF_WALLS)


