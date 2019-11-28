/* libtest.c,  Copyright (C) 1997-2001, 2009 W.H. Scholten
 *
 * BBCIM: Acorn BBC computer disk image manipulation
 *
 * Tests libbbcim
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <bbcim.h>
#include <_bbc_archive.h>


void bbcimtest_list_bbc_files(char *dir);
void bbcimtest_list_bbc_disks(char *dir);

int  bbcimtest_find_file(char *dirname, char *filename, int action);
#define BBCIM_FIND_FILE           1
#define BBCIM_CLOSE_DIR           2

int main(int argc, char **argv) {
    struct bbc_archive__diskimage image;
    struct bbc_archive__diskimage_attr image_attr;

	struct bbc_archive__file file;
	struct bbc_archive__file_attr file_attr;
	struct bbcim_file_data file_data;

	int  i, crc;
	int  st;

	char *mydisk;

	if (argc > 1) mydisk=argv[1]; else mydisk="ttt.w62";

	printf("mydisk=%s\n", mydisk);

	printf("libbbcim test\n");

	printf("listing all bbc disks:\n");
	bbcimtest_list_bbc_disks(".");

	printf("\nlisting all bbc files:\n");
	bbcimtest_list_bbc_files(".");
	printf("\n");

	image.attr = &image_attr;
	file.attr = &file_attr;

	image_attr.type = BBC_ARCHIVE__DFS_DISK;
	image_attr.sides = 1;
	image.filename = mydisk;
	printf("image.filename=%s\n", image.filename);

//	disk_catalogue_extract_crc(&image, 0, NULL, &file, SHOW_FILES | VERBOSE);
	st = bbcim_disk_cat_extract_crc(&image, 0 /*side*/, &file, BBCIM_NEW_DISK, 1);
	if (st) exit(1);
	st = bbcim_disk_cat_extract_crc(&image, 0 /*side*/, &file, BBCIM_GET_ATTRIBUTES, 1);
	if (st) exit(1);
	printf("Attributes\n");
	printf(" file %s, %d bytes\n", file.attr->name, file.attr->length);
	printf(" load=%6x exec=%6x\n", file.attr->load_address, file.attr->exec_address);

	file_data.data = (unsigned char *) malloc(file.attr->length + 1);
	st = bbcim_disk_cat_extract_crc(&image, 0 /*side*/, &file, BBCIM_LOAD_FILE, 1);
	if (st) exit(1);
	printf("Loaded file %s, %d bytes\n", file.attr->name, file.attr->length);
	printf(" load=%6x exec=%6x\n", file.attr->load_address, file.attr->exec_address);

	/* 'Proof': calculate crc from data */
	crc = 0;

	for (i=0; i<file.attr->length; i++) {
		int k;
		int filebyte = file_data.data[i];

		crc ^= (filebyte << 8);
		for (k=0; k<8; k++) {
			if (crc & 32768)
				crc = (((crc ^ 0x0810) & 32767) << 1)+1;
			else
				crc = crc << 1;
		}
	}

	printf("crc from data is %x\n", crc);
	free(file_data.data);


	return 0;
}


/* ------------------------------------------------------------------------- */
/* DEMO...  Tests all files and tells if they are bbc files */
void bbcimtest_list_bbc_files(char *dir) {
	char filename[256];
	struct bbc_archive__file file;
	struct bbc_archive__file_attr file_attr;

	file.attr = &file_attr;

	printf("checking all bbc files\n");
	while (bbcimtest_find_file(dir, filename, BBCIM_FIND_FILE) != -1) {
		file.filename = filename;
		if (!bbc_archive__read_file_attributes(filename, file.attr, 0)) {
			printf(" %s is a bbc file\n", filename);
		}
	}
}


/* DEMO...  Tests all files and tells if they are bbc disks */
void bbcimtest_list_bbc_disks(char *dir) {
	char filename[256];
	struct bbc_archive__diskimage image;
	struct bbc_archive__diskimage_attr image_attr;

	image.attr = &image_attr;

	printf("checking all bbc disks\n");
	while (bbcimtest_find_file(dir, filename, BBCIM_FIND_FILE) != -1) {
		image.filename = filename;
		if (!bbc_archive__read_disk_attributes(filename, image.attr, 0))
			printf(" %s is a bbc disk\n", filename);
	}
}


#  include <sys/types.h>
#  include <dirent.h>


/* A readdir wrapper: returns 0 if ok, -1 if no more files */
int bbcimtest_find_file(char *dirname, char *filename, int action) {
	static struct dirent *currentdirentry;
	static DIR *thisdir;
	static int dir_open;

	switch(action) {
	case BBCIM_FIND_FILE:
		if (!dir_open)
			thisdir = opendir(dirname);
		dir_open = 1;
		for (;;) {
			currentdirentry = readdir(thisdir); /* next entry */
			if (currentdirentry == NULL) { /* last entry */
				closedir(thisdir);
				dir_open = 0;
				return -1;
			}
			if (strcmp(currentdirentry->d_name, ".") && strcmp(currentdirentry->d_name, "..")) break;
		}
		strcpy(filename, currentdirentry->d_name);
		/* printf("Filename %s\n", currentdirentry->d_name); */
		break;
	case BBCIM_CLOSE_DIR:
		if (dir_open)
			closedir(thisdir);
		dir_open = 0;
	}

	return 0;
}

