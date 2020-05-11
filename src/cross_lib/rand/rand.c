

#if defined(__COMX__) || defined(__PECOM__)
static unsigned int next = 1;
int rand(void) // RAND_MAX assumed to be 32767
{
    next = next * 115 + 125;
    return next;
}

#else
static unsigned long int next = 1;
int rand(void) // RAND_MAX assumed to be 32767
{
    next = next * 1103515245 + 12345;
    return (unsigned int)(next>>16)&0x7FFF;
}
#endif