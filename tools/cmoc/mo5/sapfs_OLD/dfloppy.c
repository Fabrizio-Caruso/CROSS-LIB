/*  Lecture des disquettes Thomson sous MS-DOS
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

/* Limitations:
 *  - les faces 1 des disquettes ne sont pas accessibles,
 *  - les disquettes simple densité (format FM) ne sont pas lisibles.
 */

#ifndef SCAN_DEPEND
   #include <stdio.h>
   #include <stddef.h>
   #include <bios.h>
   #include <dpmi.h>
   #include <sys/movedata.h>
#endif

#include "floppy.h"


#define DISK_RETRY     3

struct floppy_cmd {
   int cmd;
   int head;
   int track;
   int sector;
   int nsects;
   void *buffer;
};

static int drive_type[2];
static int dpt_addr = 0;

#define DPT_SIZE      11

static unsigned char pc_dpt[DPT_SIZE];



/* SetDiskParameters:
 *  Initialise les paramètres disquette pour le format Thomson.
 */
static void SetDiskParameters(int density)
{
   unsigned char to_dpt[DPT_SIZE];

   if (!dpt_addr) {
      /* on récupère le vecteur 0x1E du BIOS qui pointe sur la table
         des paramètres de la disquette */
      _dosmemgetl(0x1E*4, 1, &dpt_addr);

      /* on sauvegarde les paramètres originaux */
      _dosmemgetb(dpt_addr, DPT_SIZE, pc_dpt);
   }

   /* on fixe les nouveaux paramètres */
   to_dpt[0x0]=0xDF;  /* spec1                      */
   to_dpt[0x1]=0x02;  /* spec2                      */
   to_dpt[0x2]=0x25;  /* motor turn off delay       */
   to_dpt[0x3]=density - 1;  /* sector size = 128*2^n */
   to_dpt[0x4]=0x10;  /* 16 sectors/track           */
   to_dpt[0x5]=0x1B;  /* gap between sectors        */
   to_dpt[0x6]=(density == 1 ? 0x80 : 0xFF);  /* data length */
   to_dpt[0x7]=0x2C;  /* gap when formatting        */
   to_dpt[0x8]=0xE5;  /* filler byte                */
   to_dpt[0x9]=0x0F;  /* head settle time           */
   to_dpt[0xA]=0x08;  /* motor start time           */
   _dosmemputb(to_dpt, DPT_SIZE, dpt_addr);

   /* reset */
   biosdisk(0, 0, 0, 0, 1, 0, NULL);
}



/* ExecCommand:
 *  Exécute la commande spécifiée (via l'interruption 13h du BIOS).
 */
static int ExecCommand(int drive, int density, struct floppy_cmd *fd_cmd)
{
   int i;
   int ret=0x10;

   SetDiskParameters(density);

   for (i=0; i<DISK_RETRY; i++) {
      ret=biosdisk(fd_cmd->cmd, drive, fd_cmd->head, fd_cmd->track,
                    fd_cmd->sector, fd_cmd->nsects, fd_cmd->buffer);

      if (ret==0)  /* commande OK? */
         break;

      if ((i>1) && (ret==0x11)) { /* commande non OK mais corrigée par ctrl? */
         ret=0;
         break;
      }

      /* reset du lecteur */
      biosdisk(0, 0, 0, 0, 1, 0, NULL);
   }

   switch (ret) {

      case 0x02:  /* address mark not found */
         return 0x04;   /* erreur sur l'adresse */

      case 0x03:  /* disk write-protected */
         return 0x01;   /* disk protégé en écriture */

      case 0x04:  /* sector not found */
      case 0x07:  /* drive parameter activity failed */
      case 0x10:  /* data read (CRC or ECC) error */
      case 0x0A:  /* bad sector flag detected */
      case 0x0B:  /* bad track flag detected */
         return 0x08;   /* erreur sur les données */

      case 0x06:  /* floppy disk removed */
      case 0x80:  /* disk timed out or failed to respond */
         return 0x10;   /* lecteur non prêt */

      default:
         return 0;  /* OK */
   }
}



