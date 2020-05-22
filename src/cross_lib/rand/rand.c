
// #define rnd_m 2345
// #define rnd_a 6789

// #define rnd_m 58653
// #define rnd_a 13849

/*
static unsigned int next = 1;
int rand(void) // RAND_MAX assumed to be 32767
{
    next = next * rnd_m + rnd_a;
    return next;
}
*/


#if defined(__COMX__) || defined(__PECOM__) || defined(__TMC600__)
    #include <comx/rca_rand.h>
#else
static unsigned long int next = 1;
int rand(void) // RAND_MAX assumed to be 32767
{
    next = next * 1103515245 + 12345;
    return (unsigned int)(next>>16)&0x7FFF;
}
#endif