/* _bbc_archive_read_attributes.c
 *
 * Functions to read Acorn BBC computer file attributes (.inf) and disk image attributes (.dsk)
 * These functions originated in 'bbcim'. Later separated for use in 'bbctape', 'bbccom' and other programs.


   Copyright (C) W.H. Scholten 1996, 1997, 2001, 2003, 2009, 2014
 
   Permission to use, copy, modify, distribute, and sell this software
   and its documentation for any purpose is hereby granted without fee,
   provided that the above copyright notice appear in all copies and
   that both that copyright notice and this permission notice appear in
   supporting documentation, and that the name of the copyright holder
   not be used in advertising or publicity pertaining to distribution
   of the software without specific, written prior permission. The
   copyright holder makes no representations about the suitability of
   this software for any purpose. It is provided "as is" without express
   or implied warranty.
  
   THE COPYRIGHT HOLDER DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS
   SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
   FITNESS, IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
   SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER
   RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF
   CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
   CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <_bbc_archive.h>

/* Prototypes for internal functions: */
int bbc_archive__string_to_hex(char *string, unsigned int *hex);
void skip_whitespace(char *string, int *ptr);
void skip_nonwhitespace(char *string, int *ptr);
int bbc_archive__diskinfo_from_string(char *string, struct bbc_archive__diskimage_attr *image);


/* Returns 0 if ok, anything else if not */
int bbc_archive__string_to_hex(char *string, unsigned int *hex) {
	unsigned int  i = 0;
	int  st;

	for (i=0; i<strlen(string); i++) {
		if (string [ i ] != ' ') break;
	}

	st = 0;
	for (   ; i<strlen(string); i++) {
		if (!isxdigit(string [ i ])) { st = -1; break; }
	}
	if (st == 0) st = sscanf(string, "%x", hex) - 1;

	return st;
}


void skip_whitespace(char *string, int *ptr) {
	while(string[*ptr] == ' ' || string[*ptr] == '\t' || string[*ptr] == '\n' || string[*ptr] == '\r')
		(*ptr)++;
}


void skip_nonwhitespace(char *string, int *ptr) {
	while(string[*ptr] != ' ' && string[*ptr] != '\t' && string[*ptr] != '\n' && string[*ptr] != '\r' && string[*ptr] != 0)
		(*ptr)++;
}


