
// DOC on VDU
// https://www.bbcbasic.co.uk/bbcbasic/manual/vdu.html#vdu22

/*
0 	Null - it does nothing.
1 	Send the next character to the printer ONLY.
2 	Enable the printer.
3 	Disable the printer.
4 	Write text at the text cursor position.
5 	Write text at the graphics cursor position.
6 	Enable output to the screen.
7 	Bell - make a short 'beep'.
8 	Move the text cursor backwards one character.
9 	Move the text cursor forwards one character.
10 	Move the text cursor down one line.
11 	Move the text cursor up one line.
12 	Clear the text area - identical to CLS.
13 	Move the text cursor to the start of the current line.
14 	Enable the auto-paging mode.
15 	Disable the auto-paging mode.
16 	Clear the graphics area - identical to CLG.
17 	Define a text colour - identical to COLOUR.
18 	Define a graphics colour - identical to GCOL.
19 	Select a colour palette.
20 	Restore the default logical colours.
21 	Disable output to the screen.
22 	Select the screen mode - identical to MODE.
23 	Program characters and control the cursor.
24 	Define a graphics window.
25 	Identical to PLOT.
26 	Restore the default text and graphics windows.
27 	Send the next character to the screen.
28 	Define a text window.
29 	Set the graphics origin.
30 	Home the text cursor to the top left of the screen.
31 	Move the text cursor - identical to TAB(x,y).
127 	Backspace and delete.
*/


#include <stdint.h>
#include <stdio.h>

static uint8_t stripes[] = {255,0,255,0,255,0,255,0};

void redefine(uint8_t ch, uint8_t *data)
{
    uint8_t i;
    
    putchar(23);
    putchar(ch);
    for(i=0;i<8;++i)
    {
        putchar(data[i]);
    }
    putchar('\n');
}


void _XL_INIT_GRAPHICS(void)
{

	__vdu_sequence(1);
    // Set mode 3
    putchar(22);
    putchar(3);
    
    // Delete graphics screen data
    // putchar(16);
    
    // Redefine character 240
    // redefine(240,stripes);
    
    // Redefine character 241
    // redefine(241,stripes);
    
    // Display some text
    // putchar('O');
    // putchar('K');
    
    // printf("hello world\n");
    
    // Display character 240
    // putchar(240);
    
    // Display character 241
    // putchar(241);
    
    // Display "hello world"
    // printf("\nhello world\n");
    
    while(1){};
}