/* FloppyReadSector:
 *  Lit le secteur spécifié sur la disquette.
 */
int FloppyReadSector(int drive, int density, int track, int sector, int nsects, unsigned char data[])
{
   struct floppy_cmd fd_cmd;

   fd_cmd.cmd    = 2;
   fd_cmd.head   = drive%2;
   fd_cmd.track  = track;
   fd_cmd.sector = sector;
   fd_cmd.nsects = nsects;
   fd_cmd.buffer = data;

   return ExecCommand(drive/2, density, &fd_cmd);
}



/* FloppyWriteSector:
 *  Ecrit le secteur spécifié sur la disquette.
 */
int FloppyWriteSector(int drive, int density, int track, int sector, int nsects, const unsigned char data[])
{
   struct floppy_cmd fd_cmd;

   fd_cmd.cmd    = 3;
   fd_cmd.head   = drive%2;
   fd_cmd.track  = track;
   fd_cmd.sector = sector;
   fd_cmd.nsects = nsects;
   fd_cmd.buffer = (unsigned char *)data;

   return ExecCommand(drive/2, density, &fd_cmd);
}



/* FloppyFormatTrack:
 *  Formate la piste en utilisant la table des headers spécifiée.
 */
int FloppyFormatTrack(int drive, int density, int track, const unsigned char header_table[])
{
   struct floppy_cmd fd_cmd;
   int format_type = 0;

   /* sélection du format de formatage */
   switch (drive_type[drive/2]) {

      case 1:
         /* 320/360 kb in 360 kb drive */
         format_type = 1;
         break;

      case 2:
         /* 320/360 kb in 1.2 Mb drive */
         format_type = 2;
         break;

      case 3:
      case 4:
      case 5:
      case 6:
         /* 720 kb in 720 kb/1.44 Mb/2.88 Mb drive */
         format_type = 4;
         break;
   }

   biosdisk(23, drive/2, 0, 0, 1, format_type, NULL);

   fd_cmd.cmd    = 5;
   fd_cmd.head   = drive%2;
   fd_cmd.track  = track;
   fd_cmd.sector = 1;
   fd_cmd.nsects = 16;
   fd_cmd.buffer = (unsigned char *)header_table;

   return ExecCommand(drive/2, density, &fd_cmd);
}



/* FloppyInit:
 *  Initialise le module de lecture de disquettes.
 */
int FloppyInit(struct floppy_info *fi, int enable_write_support)
{
   __dpmi_regs r;
   int i, num_drives = 0;

   (void) enable_write_support;

   for (i=0; i<2; i++) {
      /* get drive parameters (int 13h, function 08h) */
      r.h.ah = 0x08;
      r.h.dl = i;

      __dpmi_int(0x13, &r);

      if (r.x.flags&1) {  /* CF set? */
         drive_type[i] = 0;

         fi->drive_type[2*i] = 0;
         fi->drive_type[2*i+1] = 0;
      }
      else {
         if (r.h.bl > 6) {
            drive_type[i] = 0;

            fi->drive_type[2*i] = 0;
            fi->drive_type[2*i+1] = 0;
         }
         else {
            drive_type[i] = r.h.bl;

            fi->drive_type[2*i] = r.h.bl;
            fi->drive_type[2*i+1] = 0;  /* face 1 unsupported */
         }

         num_drives++;
      }
   }

   fi->num_drives = num_drives;
   fi->fm_support = 0;
   fi->write_support = 1;

   return num_drives;
}



/* FloppyExit:
 *  Met au repos le module de lecture de disquettes.
 */
void FloppyExit(void)
{
   if (dpt_addr) {
      /* restaure les paramètres originaux */
      _dosmemputb(pc_dpt, DPT_SIZE, dpt_addr);

      /* reset */
      biosdisk(0, 0, 0, 0, 1, 0, NULL);

      dpt_addr = 0;
   }
}

