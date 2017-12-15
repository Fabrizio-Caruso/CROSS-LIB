// change program org
// #pragma output CRT_ORG_CODE = 24500   // an alternative to -zorg on the compile line

// move the stack to available memory
#pragma output REGISTER_SP = -1      // must be placed somewhere in the 16k ram

// halt program on exit, no need to play well with basic
#pragma output CRT_ON_EXIT = 0x10001

// eliminate the malloc heap
#pragma output CLIB_MALLOC_HEAP_SIZE = 0

// eliminate the stdio heap (no files can be opened)
#pragma output CLIB_STDIO_HEAP_SIZE = 0

// eliminate unused printf converters
#pragma printf = "%c %u"    // placed used converters here, empty string if none

