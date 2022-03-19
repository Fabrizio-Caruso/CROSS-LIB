/*  SAP2
 *  Version 2.1
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

/*  version 1.0: programme fonctionnel sous MSDOS et Linux
 *          1.1: retour au format PC à la sortie du programme
 *          1.2: ajout d'un mode de fonctionnement batch
 *          2.0: support des disquettes 5"25 et de deux lecteurs PC
 *          2.1: support de la simple densité
 */


#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if defined(linux) || defined(__FreeBSD__)
#    include <locale.h>
#endif
#include "libsap.h"
#include "floppy.h"


#define SAP2_VERSION_STR "2.0.93"

#ifdef linux
   #define SAP2_PLATFORM_STR "Linux"
#else
   #define SAP2_PLATFORM_STR "MSDOS"
#endif

#define FILENAME_LENGTH 128


/* horrible hack pour supporter les accents... */
#ifdef linux
/* UTF-8 */
static char eacute[] = "\xc3\xa9";
static char egrave[] = "\xc3\xa8";
static char agrave[] = "\xc3\xa0";
static char ugrave[] = "\xc3\xb9";
#else
/* Latin-1 (ISO8859-1, etc.) */
static char eacute[] = "\x82";
static char egrave[] = "\x8a";
static char agrave[] = "\x85";
static char ugrave[] = "\x97";
#endif


/* pour l'affichage du répertoire */
#define PAGE_HEIGHT 22

static int is_fr=0;

static struct floppy_info fi;
static char *drive_type_name[7] = { "non disponible",
                                    "5\"25 - 360 ko",
                                    "5\"25 - 1.2 Mo",
                                    "3\"5 -  720 ko",
                                    "3\"5 - 1.44 Mo",
                                    "3\"5 - 2.88 Mo",
                                    "3\"5 - 2.88 Mo" };

#define IS_5_INCHES(drive) ((fi.drive_type[drive]>0) && (fi.drive_type[drive]<3))



/* term_puts:
 *  Affiche une chaîne de caractères en respectant la taille du terminal.
 */
static void term_puts(const char buffer[], int lines, int page_height)
{
   const char *start_cur, *end_cur;
   char trash[8];
   int i;
   char *unused_char;

   start_cur = end_cur = buffer;

   while (lines>page_height) {
      for (i=0; i<page_height; i++) {
         while (*end_cur++ != '\n')
            ;
      }

      fwrite(start_cur, sizeof(char), end_cur-start_cur, stdout);

      unused_char=fgets(trash, 7, stdin);
      printf(is_fr?"Appuyer sur <Return>":"Press <Return> key");
      getchar();
      printf("\n");
      ungetc('\n', stdin);

      start_cur = end_cur;
      lines -= page_height;
   }

   puts(end_cur);
   (void)unused_char;
}



/* ViewDiskDir:
 *  Affiche le répertoire d'une disquette TO.
 */
static int ViewDiskDir(int drive, int density, int page_height)
{
   unsigned char data[512*SAP_NSECTS];
   char buffer[4096];
   int lines;

   if (FloppyReadSector(drive, density, 20, 1, SAP_NSECTS, data) != 0)
      return 1;

   /* on réduit la FAT à 80 blocks pour les disquettes 5"25 DD */
   if (IS_5_INCHES(drive) && density == 2)
      memset(data + SAP_SECTSIZE1 + 1 + SAP_NTRACKS1, 0xFE, SAP_NTRACKS1);

   /* fonction bas-niveau de libSAP */
   lines = _ExtractDir(buffer, sizeof(buffer), drive, density, data);

   if ((page_height<0) || (lines<=page_height))
      puts(buffer);
   else
      term_puts(buffer, lines, page_height);

   return 0;
}



/* ViewArchiveDir:
 *  Affiche le répertoire d'une archive SAP.
 */
static int ViewArchiveDir(const char sap_name[], int page_height)
{
   char buffer[4096];
   sapID sap_file;
   int format, lines;

   if ((sap_file=sap_OpenArchive(sap_name, &format)) == SAP_ERROR)
      return 1;

   lines = sap_ListArchive(sap_file, buffer, sizeof(buffer));

   if (lines == 0)
      return 2;

   if ((page_height<0) || (lines<=page_height))
      puts(buffer);
   else
      term_puts(buffer, lines, page_height);

   sap_CloseArchive(sap_file);
   return 0;
}



