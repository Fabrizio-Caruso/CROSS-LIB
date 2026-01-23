void send_t6834(char data) __stdc
{
#asm
; __stdc: params pushed right-to-left, so on stack (low to high):
; SP+0: return address (2 bytes)
; SP+2: data (1 byte, but word-aligned)

.wait			; wait for T6834
	in a,($f2)	;
	and $02		;
	jr z,wait	;
				; nop
	ei	
	ld a,($026c)	; INTIMAG
	or $80
				; ld ($026c),a	???		
	out ($f0),a
	di
				; nop
		
	ld hl,$02	; load byte to write from stack
	add hl,sp	; 
	ld a,(hl)	        ; a<-byte to write
	
	out ($f1),a	; send a to $F1
				; nop
		
	ld a,$02	; a<-2
	out ($f5),a
	
#endasm

}

// function 0X15 CIRCLE x,y center r radius
// this work
void x_circle(char x,y,r)
{
send_t6834(0x15);
send_t6834(x);
send_t6834(y);
send_t6834(r);
}

// function 0X06 WRITE a char in the T6384 address room
// addr where to be wrote
// data to write
void x_ramwrite(char data,unsigned int addr)
{
unsigned char low,high;

low=addr&0xff;
high=(addr>>8)&0xff;
// printf("%d %d",high,low);	
send_t6834(0x06);		// RAM WRITE command,



send_t6834(high);		// Address high byte
send_t6834(low); 		// Address low byte

send_t6834(data);		// Data byte

}


int main(void)
{
    int i;

    for(i=0;i<0x511;++i)
    {
        x_ramwrite(i,0xE000+i);
    };

    while(1){};
    return 0;
}
