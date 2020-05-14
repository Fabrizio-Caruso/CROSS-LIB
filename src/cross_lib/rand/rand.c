
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

int rand(void)
{
	asm( // LFSR is shifted 'bits' times stored in R12.0
	" ldireg R8, 0x42be\n"		// COMX BASIC RND seed value
	" lda R8\n"	
	" bnz $$rand1\n"			// Do rand if high byte of seed value != 0
    "$$zerolow:\n"
	" glo R0\n"					
	" bz $$zerolow\n"			// Wait until R0.0 != 0
	" phi R15\n"				// Use R0.0 as low byte of seed value
    "$$zerohigh:\n"
	" ghi R0\n"
	" bnz $$rand2\n"			// If R0.1 != 0 use as high byte of seed value
	" br $$zerohigh\n"			// Wait until R0.1 != 0
   "$$rand1:\n"
	" phi R15\n"	// R15.1 = high byte of seed value
	" ldn 8\n"	
   "$$rand2:\n"
	" plo R15\n"	// R15.0 = low byte of seed value
	" ani 1\n"
	" str R2\n"		// Stack = bit 0 of seed value, rest is 0
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
	" dec R8\n"
	" str R8\n"		// Store high byte new seed value, 
					// R15.1 >> 1, MSB = DF, new input bit, DF = LSB
	" ani 0x7f\n"
	" phi R15\n"	// R15.1 = seed value & 0x7f, allowing only postivie numbers
	" glo R15\n"
	" shrc\n"
	" plo R15\n"	// R15.0 >> 1, MSB = DF, DF = LSB
	" inc R8\n"
	" str R8\n"		// Store low byte new seed value
	" Cretn\n");
	return 0; //this statement will never be executed but it keeps the compiler happy
}


#else
static unsigned long int next = 1;
int rand(void) // RAND_MAX assumed to be 32767
{
    next = next * 1103515245 + 12345;
    return (unsigned int)(next>>16)&0x7FFF;
}
#endif