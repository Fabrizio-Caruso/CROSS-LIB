/*  LibSAP
 *  Version 0.9.4
 *  Copyright (C) 2000-2003 Eric Botcazou
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "libsap.h"


int sap_errno;


#define FILENAME_LENGTH 128
#define MAX_ARCHIVE       8

#define SAP_MAGIC_NUM  0xB3
#define SAP_HEADER_SIZE  66
static const char sap_header[]="\0SYSTEME D'ARCHIVAGE PUKALL S.A.P. "
                               "(c) Alexandre PUKALL Avril 1998";


static const int sap_format_table[][3] = {
   { SAP_NTRACKS1, SAP_SECTSIZE1, SAP_TRACKSIZE1 },
   { SAP_NTRACKS2, SAP_SECTSIZE2, SAP_TRACKSIZE2 }
};

#define SAP_NTRACKS(f)      sap_format_table[f-SAP_FORMAT1][0]
#define SAP_SECTSIZE(f)     sap_format_table[f-SAP_FORMAT1][1]
#define SAP_TRACKSIZE(f)    sap_format_table[f-SAP_FORMAT1][2]

#define SAP_EXTSECTSIZE(f)  (SAP_SECTSIZE(f) + sizeof(sapsector_t) - SAP_SECTSIZE1)


static short int crcpuk_temp;

static short int puktable[]={
   0x0000, 0x1081, 0x2102, 0x3183,
   0x4204, 0x5285, 0x6306, 0x7387,
   0x8408, 0x9489, 0xa50a, 0xb58b,
   0xc60c, 0xd68d, 0xe70e, 0xf78f
};


#define NO_ARCHIVE     0
#define EMPTY_ARCHIVE  1
#define FILLED_ARCHIVE 2
#define FULL_ARCHIVE   3

static struct {
   int format;
   int state;
   int ntracks;
   FILE *file;
} archive[MAX_ARCHIVE+1];

#define ID_FORMAT(id)   archive[id].format
#define ID_STATE(id)    archive[id].state
#define ID_NTRACKS(id)  archive[id].ntracks
#define ID_FILE(id)     archive[id].file


/* constants for the TO logical disk format */
#define TO_NSECTS                16
#define TO_SECTOR_PER_BLOCK      8

#define TO_SECTSIZE1             255  /* not 256 */
#define TO_FAT_START1            257
#define TO_NBLOCKS1              160
#define TO_BLOCKSIZE1            (TO_SECTOR_PER_BLOCK*TO_SECTSIZE1)
#define TO_FILESIZE_MAX1         (TO_NBLOCKS1-2)
#define TO_DIR_START1            512

#define TO_SECTSIZE2             127  /* not 128 */
#define TO_FAT_START2            129
#define TO_NBLOCKS2              80
#define TO_BLOCKSIZE2            (TO_SECTOR_PER_BLOCK*TO_SECTSIZE2)
#define TO_FILESIZE_MAX2         (TO_NBLOCKS2-2)
#define TO_DIR_START2            256

#define TO_FILLER_BYTE           0xE5
#define TO_TAG_RESERVED          0xFE
#define TO_TAG_FREE              0xFF
#define TO_END_BLOCK_OFFSET      0xC0

#define TO_DIRENTRY_LENGTH       32
#define TO_NAME                  0
#define TO_NAME_LENGTH           8
#define TO_EXT                   8
#define TO_EXT_LENGTH            3
#define TO_FILE_TYPE             11
#define TO_DATA_TYPE             12
#define TO_FIRST_BLOCK           13
#define TO_END_SIZE              14
#define TO_COMMENT               16
#define TO_COMMENT_LENGTH        8
#define TO_DATE_DAY              24
#define TO_DATE_MONTH            25
#define TO_DATE_YEAR             26
#define TO_CHG_MODE              30
#define TO_CHG_CHECKSUM          31

#define TO_DIRENTRY_PER_SECTOR1  8
#define TO_NDIRENTRIES1          (TO_NSECTS-2)*TO_DIRENTRY_PER_SECTOR1

#define TO_DIRENTRY_PER_SECTOR2  4
#define TO_NDIRENTRIES2          (TO_NSECTS-2)*TO_DIRENTRY_PER_SECTOR2

static const int to_format_table[][8] = {
   { TO_SECTSIZE1, TO_FAT_START1, TO_NBLOCKS1, TO_BLOCKSIZE1,
     TO_FILESIZE_MAX1, TO_DIR_START1, TO_DIRENTRY_PER_SECTOR1, TO_NDIRENTRIES1 },
   { TO_SECTSIZE2, TO_FAT_START2, TO_NBLOCKS2, TO_BLOCKSIZE2,
     TO_FILESIZE_MAX2, TO_DIR_START2, TO_DIRENTRY_PER_SECTOR2, TO_NDIRENTRIES2 }
};

#define TO_SECTSIZE(f)             to_format_table[f-SAP_FORMAT1][0]
#define TO_FAT_START(f)            to_format_table[f-SAP_FORMAT1][1]
#define TO_NBLOCKS(f)              to_format_table[f-SAP_FORMAT1][2]
#define TO_BLOCKSIZE(f)            to_format_table[f-SAP_FORMAT1][3]
#define TO_FILESIZE_MAX(f)         to_format_table[f-SAP_FORMAT1][4]
#define TO_DIR_START(f)            to_format_table[f-SAP_FORMAT1][5]
#define TO_DIRENTRY_PER_SECTOR(f)  to_format_table[f-SAP_FORMAT1][6]
#define TO_NDIRENTRIES(f)          to_format_table[f-SAP_FORMAT1][7]


/* constants for the TO memory layout */
#define TO_BANKSIZE             16384


/* file types */
enum {
   FTYPE_BASIC,
   FTYPE_DATA,
   FTYPE_MACHINE,
   FTYPE_ASM,
   FTYPE_OTHERS,
   FTYPE_PARAGRAPHE = 0xA
};

static char ftype_sym[5] = { 'B', 'D', 'M', 'A', '*' };

enum {
   DTYPE_BINARY,
   DTYPE_ASCII = 0xFF,
   DTYPE_OTHERS = 2
};

static char dtype_sym[3] = { 'B', 'A', '*' };

struct extension_t {
   char tag[TO_EXT_LENGTH+1];
   unsigned char ftype;
   unsigned char dtype;
};

enum {
   EXT_TYPE_BAS,
   EXT_TYPE_DAT,
   EXT_TYPE_ASC,
   EXT_TYPE_ASM,
   EXT_TYPE_BIN,
   EXT_TYPE_MAP,
   EXT_TYPE_CHG,
   EXT_TYPE_CFG,
   EXT_TYPE_BAT,
   EXT_TYPE_CAR,
   EXT_TYPE_PAR,
   EXT_TYPE_UNKNOWN,
   EXT_TYPE_MAX
};

