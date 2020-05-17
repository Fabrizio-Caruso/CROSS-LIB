#ifndef _RAND_H
#define _RAND_H

#if !defined(__LCC1802__)
int rand(void);
#endif

// Normalized 15-bit pseudo-random number generator
#if defined(__NCURSES__)
    #define RAND() (rand()&0x7FFF)
#else
    #define RAND() (rand())
#endif

#endif // _RAND_H

