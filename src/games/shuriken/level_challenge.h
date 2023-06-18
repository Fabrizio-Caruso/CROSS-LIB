
// #if XSize<=20
    #define LV_CHALLENGE_WIDTH 2
// #else
    // #define LV_CHALLENGE_WIDTH (XSize/7-1)
// #endif

#define LV_CHALLENGE_NO_OF_WALLS 2


#define LV_CHALLENGE_WALLS \
    LV_CHALLENGE_NO_OF_WALLS, \
	\
    (XSize-4)/2,5, \
    4, 1, \
    \
    (XSize-4)/2,YSize-5, \
    4,1 \

#define LV_CHALLENGE_WALLS_SIZE LC_WALLS_SIZE(LV_CHALLENGE_NO_OF_WALLS)


#define LV_EXTRA_OBJECTS \
    4, \
    (XSize-4)/2-2,5,2,1,WALL, \
    (XSize-4)/2+4,5,2,1,WALL, \
    \
    (XSize-4)/2-2,YSize-5,2,1,WALL, \
    (XSize-4)/2+4,YSize-5,2,1,WALL