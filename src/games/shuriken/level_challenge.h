

#define LV_CHALLENGE_OBJECTS \
    2, \
	\
    XSize/2-3,YSize/2-2,1,5,WALL, \
	XSize/2+2,YSize/2-2,1,5,WALL  



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

