

// LEVEL 0

#define LV_HORIZ_NO_OF_OBJECTS 12

#define LV_HORIZ_OBJECTS \
    LV_HORIZ_NO_OF_OBJECTS, \
	\
	XSize/2-6,YSize-2,4,1,WALL, \
	XSize/2-6,2,4,1,WALL, \
	\
	XSize/2+2,YSize-2,4,1,WALL, \
	XSize/2+2,2,4,1,WALL, \
	\
	XSize-2,2,1,1,FREEZE, \
	XSize-2,YSize-2,1,1,RING, \
    \
	1,2,1,1,RING, \
	1,YSize-2,1,1,FREEZE, \
	\
    XSize-2,4,1,YSize-1-2-4,DIAMOND, \
    1,4,1,YSize-1-2-4,DIAMOND, \
	\
	XSize/2-2,YSize-2,4,1,DIAMOND, \
	XSize/2-2,2,4,1,DIAMOND

#define LV_HORIZ_OBJECTS_SIZE LV_OBJECTS_SIZE(LV_HORIZ_NO_OF_OBJECTS)

//

#define LV_HORIZ_SHURIKENS \
    12, 0, 0, \
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
    XSize-3,YSize-4


#define LV_HORIZ_SHURIKENS_SIZE LV_SHURIKENS_SIZE(12,0,0)


// 

#define LV_HORIZ_WALLS 0
#define LV_HORIZ_WALLS_SIZE LC_WALLS_SIZE(LV_HORIZ_WALLS)