/* CreateEmptyArchive:
 *  Crée une archive SAP vide (mais formatée).
 */
static int CreateEmptyArchive(const char sap_name[], int format, int capacity)
{
   sapID sap_file;

   if ((sap_file=sap_CreateArchive(sap_name, format)) == SAP_ERROR)
      return 1;

   sap_FormatArchive(sap_file, capacity);
   sap_CloseArchive(sap_file);

   return 0;
}



/* BuildSectorMap:
 *  Construit la carte des secteurs d'une piste
 *  en fonction du facteur d'entrelacement.
 */
static void BuildSectorMap(int *sector_map, int factor)
{
   int sect, loc=0;

   /* mise à zéro de la table */
   memset(sector_map, 0, sizeof(int)*SAP_NSECTS);

   for (sect=1; sect<=SAP_NSECTS; sect++) {
      while (sector_map[loc] != 0)
         loc=(loc+1)%SAP_NSECTS;

      sector_map[loc]=sect;

      loc=(loc+factor)%SAP_NSECTS;
   }
}



/* FormatDisk:
 *  Formate une disquette 3"5 - 720 ko au format Thomson.
 */
static int FormatDisk(int drive, int density, int factor, int verbose)
{
   int num_tracks, sect_size;
   int track, sect, pos, i;
   int sector_map[SAP_NSECTS];
   unsigned char header_table[512];
   unsigned char buffer[512*SAP_NSECTS];

   /* détermination du nombre de pistes */
   if (IS_5_INCHES(drive))
      num_tracks = SAP_NTRACKS2;
   else
      num_tracks = SAP_NTRACKS1;

   /* et de la taille du secteur */
   if (density == 1)
      sect_size = SAP_SECTSIZE2;
   else
      sect_size = SAP_SECTSIZE1;

   /* construction de la carte des secteurs pour chaque piste,
      à partir du facteur d'entrelacement */
   BuildSectorMap(sector_map, factor);

   /* formatage des pistes */
   for (track=0; track<num_tracks; track++) {
      if (verbose) {
         if (is_fr)
             printf("\r formatage piste %d", track);
         else
             printf("\r formatting track %d", track);
         fflush(stdout);
      }

      /* construction de la table des headers */
      for (sect=1, pos=0; sect<=SAP_NSECTS; sect++, pos+=4) {
         header_table[pos]   = track;
         header_table[pos+1] = 0;
         header_table[pos+2] = sector_map[sect-1];
         header_table[pos+3] = density - 1;
      }

      if (FloppyFormatTrack(drive, density, track, header_table) != 0)
         return 1;
   }

   if (verbose) {
      if (is_fr)
          printf("\n construction de la FAT (piste 20)");
      else
          printf("\n building FAT (track 20)");
      fflush(stdout);
   }

   for (i=0; i<sect_size*SAP_NSECTS; i++)
      buffer[i] = 0xFF;

   /* pour MS-DOS (voir teo/src/dos/ddisk.c) */
   buffer[i]=0;

   if (FloppyWriteSector(drive, density, 20, 1, SAP_NSECTS, buffer) != 0)
      return 2;

   buffer[0] = 0;
   buffer[41] = 0xFE;
   buffer[42] = 0xFE;

   /* pour MS-DOS (voir teo/src/dos/ddisk.c) */
   buffer[sect_size] = 0;

   if (FloppyWriteSector(drive, density, 20, 2, 1, buffer) != 0)
      return 2;

   if (verbose)
      printf("\n");

   return 0;
}



/* shrink_fat_160_to_80:
 *  Réduit la taille de la FAT de 160 à 80 blocks.
 */
static void shrink_fat_160_to_80(unsigned char fat_data[])
{
   int i;

   for (i=0; i<SAP_NTRACKS1; i++) {
      if (fat_data[1 + SAP_NTRACKS1 + i] != 0xFF)
         return;
   }

   memset(fat_data + 1 + SAP_NTRACKS1, 0xFE, SAP_NTRACKS1);
}