int bbc_archive__read_file_attributes(char *filename, struct bbc_archive__file_attr *file_attr, int verbose) {
	char *info_arg[] = {
		"TAPE", "filename", "load address", "exec address", "length", "locked", "CRC=", "crc", "NEXT", "next tapefile"
	};
//	int  info_optional[] = { 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1 };
	int  info_optional[] = { 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1 }; /* Optional CRC */
	int  st;
	char *filename_inf = NULL;
	FILE *fp_inf = NULL;
	char line[ 257 ];
	int  item_wanted;
	int  status = -1; /* function's return status */

	file_attr->name = NULL;
	file_attr->next_tape_file = NULL;
	file_attr->crc = -1;

	filename_inf = (char *) malloc(strlen(filename)+1+4);
	if (filename_inf == NULL) {
		fprintf(stderr, "Out of memory\n");
		return -1;
	}

	strcpy(filename_inf, filename);
	strcat(filename_inf, ".inf");

	fp_inf = fopen(filename_inf, "r");
	if (fp_inf == NULL) {
		if (verbose) printf("Unable to open %s\n", filename_inf);
		goto leave_read_file_attributes;
	}

#define BBC_ARCHIVE_ATTR_TAPE          0
#define BBC_ARCHIVE_ATTR_NAME          1
#define BBC_ARCHIVE_ATTR_LOADADDR      2
#define BBC_ARCHIVE_ATTR_EXECADDR      3
#define BBC_ARCHIVE_ATTR_LENGTH        4
#define BBC_ARCHIVE_ATTR_LOCKED        5
#define BBC_ARCHIVE_ATTR_CRCEQUALS     6
#define BBC_ARCHIVE_ATTR_CRC           7
#define BBC_ARCHIVE_ATTR_NEXT          8
#define BBC_ARCHIVE_ATTR_NEXTTAPEFILE  9

	item_wanted = BBC_ARCHIVE_ATTR_TAPE;


	fgets(line, 256, fp_inf);
	line[256] = 0;

	int str_start = 0;
	int str_end = 0;
	char string[256];

	for (;;) {
		str_start = str_end;
		skip_whitespace(line, &str_start);
		str_end = str_start;
		skip_nonwhitespace(line, &str_end);
		if (str_end == str_start) {
			while (info_optional[ item_wanted ] && item_wanted <= BBC_ARCHIVE_ATTR_NEXTTAPEFILE) item_wanted ++;
			if (item_wanted >= BBC_ARCHIVE_ATTR_NEXTTAPEFILE) break;

			if (verbose) fprintf(stderr, "%s: missing argument (%d: %s)\n", filename_inf, item_wanted, info_arg[ item_wanted ]);
			goto leave_read_file_attributes;
		}
		strncpy(string, line+str_start, str_end - str_start);
		string[str_end - str_start] = 0;
		//printf("string=%s (len=%d)\n", string, strlen(string));


		switch (item_wanted) {
		case BBC_ARCHIVE_ATTR_TAPE:
			if (!strcmp(string, "TAPE")) {
				item_wanted ++;
				file_attr->type = BBC_ARCHIVE__FILETYPE__TAPE;
				continue;
			}

			/* Disk file */
			file_attr->type = BBC_ARCHIVE__FILETYPE__DFS;
			/* Fall through .. */
			item_wanted ++;

		case BBC_ARCHIVE_ATTR_NAME:
			if (file_attr->type == BBC_ARCHIVE__FILETYPE__DFS) {
				file_attr->name = strdup(string);
				if (file_attr->name == NULL) {
					fprintf(stderr, "Out of memory\n");
					exit(1);
				}
			}
			if (file_attr->type == BBC_ARCHIVE__FILETYPE__TAPE) {
				int  i;

				file_attr->name = (char *) malloc(strlen(string)+1+2);

				if (strlen(string) > 10) {
					if (verbose) fprintf(stderr, "%s: tape name %s: too long\n", filename_inf, string);
					goto leave_read_file_attributes;
				}

				/* Tape filenames converted to $.<tapename> for uniform
				 * handling of tape/disk. */
				file_attr->name[0] = '$';
				file_attr->name[1] = '.';
				for (i=strlen(string); i>=0; i--) file_attr->name[i+2] = string[i];
			}

			break;

		case BBC_ARCHIVE_ATTR_LOADADDR:
			st = bbc_archive__string_to_hex(string, &file_attr->load_address);

			if (st != 0) {
				if (verbose) fprintf(stderr, "%s: read load address failed\n", filename_inf);
				goto leave_read_file_attributes;
			}
			break;

		case BBC_ARCHIVE_ATTR_EXECADDR:
			st = bbc_archive__string_to_hex(string, &file_attr->exec_address);
			if (st != 0) {
				if (verbose) fprintf(stderr, "%s: read exec address failed\n", filename_inf);
				goto leave_read_file_attributes;
			}
			break;

		case BBC_ARCHIVE_ATTR_LENGTH:
			st = bbc_archive__string_to_hex(string, &file_attr->length);
			if (st == 0) {
				break;
			}
			/* Fall through .. */
			item_wanted ++;

		case BBC_ARCHIVE_ATTR_LOCKED:
			file_attr->attributes = 0;
			if ((!strcmp(string, "L")) || (!strcmp(string, "Locked"))) {
				file_attr->attributes |= BBC_ARCHIVE__ATTR__LOCKED;
				break;
			}

			/* Fall through .. */
			item_wanted ++;

		case BBC_ARCHIVE_ATTR_CRCEQUALS:
			//printf("BBC_ARCHIVE_ATTR_CRCEQUALS:\n");
			if (!strncmp(string, "CRC=", 4)) {
				str_end = str_start+4;
			}
			else {
				//if (debug) printf("CRC not found\n");
				/* Crc not found */
				//file_attr->crc = -1;
				/* should be a fatal error */
				//goto leave_read_file_attributes;
				item_wanted ++; /* skip BBC_ARCHIVE_ATTR_CRC */
			}
			break;

		case BBC_ARCHIVE_ATTR_CRC:
			//printf("BBC_ARCHIVE_ATTR_CRC:\n");
		{
			unsigned int tmp_crc;
			st = bbc_archive__string_to_hex(string, &tmp_crc);
			file_attr->crc = (int) tmp_crc;
		}
			if (st != 0) {
				file_attr->crc = -1;
				/* not a fatal error */
				//goto leave_read_file_attributes;
			}
			else {
				/* Check the range */
				if (file_attr->crc < 0 || file_attr->crc > 65535) {
					file_attr->crc = -1;
				}
			}
			break;

		case BBC_ARCHIVE_ATTR_NEXT:
			if (strcmp(string, "NEXT")) item_wanted ++; /* no NEXT so skip NEXTTAPEFILE */
			break;
		case BBC_ARCHIVE_ATTR_NEXTTAPEFILE:
			file_attr->next_tape_file=strdup(string);
			break;
		} /* case */

		item_wanted ++;

	}

	status = 0;

  leave_read_file_attributes:
	if (filename_inf) free(filename_inf);
	if (fp_inf) fclose(fp_inf);

	return status;
}


