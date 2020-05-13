
// #define rnd_m 2345
// #define rnd_a 6789

// #define rnd_m 58653
// #define rnd_a 13849

// static unsigned int next = 1;
// int rand(void) // RAND_MAX assumed to be 32767
// {
    // next = next * rnd_m + rnd_a;
    // return next;
// }

#if defined(__COMX__) || defined(__PECOM__)




unsigned short int _rand_aux(unsigned char bits){
	asm( // LFSR is shifted 'bits' times stored in R12.0
	" ldireg R8, 0x407e\n"	
	" lda R8\n"	
	" phi R15\n"	
	" ldn 8\n"	
	" plo R15\n"	// R15&D = low byte of seed value
    "$$loop:\n"
	" glo R15\n"
	" ani 1\n"
	" str R2\n"		// Stack = bit 0 seed value, rest is 0
	" glo R15\n"	// D = low byte of seed value
	" shr\n"	    
	" shr\n"		
	" xor\n"		// D bit 0 = bit 0 XOR bit 2
	" plo R9\n"
	" ani 1\n"
	" str R2\n"		// Stack = bit 0 XOR bit 2
	" glo R9\n"
	" shr\n"		
	" xor\n"		// D bit 0 = bit 0/2 XOR bit 3
	" plo R9\n"
	" ani 1\n"
	" str R2\n"		// Stack = bit 0/2 XOR bit 3
	" glo R9\n"
	" shr\n"		
	" shr\n"		
	" xor\n"		// D bit 0 = bit 0/2/3  XOR bit 5
	" shr\n"		// DF = new input bit
	" ghi R15\n"
	" shrc\n"
	" phi R15\n"	// R15.1 >> 1, MSB = DF, new input bit, DF = LSB
	" dec R8\n"
	" str R8\n"		// Store high byte new seed value
	" glo R15\n"
	" shrc\n"
	" plo R15\n"	// R15.0 >> 1, MSB = DF, DF = LSB
	" inc R8\n"
	" str R8\n"		// Store low byte new seed value
	" dec R12\n"
	" glo R12\n"
	" lbnz $$loop\n"
	" Cretn\n");
	return 0; //this statement will never be executed but it keeps the compiler happy
}

int rand(void)
{ 
    return  _rand_aux(1)&0x7FFF;
}


#else
static unsigned long int next = 1;
int rand(void) // RAND_MAX assumed to be 32767
{
    next = next * 1103515245 + 12345;
    return (unsigned int)(next>>16)&0x7FFF;
}
#endif