static struct extension_t ext_type[EXT_TYPE_MAX] = {
   { "BAS", FTYPE_BASIC,      DTYPE_BINARY },
   { "DAT", FTYPE_DATA,       DTYPE_ASCII  },
   { "ASC", FTYPE_DATA,       DTYPE_ASCII  },
   { "ASM", FTYPE_ASM,        DTYPE_ASCII  },
   { "BIN", FTYPE_MACHINE,    DTYPE_BINARY },
   { "MAP", FTYPE_MACHINE,    DTYPE_BINARY },
   { "CHG", FTYPE_DATA,       DTYPE_ASCII  },
   { "CFG", FTYPE_MACHINE,    DTYPE_BINARY },
   { "BAT", FTYPE_BASIC,      DTYPE_BINARY },
   { "CAR", FTYPE_DATA,       DTYPE_ASCII  },
   { "PAR", FTYPE_PARAGRAPHE, DTYPE_BINARY },
   { "???", FTYPE_MACHINE,    DTYPE_BINARY }
};


#define MIN(x,y)     (((x) < (y)) ? (x) : (y))



/************************************************/
/***    private functions: helper routines    ***/
/************************************************/


/* get_id:
 *  Finds the first empty id, returns 0 if has failed.
 */
static int get_id(void)
{
   int i;

   for (i=MAX_ARCHIVE; i>0; i--) {
      if (archive[i].state == NO_ARCHIVE)
         break;
   }

   return i;
}



/* clean_string:
 *  Helper function to clean up character strings.
 */
static inline void clean_string(unsigned char *str)
{
   while (*str) {
      if ((*str<32) || (*str>127))
         *str = '#';

      str++;
   }
}



/************************************************/
/***   private functions: SAP format support  ***/
/************************************************/


/* crc_pukall:
 *  Computes the new CRC from the c data.
 */
static void crc_pukall(short int c)
{
   short int index;

   index = (crcpuk_temp ^ c) & 0xf;
   crcpuk_temp = ((crcpuk_temp>>4) & 0xfff) ^ puktable[index];

   c >>= 4;

   index = (crcpuk_temp ^ c) & 0xf;
   crcpuk_temp = ((crcpuk_temp>>4) & 0xfff) ^ puktable[index];
}



/* do_crc:
 *  Computes the CRC for the specified SAP sector.
 */
static void do_crc(sapsector_t *sapsector, int format)
{
   int i;

   crcpuk_temp = 0xffff;

   crc_pukall(sapsector->format);
   crc_pukall(sapsector->protection);
   crc_pukall(sapsector->track);
   crc_pukall(sapsector->sector);

   for (i=0; i<SAP_SECTSIZE(format); i++)
      crc_pukall(sapsector->data[i]);
}



/* do_read_sector: 
 *  Performs the low-level read operation for the specified sector.
 */
static void do_read_sector(sapID id, sapsector_t *sapsector)
{
   unsigned char buffer[sizeof(sapsector_t)];
   int format = ID_FORMAT(id);
   int i;
   size_t unused_size_t;

   unused_size_t=fread(buffer, sizeof(char), SAP_EXTSECTSIZE(format), ID_FILE(id));

   sapsector->format     = buffer[0];
   sapsector->protection = buffer[1];
   sapsector->track      = buffer[2];
   sapsector->sector     = buffer[3];

   for (i=0; i<SAP_SECTSIZE(format); i++)
       sapsector->data[i] = buffer[4+i]^SAP_MAGIC_NUM;

   sapsector->data[SAP_SECTSIZE(format)] = buffer[4+i];
   sapsector->data[SAP_SECTSIZE(format)+1] = buffer[4+i+1];
   (void)unused_size_t;
}



/* do_write_sector:
 *  Performs the low-level write operation for the specified sector.
 */
static void do_write_sector(sapID id, sapsector_t *sapsector)
{
   unsigned char buffer[sizeof(sapsector_t)];
   int format = ID_FORMAT(id);
   int i;

   /* fill in the write buffer */
   buffer[0] = sapsector->format;
   buffer[1] = sapsector->protection;
   buffer[2] = sapsector->track;
   buffer[3] = sapsector->sector;

   for (i=0; i<SAP_SECTSIZE(format); i++)
      buffer[4+i] = sapsector->data[i]^SAP_MAGIC_NUM;

   /* compute the CRC */
   do_crc(sapsector, format);
   buffer[4+i] = sapsector->data[SAP_SECTSIZE(format)] = (crcpuk_temp>>8)&0xff;
   buffer[4+i+1] = sapsector->data[SAP_SECTSIZE(format)+1] = crcpuk_temp&0xff;

   fwrite(buffer, sizeof(char), SAP_EXTSECTSIZE(format), ID_FILE(id));
}



/* seek_pos:
 *  Seeks the specified position in the SAP archive.
 */
static void seek_pos(sapID id, int track, int sect)
{
   int pos;

   pos = SAP_HEADER_SIZE + (track*SAP_NSECTS + sect-1) * SAP_EXTSECTSIZE(ID_FORMAT(id));

   fseek(ID_FILE(id), pos, SEEK_SET);
}



/************************************************/
/***  private functions: Thomson DOS support  ***/
/************************************************/



/* get_file_size:
 *  Returns the size in blocks of the specified file on disk.
 */
static int get_file_size(int format, int n, const unsigned char trk20_data[])
{
   const unsigned char *fat_data, *entry_data;
   int block, size = 0;

   fat_data = trk20_data + TO_FAT_START(format);
   entry_data = trk20_data + TO_DIR_START(format) + n*TO_DIRENTRY_LENGTH;

   block = entry_data[TO_FIRST_BLOCK];

   while ((block<TO_NBLOCKS(format)) && (size<TO_FILESIZE_MAX(format))) {
      size++;
      block = fat_data[block];
   }

   return size;
}



/* get_dskf:
 *  Returns the amount of free space in blocks on disk.
 */
static int get_dskf(int format, const unsigned char trk20_data[])
{
   const unsigned char *fat_data;
   int i, dskf = 0;

   fat_data = trk20_data + TO_FAT_START(format);

   for (i=0; i<TO_NBLOCKS(format); i++) {
      if (fat_data[i] == TO_TAG_FREE)
         dskf++;
   }

   return dskf;
}



/* extract_dir_entry:
 *  Extracts one dir entry and returns the number of characters.
 */