/* PackArchive:
 *  Transfère le contenu d'une disquette TO dans une archive SAP.
 */
static int PackArchive(const char sap_name[], int drive, int density, int verbose)
{
   int num_tracks, sect_size;
   int track, sect;
   sapID sap_file;
   sapsector_t sapsector;
   unsigned char buffer[512*SAP_NSECTS];

   if ((sap_file=sap_CreateArchive(sap_name, density == 1 ? SAP_FORMAT2 : SAP_FORMAT1)) == SAP_ERROR)
      return 1;

   /* détermination du nombre de pistes */
   if (IS_5_INCHES(drive))
      num_tracks = SAP_NTRACKS2;
   else
      num_tracks = SAP_NTRACKS1;

   /* et de la taille du secteur */
   if (density == 1)
      sect_size = SAP_SECTSIZE2;
   else
      sect_size = SAP_SECTSIZE1;

   for (track=0; track<num_tracks; track++) {
      if (verbose) {
         if (is_fr)
             printf("\r lecture piste %d", track);
         else
             printf("\r reading track %d", track);
         fflush(stdout);
      }

      if (FloppyReadSector(drive, density, track, 1, SAP_NSECTS, buffer) != 0) {
         /* erreur dans la piste, on essaie secteur par secteur */
         for (sect=1; sect<=SAP_NSECTS; sect++) {
            sapsector.format     = 0;
            sapsector.protection = 0;
            sapsector.track      = track;
            sapsector.sector     = sect;

            if (FloppyReadSector(drive, density, track, sect, 1, buffer) != 0) {
               /* erreur de lecture du secteur */
               if (verbose)
               {
                  if (is_fr)
                      printf("\n   secteur %d illisible", sect);
                  else
                      printf("\n   sector %d unreadable", sect);
               }

               sapsector.format = 4;
               memset(sapsector.data, 0xF7, sect_size);
            }
            else {
               memcpy(sapsector.data, buffer, sect_size);
            }

            /* traitement spécial de la FAT pour les disquettes 5"25 DD */
            if ((track == 20) && (sect == 2) && IS_5_INCHES(drive) && (density == 2))
               shrink_fat_160_to_80(sapsector.data);

            sap_FillArchive(sap_file, &sapsector);
         }

         if ((track < num_tracks-1) && verbose)
            printf("\n");
      }
      else {
         for (sect=1; sect<=SAP_NSECTS; sect++) {
            sapsector.format     = 0;
            sapsector.protection = 0;
            sapsector.track      = track;
            sapsector.sector     = sect;

            memcpy(sapsector.data, buffer+(sect-1)*sect_size, sect_size);

            /* traitement spécial de la FAT pour les disquettes 5"25 DD */
            if ((track == 20) && (sect == 2) && IS_5_INCHES(drive) && (density == 2))
               shrink_fat_160_to_80(sapsector.data);

            sap_FillArchive(sap_file, &sapsector);
         }
      }
   }

   if (verbose)
      printf("\n");

   sap_CloseArchive(sap_file);
   return 0;
}



/* UnpackArchive:
 *  Transfère le contenu d'une archive SAP vers une disquette TO.
 */
static int UnpackArchive(const char sap_name[], int drive, int density, int verbose)
{
   int format, num_tracks, sect_size;
   int track, sect;
   sapID sap_file;
   sapsector_t sapsector;
   unsigned char buffer[512*SAP_NSECTS];

   if ((sap_file=sap_OpenArchive(sap_name, &format)) == SAP_ERROR)
      return 1;

   /* vérification du format */
   if (((format == SAP_FORMAT1) && (density != 2)) || ((format == SAP_FORMAT2) && (density != 1)))
      return 1;

   /* détermination du nombre de pistes */
   if (IS_5_INCHES(drive))
      num_tracks = SAP_NTRACKS2;
   else
      num_tracks = SAP_NTRACKS1;

   /* et de la taille du secteur */
   if (density == 1)
      sect_size = SAP_SECTSIZE2;
   else
      sect_size = SAP_SECTSIZE1;

   for (track=0; track<num_tracks; track++) {
      if (verbose) {
         if (is_fr)
             printf("\r %scriture piste %d", eacute, track);
         else
             printf("\r writing track %d",track);
         fflush(stdout);
      }

      for (sect=1; sect<=SAP_NSECTS; sect++) {
         sapsector.format     = 0;
         sapsector.protection = 0;
         sapsector.track      = track;
         sapsector.sector     = sect;

         if (sap_ReadSector(sap_file, track, sect, &sapsector) == SAP_ERROR) {
            sap_CloseArchive(sap_file);
            return 2;
         }
         else {
            memcpy(buffer+(sect-1)*sect_size, sapsector.data, sect_size);
         }
      }

      /* pour MS-DOS (voir teo/src/dos/ddisk.c) */
      buffer[SAP_NSECTS*sect_size] = 0;

      if (FloppyWriteSector(drive, density, track, 1, SAP_NSECTS, buffer) != 0) {
         sap_CloseArchive(sap_file);
         return 3;
      }
   }

   if (verbose)
      printf("\n");

   sap_CloseArchive(sap_file);
   return 0;
}



