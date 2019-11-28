/* BBCIM: Acorn BBC computer diskimage manipulation
 *
 * archive_test.c
 * ENGLISH: reads file attributes from the inf/dsk files
 *
 * this file is public domain, by W.H.Scholten 2001, 2009
 */



#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <_bbc_archive.h>

int main(int argc, char **argv) {
	int  st;
//	struct bbc_archive__file file;
	struct bbc_archive__file_attr file_attr;

//	struct bbc_archive__diskimage disk;
	struct bbc_archive__diskimage_attr disk_attr;

	if (argc < 2) {
		fprintf(stderr, "use: test <filename>\n");
		exit(1);
	}

//	file.filename = argv[1];
//	st = bbc_archive__read_file_attributes(file.filename, &file_attr, 1);

	st = bbc_archive__read_file_attributes(argv[1], &file_attr, 1);
	if (st < 0) {
		fprintf(stderr, "bbc_archive__read_file_attributes returned error\n");
		bbc_archive__free_file_attributes(&file_attr);
	}
	else {
		int  i;
		char printable_name [ 16 ];
		int  printable_all = 1;

//		file.attr = file_attr;

		strcpy(printable_name, file_attr.name);
		for (i=0; i<strlen(file_attr.name); i++) {
			if (printable_name[i] < 32 || printable_name[i] >= 127) {
				printable_name[i] = '?';
				printable_all = 0;
			}
		}

		printf("info gives: %s %s %6X %6X %s CRC=%4X\n", printable_name, printable_all ? "" : " (contains unprintable characters) ", file_attr.load_address, file_attr.exec_address, ((file_attr.attributes & BBC_ARCHIVE__ATTR__LOCKED) ? "Locked" : "Not locked"), file_attr.crc);
		printf("file_attr.type=%d\n", file_attr.type);
		printf("file_attr.attributes=%d\n", file_attr.attributes);
		bbc_archive__free_file_attributes(&file_attr);
	}

	printf("\n");

	st = bbc_archive__read_disk_attributes(argv[1], &disk_attr, 1);
	if (st < 0) {
		fprintf(stderr, "bbc_archive__read_disk_attributes returned error\n");
		//bbc_archive__free_disk_attributes(&disk_attr);
	}
	else {
		printf(".dsk gives:\n"
			   " type=%d, sides=%d, tracks=%d, sec per track=%d, bps=%d\n", disk_attr.type, disk_attr.sides, disk_attr.tracks, disk_attr.sectors_per_track, disk_attr.bytes_per_sector);
	}

	return 0;
}
