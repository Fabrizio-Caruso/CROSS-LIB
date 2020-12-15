#ifndef _INIT_IMAGES_H
#define _INIT_IMAGES_H

#if !defined(NO_COLOR)
    #define _SNAKE_COLOR COLOR_GREEN
    
    #define _WALL_COLOR COLOR_YELLOW
#endif

#if defined(NO_CONST_IMAGES)
    void INIT_IMAGES();
#else
    #define INIT_IMAGES()
#endif

#endif // _INIT_IMAGES_H

