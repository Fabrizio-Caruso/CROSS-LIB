
/*
    XSize/3-LV_CROSS_WIDTH+1,4+YSize/4+1,LV_CROSS_WIDTH+1,1,DIAMOND,_XL_GREEN, 
    XSize-1-XSize/3-1,4+YSize/4+1,LV_CROSS_WIDTH+1,1,DIAMOND,_XL_GREEN, 
    
    XSize/3-LV_CROSS_WIDTH+1,YSize-4-YSize/4-1,LV_CROSS_WIDTH+1,1,DIAMOND,_XL_GREEN, 
    XSize-1-XSize/3-1,YSize-4-YSize/4-1,LV_CROSS_WIDTH+1,1,DIAMOND,_XL_GREEN 
*/


// LEVEL 2

#if XSize>40
    #define LV_CROSS_WIDTH XSize/7
#else
    #define LV_CROSS_WIDTH XSize/5
#endif

#define LV_CROSS_NO_OF_OBJECTS 20

#define LV_CROSS_OBJECTS \
    LV_CROSS_NO_OF_OBJECTS, \
	\
    XSize/3,4,1,YSize/4,WALL,_XL_RED, \
    XSize-1-XSize/3,4,1,YSize/4,WALL,_XL_RED, \
    \
    XSize/3,YSize-4-YSize/4,1,YSize/4+1,WALL,_XL_RED, \
    XSize-1-XSize/3,YSize-4-YSize/4+1,1,YSize/4,WALL,_XL_RED, \
    \
    XSize/3-LV_CROSS_WIDTH+1,4+YSize/4,LV_CROSS_WIDTH,1,WALL,_XL_RED, \
    XSize-1-XSize/3,4+YSize/4,LV_CROSS_WIDTH,1,WALL,_XL_RED, \
    \
    XSize/3-LV_CROSS_WIDTH+1,YSize-4-YSize/4,LV_CROSS_WIDTH,1,WALL,_XL_RED, \
    XSize-1-XSize/3,YSize-4-YSize/4,LV_CROSS_WIDTH,1,WALL,_XL_RED, \
    \
	\
    XSize/3-1,4,1,YSize/4,DIAMOND,_XL_GREEN, \
    XSize-1-XSize/3+1,4,1,YSize/4,DIAMOND,_XL_GREEN, \
    \
    XSize/3-1,YSize-4-YSize/4+1,1,YSize/4,DIAMOND,_XL_GREEN, \
    XSize-1-XSize/3+1,YSize-4-YSize/4+1,1,YSize/4,DIAMOND,_XL_GREEN, \
    \
    XSize/3-LV_CROSS_WIDTH+1,4+YSize/4-1,LV_CROSS_WIDTH-2,1,DIAMOND,_XL_GREEN, \
    XSize-1-XSize/3+2,4+YSize/4-1,LV_CROSS_WIDTH-2,1,DIAMOND,_XL_GREEN, \
    \
    XSize/3-LV_CROSS_WIDTH+1,YSize-4-YSize/4+1,LV_CROSS_WIDTH-2,1,DIAMOND,_XL_GREEN, \
    XSize-1-XSize/3+2,YSize-4-YSize/4+1,LV_CROSS_WIDTH-2,1,DIAMOND,_XL_GREEN, \
    \
    XSize/3+1,4,1,YSize/4+1,DIAMOND,_XL_GREEN, \
    XSize-1-XSize/3-1,4,1,YSize/4+1,DIAMOND,_XL_GREEN, \
    \
    XSize/3+1,YSize-4-YSize/4,1,YSize/4+1,DIAMOND,_XL_GREEN, \
    XSize-1-XSize/3-1,YSize-4-YSize/4,1,YSize/4+1,DIAMOND,_XL_GREEN \
    \



#define LV_CROSS_OBJECTS_SIZE LV_OBJECTS_SIZE(LV_CROSS_NO_OF_OBJECTS)


//

#define LV_CROSS_SHURIKENS \
    0,0,0

#define LV_CROSS_SHURIKENS_SIZE LV_SHURIKENS_SIZE(0,0,0)



#define LV_CROSS_NO_OF_WALLS 4

#define LV_CROSS_WALLS \
    LV_CROSS_NO_OF_WALLS, \
    XSize/3+1+1,YSize-4-YSize/4+1, \
    XSize-2*(XSize/3)-4,1,\
    SHURIKEN,_XL_CYAN,20, \
    \
    XSize/3+1+1,4+YSize/4-1,\
    XSize-2*(XSize/3)-4,1,\
    SHURIKEN,_XL_CYAN,20, \
    \
    XSize/3-LV_CROSS_WIDTH+1,4+YSize/4+1,\
    1,YSize-9-2*(YSize/4),\
    SHURIKEN,_XL_CYAN,20, \
    \
    XSize-XSize/4,4+YSize/4+1,\
    1,YSize-9-2*(YSize/4),\
    SHURIKEN,_XL_CYAN,20  


#define LV_CROSS_WALLS_SIZE LC_WALLS_SIZE(LV_CROSS_NO_OF_WALLS)


