#ifndef _RAND_H
#define _RAND_H

int rand(void);

// Normalized 15-bit pseudo-random number generator
#if defined(__NORMALIZED_RAND) || defined(__NCURSES__)
    #define _XL_RAND() (rand()&0x7FFF)
#else
    #define _XL_RAND() (rand())
#endif

#endif // _RAND_H

