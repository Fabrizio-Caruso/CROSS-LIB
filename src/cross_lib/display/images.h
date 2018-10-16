#ifndef _IMAGES_H
#define _IMAGES_H


#if !defined(ANIMATE_PLAYER)
	Image PLAYER_IMAGE;
#else
	#if defined(USE_MEMORY_BUFFERS) || defined(USE_TINY_MEMORY_BUFFERS) || defined(USE_LIGHT_MEMORY_BUFFERS)
		extern Image PLAYER_DOWN;
		extern Image PLAYER_UP;
		extern Image PLAYER_RIGHT;
		extern Image PLAYER_LEFT;	
	#else
		Image PLAYER_DOWN;
		Image PLAYER_UP;
		Image PLAYER_RIGHT;
		Image PLAYER_LEFT;	
	#endif
#endif

#if defined(USE_MEMORY_BUFFERS) || defined(USE_TINY_MEMORY_BUFFERS) || defined(USE_LIGHT_MEMORY_BUFFERS)
	extern Image GHOST_IMAGE;
	extern Image BOMB_IMAGE;
#else
	Image GHOST_IMAGE;
	Image BOMB_IMAGE;	
#endif

#if !defined(NO_DEAD_GHOSTS)
	Image DEAD_GHOST_IMAGE;
#endif

#endif // _IMAGES_H

