
// LEVEL 1 SHOWN AS 2

#if XSize<=20
    #define LV_VERT_BARS_WIDTH 2
#elif XSize<32
    #define LV_VERT_BARS_WIDTH (XSize/7-1)
#else
    #define LV_VERT_BARS_WIDTH (XSize/4-1)
#endif


#if XSize>=20
    #if YSize>25
        #define LV_VERT_BARS_NO_OF_OBJECTS 14

        #define LV_VERT_BARS_OBJECTS \
            LV_VERT_BARS_NO_OF_OBJECTS, \
            \
            5+LV_VERT_BARS_WIDTH,4,1,YSize-7,WALL, \
            XSize-6-LV_VERT_BARS_WIDTH,4,1,YSize-7,WALL, \
            \
            3,4,1,YSize-7,WALL, \
            XSize-4,4,1,YSize-7,WALL, \
            \
            XSize-3,4,1,1,RING, \
            XSize-3,YSize-4,1,1,FREEZE, \
            2,4,1,1,RING, \
            2,YSize-4,1,1,FREEZE, \
            \
            2,5,1,YSize-9,DIAMOND, \
            4,5,1,YSize-9,DIAMOND, \
            6+LV_VERT_BARS_WIDTH,7+4,1,YSize-7-6-8,DIAMOND, \
            XSize-7-LV_VERT_BARS_WIDTH,7+4,1,YSize-7-6-8,DIAMOND, \
            XSize-5,5,1,YSize-9,DIAMOND,  \
            XSize-3,5,1,YSize-9,DIAMOND
    #elif YSize>=16

        #define LV_VERT_BARS_NO_OF_OBJECTS 14

        #define LV_VERT_BARS_OBJECTS \
            LV_VERT_BARS_NO_OF_OBJECTS, \
            \
            5+LV_VERT_BARS_WIDTH,4,1,YSize-7,WALL, \
            XSize-6-LV_VERT_BARS_WIDTH,4,1,YSize-7,WALL, \
            \
            3,4,1,YSize-7,WALL, \
            XSize-4,4,1,YSize-7,WALL, \
            \
            XSize-3,4,1,1,RING, \
            XSize-3,YSize-4,1,1,FREEZE, \
            2,4,1,1,RING, \
            2,YSize-4,1,1,FREEZE, \
            \
            2,5,1,YSize-9,DIAMOND, \
            4,5,1,YSize-9,DIAMOND, \
            6+LV_VERT_BARS_WIDTH,7,1,YSize-7-6,DIAMOND, \
            XSize-7-LV_VERT_BARS_WIDTH,7,1,YSize-7-6,DIAMOND, \
            XSize-5,5,1,YSize-9,DIAMOND,  \
            XSize-3,5,1,YSize-9,DIAMOND
    #else
        #define LV_VERT_BARS_NO_OF_OBJECTS 14

        #define LV_VERT_BARS_OBJECTS \
            LV_VERT_BARS_NO_OF_OBJECTS, \
            \
            5+LV_VERT_BARS_WIDTH,4,1,YSize-7,WALL, \
            XSize-6-LV_VERT_BARS_WIDTH,4,1,YSize-7,WALL, \
            \
            3,4,1,YSize-7,WALL, \
            XSize-4,4,1,YSize-7,WALL, \
            \
            XSize-3,4,1,1,RING, \
            XSize-3,YSize-4,1,1,FREEZE, \
            2,4,1,1,RING, \
            2,YSize-4,1,1,FREEZE, \
            \
            2,5,1,3,DIAMOND, \
            4,5,1,3,DIAMOND, \
            6+LV_VERT_BARS_WIDTH,7,1,3,DIAMOND, \
            XSize-7-LV_VERT_BARS_WIDTH,7,1,3,DIAMOND, \
            XSize-5,5,1,3,DIAMOND,  \
            XSize-3,5,1,3,DIAMOND

    #endif

#else
    #if YSize>=16
        #define LV_VERT_BARS_NO_OF_OBJECTS 10

        #define LV_VERT_BARS_OBJECTS \
            LV_VERT_BARS_NO_OF_OBJECTS, \
            \
            3,4,1,YSize-7,WALL, \
            XSize-4,4,1,YSize-7,WALL, \
            \
            XSize-3,4,1,1,RING, \
            XSize-3,YSize-4,1,1,FREEZE, \
            2,4,1,1,RING, \
            2,YSize-4,1,1,FREEZE, \
            \
            2,5,1,YSize-9,DIAMOND, \
            4,5,1,YSize-9,DIAMOND, \
            \
            XSize-5,5,1,YSize-9,DIAMOND,  \
            XSize-3,5,1,YSize-9,DIAMOND    
    #else
        #define LV_VERT_BARS_NO_OF_OBJECTS 10

        #define LV_VERT_BARS_OBJECTS \
            LV_VERT_BARS_NO_OF_OBJECTS, \
            \
            3,4,1,YSize-7,WALL, \
            XSize-4,4,1,YSize-7,WALL, \
            \
            XSize-3,4,1,1,RING, \
            XSize-3,YSize-4,1,1,FREEZE, \
            2,4,1,1,RING, \
            2,YSize-4,1,1,FREEZE, \
            \
            2,5,1,2,DIAMOND, \
            4,5,1,2,DIAMOND, \
            \
            XSize-5,5,1,2,DIAMOND,  \
            XSize-3,5,1,2,DIAMOND    
    #endif
#endif

#define LV_VERT_BARS_OBJECTS_SIZE LV_OBJECTS_SIZE(LV_VERT_BARS_NO_OF_OBJECTS)


//

#define LV_VERT_BARS_SHURIKENS \
    2, 4, 0, \
	\
    3,3, \
    3,YSize-3, \
    \
    \
	\
    6,3, \
    1,3, \
    XSize-2,6, \
    XSize-2-5,6 \
	\
	\

#define LV_VERT_BARS_SHURIKENS_SIZE LV_SHURIKENS_SIZE(2,4,0)



#define LV_VERT_BARS_NO_OF_WALLS 2

#if XSize>=20
    #define LV_VERT_BARS_WALLS \
        LV_VERT_BARS_NO_OF_WALLS, \
        \
        6+LV_VERT_BARS_WIDTH,5, \
        XSize-12-2*LV_VERT_BARS_WIDTH, 1, \
        \
        6+LV_VERT_BARS_WIDTH,YSize-5, \
        XSize-12-2*LV_VERT_BARS_WIDTH,1

#else
    #define LV_VERT_BARS_WALLS \
        LV_VERT_BARS_NO_OF_WALLS, \
        \
        5,5, \
        XSize-8-2*1, 1, \
        \
        5,YSize-5, \
        XSize-8-2*1,1
#endif

#define LV_VERT_BARS_WALLS_SIZE LC_WALLS_SIZE(LV_VERT_BARS_NO_OF_WALLS)


