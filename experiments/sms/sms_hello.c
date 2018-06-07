#include <stdio.h>

#include <sms.h>

#define VDP_REG_HBL_COUNTER 0x8A


int main()
{

	unsigned char pal1[] = {0x2A, 0x3F, 0x0F, 0x00, 0x01, 0x02, 0x03, 0x17,
					0x15, 0x35, 0x1D, 0x3D, 0x17, 0x37, 0x1F, 0x03};

	unsigned char pal2[] = {0x2A, 0x3F, 0x0F, 0x00, 0x10, 0x20, 0x30, 0x35,
					0x15, 0x35, 0x1D, 0x3D, 0x17, 0x10, 0x20, 0x03};	
	load_palette(pal1, 0, 16); 
	load_palette(pal2, 16, 16); 

	set_vdp_reg(VDP_REG_FLAGS1, VDP_REG_FLAGS1_BIT7 | VDP_REG_FLAGS1_SCREEN); 	
	
	
	gotoxy(2,2);
	printf("Hello world 0!\n");	

	gotoxy(2,4);
	printf("Key pressed 1\n");	

	gotoxy(2,6);
	printf("Key pressed 2\n");	

	gotoxy(2,8);
	printf("Key pressed 3\n");	
		
	while(1){};
		
	return 0;
}

