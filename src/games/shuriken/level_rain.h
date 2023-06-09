
// LEVEL 1

#define LV_RAIN_NO_OF_OBJECTS 13

#define LV_RAIN_OBJECTS \
	LV_RAIN_NO_OF_OBJECTS, \
	\
	XSize-2,3,1,1,RING,_XL_WHITE, \
	XSize-2,YSize-2,1,1,FREEZE,_XL_CYAN, \
	\
	1,3,1,1,RING,_XL_WHITE, \
	1,YSize-2,1,1,FREEZE,_XL_CYAN, \
	\
    4,YSize-4,4,1,WALL,_XL_YELLOW, \
    XSize-8,YSize-4,4,1,WALL,_XL_YELLOW, \
	\
    8,4,1,3,BLOCK,_XL_GREEN, \
    XSize-9,4,1,3,BLOCK,_XL_GREEN, \
	\
    XSize-2,8,1,YSize-1-2-8,DIAMOND,_XL_GREEN, \
    1,8,1,YSize-1-2-8,DIAMOND,_XL_GREEN, \
	\
	8,YSize-2,XSize-1-3-12,1,DIAMOND,_XL_GREEN, \
	\
    4,YSize-5,4,1,DIAMOND,_XL_GREEN, \
    XSize-8,YSize-5,4,1,DIAMOND,_XL_GREEN \

#define LV_RAIN_OBJECTS_SIZE LV_OBJECTS_SIZE(LV_RAIN_NO_OF_OBJECTS)

//

#define LV_RAIN_SHURIKENS \
	5, 0, 6, \
	\
    3,8, \
    XSize-3,YSize-6, \
	4,YSize-3, \
	5,YSize/2+3, \
	XSize-5,3, \
	\
	7, \
	2, \
	XSize-3, \
	XSize-8, \
	5, \
	XSize-6

#define LV_RAIN_SHURIKENS_SIZE LV_SHURIKENS_SIZE(5,0,6)

// 

#define LV_RAIN_NO_OF_WALLS 0


#define LV_RAIN_WALLS \
	0
	

#define LV_RAIN_WALLS_SIZE LC_WALLS_SIZE(LV_RAIN_NO_OF_WALLS)

