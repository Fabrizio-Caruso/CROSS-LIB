/* Entry point for C runtime initilization code
*
* This will set initial values for data memory and otherwise create
* the conditions needed for the C environment */
extern int main(void);

#ifdef __cplusplus
extern "C"
#endif

void _start2(void)
{
  /* Fill .data section with initial values */
  {
    extern int __VAL_START;
    extern int __DATA_START;
    extern int __DATA_END;
    char *src = (char*)&__VAL_START;
    char *dst = (char*)&__DATA_START;
    while(dst < (char*)&__DATA_END)
    {
      *dst++ = *src++;
    }
  } 

  /* Erase .bss section */
  {
    extern int __BSS_START;
    extern int __BSS_END;
    char *dst = (char*)&__BSS_START;
    while(dst < (char*)&__BSS_END)
    {
      *dst++ = 0;
    }
  }

#ifdef __cplusplus
  /* Call all initial constructors.
  * The .ctors section is filled with function pointers to constructors. 
  * Iterate through them and call each function. */
  {
    extern int __CTOR_START;
    extern int __CTOR_END;
    typedef void(*funcptr)(void);
    funcptr *ctor = (funcptr*)&__CTOR_START;
    while(ctor < (funcptr*)&__CTOR_END)
    {
      (*(ctor++))();
    }
  }
#endif

  /* Start running C code */
  main();

  /* We cannot properly handle a return from C code.
  * Instead, just spin here until the console is reset */
  while(1);
}

#ifdef __cplusplus
extern "C"
#endif
void _start(void)
{
  /* Disable interrupts
  *
  * This will disable console reset via keyboard, music playback,
  * sprite automotion, etc. But will make the environment much simpler
  * to work in. Users can re-enable interrupts later if they wish. */
  asm("\tlimi 0");

  /* Set initial workspace
  *
  * The registers will be located at the start of scratchpad memory */
  asm("\tlwpi >8300");

  /* Create the stack before declaring any variables */
  asm("\tli sp, >4000");

  /* Call the second part directly to prevent a stack for this function */
  asm("\tb @_start2");
}


