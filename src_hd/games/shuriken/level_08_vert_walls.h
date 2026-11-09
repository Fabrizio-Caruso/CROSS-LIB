


// LEVEL 8 SHOWN AS 9

#define LV_VERT_WALLS_WIDTH (XSize/4)


#define LV_VERT_WALLS_NO_OF_OBJECTS 12

#if YSize>=16
    #define LV_VERT_WALLS_OBJECTS \
        LV_VERT_WALLS_NO_OF_OBJECTS, \
        \
        LV_VERT_WALLS_WIDTH,2,1,YSize-7,WALL, \
        XSize-1-LV_VERT_WALLS_WIDTH,6,1,YSize-7,WALL, \
        \
        1,YSize-2,1,1,RING, \
        XSize-2,2,1,1,RING, \
        \
        LV_VERT_WALLS_WIDTH+1,2,1,1,FREEZE, \
        XSize-1-LV_VERT_WALLS_WIDTH-1,YSize-2,1,1,FREEZE, \
        \
        XSize/2+1,YSize/2-1,1,4,BLOCK, \
        XSize/2-2,YSize/2-1,1,4,BLOCK, \
        \
        LV_VERT_WALLS_WIDTH-1,2,1,YSize-7,DIAMOND, \
        XSize-1-LV_VERT_WALLS_WIDTH+1,6,1,YSize-7,DIAMOND, \
        \
        2,YSize-2,LV_VERT_WALLS_WIDTH-2,1,DIAMOND, \
        XSize-1-LV_VERT_WALLS_WIDTH+1,2,LV_VERT_WALLS_WIDTH-2,1,DIAMOND
#else
    #define LV_VERT_WALLS_OBJECTS \
        LV_VERT_WALLS_NO_OF_OBJECTS, \
        \
        LV_VERT_WALLS_WIDTH,2,1,3,WALL, \
        XSize-1-LV_VERT_WALLS_WIDTH,6,1,3,WALL, \
        \
        1,YSize-2,1,1,RING, \
        XSize-2,2,1,1,RING, \
        \
        LV_VERT_WALLS_WIDTH+1,2,1,1,FREEZE, \
        XSize-1-LV_VERT_WALLS_WIDTH-1,YSize-2,1,1,FREEZE, \
        \
        XSize/2+1,YSize/2-1,1,4,BLOCK, \
        XSize/2-2,YSize/2-1,1,4,BLOCK, \
        \
        LV_VERT_WALLS_WIDTH-1,2,1,3,DIAMOND, \
        XSize-1-LV_VERT_WALLS_WIDTH+1,6,1,3,DIAMOND, \
        \
        2,YSize-2,LV_VERT_WALLS_WIDTH-2,1,DIAMOND, \
        XSize-1-LV_VERT_WALLS_WIDTH+1,2,LV_VERT_WALLS_WIDTH-2,1,DIAMOND

#endif

#define LV_VERT_WALLS_OBJECTS_SIZE LV_OBJECTS_SIZE(LV_VERT_WALLS_NO_OF_OBJECTS)


//
#if XSize>=20

    #define LV_VERT_WALLS_SHURIKENS \
        4, 6, 0, \
        \
        XSize-3,3, \
        3,YSize-3, \
        XSize-5,4, \
        5,YSize-4, \
        \
        \
        \
        2,3, \
        LV_VERT_WALLS_WIDTH+1,3, \
        XSize-3,6, \
        XSize-1-LV_VERT_WALLS_WIDTH-1,6, \
        LV_VERT_WALLS_WIDTH+2,3, \
        XSize-1-LV_VERT_WALLS_WIDTH-1-1,6  

    #define LV_VERT_WALLS_SHURIKENS_SIZE LV_SHURIKENS_SIZE(4,6,0)

#else
    
    #define LV_VERT_WALLS_SHURIKENS \
        4, 4, 0, \
        \
        XSize-3,3, \
        3,YSize-3, \
        XSize-5,4, \
        5,YSize-4, \
        \
        \
        \
        1,3, \
        LV_VERT_WALLS_WIDTH+1,3, \
        XSize-2,6, \
        XSize-1-LV_VERT_WALLS_WIDTH-1,6

    #define LV_VERT_WALLS_SHURIKENS_SIZE LV_SHURIKENS_SIZE(4,4,0)
#endif


#define LV_VERT_WALLS_NO_OF_WALLS 2


#define LV_VERT_WALLS_WALLS \
    LV_VERT_WALLS_NO_OF_WALLS, \
	\
    LV_VERT_WALLS_WIDTH,YSize-5, \
    1, 4, \
    \
    XSize-1-LV_VERT_WALLS_WIDTH,2, \
    1, 4 \

#define LV_VERT_WALLS_WALLS_SIZE LC_WALLS_SIZE(LV_VERT_WALLS_NO_OF_WALLS)


