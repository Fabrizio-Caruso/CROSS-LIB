


// LEVEL 2



#define LV_CROSS_NO_OF_OBJECTS 24

#define LV_CROSS_OBJECTS \
    LV_CROSS_NO_OF_OBJECTS, \
	\
    XSize/3,4,1,YSize/4,WALL,_XL_YELLOW, \
    XSize-1-XSize/3,4,1,YSize/4,WALL,_XL_YELLOW, \
    \
    XSize/3,YSize-4-YSize/4,1,YSize/4+1,WALL,_XL_YELLOW, \
    XSize-1-XSize/3,YSize-4-YSize/4+1,1,YSize/4,WALL,_XL_YELLOW, \
    \
    XSize/3-XSize/4+1,4+YSize/4,XSize/4,1,WALL,_XL_YELLOW, \
    XSize-1-XSize/3,4+YSize/4,XSize/4,1,WALL,_XL_YELLOW, \
    \
    XSize/3-XSize/4+1,YSize-4-YSize/4,XSize/4,1,WALL,_XL_YELLOW, \
    XSize-1-XSize/3,YSize-4-YSize/4,XSize/4,1,WALL,_XL_YELLOW, \
    \
	\
    XSize/3-1,4,1,YSize/4,DIAMOND,_XL_GREEN, \
    XSize-1-XSize/3+1,4,1,YSize/4,DIAMOND,_XL_GREEN, \
    \
    XSize/3-1,YSize-4-YSize/4+1,1,YSize/4,DIAMOND,_XL_GREEN, \
    XSize-1-XSize/3+1,YSize-4-YSize/4+1,1,YSize/4,DIAMOND,_XL_GREEN, \
    \
    XSize/3-XSize/4+1,4+YSize/4-1,XSize/4-2,1,DIAMOND,_XL_GREEN, \
    XSize-1-XSize/3+2,4+YSize/4-1,XSize/4-2,1,DIAMOND,_XL_GREEN, \
    \
    XSize/3-XSize/4+1,YSize-4-YSize/4+1,XSize/4-2,1,DIAMOND,_XL_GREEN, \
    XSize-1-XSize/3+2,YSize-4-YSize/4+1,XSize/4-2,1,DIAMOND,_XL_GREEN, \
    \
    XSize/3+1,4,1,YSize/4+1,DIAMOND,_XL_GREEN, \
    XSize-1-XSize/3-1,4,1,YSize/4+1,DIAMOND,_XL_GREEN, \
    \
    XSize/3+1,YSize-4-YSize/4,1,YSize/4+1,DIAMOND,_XL_GREEN, \
    XSize-1-XSize/3-1,YSize-4-YSize/4,1,YSize/4+1,DIAMOND,_XL_GREEN, \
    \
    XSize/3-XSize/4+1,4+YSize/4+1,XSize/4+1,1,DIAMOND,_XL_GREEN, \
    XSize-1-XSize/3-1,4+YSize/4+1,XSize/4+1,1,DIAMOND,_XL_GREEN, \
    \
    XSize/3-XSize/4+1,YSize-4-YSize/4-1,XSize/4+1,1,DIAMOND,_XL_GREEN, \
    XSize-1-XSize/3-1,YSize-4-YSize/4-1,XSize/4+1,1,DIAMOND,_XL_GREEN \


#define LV_CROSS_OBJECTS_SIZE LV_OBJECTS_SIZE(LV_CROSS_NO_OF_OBJECTS)


//

#define LV_CROSS_SHURIKENS \
    0,0,0

#define LV_CROSS_SHURIKENS_SIZE LV_SHURIKENS_SIZE(0,0,0)



#define LV_CROSS_NO_OF_WALLS 0

#define LV_CROSS_WALLS \
    LV_CROSS_NO_OF_WALLS

#define LV_CROSS_WALLS_SIZE LC_WALLS_SIZE(LV_CROSS_NO_OF_WALLS)


