// BOSS LEVELS: 3, 7, 11 SHOWN AS 4, 8, 12


#if YSize>=20
	#define LV_CHALLENGE_OBJECTS \
		4, \
		\
		XSize/2-3,YSize/2-2,1,5,WALL, \
		XSize/2+2,YSize/2-2,1,5,WALL, \
		\
		(XSize-4)/2-3,YSize-5,1,1,RING, \
		(XSize-4)/2+6,YSize-5,1,1,FREEZE
#elif YSize>=18
	#define LV_CHALLENGE_OBJECTS \
		4, \
		\
		XSize/2-3,YSize/2-1,1,3,WALL, \
		XSize/2+2,YSize/2-1,1,3,WALL, \
		\
		(XSize-4)/2-3,YSize-5,1,1,RING, \
		(XSize-4)/2+6,YSize-5,1,1,FREEZE

#else
	#define LV_CHALLENGE_OBJECTS \
		4, \
		\
		XSize/2-3,YSize/2,1,1,WALL, \
		XSize/2+2,YSize/2,1,1,WALL, \
		\
		(XSize-4)/2-3,YSize-5,1,1,RING, \
		(XSize-4)/2+6,YSize-5,1,1,FREEZE

#endif


#define LV_CHALLENGE_NO_OF_WALLS 2

#define LV_CHALLENGE_WALLS \
	LV_CHALLENGE_NO_OF_WALLS, \
	\
	(XSize-4)/2,5, \
	4, 1, \
	\
	(XSize-4)/2,YSize-5, \
	4,1


#define LV_CHALLENGE_WALLS_SIZE LC_WALLS_SIZE(LV_CHALLENGE_NO_OF_WALLS)

