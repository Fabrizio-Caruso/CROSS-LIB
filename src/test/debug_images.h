#ifndef _IMAGES_H
#define _IMAGES_H

#define _PLAYER_LEFT 42

typedef unsigned char uint8_t;

struct ImageStruct
{
	uint8_t _imageData;
	#if !defined(NO_COLOR)
		uint8_t _color;
	#endif
};

typedef struct ImageStruct Image;

#if !defined(ANIMATE_PLAYER)
	Image PLAYER_IMAGE;
#else

        // #if !defined(__LCC1802__)
    Image PLAYER_DOWN;
    Image PLAYER_UP;
    Image PLAYER_RIGHT;
    Image PLAYER_LEFT;
        // #endif

#endif


	Image GHOST_IMAGE;
	Image BOMB_IMAGE;	


#if !defined(NO_DEAD_GHOSTS)
	Image DEAD_GHOST_IMAGE;
#endif





#endif // _IMAGES_H

