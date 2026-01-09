
#include <sys/ioctl.h>
void _XL_INIT_GRAPHICS(void)
{
    int height = 4;
    console_ioctl(IOCTL_GENCON_SET_FONT_H,&height);
}

