 def _start
 def _init_data

# Entry point for C runtime initilization code
#
# This will set initial values for data memory and otherwise create
# the conditions needed for the C environment
_start:

# Disable interrupts
#
# This will disable console reset via keyboard, music playback,
# sprite automotion, etc. But will make the environment much simpler
# to work in. Users can re-enable interrupts later if they wish.
  limi 0

# Set initial workspace
#
# The registers will be located at the start of scratchpad memory 
  lwpi >8300

# Initialize data segment to known values
  li   r0, _init_data  # Point to data initialization structure
  mov  *r0+, r1        # R1 = Start of data section
  mov  *r0+, r2        # R2 = Location of initial data
  mov  *r0+, r3        # R3 = Size of data section
  jeq  data_copy_end   # If size is 0, skip the loop
  
  # Copy loop for data initialization
data_copy_top:
  mov  *r2+, *r1+      
  dect r3
  jgt  data_copy_top
data_copy_end:

# Initialize BSS segment to zeroes
  mov  *r0+, r1        # R1 = Start of BSS section
  mov  *r0, r2         # R2 = Size of BSS section
  jeq  bss_clear_end   # If size is 0, skip the loop

  # Clearing loop for BSS initialization
bss_clear_top:
  clr  *r1+
  dect r2
  jgt  bss_clear_top
bss_clear_end:

# zero the 'zero value' at >8320
  clr @>8320

# Create the stack
  li sp, >4000

# Start running C code
  bl @main

# reset the console on return
  blwp @>0000


# Data initialization structure
#
# This structure will be filled in by the ELF conversion program
# after link time.
_init_data:
  data   # Start of data section
  data   # Location of initial contents of data section
  data   # Size of data section
  data   # Start of BSS section
  data   # Size of BSS section

