
// LEVEL 6 SHOWN AS 7

#define LV_RAIN_NO_OF_OBJECTS 13

#define LV_RAIN_OBJECTS \
	LV_RAIN_NO_OF_OBJECTS, \
	\
	XSize-2,3,1,1,RING, \
	XSize-2,YSize-2,1,1,FREEZE, \
	\
	1,3,1,1,RING, \
	1,YSize-2,1,1,FREEZE, \
	\
    4,YSize-4,4,1,WALL, \
    XSize-8,YSize-4,4,1,WALL, \
	\
    8,4,1,4,BLOCK, \
    XSize-9,4,1,4,BLOCK, \
	\
    XSize-2,8,1,YSize-1-2-8,DIAMOND, \
    1,8,1,YSize-1-2-8,DIAMOND, \
	\
	8,YSize-2,XSize-1-3-12,1,DIAMOND, \
	\
    4,YSize-5,4,1,DIAMOND, \
    XSize-8,YSize-5,4,1,DIAMOND \

#define LV_RAIN_OBJECTS_SIZE LV_OBJECTS_SIZE(LV_RAIN_NO_OF_OBJECTS)

//

#if XSize>=20
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
#elif YSize>=20
    #define LV_RAIN_SHURIKENS \
        5, 0, 4, \
        \
        3,8, \
        XSize-3,YSize-6, \
        4,YSize-3, \
        5,YSize/2+3, \
        XSize-5,3, \
        \
        2, \
        XSize-3, \
        4, \
        XSize-5

    #define LV_RAIN_SHURIKENS_SIZE LV_SHURIKENS_SIZE(5,0,4)  
#else
    #define LV_RAIN_SHURIKENS \
        4, 0, 4, \
        \
        3,8, \
        XSize-3,YSize-6, \
        4,YSize-3, \
        XSize-5,3, \
        \
        2, \
        XSize-3, \
        4, \
        XSize-5

    #define LV_RAIN_SHURIKENS_SIZE LV_SHURIKENS_SIZE(4,0,4)      
#endif


// 

#define LV_RAIN_NO_OF_WALLS 0


#define LV_RAIN_WALLS \
	0
	

#define LV_RAIN_WALLS_SIZE LC_WALLS_SIZE(LV_RAIN_NO_OF_WALLS)

