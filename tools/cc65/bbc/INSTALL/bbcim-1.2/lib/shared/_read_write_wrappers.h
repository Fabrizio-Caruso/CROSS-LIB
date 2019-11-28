/* _read_write_wrappers.h, by W.H.Scholten 2001,2002,2014
 *
 * Read/write wrappers (signals etc. can cause short read/write counts)
 */
#ifndef _whs__read_write_wrappers_h
#define _whs__read_write_wrappers_h

#include <stdio.h>
int my_write(int fd, void *data, int nbytes);
int my_read(int fd, void *data, int nbytes);
int my_fwrite(FILE *fp, void *data, int length, char *fn);
int my_fread(FILE *fp, void *data, int length, char *fn);

#endif /* _whs__read_write_wrappers_h */