static int extract_dir_entry(int format, char buffer[], int buffer_size, int n, const unsigned char trk20_data[])
{
   const unsigned char *entry_data;
   char name[TO_NAME_LENGTH+1], ext[TO_EXT_LENGTH+1], date[9], *comment;
   unsigned char ftype, dtype;
   int i, len, size;

   entry_data = trk20_data + TO_DIR_START(format) + n*TO_DIRENTRY_LENGTH;

   comment = malloc(TO_COMMENT_LENGTH+1);

   if ((entry_data[TO_NAME] == 0) || (entry_data[TO_NAME] == TO_TAG_FREE))
      return 0;

   if (entry_data[TO_NAME] == 1) {
      /* display only comment (if any) */
      if (entry_data[TO_COMMENT]) {
         comment[0] = '\0';
         strncat(comment, (const char *)(entry_data+TO_COMMENT), TO_COMMENT_LENGTH);

         for (i=1; i<TO_COMMENT_LENGTH; i++) {
            if (comment[i] == 18) { /* DC2 (repetition) ? */
               len = strlen(comment);
               comment = realloc(comment, len + comment[i+1]);
               memmove(comment+i+comment[i+1], comment+i+2, len - (i+2) + 1);
               memset(comment+i, comment[i-1], comment[i+1]);
            }
         }

         return sprintf(buffer, "%30s%s\n", "", comment);
      }
      else {
         return 0;
      }
   }

   /* name */
   name[0] = 0;
   strncat(name, (const char *)(entry_data+TO_NAME), TO_NAME_LENGTH);
   clean_string((unsigned char *)name);

   /* extension */
   ext[0] = 0;
   strncat(ext, (const char *)(entry_data+TO_EXT), TO_EXT_LENGTH);
   clean_string((unsigned char *)ext);

   /* file type */
   ftype = entry_data[TO_FILE_TYPE];
   ftype = (ftype > 3 ? ftype_sym[4] : ftype_sym[ftype]);

   /* data type */
   dtype = entry_data[TO_DATA_TYPE];
   dtype = (dtype == 0 ? dtype_sym[0] : (dtype == 0xFF ? dtype_sym[1] : dtype_sym[2]));

   /* size */
   size = get_file_size(format, n, trk20_data) * ((TO_BLOCKSIZE(format) + 1024) / 1024);

   /* date */
   if ((entry_data[TO_DATE_DAY] >= 1) && (entry_data[TO_DATE_DAY] <= 31) &&
       (entry_data[TO_DATE_MONTH] >= 1) && (entry_data[TO_DATE_MONTH] <= 12) &&
       (entry_data[TO_DATE_YEAR] <= 99))
      sprintf(date, "%2d-%2d-%2d", entry_data[TO_DATE_DAY], entry_data[TO_DATE_MONTH], entry_data[TO_DATE_YEAR]);
   else
      date[0] = 0;

   /* comment */
   comment[0] = 0;
   if (entry_data[TO_COMMENT])
      strncat(comment, (const char *)(entry_data+TO_COMMENT), TO_COMMENT_LENGTH);

   /* display the entry */
   i = sprintf(buffer, "%-8s %-3s %c %c %-3d %-8s %s\n", name, ext, ftype, dtype, size, date, comment);
   free(comment);

   return i;

   (void) buffer_size;  /* DJGPP 2.03 lacks snprintf() */
}



/* get_filename:
 *  Detects whether the file entry is valid and returns the filename if so.
 */
static int get_filename(char filename[], const unsigned char entry_data[])
{
   int i, j;

   if ((entry_data[TO_NAME] == 0) || (entry_data[TO_NAME] == TO_TAG_FREE))
      return -1;

   /* name */
   strncpy(filename, (const char *)(entry_data+TO_NAME), TO_NAME_LENGTH);

   for (i=0; i<TO_NAME_LENGTH; i++) {
      if (filename[i] == 32)
         break;
   }

   filename[i++] = '.';
   filename[i] = 0;

   /* extension */
   strncat(filename, (const char *)(entry_data+TO_EXT), TO_EXT_LENGTH);

   for (j=0; j<TO_EXT_LENGTH; j++) {
      if (filename[i+j] == 32)
         break;
   }

   if (filename[i+j-1] == '.')
      filename[i+j-1] = 0;
   else
      filename[i+j] = 0;

   return 0;
}



/* decode_filename:
 *  Decodes the specified raw filename into a valid dir entry.
 */
static void decode_filename(unsigned char entry_data[], const char filename[], int file_size)
{
   const char *p;
   char shortname[128];
   char ext[TO_EXT_LENGTH+1];
   int i, type, len, ext_len = 0;
   FILE *file;

   /* zero the entry data */
   memset(entry_data + TO_NAME, 32, TO_NAME_LENGTH + TO_EXT_LENGTH);
   entry_data[TO_COMMENT] = 0;
   memset(entry_data + TO_COMMENT+1, 32, TO_COMMENT_LENGTH-1);
   memset(entry_data + TO_DATE_DAY, 0, TO_DIRENTRY_LENGTH - TO_DATE_DAY);

   /* find short name */
   len = strlen(filename);
   p = filename + len - 1;   

   while ((p>=filename) && (*p != '/'))
      p--;

   shortname[0] = 0;
   strncat(shortname, p+1, sizeof(shortname) - 1);

   /* find extension */
   len = strlen(shortname);
   p = shortname + len - 1;

   while ((p>=shortname) && (*p != '.'))
      p--;

   if (p < shortname) {  /* no extension? */
      memcpy(entry_data+TO_NAME, shortname, MIN(TO_NAME_LENGTH, len));
      type = EXT_TYPE_MAX-1;
   }
   else {
      /* name */
      memcpy(entry_data+TO_NAME, shortname, MIN(TO_NAME_LENGTH, p - shortname));

      /* extension */
      ext_len = MIN(TO_EXT_LENGTH, shortname + len - 1 - p);
      memcpy(entry_data+TO_EXT, p+1, ext_len);

      /* build upper case extension */
      ext[0] = 0;
      strncat(ext, p+1, ext_len);
      for (i=0; i<ext_len; i++)
         ext[i] = toupper(ext[i]);

      /* search for a standard extension */
      for (type=0; type<EXT_TYPE_MAX-1; type++) {
         if (strcmp(ext_type[type].tag, ext) == 0)
            break;
      }
   }

   /* set file and data types */
   entry_data[TO_FILE_TYPE] = ext_type[type].ftype;
   entry_data[TO_DATA_TYPE] = ext_type[type].dtype;

   /* special treatment */
   switch (type) {

      case EXT_TYPE_BAS:
         /* differentiate ASCII from BINARY data */
         file = fopen(filename, "rb");
         if (file) {
            if ((fgetc(file) == '\r') && (fgetc(file) == '\n'))
               entry_data[TO_DATA_TYPE] = DTYPE_ASCII;
            fclose(file);
         }
         break;

      case EXT_TYPE_CHG:
         /* automatic launch + number of requested banks */
         entry_data[TO_CHG_MODE] = 0x80 | ((file_size/TO_BANKSIZE)&0x3F);

         /* 8-bit checksum */
         for (i=0; i<TO_NAME_LENGTH; i++)
            entry_data[TO_CHG_CHECKSUM] += entry_data[i];
         break;
   }
}



/* seek_file:
 *  Seeks a file in the specified directory and returns its index number if found.
 */
static int seek_file(int format, const char filename[], const unsigned char dir_data[])
{
   char entry_name[TO_NAME_LENGTH + 1 + TO_EXT_LENGTH + 1];
   int i;

   for (i=0; i<TO_NDIRENTRIES(format); i++) {
      if ((get_filename(entry_name, dir_data+i*TO_DIRENTRY_LENGTH) == 0) && (strcmp(entry_name, filename) == 0))
         return i;
   }

   return -1;
}



