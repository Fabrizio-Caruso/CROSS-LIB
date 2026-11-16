

void _XL_INIT_GRAPHICS(void)
{
    // Necessary to have .vz file correctly started on several emulators that do not load .vz as they should
    *((unsigned char *)0x7839) |= 1; 
}
