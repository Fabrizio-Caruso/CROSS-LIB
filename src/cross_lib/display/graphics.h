#  if defined(__C64__)
	#include "cc65/c64/c64_memory_mapped_settings.h"
#elif defined(__CBM__) && defined(MEMORY_MAPPED)
	#include "cc65/petscii_memory_mapped_settings.h"
#elif defined(__CMOC__) && !defined(__WINCMOC__)
	#include "cmoc/cmoc_memory_mapped_settings.h"
#else
	#include "default_graphics_settings.h"
#endif 