/* wildcard handling code by Michael Bulkin (M.A.Bukin@inp.nsk.su) */
#define WCD_MATCH_TRY 0
#define WCD_MATCH_ONE 1
#define WCD_MATCH_ANY 2


struct WCD_MATCH_DATA
{
   int type;
   const char *s1;
   const char *s2;
};



/* wcdcmp:
 *  Compares two strings ('*' matches any number of characters,
 *  '?' matches any character).
 */
static int wcdcmp(const char *s1, const char *s2)
{
   static unsigned int size = 0;
   static struct WCD_MATCH_DATA *data = NULL;
   const char *s1end;
   int index, c1, c2;

   /* handle NULL arguments */
   if ((!s1) && (!s2)) {
      if (data) {
         free(data);
         data = NULL;
      }

      return 1;
   }

   s1end = s1 + strlen(s1);

   /* allocate larger working area if necessary */
   if (data && (size < strlen(s2))) {
      free(data);
      data = NULL;
   }

   if (!data) {
      size = strlen(s2);
      data = malloc(sizeof(struct WCD_MATCH_DATA) * size * 2 + 1);
      if (!data)
         return 1;
   }

   index = 0;
   data[0].s1 = s1;
   data[0].s2 = s2;
   data[0].type = WCD_MATCH_TRY;

   while (index >= 0) {
      s1 = data[index].s1;
      s2 = data[index].s2;
      c1 = *s1;
      c2 = *s2;

      switch (data[index].type) {

      case WCD_MATCH_TRY:
         if (c2 == 0) {
            /* pattern exhausted */
            if (c1 == 0)
               return 0;
            else
               index--;
         }
         else if (c1 == 0) {
            /* string exhausted */
            while (*s2 == '*')
               s2++;
            if (*s2 == 0)
               return 0;
            else
               index--;
         }
         else if (c2 == '*') {
            /* try to match the rest of pattern with empty string */
            data[index++].type = WCD_MATCH_ANY;
            data[index].s1 = s1end;
            data[index].s2 = s2 + 1;
            data[index].type = WCD_MATCH_TRY;
         }
         else if ((c2 == '?') || (c1 == c2)) {
            /* try to match the rest */
            data[index++].type = WCD_MATCH_ONE;
            data[index].s1 = s1 + 1;
            data[index].s2 = s2 + 1;
            data[index].type = WCD_MATCH_TRY;
         }
         else
            index--;
         break;

      case WCD_MATCH_ONE:
         /* the rest of string did not match, try earlier */
         index--;
         break;

      case WCD_MATCH_ANY:
         /* rest of string did not match, try add more chars to string tail */
         if (--data[index + 1].s1 >= s1) {
            data[index + 1].type = WCD_MATCH_TRY;
            index++;
         }
         else
            index--;
         break;

      default:
         /* this is a bird? This is a plane? No it's a bug!!! */
         return 1;
      }
   }

   return 1;
}



/* seek_wilcard:
 *  Seeks a wildcard in the specified directory and returns its index number if found.
 */
static int seek_wildcard(int format, char filename[], const char pattern[], const unsigned char dir_data[])
{
   char entry_name[TO_NAME_LENGTH + 1 + TO_EXT_LENGTH + 1];
   static int i = 0;
   int j;

   while (i<TO_NDIRENTRIES(format)) {
      j = i++;

      if ((get_filename(entry_name, dir_data+j*TO_DIRENTRY_LENGTH) == 0) && (wcdcmp(entry_name, pattern) == 0)) {
         strcpy(filename, entry_name);
         return j;
      }
   }

   i = 0;

   /* to avoid leaking memory */
   wcdcmp(NULL, NULL);

   return -1;
}



/* find_free_block_sym:
 *  Finds a free block using a symmetrical search.
 */
static int find_free_block_sym(int format, unsigned char fat_data[])
{
   int block;

   block = TO_NBLOCKS(format)/2 - 1;

   while (block < TO_NBLOCKS(format)) {
      if (fat_data[block] == TO_TAG_FREE)
         return block;

      if (fat_data[TO_NBLOCKS(format) - block - 2] == TO_TAG_FREE)  /* fat_data[-1] is always 0 */
         return TO_NBLOCKS(format) - block - 2;

      block++;
   }

   return -1;
}



/* find_free_block_rel:
 *  Finds a free block using a relative search.
 */
static int find_free_block_rel(int format, int block, unsigned char fat_data[])
{
   if (block < TO_NBLOCKS(format)/2) {
      while (block >= 0) {
         if (fat_data[block] == TO_TAG_FREE)
            break;

         block--;
      }
   }
   else {
      while (block < TO_NBLOCKS(format)) {
         if (fat_data[block] == TO_TAG_FREE)
            break;

         block++;
      }
   }

   if ((block<0) || (block>=TO_NBLOCKS(format)))
      block = find_free_block_sym(format, fat_data);

   return block;
}



/* do_add_file:
 *  Performs the low-level add operation for the specified file entry.
 */
static void do_add_file(sapID id, FILE *file, int file_size, unsigned char entry_data[], int n, unsigned char trk20_data[])
{
   sapsector_t sapsector;
   unsigned char *fat_data;
   unsigned int block, next_block;
   int sect;
   size_t unused_size_t;

   /* find the first block */
   fat_data = trk20_data + TO_FAT_START(ID_FORMAT(id));

   block = find_free_block_sym(ID_FORMAT(id), fat_data);
   entry_data[TO_FIRST_BLOCK] = block;

   sapsector.format = 0;
   sapsector.protection = 0;

   /* write full blocks */
   while (file_size > TO_BLOCKSIZE(ID_FORMAT(id))) {
      sapsector.track =  block/2;
      sapsector.sector = 1 + (block%2 ? TO_SECTOR_PER_BLOCK : 0);
      seek_pos(id, sapsector.track, sapsector.sector);

      for (sect=0; sect<TO_SECTOR_PER_BLOCK; sect++) {
         unused_size_t=fread(sapsector.data, sizeof(char), TO_SECTSIZE(ID_FORMAT(id)), file);
         do_write_sector(id, &sapsector);
         sapsector.sector++;
      }

      fat_data[block] = TO_TAG_RESERVED;  /* temporarily reserved */
      next_block = find_free_block_rel(ID_FORMAT(id), block, fat_data);

      fat_data[block] = next_block;
      block = next_block;

      file_size -= TO_BLOCKSIZE(ID_FORMAT(id));
   }

   /* write remaining full sectors */
   sapsector.track = block/2;
   sapsector.sector = 1 + (block%2 ? TO_SECTOR_PER_BLOCK : 0);
   seek_pos(id, sapsector.track, sapsector.sector);

   fat_data[block] = TO_END_BLOCK_OFFSET + 1;

   while (file_size > TO_SECTSIZE(ID_FORMAT(id))) {
      unused_size_t=fread(sapsector.data, sizeof(char), TO_SECTSIZE(ID_FORMAT(id)), file);
      do_write_sector(id, &sapsector);
      sapsector.sector++;

      fat_data[block]++;
      file_size -= TO_SECTSIZE(ID_FORMAT(id));
   }

   /* write remaining individual bytes */
   unused_size_t=fread(sapsector.data, sizeof(char), TO_SECTSIZE(ID_FORMAT(id)), file);
   do_write_sector(id, &sapsector);

   entry_data[TO_END_SIZE] = file_size>>8;
   entry_data[TO_END_SIZE+1] = file_size;

   /* write the new entry into the directory */
   memcpy(trk20_data + TO_DIR_START(ID_FORMAT(id)) + n*TO_DIRENTRY_LENGTH, entry_data, TO_DIRENTRY_LENGTH);
   (void)unused_size_t;
}



