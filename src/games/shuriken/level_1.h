
// LEVEL 1

#define LV_1_NO_OF_OBJECTS 11

#define LV_1_OBJECTS \
	LV_1_NO_OF_OBJECTS, \
	\
    XSize-2,8,1,YSize-1-2-8,DIAMOND,_XL_GREEN, \
    1,8,1,YSize-1-2-8,DIAMOND,_XL_GREEN, \
	\
	4,YSize-2,XSize-1-3-4,1,DIAMOND,_XL_GREEN, \
	\
	XSize-2,3,1,1,RING,_XL_WHITE, \
	XSize-2,YSize-2,1,1,FREEZE,_XL_CYAN, \
	\
	1,3,1,1,RING,_XL_WHITE, \
	1,YSize-2,1,1,FREEZE,_XL_CYAN, \
	\
    5,YSize-5,5,2,WALL,_XL_YELLOW, \
    XSize-10,YSize-5,5,2,WALL,_XL_YELLOW, \
	\
    8,4,1,2,BLOCK,_XL_GREEN, \
    XSize-9,4,1,2,BLOCK,_XL_GREEN   

#define LV_1_OBJECTS_SIZE LV_OBJECTS_SIZE(LV_1_NO_OF_OBJECTS)

//

#define LV_1_SHURIKENS \
	2, 0, 4, \
	\
    3,8, \
    XSize-3,YSize-6, \
	\
	\
	\
	7, \
	2, \
	XSize-3, \
	XSize-8

#define LV_1_SHURIKENS_SIZE LV_SHURIKENS_SIZE(2,0,4)

// 

#define LV1_WALLS 0
#define LV1_WALLS_SIZE LC_WALLS_SIZE(0)