/* get_drive:
 *  Helper pour obtenir le numéro de lecteur.
 */
static void get_drive(int *drive, int *density)
{
   char trash[8];
   char *unused_char;

   if (fi.num_drives > 1) {
      do {
         if (is_fr)
             printf(" num%sro du lecteur: ", eacute);
         else
             printf(" drive number: ");
         if (!scanf("%d", drive))
            unused_char=fgets(trash, 7, stdin);
      }
      while ((*drive<0) || (*drive>3) || (fi.drive_type[*drive]==0));
   }
   else {
      for (*drive=0; *drive<4; (*drive)++) {
         if (fi.drive_type[*drive] > 0)
            break;
      }
   }

   if (fi.fm_support && IS_5_INCHES(*drive)) {
      do {
         if (is_fr)
             printf(" densit%s (1:simple, 2:double): ", eacute);
         else
             printf(" density (1:simple, 2:double): ");

         if (!scanf("%d", density))
            unused_char=fgets(trash, 7, stdin);
      }
      while ((*density<1) || (*density>2));
   }
   else
      *density = 2;
   (void)unused_char;
}



/* interactive_main:
 *  Point d'entrée du programme interactif.
 */
static void interactive_main(void)
{
   int c, ret;
   char trash[8];
   int drive, density, format, factor;
   char sap_name[FILENAME_LENGTH];
   char *unused_char;
   int unused_int;

   while (1) {
      if (is_fr)
      {
          printf("SAP2, syst%sme d'archivage pour disquettes 3\"5 et 5\"25 Thomson\n", egrave);
          printf("version "SAP2_VERSION_STR" ("SAP2_PLATFORM_STR") copyright (C) 2000-2003 Eric Botcazou\n");
          printf(" bas%s sur SAP copyright (C) Alexandre Pukall Avril 1998\n\n", eacute);
      }
      else
      {
          printf("SAP2, archiving system for Thomson 3\"5 and 5\"25 floppies\n");
          printf("version "SAP2_VERSION_STR" ("SAP2_PLATFORM_STR") copyright (C) 2000-2003 Eric Botcazou\n");
          printf(" based on SAP copyright (C) Alexandre Pukall April 1998\n\n");
      }

      if (fi.drive_type[0] > 0) {
         if (is_fr)
             printf("Lecteur A: (%s) --> lecteur 0", drive_type_name[fi.drive_type[0]]);
         else
             printf("Drive A: (%s) --> drive 0", drive_type_name[fi.drive_type[0]]);

         if (fi.drive_type[1] > 0)
         {
            if (is_fr)
                printf(" + lecteur 1\n");
            else
                printf(" + drive 1\n");
         }
         else
            printf("\n");
      }
      else {
         if (is_fr)
             printf("Lecteur A: (%s)\n", drive_type_name[0]);
         else
             printf("Drive A: (%s)\n", drive_type_name[0]);
      }

      if (fi.drive_type[2] > 0) {
         if (is_fr)
             printf("Lecteur B: (%s) --> lecteur 2", drive_type_name[fi.drive_type[2]]);
         else
             printf("Drive B: (%s) --> drive 2", drive_type_name[fi.drive_type[2]]);

         if (fi.drive_type[3] > 0)
         {
             if (is_fr)
                 printf(" + lecteur 3\n\n");
             else
                 printf(" + drive 3\n\n");
         }
         else
            printf("\n\n");
      }
      else {
         if (is_fr)
             printf("Lecteur B: (%s)\n\n", drive_type_name[0]);
         else
             printf("Drive B: (%s)\n\n", drive_type_name[0]);
      }

      if (is_fr)
      {
          printf("Transfert TO-->PC:\n");
          printf(" 1. Visualiser le contenu d'une disquette Thomson\n");
          printf(" 2. Cr%ser une archive SAP vide\n", eacute);
          printf(" 3. Archiver une disquette Thomson vers une archive SAP\n");

          printf("Transfert PC-->TO:\n");
          printf(" 4. Visualiser le contenu d'une archive SAP\n");
          printf(" 5. Formater une disquette 3\"5 (720 ko) ou 5\"25 au format Thomson\n");
          printf(" 6. D%ssarchiver une archive SAP vers une disquette Thomson\n", eacute);
          printf("Autres commandes:\n");
          printf(" 7. Quitter\n\n");
      }
      else
      {
          printf("TO-->PC transfer:\n");
          printf(" 1. Display directory of a Thomson floppy\n");
          printf(" 2. Create an empty SAP archive\n");
          printf(" 3. Archive a Thomson floppy to a SAP archive\n");
          printf("PC-->TO transfer:\n");
          printf(" 4. Display directory of a SAP archive\n");
          printf(" 5. Initialize a Thomson formatted 3\"5 (720 ko) or 5\"25 floppy\n");
          printf(" 6. Unarchive a SAP archive to a Thomson floppy\n");
          printf("Other commands:\n");
          printf(" 7. Quit\n\n");
      }

      do {
         if (is_fr)
             printf("Votre choix: ");
         else
             printf("Your choice: ");
         if (!scanf("%d", &c))
            unused_char=fgets(trash, 7, stdin);
      }
      while ((c<1) || (c>7));

      switch (c) {

         case 1:
            if (is_fr)
                printf("Visualisation du contenu d'une disquette:\n");
            else
                printf("Displaying disk directory:\n");

            get_drive(&drive, &density);
            printf("\n");

            ret = ViewDiskDir(drive, density, PAGE_HEIGHT);
            if (ret == 1)
            {
               if (is_fr)
                   printf("*** Erreur: r%spertoire illisible ***\n", eacute);
               else
                   printf("*** Error: directory unreadable ***\n");
            }
            break;

         case 2:
            if (is_fr)
               printf("Cr%sation d'une archive vide:\n", eacute);
            else
               printf("Creating an empty archive:\n");


            if (is_fr)
                printf(" nom de l'archive %s cr%ser (sans extension): ", agrave, eacute);
            else
                printf(" name of archive to create (without extension): ");
            unused_int=scanf("%s", sap_name);
            strcat(sap_name, ".sap");

            do {
               if (is_fr)
                   printf(" format (1:5\"25 SD, 2:5\"25 DD, 3:3\"5 DD): ");
               else
                   printf(" format type (1:5\"25 SD, 2:5\"25 DD, 3:3\"5 DD): ");
               if (!scanf("%d", &format))
                  unused_char=fgets(trash, 7, stdin);
            }
            while ((format<1) || (format>3));

            ret = CreateEmptyArchive(sap_name, (format == 1 ? SAP_FORMAT2 : SAP_FORMAT1), (format == 3 ? SAP_TRK80 : SAP_TRK40));
            if (ret == 1)
            {
               if (is_fr)
                   printf("*** Erreur: impossible de cr%ser le fichier %s ***\n", eacute, sap_name);
               else
                   printf("*** Error: impossible to create the file %s ***\n", sap_name);
            }
            break;

         case 3:
            if (is_fr)
                printf("Archivage d'une disquette:\n");
            else
                printf("Archiving a disk:\n");

            if (is_fr)
                printf(" nom de l'archive %s cr%ser (sans extension): ", agrave, eacute);
            else
                printf(" name of the archive to create (without extension): ");
            unused_int=scanf("%s", sap_name);
            strcat(sap_name, ".sap");

            get_drive(&drive, &density);
            ret = PackArchive(sap_name, drive, density, 1);
            if (ret == 1)
            {
               if (is_fr)
                   printf("*** Erreur: impossible de cr%ser le fichier %s ***\n", eacute, sap_name);
               else
                   printf("*** Error: unable to create the file %s ***\n", sap_name);
            }

            break;

         case 4:
            if (is_fr)
                printf("Visualisation du contenu d'une archive:\n");
            else
                printf("Displaying archive directory:\n");

            if (is_fr)
                printf(" nom de l'archive (sans extension): ");
            else
                printf(" name of the archive (without extension): ");

            unused_int=scanf("%s", sap_name);
            strcat(sap_name, ".sap");

            printf("\n");

            ret = ViewArchiveDir(sap_name, PAGE_HEIGHT);
            if (ret == 1)
            {
                if (is_fr)
                   printf("*** Erreur: impossible d'ouvrir le fichier %s ***\n", sap_name);
                else
                   printf("*** Error: unable to open the file %s ***\n", sap_name);
            }
            else if (ret == 2)
            {
                if (is_fr)
                    printf("*** Erreur: archive SAP corrompue ***\n");
                else
                    printf("*** Error: SAP archive corrupted ***\n");
            }
            break;

         case 5:
            if (is_fr)
            {
                printf("Formatage d'une disquette au format Thomson:\n");
                printf(" Si la disquette est une 3\"5 - 1.44 Mo, occulter l'encoche\n");
                printf(" de droite (recto et verso) avec un morceau de scotch.\n");
            }
            else
            {
                printf("Initializing a Thomson formatted floppy:\n");
                printf(" If the floppy is 3\"5 - 1.44 Mo, obsruct the high-capacity\n");
                printf(" hole (both sides) with a piece of tape.\n");
            }

            get_drive(&drive, &density);

            do {
               if (is_fr)
                   printf(" facteur d'entrelacement ([1..%d], %d recommand%s): ", SAP_NSECTS-1, (SAP_NSECTS-1)/2, eacute);
               else
                   printf(" interleaving ([1..%d], %d recommanded): ", SAP_NSECTS-1, (SAP_NSECTS-1)/2);
               if (!scanf("%d", &factor))
                  unused_char=fgets(trash, 7, stdin);
            }
            while ((factor<1) || (factor>(SAP_NSECTS-1)));

            ret = FormatDisk(drive, density, factor, 1);
            if (ret == 1)
            {
               if (is_fr)
                   printf("\n*** Erreur: impossible de formater la disquette ***\n");
               else
                   printf("\n*** Error: unable to format floppy ***\n");
            }
            else if (ret == 2)
            {
               if (is_fr)
                   printf("*** Erreur: impossible d'%scrire sur la disquette ***\n", eacute);
               else
                   printf("*** Error: unable to write on floppy ***\n");
            }
            break;

         case 6:
            if (is_fr)
                printf("D%ssarchivage vers une disquette:\n", eacute);
            else
                printf("Unarchiving to a floppy:\n");

            get_drive(&drive, &density);

            if (is_fr)
                printf(" nom de l'archive (sans extension): ");
            else
                printf(" archive name (without extension): ");
            unused_int=scanf("%s", sap_name);
            strcat(sap_name, ".sap");

            ret = UnpackArchive(sap_name, drive, density, 1);
            if (ret == 1)
            {
               if (is_fr)
                   printf("*** Erreur: impossible d'ouvrir le fichier %s ***\n", sap_name);
               else
                   printf("*** Error: unable to open the file %s ***\n", sap_name);
            }
            else if (ret == 2)
            {
               if (is_fr)
                   printf("*** Erreur: archive SAP corrompue ***\n");
               else
                   printf("*** Error: SAP archive corrupted ***\n");
            }
            else if (ret == 3)
            {
               if (is_fr)
                   printf("\n*** Erreur: impossible d'%scrire sur la disquette ***\n", eacute);
               else
                   printf("\n*** Error: unable to write on floppy ***\n");
            }
            break;

         case 7:
            return;
      }

      unused_char=fgets(trash, 7, stdin);
      if (is_fr)
          printf("Appuyer sur <Return>");
      else
          printf("Press <Return>");
      fflush(stdout);
      getchar();
      printf("\n");
   }
   (void)unused_char;
   (void)unused_int;
}