/* do_delete_file:
 *  Performs the low-level delete operation for the specified file entry and
 *  returns the size of file.
 */
static int do_delete_file(sapID id, const char filename[], int n, unsigned char trk20_data[])
{
   unsigned char *fat_data, *entry_data;
   int block, old_block, size = 0;

   (void) filename;

   fat_data = trk20_data + TO_FAT_START(ID_FORMAT(id));
   entry_data = trk20_data + TO_DIR_START(ID_FORMAT(id)) + n*TO_DIRENTRY_LENGTH;

   /* delete the file entry */
   entry_data[TO_NAME] = 0;

   /* read start block */
   block = entry_data[TO_FIRST_BLOCK];

   /* mark FAT entries with the TO_TAG_FREE tag */
   while ((block<TO_NBLOCKS(ID_FORMAT(id))) && (size<TO_FILESIZE_MAX(ID_FORMAT(id)))) {
      old_block = block;
      block = fat_data[block];
      fat_data[old_block] = TO_TAG_FREE;
      size++;
   }

   size *= TO_BLOCKSIZE(ID_FORMAT(id));

   if (block > TO_END_BLOCK_OFFSET+1)
      size += (block - TO_END_BLOCK_OFFSET - 1) * TO_SECTSIZE(ID_FORMAT(id));

   size += (entry_data[TO_END_SIZE]<<8) + entry_data[TO_END_SIZE+1];

   return size;
}



/* do_extract_file:
 *  Performs the low-level extract operation for the specified file entry and
 *  returns the size of file.
 */
static int do_extract_file(sapID id, const char filename[], int n, unsigned char trk20_data[])
{
   sapsector_t sapsector;
   unsigned char *fat_data, *entry_data;
   int sect, end_size, size = 0;
   unsigned int block;
   FILE *file;

   fat_data = trk20_data + TO_FAT_START(ID_FORMAT(id));
   entry_data = trk20_data + TO_DIR_START(ID_FORMAT(id)) + n*TO_DIRENTRY_LENGTH;

   /* read start block */
   block = entry_data[TO_FIRST_BLOCK];

   if ((file=fopen(filename, "wb")) == NULL) {
      sap_errno = SAP_EPERM;
      return 0;
   }

   /* extract full blocks */
   while ((fat_data[block]<TO_NBLOCKS(ID_FORMAT(id))) && (size<TO_FILESIZE_MAX(ID_FORMAT(id)))) {
      seek_pos(id, block/2, 1 + (block%2 ? TO_SECTOR_PER_BLOCK : 0));

      for (sect=0; sect<TO_SECTOR_PER_BLOCK; sect++) {
         do_read_sector(id, &sapsector);
         fwrite(sapsector.data, sizeof(char), TO_SECTSIZE(ID_FORMAT(id)), file);
      }

      block = fat_data[block];
      size++;
   }

   size *= TO_BLOCKSIZE(ID_FORMAT(id));

   /* extract remaining full sectors */
   seek_pos(id, block/2, 1 + (block%2 ? TO_SECTOR_PER_BLOCK : 0));

   for (sect=0; sect<(fat_data[block] - TO_END_BLOCK_OFFSET - 1); sect++) {
      do_read_sector(id, &sapsector);
      fwrite(sapsector.data, sizeof(char), TO_SECTSIZE(ID_FORMAT(id)), file);
      size += TO_SECTSIZE(ID_FORMAT(id));
   }

   /* extract remaining individual bytes */
   do_read_sector(id, &sapsector);
   end_size = (entry_data[TO_END_SIZE]<<8) + entry_data[TO_END_SIZE+1];
   fwrite(sapsector.data, sizeof(char), end_size, file);
   size += end_size;

   fclose(file);

   return size;
}



/************************************************/
/***       public low-level functions         ***/
/************************************************/


/* _ExtractDir:
 *  Extract the directory from track 20 and returns the number of lines.
 */
int _ExtractDir(char buffer[], int buffer_size, int drive, int density, const unsigned char trk20_data[])
{
   unsigned char disk_name[TO_NAME_LENGTH+1];
   int dskf, i, len, pos, lines = 0;
   int format = (density == 1 ? SAP_FORMAT2 : SAP_FORMAT1);

   /* name of the volume */
   strncpy((char *)disk_name, (const char *)trk20_data, TO_NAME_LENGTH);

   if ((disk_name[0] == 0) || (disk_name[0] == TO_TAG_FREE)) {
      strcpy((char *)disk_name, "No Name ");
   }
   else {
      disk_name[TO_NAME_LENGTH] = 0;
      clean_string(disk_name);
   }

   /* dskf */
   dskf = get_dskf(format, trk20_data) * ((TO_BLOCKSIZE(format) + 1024) / 1024);

   /* header */
   pos = sprintf(buffer, "%s density   %d:%s DSKF = %d\n", (density == 1 ? "Single" : "Double"), drive, disk_name, dskf);
   lines++;

   /* directory entries */
   for (i=0; i<TO_NDIRENTRIES(format); i++) {
      len = extract_dir_entry(format, buffer+pos, buffer_size-pos, i, trk20_data);

      if (len>0) {
         pos += len;
         lines++;
      }
   }

   return lines;
}



/* _ForEachFile:
 *  Executes the specified callback funtion on each file matching the pattern.
 */
int _ForEachFile(sapID id, const char pattern[], sapfilecb_t callback, int save_back)
{
   char filename[TO_NAME_LENGTH + 1 + TO_EXT_LENGTH + 1];
   unsigned char trk20_data[SAP_TRACKSIZE1], *dir_data;
   int n, ret = 0;

   switch (ID_STATE(id)) {

      case NO_ARCHIVE:
         sap_errno = SAP_EINVAL;
         return 0;

      case EMPTY_ARCHIVE:
         sap_errno = SAP_EBUSY;
         return 0;

      case FILLED_ARCHIVE:
         sap_errno = SAP_EBUSY;
         return 0;

      case FULL_ARCHIVE:
         break;
   }

   /* read track 20 */
   sap_ReadSectorEx(id, 20, 1, SAP_NSECTS, trk20_data);

   dir_data = trk20_data + TO_DIR_START(ID_FORMAT(id));

   if (strpbrk(pattern, "?*")) {  /* wildcards? */
      while ((n=seek_wildcard(ID_FORMAT(id), filename, pattern, dir_data)) >= 0)
         ret += callback(id, filename, n, trk20_data);
   }
   else {
      n = seek_file(ID_FORMAT(id), pattern, dir_data);

      if (n<0)
         sap_errno = SAP_ENOENT;
      else
         ret = callback(id, pattern, n, trk20_data);
   }

   if (save_back) {
      /* save track 20 */
      sap_WriteSectorEx(id, 20, 1, SAP_NSECTS, trk20_data);
   }

   return ret;
}