void bbc_archive__free_file_attributes(struct bbc_archive__file_attr *file_attr) {
	if (file_attr->name)
		free(file_attr->name);

	if (file_attr->next_tape_file)
		free(file_attr->next_tape_file);
}


/* Should be internal? But used in bbcim for command line processing */
int bbc_archive__diskinfo_from_string(char *string, struct bbc_archive__diskimage_attr *image) {
	if (!strncmp(string, "DFS", 10)) image->type = BBC_ARCHIVE__DFS_DISK;
	else if (!strncmp(string, "W62", 10)) image->type = BBC_ARCHIVE__W62_DISK;
	else if (!strncmp(string, "DDOS", 10)) image->type = BBC_ARCHIVE__DDOS_DISK;
	else if (!strncmp(string, "SOLIDISK_DDFS", 20)) image->type = BBC_ARCHIVE__SOLIDISK_DDFS_DISK;
	else if (!strncmp(string, "ADFS", 10)) image->type = BBC_ARCHIVE__ADFS_DISK;
	else if (!strncmp(string, "HDFS", 10)) image->type = BBC_ARCHIVE__HDFS_DISK;
	else if (!strncmp(string, "DS", 10)) image->sides = 2;
	else if (!strncmp(string, "SS", 10)) image->sides = 1;
	else return -1;

	return 0;
}


int bbc_archive__read_disk_attributes(char *diskname, struct bbc_archive__diskimage_attr *image_attr, int verbose) {
	FILE *fpdsk;
	char *dsk;
	char type [ 11 ];
	char sides [ 11 ];
	int  status = -1;

	dsk = (char *) malloc(strlen(diskname)+1+4);
	if (dsk == NULL) {
		fprintf(stderr, "Out of memory\n");
		return -1;
	}
	strcpy(dsk, diskname);
	strcat(dsk, ".dsk");

	fpdsk = fopen(dsk, "r");
	if (fpdsk == NULL) {
		if (verbose) fprintf(stderr, "Unable to open %s\n", dsk);
		return -1;
	}

	/* Read disk attributes */
	if (fscanf(fpdsk, "%10s %10s %dT %dS %dB", type, sides, &image_attr->tracks, &image_attr->sectors_per_track, &image_attr->bytes_per_sector) != 5) {
		if (verbose) fprintf(stderr, "Bad .dsk format\n");
		goto leave_read_disk_attributes;
	}

	if (bbc_archive__diskinfo_from_string(type, image_attr) || bbc_archive__diskinfo_from_string(sides, image_attr)) {
		if (verbose) fprintf(stderr, "Bad .dsk format\n");
		goto leave_read_disk_attributes;
	}
	status = 0;

  leave_read_disk_attributes:
	if (dsk) free(dsk);
	if (fpdsk) fclose(fpdsk);

	return status;
}


void bbc_archive__free_disk_attributes(struct bbc_archive__diskimage_attr *disk) {
}
