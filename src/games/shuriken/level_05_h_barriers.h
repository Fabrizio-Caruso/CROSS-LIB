
// LEVEL 3

#define LV_H_BARRIERS_NO_OF_OBJECTS 24

#define LV_H_BARRIERS_BAR_SIZE XSize/7

#if YSize>=16
    #define LV_H_BARRIERS_OBJECTS \
        LV_H_BARRIERS_NO_OF_OBJECTS, \
        \
        XSize-2,4,1,YSize-1-2-4,WALL, \
        1,4,1,YSize-1-2-4,WALL, \
        \
        5,YSize-2,3,1,WALL, \
        5,2,3,1,WALL, \
        XSize-8,YSize-2,3,1,WALL, \
        XSize-8,2,3,1,WALL, \
        \
        \
        XSize-2,3,1,1,FREEZE, \
        XSize-2,YSize-2,1,1,RING, \
        1,3,1,1,FREEZE, \
        1,YSize-2,1,1,RING, \
        \
        8,4,1,2,BLOCK, \
        XSize-9,4,1,2,BLOCK, \
        \
        5,8, 1,1, WALL, \
        6+LV_H_BARRIERS_BAR_SIZE,8, 1,1, WALL, \
        5,YSize-6,1,1,WALL, \
        6+LV_H_BARRIERS_BAR_SIZE,YSize-6,1,1,WALL, \
        \
        XSize-7-LV_H_BARRIERS_BAR_SIZE,8, 1,1, WALL, \
        XSize-6,8, 1,1, WALL, \
        XSize-7-LV_H_BARRIERS_BAR_SIZE,YSize-6,1,1,WALL, \
        XSize-6,YSize-6,1,1,WALL, \
        \
        8,YSize-2,XSize-16,1,DIAMOND, \
        8,2,XSize-16,1,DIAMOND, \
        \
        XSize-2-1,7,1,YSize-1-2-4-6,DIAMOND, \
        2,7,1,YSize-1-2-4-6,DIAMOND \
        

#else
    #define LV_H_BARRIERS_OBJECTS \
        LV_H_BARRIERS_NO_OF_OBJECTS, \
        \
        XSize-2,4,1,YSize-1-2-4,WALL, \
        1,4,1,YSize-1-2-4,WALL, \
        \
        5,YSize-2,3,1,WALL, \
        5,2,3,1,WALL, \
        XSize-8,YSize-2,3,1,WALL, \
        XSize-8,2,3,1,WALL, \
        \
        \
        XSize-2,3,1,1,FREEZE, \
        XSize-2,YSize-2,1,1,RING, \
        1,3,1,1,FREEZE, \
        1,YSize-2,1,1,RING, \
        \
        8,4,1,2,BLOCK, \
        XSize-9,4,1,2,BLOCK, \
        \
        5,8, 1,1, WALL, \
        6+LV_H_BARRIERS_BAR_SIZE,8, 1,1, WALL, \
        5,3,1,1,WALL, \
        6+LV_H_BARRIERS_BAR_SIZE,3,1,1,WALL, \
        \
        XSize-7-LV_H_BARRIERS_BAR_SIZE,8, 1,1, WALL, \
        XSize-6,8, 1,1, WALL, \
        XSize-7-LV_H_BARRIERS_BAR_SIZE,3,1,1,WALL, \
        XSize-6,3,1,1,WALL, \
        \
        8,YSize-2,XSize-16,1,DIAMOND, \
        8,2,XSize-16,1,DIAMOND, \
        \
        XSize-2-1,7,1,3,DIAMOND, \
        2,7,1,3,DIAMOND \

#endif

#define LV_H_BARRIERS_OBJECTS_SIZE LV_OBJECTS_SIZE(LV_H_BARRIERS_NO_OF_OBJECTS)


//

#if XSize>=20  && !defined(FEWER_SHURIKENS)
    #define LV_H_BARRIERS_SHURIKENS \
        3,8,0, \
        \
        3,YSize-3, \
        XSize-5,6, \
        5,3, \
        \
        \
        3,3, \
        XSize-4,4, \
        7,5, \
        XSize-8,6, \
        6,YSize-3, \
        XSize-7,YSize-3, \
        4,7, \
        XSize-5,YSize-6 \
        \
        \

    #define LV_H_BARRIERS_SHURIKENS_SIZE LV_SHURIKENS_SIZE(3,8,0)


#else
    
    #define LV_H_BARRIERS_SHURIKENS \
        3,6,0, \
        \
        3,YSize-3, \
        XSize-5,6, \
        5,3, \
        \
        \
        3,3, \
        XSize-4,4, \
        6,YSize-3, \
        XSize-7,YSize-3, \
        4,7, \
        XSize-5,YSize-6 \
        \
        \

    #define LV_H_BARRIERS_SHURIKENS_SIZE LV_SHURIKENS_SIZE(3,6,0)

#endif




//

#define LV_H_BARRIERS_NO_OF_WALLS 4

#define LV_H_BARRIERS_WALLS \
    LV_H_BARRIERS_NO_OF_WALLS, \
	\
    6,8, \
    LV_H_BARRIERS_BAR_SIZE,1, \
    \
    6,YSize-6, \
    LV_H_BARRIERS_BAR_SIZE,1,\
    \
    XSize-7-XSize/7+1,8, \
    LV_H_BARRIERS_BAR_SIZE,1, \
    \
    XSize-7-XSize/7+1,YSize-6, \
    LV_H_BARRIERS_BAR_SIZE,1

#define LV_H_BARRIERS_WALLS_SIZE LC_WALLS_SIZE(LV_H_BARRIERS_NO_OF_WALLS)