/************************************************/
/***  API functions: physical format support  ***/
/************************************************/


/* OpenArchive:
 *  Opens an existing archive,
 *  returns the sapID on success or SAP_ERROR on error.
 */
sapID sap_OpenArchive(const char filename[], int *format)
{
   char header[SAP_HEADER_SIZE];
   sapID id;
   size_t unused_size_t;

   /* seek for an empty id */
   if (!(id=get_id())) {
      sap_errno = SAP_ETOOMANY;
      return SAP_ERROR;
   }

   if ((ID_FILE(id)=fopen(filename, "rb+")) == NULL) {
      sap_errno = SAP_ENOENT;
      return SAP_ERROR;
   }

   /* read the header */ 
   unused_size_t=fread(header, sizeof(char), SAP_HEADER_SIZE, ID_FILE(id));

   /* find the format */
   if ((header[0] != SAP_FORMAT1) && (header[0] != SAP_FORMAT2)) {
      fclose(ID_FILE(id));
      sap_errno = SAP_EBADF;
      return SAP_ERROR;
   }

   *format = header[0];
   
   /* check the header */
   header[0] = 0;

   if (strncmp(header, sap_header, SAP_HEADER_SIZE) != 0) {
      fclose(ID_FILE(id));
      sap_errno = SAP_EBADF;
      return SAP_ERROR;
   }

   ID_FORMAT(id) = *format;
   ID_STATE(id) = FULL_ARCHIVE;
   ID_NTRACKS(id) = SAP_NTRACKS(*format);

   return id;
   (void)unused_size_t;
}



/* CreateArchive:
 *  Creates an archive skeleton and writes the header,
 *  returns the sapID on success or SAP_ERROR on error.
 */
sapID sap_CreateArchive(const char filename[], int format)
{
   char header[SAP_HEADER_SIZE];
   sapID id;

   if ((format != SAP_FORMAT1) && (format != SAP_FORMAT2)) {
      sap_errno = SAP_EINVAL;
      return SAP_ERROR;
   }

   /* seek for an empty id */
   if (!(id=get_id())) {
      sap_errno = SAP_ETOOMANY;
      return SAP_ERROR;
   }

   if ((ID_FILE(id)=fopen(filename, "wb")) == NULL) {
      sap_errno = SAP_EPERM;
      return SAP_ERROR;
   }

   /* write the header */
   memcpy(header, sap_header, SAP_HEADER_SIZE);
   header[0] = format;

   fwrite(header, sizeof(char), SAP_HEADER_SIZE, ID_FILE(id));

   ID_FILE(id) = freopen(filename, "rb+", ID_FILE(id));
   seek_pos(id, 0 ,1);

   ID_FORMAT(id) = format;
   ID_STATE(id) = EMPTY_ARCHIVE;
   ID_NTRACKS(id) = 0;

   return id;
}



/* CloseArchive:
 *  Closes the archive, filling it up with empty tracks if needed,
 *  returns SAP_OK on success or SAP_ERROR on error.
 */
int sap_CloseArchive(sapID id)
{
   sapsector_t sapsector;
   int format = ID_FORMAT(id);
   int track, sect;

   switch (ID_STATE(id)) {

      case NO_ARCHIVE:
         sap_errno = SAP_EINVAL;
         return SAP_ERROR;

      case EMPTY_ARCHIVE:
      case FILLED_ARCHIVE:
         sapsector.format = 0;
         sapsector.protection = 0;
         memset(sapsector.data, TO_FILLER_BYTE, SAP_SECTSIZE(format));

         for (track=ID_NTRACKS(id); track<SAP_NTRACKS(format); track++) {
            for (sect=1; sect<=SAP_NSECTS; sect++) {
               sapsector.track = track;
               sapsector.sector = sect;
               do_write_sector(id, &sapsector);
            }
         }
         break;

      case FULL_ARCHIVE:
         break;
   }

   fclose(ID_FILE(id));
   ID_STATE(id) = NO_ARCHIVE;

   return SAP_OK;
}



/* FillArchive:
 *  Fills an empty archive sector by sector, it's up to
 *  the user to call it the right number of times,
 *  returns SAP_OK on success or SAP_ERROR on error.
 */
int sap_FillArchive(sapID id, sapsector_t *sapsector)
{
   static int sect;

   switch (ID_STATE(id)) {

      case NO_ARCHIVE:
         sap_errno = SAP_EINVAL;
         return SAP_ERROR;

      case EMPTY_ARCHIVE:
         ID_STATE(id) = FILLED_ARCHIVE;
         sect = 1;
         /* no break */
#if defined(__GNUC__) && (__GNUC__ >= 7)
         __attribute__ ((fallthrough));
#endif

      case FILLED_ARCHIVE:
         do_write_sector(id, sapsector);

         if (++sect == SAP_NSECTS+1) {
            sect = 1;

            if (++ID_NTRACKS(id) == SAP_NTRACKS(ID_FORMAT(id)))
               ID_STATE(id) = FULL_ARCHIVE;
         }
         break;

      case FULL_ARCHIVE:
         sap_errno = SAP_ENOSPC;
         return SAP_ERROR;
   } /* end of switch */

   return SAP_OK;
}



/* ReadSector:
 *  Reads the specified sector and returns SAP_OK or a flagged code:
 *    SAP_NO_STD_FMT: non standard format
 *    SAP_PROTECTED : protected sector
 *    SAP_BAD_SECTOR: bad sector identifiers
 *    SAP_CRC_ERROR : CRC error
 */
int sap_ReadSector(sapID id, int track, int sect, sapsector_t *sapsector)
{
   int flag = SAP_OK;
   int format = ID_FORMAT(id);

   switch (ID_STATE(id)) {

      case NO_ARCHIVE:
         sap_errno = SAP_EINVAL;
         return SAP_ERROR;

      case EMPTY_ARCHIVE:
         sap_errno = SAP_EEMPTY;
         return SAP_ERROR;

      case FILLED_ARCHIVE:
         sap_errno = SAP_EBUSY;
         return SAP_ERROR;

      case FULL_ARCHIVE:
         break;
   }

   seek_pos(id, track, sect);
   do_read_sector(id, sapsector);

   if (sapsector->format != 0)
      flag |= SAP_NO_STD_FMT;

   if (sapsector->protection != 0)
      flag |= SAP_PROTECTED;

   if ((sapsector->track != track) || (sapsector->sector != sect))
      flag |= SAP_BAD_SECTOR;

   do_crc(sapsector, format);

   if ((sapsector->data[SAP_SECTSIZE(format)] != ((crcpuk_temp>>8)&0xff))
        || (sapsector->data[SAP_SECTSIZE(format)] != (crcpuk_temp&0xff)))
      flag |= SAP_CRC_ERROR;

   return flag;
}



