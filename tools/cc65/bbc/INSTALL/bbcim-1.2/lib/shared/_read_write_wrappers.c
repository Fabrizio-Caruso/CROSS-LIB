/* _read_write_wrappers.c, Copyright (C) W.H.Scholten 2001,2002,2014
 *
 * Read/write wrappers (signals etc. can cause short read/write counts)
 */

#ifndef _whs__read_write_wrappers_c
#define _whs__read_write_wrappers_c

#include <stdio.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <stdlib.h>

#include <errno.h>
/* Prototypes: */
int my_write(int fd, void *data, int nbytes);
int my_read(int fd, void *data, int nbytes);
int my_fwrite(FILE *fp, void *data, int length, char *fn);
int my_fread(FILE *fp, void *data, int length, char *fn);


/* low level io: write/read wrappers: */
int my_write(int fd, void *data, int nbytes) {
	int bytes_written = 0;
	int bytes_left = nbytes;

	for (;;) {
		int st = write(fd, data, bytes_left);
		if (st < 0 && errno != EINTR) {
			//perror(write_file);
			//exit(1);
			return -1;
		}
		bytes_written += st;
		bytes_left -= st;
		if (bytes_written == nbytes) break;
	}

	return bytes_written;
}


int my_read(int fd, void *data, int nbytes) {
	int bytes_read = 0;
	int bytes_left = nbytes;

	for (;;) {
		int st = read(fd, data, bytes_left);
		if (st < 0 && errno != EINTR) {
			perror("my_read");
			//perror(write_file);
			//exit(1);
			return -1;
		}
		bytes_read += st;
		bytes_left -= st;
		if (bytes_read == nbytes || st == 0) break; /* done or EOF */
	}

	return bytes_read;
}

#if 0
int my_read_byte(int fd) {
	unsigned char my_byte;
	for (;;) {
		int st = read(fd, &my_byte, 1);
		if (st < 0 && errno != EINTR) {
			perror("write byte");
			exit(1);
		}
		if (st == 1) break;
	}

	return my_byte;
}
#endif


/* stdio: fwrite/fread wrappers: */
int my_fwrite(FILE *fp, void *data, int length, char *fn) {
	int  st;
	st = fwrite(data, 1, length, fp);

	if (st != length) {
		perror(fn);
		exit(1);
	}

	return 0;
}

/*  -> returns the number of bytes NOT read */
int my_fread(FILE *fp, void *data, int length, char *fn) {
	int  st;
	st = fread(data, 1, length, fp);

	if (st != length) {
		if (!feof(fp)) {
			perror(fn);
			exit(1);
		}
	}

	return length-st;
}

#endif /* _whs__read_write_wrappers_c */
