

// LEVEL 0

#define LV_MANY_NO_OF_OBJECTS 18

#define LV_MANY_OBJECTS \
    LV_MANY_NO_OF_OBJECTS, \
	\
	XSize/2-6,YSize-3,4,2,WALL, \
	XSize/2-6,2,4,2,WALL, \
	\
	XSize/2+2,YSize-3,4,2,WALL, \
	XSize/2+2,2,4,2,WALL, \
    \
    XSize/2-5,YSize/2,3,1,BLOCK, \
    XSize/2+2,YSize/2,3,1,BLOCK, \
    \
    XSize/2-2,YSize/2,1,1,FREEZE, \
    XSize/2+1,YSize/2,1,1,FREEZE, \
	\
	XSize-2,2,1,1,FREEZE, \
	XSize-2,YSize-2,1,1,RING, \
    \
	1,2,1,1,FREEZE, \
	1,YSize-2,1,1,RING, \
	\
    XSize/2-6,YSize/2,1,1,RING, \
    XSize/2+5,YSize/2,1,1,RING, \
    \
    XSize-2,4,1,YSize-1-2-4,DIAMOND, \
    1,4,1,YSize-1-2-4,DIAMOND, \
	\
	XSize/2-2,YSize-3,4,2,DIAMOND, \
	XSize/2-2,2,4,2,DIAMOND

#define LV_MANY_OBJECTS_SIZE LV_OBJECTS_SIZE(LV_MANY_NO_OF_OBJECTS)



#define LV_MANY_SHURIKENS \
    10, 4, 4, \
	\
    3,1+3, \
    3,1+5, \
    3,1+7, \
    XSize-4,1+6, \
    3,YSize-1-6, \
    3,YSize-1-4, \
    XSize-4,1+4, \
    XSize-4,YSize-1-7, \
    XSize-4,YSize-1-5, \
    XSize-4,YSize-1-3, \
    \
    2,4, \
    XSize-1-2,4, \
    4,4, \
    XSize-1-4,5, \
    \
    2, \
    XSize-1-2, \
    3, \
    XSize-1-3


#define LV_MANY_SHURIKENS_SIZE LV_SHURIKENS_SIZE(10,4,4)


// 

#define LV_MANY_WALLS 0
#define LV_MANY_WALLS_SIZE LC_WALLS_SIZE(LV_MANY_WALLS)

