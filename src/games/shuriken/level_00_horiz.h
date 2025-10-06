
// LEVEL 0 SHOWN AS 1

#if XSize>=20
    #define MINI_WALL_SIZE 4
    #define MINI_WALL_OFFSET 0
#else
    #define MINI_WALL_SIZE 3
    #define MINI_WALL_OFFSET 1
#endif

#define LV_HORIZ_NO_OF_OBJECTS 18

#define LV_HORIZ_OBJECTS \
    LV_HORIZ_NO_OF_OBJECTS, \
	\
	XSize/2-6+MINI_WALL_OFFSET,YSize-2,MINI_WALL_SIZE,1,WALL, \
	XSize/2-6+MINI_WALL_OFFSET,2,MINI_WALL_SIZE,1,WALL, \
	\
	XSize/2+2,YSize-2,MINI_WALL_SIZE,1,WALL, \
	XSize/2+2,2,MINI_WALL_SIZE,1,WALL, \
	\
	XSize-2,2,1,1,FREEZE, \
	XSize-2,YSize-2,1,1,RING, \
    \
	1,2,1,1,RING, \
	1,YSize-2,1,1,FREEZE, \
    \
    (XSize-4)/2-2,5,2,1,WALL, \
    (XSize-4)/2+4,5,2,1,WALL, \
    \
    (XSize-4)/2-3,5,1,1,FREEZE, \
    (XSize-4)/2+6,5,1,1,RING, \
    \
    (XSize-4)/2-2,YSize-5,2,1,WALL, \
    (XSize-4)/2+4,YSize-5,2,1,WALL, \
	\
    XSize-2,4,1,YSize-1-2-4,DIAMOND, \
    1,4,1,YSize-1-2-4,DIAMOND, \
	\
	XSize/2-2,YSize-2,4,1,DIAMOND, \
	XSize/2-2,2,4,1,DIAMOND

#define LV_HORIZ_OBJECTS_SIZE LV_OBJECTS_SIZE(LV_HORIZ_NO_OF_OBJECTS)

//

#if !defined(FEWER_SHURIKENS) & XSize>=20
    #define LV_HORIZ_SHURIKENS \
        12, 0, 0, \
        \
        2,4, \
        2,7, \
        2,10, \
        2,YSize-9, \
        2,YSize-6, \
        2,YSize-3, \
        \
        XSize-3,3, \
        XSize-3,6, \
        XSize-3,9, \
        XSize-3,YSize-10, \
        XSize-3,YSize-7, \
        XSize-3,YSize-4
    
    #define LV_HORIZ_SHURIKENS_SIZE LV_SHURIKENS_SIZE(12,0,0)
 
#else
	#define LV_HORIZ_SHURIKENS \
		8, 0, 0, \
		\
		2,4, \
		2,7, \
		2,YSize-9, \
		2,YSize-3, \
		\
		XSize-3,3, \
		XSize-3,6, \
		XSize-3,YSize-10, \
		XSize-3,YSize-4 
		
	#define LV_HORIZ_SHURIKENS_SIZE LV_SHURIKENS_SIZE(8,0,0)

#endif



// 

#define LV_HORIZ_WALLS 0
#define LV_HORIZ_WALLS_SIZE LC_WALLS_SIZE(LV_HORIZ_WALLS)