#define COMMAND_MAX  8

static char *short_command[] = {"-h", "-v", "-d", "-c", "-p", "-t", "-f", "-u"};
static char *long_command[] = {"--help", "--version", "--dir", "--create", "--pack", "--list", "--format", "--unpack"};



/* usage:
 *  Affiche les commandes d'utilisation du programme et quitte.
 */
static void usage(const char prog_name[])
{
   fprintf(stderr, "Usage: %s  -h --help | -v --version | -t --list | -p --pack | -u --unpack\n", prog_name);
   fprintf(stderr, "               -c --create | -d --dir | -f --format\n");
   exit(EXIT_FAILURE);
}



/* main:
 *  Point d'entrée du programme.
 */
int main(int argc, char *argv[])
{
   int ret = 0;
   int i;

#ifdef DJGPP
#ifdef FRENCH_LANGUAGE
    is_fr = 1;
#else
    is_fr = 0;
#endif
#else
   char *lang=getenv("LANG");
    if (lang==NULL) lang="fr_FR";
    //setlocale(LC_ALL, "fr_FR.UTF8");
    if (strncmp(lang,"fr",2)==0)
        is_fr=1;
    else
        is_fr=0;
#endif

   if (argc < 2) { /* no argument? */
      if (FloppyInit(&fi, 1) > 0) {
         interactive_main();
         FloppyExit();
      }
      else {
         ret = 1;
      }
   }
   else {
      if (argv[1][0] == '-') {

         switch (argv[1][1]) {

            case '-':  /* long commands */
               for (i=0; i<COMMAND_MAX; i++) {
                  if (strcmp(argv[1], long_command[i]) == 0) {
                     argv[1] = short_command[i];
                     return main(argc, argv);
                  }
               }

               usage(argv[0]);
               break;

            case 'h':  /* help */
               if (is_fr)
               {
                   printf("SAP2 est un syst%sme d'archivage pour disquettes 3\"5 et 5\"25 Thomson. Il permet\n", egrave);
                   printf("de cr%ser des images de disquettes sous forme d'archive SAP.\n\n", eacute);
                   printf("Usage:\n");
                   printf("    %s (mode interactif)\n", argv[0]);
                   printf("    %s commande1 archive.sap [lecteur] [densit%s]\n", argv[0], eacute);
                   printf("    %s commande2 archive.sap [nb pistes] [densit%s]\n", argv[0], eacute);
                   printf("    %s commande3 lecteur [densit%s] [entrelacement]\n", argv[0], eacute);
                   printf("o%s la commande1 est prise parmi les suivantes:\n", ugrave);
                   printf("  -h, --help          affiche cette aide\n");
                   printf("  -v, --version       affiche la version du programme\n");
                   printf("  -t, --list          affiche la liste des fichiers de l'archive SAP\n");
                   printf("  -p, --pack          archive une disquette Thomson vers une archive SAP\n");
                   printf("  -u, --unpack        d%ssarchive une archive SAP vers une disquette Thomson\n", eacute);
                   printf("et o%s la commande2 est prise parmi les suivantes:\n", ugrave);
                   printf("  -c, --create        cr%se une archive SAP vide\n", eacute);
                   printf("et o%s la commande3 est prise parmi les suivantes:\n", ugrave);
                   printf("  -d, --dir           affiche le contenu d'une disquette Thomson\n");
                   printf("  -f, --format        formate une disquette au format Thomson\n");
               }
               else
               {
                   printf("SAP2 is an archiving system for Thomson 3\"5 et 5\"25 floppies. It allows you\n");
                   printf("to create disk images as SAP archives.\n\n");
                   printf("Usage:\n");
                   printf("    %s (interactive mode)\n", argv[0]);
                   printf("    %s command1 archive.sap [lecteur] [density]\n", argv[0]);
                   printf("    %s command2 archive.sap [nb tracks] [density]\n", argv[0]);
                   printf("    %s command3 drive [density] [interleave]\n", argv[0]);
                   printf("where command1 is token among the following:\n");
                   printf("  -h, --help          display this help\n");
                   printf("  -v, --version       display program version\n");
                   printf("  -t, --list          display the list of the SAP archive files\n");
                   printf("  -p, --pack          archive a Thomson floppy to a SAP archive\n");
                   printf("  -u, --unpack        unarchive a SAP archive to a Thomson floppy\n");
                   printf("and where commande2 is token among the following:\n");
                   printf("  -c, --create        create an empty SAP archive\n");
                   printf("and where commande3 is token among the following:\n");
                   printf("  -d, --dir           display dir of a Thomson floppy\n");
                   printf("  -f, --format        create a Thomson formatted floppy\n");
               }
               break;

            case 'v':  /* version */
               if (is_fr)
                   printf("SAP2 version "SAP2_VERSION_STR" pour "SAP2_PLATFORM_STR", copyright (C) 2000-2003 Eric Botcazou.\n");
               else
                   printf("SAP2 version "SAP2_VERSION_STR" for "SAP2_PLATFORM_STR", copyright (C) 2000-2003 Eric Botcazou.\n");
               break;

            case 'd':  /* dir */
               if (argc < 3)
                  usage(argv[0]);

               if (FloppyInit(&fi, 0) > 0) {
                  ret = ViewDiskDir(atoi(argv[2]), argc > 3 ? atoi(argv[3]) : 2, -1);
                  FloppyExit();
               }
               else {
                  ret = 1;
               }
               break;

            case 'c':  /* create */
               if (argc < 3)
                  usage(argv[0]);

               if (argc < 4) {
                  ret = CreateEmptyArchive(argv[2], SAP_FORMAT1, SAP_TRK80);
               }
               else if (argc == 4){
                  if (atoi(argv[3]) == 40)
                     ret = CreateEmptyArchive(argv[2], SAP_FORMAT1, SAP_TRK40);
                  else if (atoi(argv[3]) == 80)
                     ret = CreateEmptyArchive(argv[2], SAP_FORMAT1, SAP_TRK80);
                  else
                     ret = 1;
               }
               else {
                  if (atoi(argv[3]) == 40)
                     ret = CreateEmptyArchive(argv[2], (atoi(argv[4]) == 1 ? SAP_FORMAT2 : SAP_FORMAT1), SAP_TRK40);
                  else if (atoi(argv[3]) == 80)
                     ret = CreateEmptyArchive(argv[2], (atoi(argv[4]) == 1 ? SAP_FORMAT2 : SAP_FORMAT1), SAP_TRK80);
                  else
                     ret = 1;
               }
               break;

            case 'p':  /* pack */
               if (argc < 4)
                  usage(argv[0]);

               if (FloppyInit(&fi, 0) > 0) {
                  ret = PackArchive(argv[2], atoi(argv[3]), argc > 4 ? atoi(argv[4]) : 2, 0);
                  FloppyExit();
               }
               else {
                  ret = 1;
               }
               break;

            case 't':  /* list */
               if (argc < 3)
                  usage(argv[0]);

               ret = ViewArchiveDir(argv[2], -1);
               break;

            case 'f':  /* format */
               if (argc < 3)
                  usage(argv[0]);

               if (FloppyInit(&fi, 1) > 0) {
                  if (argc == 3)
                     ret = FormatDisk(atoi(argv[2]), 2, (SAP_NSECTS-1)/2, 0);
                  else if (argc == 4)
                     ret = FormatDisk(atoi(argv[2]), atoi(argv[3]), (SAP_NSECTS-1)/2, 0);
                  else
                     ret = FormatDisk(atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), 0);

                  FloppyExit();
               }
               else {
                  ret = 1;
               }
               break;

            case 'u':  /* unpack */
               if (argc < 4)
                  usage(argv[0]);

               if (FloppyInit(&fi, 1) > 0) {
                  ret = UnpackArchive(argv[2], atoi(argv[3]), argc > 4 ? atoi(argv[4]) : 2, 0);
                  FloppyExit();
               }
               else {
                  ret = 1;
               }
               break;

            default:
               usage(argv[0]);
         }  /* end of switch */
      }
      else {
         usage(argv[0]);
      }
   }

   return ret;
}