/* ReadSectorEx:
 *  Reads one or more sectors from the same track,
 *  returns SAP_OK on success or SAP_ERROR on error.
 */
int sap_ReadSectorEx(sapID id, int track, int sect, int nsects, unsigned char data[])
{
   sapsector_t sapsector;
   int format = ID_FORMAT(id);
   int i;

   switch (ID_STATE(id)) {

      case NO_ARCHIVE:
         sap_errno = SAP_EINVAL;
         return SAP_ERROR;

      case EMPTY_ARCHIVE:
         sap_errno = SAP_EEMPTY;
         return SAP_ERROR;

      case FILLED_ARCHIVE:
         sap_errno = SAP_EBUSY;
         return SAP_ERROR;

      case FULL_ARCHIVE:
         break;
   }

   seek_pos(id, track, sect);

   for (i=0; i<nsects; i++) {
      do_read_sector(id, &sapsector);
      memcpy(data + i*SAP_SECTSIZE(format), sapsector.data, SAP_SECTSIZE(format));
   }

   return SAP_OK;
}



/* WriteSector:
 *  Writes the specified sector and
 *  returns SAP_OK on success or SAP_ERROR on error.
 */
int sap_WriteSector(sapID id, int track, int sect, sapsector_t *sapsector)
{
   switch (ID_STATE(id)) {

      case NO_ARCHIVE:
         sap_errno = SAP_EINVAL;
         return SAP_ERROR;

      case EMPTY_ARCHIVE:
         sap_errno = SAP_EEMPTY;
         return SAP_ERROR;

      case FILLED_ARCHIVE:
         sap_errno = SAP_EBUSY;
         return SAP_ERROR;

      case FULL_ARCHIVE:
         break;
   }

   seek_pos(id, track, sect);
   do_write_sector(id, sapsector);

   return SAP_OK;
}



/* WriteSectorEx:
 *  Writes one or more sectors into the same track,
 *  returns SAP_OK on success or SAP_ERROR on error.
 */
int sap_WriteSectorEx(sapID id, int track, int sect, int nsects, const unsigned char data[])
{
   sapsector_t sapsector;
   int format = ID_FORMAT(id);
   int i;

   switch (ID_STATE(id)) {

      case NO_ARCHIVE:
         sap_errno = SAP_EINVAL;
         return SAP_ERROR;

      case EMPTY_ARCHIVE:
         sap_errno = SAP_EEMPTY;
         return SAP_ERROR;

      case FILLED_ARCHIVE:
         sap_errno = SAP_EBUSY;
         return SAP_ERROR;

      case FULL_ARCHIVE:
         break;
   }

   seek_pos(id, track, sect);

   sapsector.format = 0;
   sapsector.protection = 0;
   sapsector.track = track;

   for (i=0; i<nsects; i++) {
      memcpy(sapsector.data, data + i*SAP_SECTSIZE(format), SAP_SECTSIZE(format));
      sapsector.sector = sect + i;
      do_write_sector(id, &sapsector);
   }

   return SAP_OK;
}



/************************************************/
/***  API functions: logical format support   ***/
/************************************************/


/* FormatArchive:
 *  Formats an archive using the Thomson BASIC DOS format,
 *  returns SAP_OK on success or SAP_ERROR on error.
 */
int sap_FormatArchive(sapID id, int capacity)
{
   int format = ID_FORMAT(id);
   int track, sect;

   sapsector_t sapsector;

   switch (ID_STATE(id)) {

      case NO_ARCHIVE:
         sap_errno = SAP_EINVAL;
         return SAP_ERROR;

      case EMPTY_ARCHIVE:
         break;

      case FILLED_ARCHIVE:
         sap_errno = SAP_EBUSY;
         return SAP_ERROR;

      case FULL_ARCHIVE:
         seek_pos(id, 0, 1);
         break;
   }

   if (!(capacity == SAP_TRK40) && !(capacity == SAP_TRK80)) {
      sap_errno = SAP_EINVAL;
      return SAP_ERROR;
   }

   sapsector.format = 0;
   sapsector.protection = 0;
   memset(sapsector.data, TO_FILLER_BYTE, SAP_SECTSIZE(format));

   for (track=0; track<SAP_NTRACKS(format); track++) {
      for (sect=1; sect<SAP_NSECTS+1; sect++) {
         sapsector.track = track;
         sapsector.sector = sect;
         do_write_sector(id, &sapsector);
      }
   }

   /* write track 20 */
   sapsector.track = 20;
   memset(sapsector.data, TO_TAG_FREE, SAP_SECTSIZE(format));
   seek_pos(id, 20, 1);

   for (sect=1; sect<SAP_NSECTS+1; sect++) {
      sapsector.sector = sect;
      do_write_sector(id, &sapsector);
   }

   /* FAT */
   sapsector.track = 20;
   sapsector.sector = 2;
   memset(sapsector.data, TO_TAG_RESERVED, SAP_SECTSIZE(format));

   /* first byte */
   sapsector.data[0] = 0;

   /* lower zone */
   memset(sapsector.data + 1, TO_TAG_FREE, 40);

   /* upper zone */
   if (capacity == SAP_TRK80)  /* not (format == SAP_FORMAT1) */
      memset(sapsector.data + 43, TO_TAG_FREE, TO_NBLOCKS1 - 43 + 1);
   else
      memset(sapsector.data + 43, TO_TAG_FREE, TO_NBLOCKS2 - 43 + 1);

   seek_pos(id, 20, 2);
   do_write_sector(id, &sapsector);

   ID_STATE(id) = FULL_ARCHIVE;
   ID_NTRACKS(id) = SAP_NTRACKS(format);

   return SAP_OK;
}



/* ListArchive:
 *  Builds a list of files contained in the archive,
 *  returns the number of lines on success or 0 on error.
 */
int sap_ListArchive(sapID id, char buffer[], int buffer_size)
{
   unsigned char trk20_data[SAP_TRACKSIZE1];

   switch (ID_STATE(id)) {

      case NO_ARCHIVE:
         sap_errno = SAP_EINVAL;
         return 0;

      case EMPTY_ARCHIVE:
         sap_errno = SAP_EEMPTY;
         return 0;

      case FILLED_ARCHIVE:
         sap_errno = SAP_EBUSY;
         return 0;

      case FULL_ARCHIVE:
         break;
   }

   /* read track 20 */
   sap_ReadSectorEx(id, 20, 1, SAP_NSECTS, trk20_data);

   return _ExtractDir(buffer, buffer_size, 0, ID_FORMAT(id) == SAP_FORMAT1 ? 2 : 1, trk20_data);
}



