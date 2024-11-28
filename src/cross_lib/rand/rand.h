#ifndef _RAND_H
#define _RAND_H

// Normalized 15-bit pseudo-random number generator
#if defined(__NORMALIZED_RAND) || defined(__NCURSES__) || defined(__TERMINAL__)
    #define _XL_RAND() (rand()&0x7FFF)
#else
    #define _XL_RAND() (rand())
#endif

#endif // _RAND_H

