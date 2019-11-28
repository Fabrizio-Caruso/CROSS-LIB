/* _bbc_filenames.c
 *
 * By W.H.Scholten 1996-2001,2009
 * This file is released to the public domain.
 */


#include <stdio.h>
#include <string.h>

#include "_bbc_filenames.h"

/* Following code is from bbcim 0.99: */
char _bbc_filename_chars_bad[]     = { '\\', '/', ':', '*', '?', '\"', '<', '>', '|', '`', '\'' };
char _bbc_filename_chars_replace[] = { '_' , '_', '_', '_', '_', '_' , '_', '_', '_', '_', '_'  };

/* Number of char's to be checked for replacement */
#define  no_filename_chars  (sizeof(_bbc_filename_chars_bad) / sizeof(char))

void bbc_replace_bad_chars(char *unix_bbcfile_naam) {
	/* Name check: replace certain characters in filenames (most filesystems
	 * can't handle all characters from bbc filenames)
	 */
	int i,j;

	for (j=0; j<strlen(unix_bbcfile_naam); j++) {
		if (unix_bbcfile_naam[j] < 32 || unix_bbcfile_naam[j] > 126)
			unix_bbcfile_naam[j] = '_';
		else {
			for (i=0; i<no_filename_chars; i++) {
				if (unix_bbcfile_naam[j] == _bbc_filename_chars_bad[i]) {
					unix_bbcfile_naam[j] = _bbc_filename_chars_replace[i];
					break;
				}
			}
		}
	}
}


/* returns True if all chars are printable (>=32 and <=126), False if not;
 * Unprintable characters are replaced by '?' */
int bbc_printable_bbcfilename(char *name) {
	int printable = True;
	int i;

	for (i=strlen(name)-1; i>=0; i--) {
		if (((unsigned char)name[i]) < 32 || ((unsigned char)name[i]) > 126) {
			name[i] = '?';
			printable = False;
		}
	}

	return printable;
}


/* Rename a file by appending a number.
 * The function checks to see whether that file exists, if not it returns with value 1.
 * If the file name is too long it returns with value -1 without completing the task.
 */
int bbc_rename_sequential(char *name) {
	FILE *fp;
	static char new_name[ 256 ];
	int i = 0;

	if (strlen(name) > 250) return -1;

	strcpy(new_name, name);
	for(;;) {
		fp = fopen(new_name, "r");
		if (fp == NULL) break;
		fclose(fp);
		strcpy(new_name, name);
		sprintf(new_name+strlen(new_name), "%d", i);
		i++;
		if (i > 100000) return -1; /* resulting name is too long */
	}
	strcpy(name, new_name);

	return 1;
}