/* AddFile:
 *  Adds the specified file to the archive,
 *  returns the size of the file in bytes on success or 0 on error.
 */
int sap_AddFile(sapID id, const char filename[])
{
   unsigned char entry_data[TO_DIRENTRY_LENGTH];
   unsigned char trk20_data[SAP_TRACKSIZE1], *fat_data, *dir_data;
   int free_n=-1, prev_n=-1;
   int i, dskf, file_size = 0;
   FILE *file;

   switch (ID_STATE(id)) {

      case NO_ARCHIVE:
         sap_errno = SAP_EINVAL;
         return 0;

      case EMPTY_ARCHIVE:
         sap_errno = SAP_EBUSY;
         return 0;

      case FILLED_ARCHIVE:
         sap_errno = SAP_EBUSY;
         return 0;

      case FULL_ARCHIVE:
         break;
   }

   /* open the file */
   if ((file=fopen(filename, "rb")) == NULL) {
      sap_errno = SAP_ENOENT;
      return 0;
   }

   /* find size of the file */
   while (fgetc(file) != EOF)
      file_size++;

   if (file_size == 0) {
      fclose(file);
      sap_errno = SAP_ENFILE;
      return 0;
   }

   fseek(file, 0, SEEK_SET);

   /* decode the filename */
   decode_filename(entry_data, filename, file_size);

   /* read track 20 */
   sap_ReadSectorEx(id, 20, 1, SAP_NSECTS, trk20_data);

   fat_data = trk20_data + TO_FAT_START(ID_FORMAT(id));
   dir_data = trk20_data + TO_DIR_START(ID_FORMAT(id));

   /* simultaneously seek already free entry and previous entry */
   for (i=0; i<TO_NDIRENTRIES(ID_FORMAT(id)); i++) {
      if ((dir_data[i*TO_DIRENTRY_LENGTH] == 0) || (dir_data[i*TO_DIRENTRY_LENGTH] == TO_TAG_FREE)) {
         if (free_n<0)
            free_n = i;
      }
      else {
         if (strncmp((const char *)(dir_data+i*TO_DIRENTRY_LENGTH), (const char *)entry_data, TO_NAME_LENGTH + TO_EXT_LENGTH) == 0) {
            prev_n = i;
            break;
         }
      }
   }

   if ((free_n<0) && (prev_n<0)) {
      fclose(file);
      sap_errno = SAP_ENOSPC;
      return 0;
   }

   /* test for enough free disk space */
   dskf = get_dskf(ID_FORMAT(id), trk20_data);

   if (prev_n >=0)
      dskf += get_file_size(ID_FORMAT(id), prev_n, trk20_data);

   if ((dskf*TO_BLOCKSIZE(ID_FORMAT(id))) < file_size) {
      sap_errno = SAP_EFBIG;
      fclose(file);
      return 0;
   }

   /* delete previous entry */
   if (prev_n >= 0) {
      do_delete_file(id, filename, prev_n, trk20_data);
      free_n = prev_n;
   }

   /* phew! we can finally add the file... */
   do_add_file(id, file, file_size, entry_data, free_n, trk20_data);

   /* update directory and FAT */
   sap_WriteSectorEx(id, 20, 1, SAP_NSECTS, trk20_data);

   fclose(file);

   return file_size;
   (void)fat_data;
}



/* DeleteFile:
 *  Deletes the specified files from the archive,
 *  returns the total size of the files in bytes on success or 0 on error.
 */
int sap_DeleteFile(sapID id, const char pattern[])
{
   return _ForEachFile(id, pattern, do_delete_file, 1);
}



/* ExtractFile:
 *  Extracts the specified files from the archive,
 *  returns the total size of the files in bytes on success or 0 on error.
 */
int sap_ExtractFile(sapID id, const char pattern[])
{
   return _ForEachFile(id, pattern, do_extract_file, 0);
}



/* GetFileInfo:
 *  Fills in a structure with the info for the specified file
 *  returns SAP_OK on success or SAP_ERROR on error.
 */
int sap_GetFileInfo(sapID id, const char filename[], sapfileinfo_t *info)
{
   unsigned char trk20_data[SAP_TRACKSIZE1], *dir_data, *fat_data, *entry_data;
   struct tm tim;
   int n, block, i=0;

   switch (ID_STATE(id)) {

      case NO_ARCHIVE:
         sap_errno = SAP_EINVAL;
         return 0;

      case EMPTY_ARCHIVE:
         sap_errno = SAP_EEMPTY;
         return 0;

      case FILLED_ARCHIVE:
         sap_errno = SAP_EBUSY;
         return 0;

      case FULL_ARCHIVE:
         break;
   }

   /* read track 20 */
   sap_ReadSectorEx(id, 20, 1, SAP_NSECTS, trk20_data);

   dir_data = trk20_data + TO_DIR_START(ID_FORMAT(id));

   n = seek_file(ID_FORMAT(id), filename, dir_data);

   if (n<0) {
      sap_errno = SAP_ENOENT;
      return SAP_ERROR;
   }

   fat_data = trk20_data + TO_FAT_START(ID_FORMAT(id));
   entry_data = trk20_data + TO_DIR_START(ID_FORMAT(id)) + n*TO_DIRENTRY_LENGTH;

   /* block chain */
   info->nblocks = get_file_size(ID_FORMAT(id), n, trk20_data);
   info->block = malloc(info->nblocks*sizeof(int));

   block = entry_data[TO_FIRST_BLOCK];

   while ((block<TO_NBLOCKS(ID_FORMAT(id))) && (i<TO_FILESIZE_MAX(ID_FORMAT(id)))) {
      info->block[i++] = block;
      block = fat_data[block];
   }

   /* common attributes */
   info->size = info->nblocks*TO_BLOCKSIZE(ID_FORMAT(id)) + entry_data[TO_END_SIZE];
   info->file_type = entry_data[TO_FILE_TYPE];
   info->data_type = entry_data[TO_DATA_TYPE];

   /* date */
   if ((entry_data[TO_DATE_DAY] >= 1) && (entry_data[TO_DATE_DAY] <= 31) &&
       (entry_data[TO_DATE_MONTH] >= 1) && (entry_data[TO_DATE_MONTH] <= 12) &&
       (entry_data[TO_DATE_YEAR] <= 99)) {
      memset(&tim, 0, sizeof(struct tm));
      tim.tm_mday = entry_data[TO_DATE_DAY];
      tim.tm_mon = entry_data[TO_DATE_MONTH];
      tim.tm_year = entry_data[TO_DATE_YEAR];
      info->date = mktime(&tim);
   }
   else {
      info->date = 0;
   }

   /* comment */
   info->comment[0] = '\0';
   if (entry_data[TO_COMMENT])
      strncat(info->comment, (const char *)(entry_data+TO_COMMENT), TO_COMMENT_LENGTH);

   return SAP_OK;
   (void)fat_data;
}

