/*  Lecture des disquettes Thomson sous Linux
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


#include "floppy.h"

#if defined(linux)
#ifndef SCAN_DEPEND
   #include <stdio.h>
   #include <fcntl.h>
   #include <unistd.h>
   #include <sys/ioctl.h>
   #include <linux/fd.h>
   #include <linux/fdreg.h>
#endif


#define DISK_RETRY   3

static int rw_flag;
static int fd[2] = {-1, -1};
static int drive_type[2];

#define IS_5_INCHES(drive) ((drive_type[drive]>0) && (drive_type[drive]<3))

#define SET_NO_MULTITRACK(lval)  (lval &= ~0x80)
#define SET_NO_MFM(lval)  (lval &= ~0x40)



/* ResetDrive:
 *  Réinitialise le lecteur de disquettes.
 */
static void ResetDrive(int drive, int density)
{
   struct floppy_struct fd_prm;

   fd_prm.head    = (density == 1 ? 1 : 2);  /* nr of heads */
   fd_prm.track   = (IS_5_INCHES(drive) ? 40 : 80);  /* nr of tracks */
   fd_prm.sect    = (density == 1 ? 4 : 8);  /* sectors per track (512-byte sector) */
   fd_prm.size    = fd_prm.head * fd_prm.track * fd_prm.sect;  /* nr of sectors total */
   fd_prm.stretch = 0;
   fd_prm.gap     = 0x1B;
   fd_prm.rate    = (density == 1 ? 0xB1 : 0x3A);  /* cf /usr/include/linux/fd.h */
   fd_prm.spec1   = 0xDF;
   fd_prm.fmt_gap = 0x2C;

   ioctl(fd[drive], FDSETPRM, &fd_prm);
}



/* OpenDrive:
 *  Obtient le descripteur de fichier pour le lecteur de disquettes.
 */
static int OpenDrive(int drive, int density)
{
   char dev_str[16]="";

   if (drive > (1 << 5))
       return 0;

   snprintf(dev_str, sizeof(dev_str), "/dev/fd%d", drive);

   if ((fd[drive]=open(dev_str, O_RDWR | O_NDELAY))<0)
       return 0;

   ResetDrive(drive, density);
   return 1;

/*
   if (rw_flag) {
      if ((fd[drive]=open(dev_str, O_RDWR | O_NDELAY))>=0)
         goto success;
   }
   else {
      if ((fd[drive]=open(dev_str, O_RDONLY | O_NDELAY))>=0)
         goto success;
   }

   return 0;

  success:
    ResetDrive(drive, density);
    return 1;
*/
}



/* ExecCommand:
 *  Exécute la commande spécifiée via l'appel ioctl() FDRAWCMD.
 */
static int ExecCommand(int drive, int density, struct floppy_raw_cmd *fd_cmd)
{
   int i, ret = 0;

   if (fd[drive]<0 && !OpenDrive(drive, density))
      return 0x10;  /* lecteur non prêt */

   for (i=0; i<DISK_RETRY; i++) {
      ret=ioctl(fd[drive], FDRAWCMD, fd_cmd);

      if (ret >= 0)
         break;

      ResetDrive(drive, density);
   }

   if (ret<0)
      return 0x10;  /* lecteur non prêt */

#ifdef DEBUG
   printf("fd_cmd reply: ");
   for (i=0; i<3; i++)
      printf("ST%d=%02x ", i, fd_cmd->reply[i]);
   printf("\n");
#endif

   switch (fd_cmd->reply[1]) { /* ST1 */

      case 0x01:  /* Missing Address Mark */
         return 0x04;   /* erreur sur l'adresse */

      case 0x02:  /* Write Protected */
         return 0x01;   /* disk protégé en écriture */

      case 0x04:  /* No Data - unreadable */
         return 0x08;   /* erreur sur les données */

      case 0x20:  /* CRC error in data or addr */
         if (fd_cmd->reply[2]==0x20)
            return 0x08;   /* erreur sur les données */
         else
            return 0x04;   /* erreur sur l'adresse */

      default:
         return 0;  /* OK */
   }
}



/* FloppyReadSector:
 *  Lit le secteur spécifié sur la disquette.
 */
int FloppyReadSector(int drive, int density, int track, int sector, int nsects, unsigned char data[])
{
   struct floppy_raw_cmd fd_cmd;
   int pc_drive = drive/2;

   /* paramètres de commande */
   fd_cmd.flags  = FD_RAW_READ | FD_RAW_INTR | FD_RAW_NEED_SEEK;
   fd_cmd.data   = data;
   fd_cmd.length = (density == 1 ? 128 : 256)*nsects;  /* buffer length */
   fd_cmd.rate   = IS_5_INCHES(pc_drive) ? 1 : 2;
   fd_cmd.track  = IS_5_INCHES(pc_drive) ? track*2 : track;  /* physical cylinder */

   fd_cmd.cmd[0] = FD_READ;
   fd_cmd.cmd[1] = (drive%2) << 2;  /* physical head */
   fd_cmd.cmd[2] = track;  /* logical cylinder */
   fd_cmd.cmd[3] = 0;  /* logical head */
   fd_cmd.cmd[4] = sector;
   fd_cmd.cmd[5] = density - 1;  /* sector size = 128*2^n */
   fd_cmd.cmd[6] = 16;  /* num sectors */
   fd_cmd.cmd[7] = 0x1B;  /* gap */
   fd_cmd.cmd[8] = 0xFF;
   fd_cmd.cmd_count = 9;

   SET_NO_MULTITRACK(fd_cmd.cmd[0]);

   if (density == 1)
      SET_NO_MFM(fd_cmd.cmd[0]);  /* FM coding */

   return ExecCommand(pc_drive, density, &fd_cmd);
}



/* FloppyWriteSector:
 *  Ecrit le secteur spécifié sur la disquette.
 */
int FloppyWriteSector(int drive, int density, int track, int sector, int nsects, const unsigned char data[])
{
   struct floppy_raw_cmd fd_cmd;
   int pc_drive = drive/2;

   /* paramètres de commande */
   fd_cmd.flags  = FD_RAW_WRITE | FD_RAW_INTR | FD_RAW_NEED_SEEK;
   fd_cmd.data   = (unsigned char *)data;
   fd_cmd.length = (density == 1 ? 128 : 256)*nsects;  /* buffer length */
   fd_cmd.rate   = IS_5_INCHES(pc_drive) ? 1 : 2;
   fd_cmd.track  = IS_5_INCHES(pc_drive) ? track*2 : track;  /* physical cylinder */

   fd_cmd.cmd[0] = FD_WRITE;
   fd_cmd.cmd[1] = (drive%2) << 2;  /* physical head */
   fd_cmd.cmd[2] = track;  /* logical cylinder */
   fd_cmd.cmd[3] = 0;  /* logical head */
   fd_cmd.cmd[4] = sector;
   fd_cmd.cmd[5] = density - 1;  /* sector size = 128*2^n */
   fd_cmd.cmd[6] = 16;  /* num sectors */
   fd_cmd.cmd[7] = 0x1B;  /* gap */
   fd_cmd.cmd[8] = 0xFF;
   fd_cmd.cmd_count = 9;

   SET_NO_MULTITRACK(fd_cmd.cmd[0]);

   if (density == 1)
      SET_NO_MFM(fd_cmd.cmd[0]);  /* FM coding */

   return ExecCommand(pc_drive, density, &fd_cmd);
}



/* FloppyFormatTrack:
 *  Formate la piste en utilisant la table des headers spécifiée.
 */
int FloppyFormatTrack(int drive, int density, int track, const unsigned char header_table[])
{
   struct floppy_raw_cmd fd_cmd;
   int pc_drive = drive/2;

   /* paramètres de commande */
   fd_cmd.flags  = FD_RAW_WRITE | FD_RAW_INTR | FD_RAW_NEED_SEEK;
   fd_cmd.data   = (unsigned char *)header_table;
   fd_cmd.length = 64;
   fd_cmd.rate   = IS_5_INCHES(pc_drive) ? 1 : 2;
   fd_cmd.track  = IS_5_INCHES(pc_drive) ? track*2 : track;  /* physical cylinder */

   fd_cmd.cmd[0] = FD_FORMAT;
   fd_cmd.cmd[1] = (drive%2) << 2;
   fd_cmd.cmd[2] = density - 1;  /* sector size = 128*2^n */
   fd_cmd.cmd[3] = 16;  /* num sectors */
   fd_cmd.cmd[4] = 0x2C;
   fd_cmd.cmd[5] = 0xE5;  /* filler_byte */
   fd_cmd.cmd_count = 6;

   SET_NO_MULTITRACK(fd_cmd.cmd[0]);  /* nop */

   if (density == 1)
      SET_NO_MFM(fd_cmd.cmd[0]);  /* FM coding */

   return ExecCommand(pc_drive, density, &fd_cmd);
}



/* FloppyInit:
 *  Initialise le module de lecture de disquettes.
 */
int FloppyInit(struct floppy_info *fi, int enable_write_support)
{
   struct floppy_drive_params fd_params;
   char dev_str[16];
   int i, num_drives = 0;

   for (i=0; i<2; i++) {
      /* get drive type */
      snprintf(dev_str, sizeof(dev_str), "/dev/fd%d", i);

      if ((fd[i]=open(dev_str, O_RDWR | O_NDELAY))<0) {
         drive_type[i] = 0;
         fi->drive_type[2*i] = 0;
         fi->drive_type[2*i+1] = 0;
      }
      else {
         ioctl(fd[i], FDGETDRVPRM, &fd_params);
         close(fd[i]);
         fd[i] = -1;

         if (fd_params.cmos > 6) {
            drive_type[i] = 0;
            fi->drive_type[2*i] = 0;
            fi->drive_type[2*i+1] = 0;
         }
         else {
            drive_type[i] = fd_params.cmos;
            fi->drive_type[2*i] = fd_params.cmos;
            fi->drive_type[2*i+1] = fd_params.cmos;
         }

         num_drives++;
      }
   }

   fi->num_drives = num_drives;
   fi->fm_support = 1;
   fi->write_support = enable_write_support;

   rw_flag = enable_write_support;
   
   return num_drives;
}



/* FloppyExit:
 *  Met au repos le module de lecture de disquettes.
 */
void FloppyExit(void)
{
   int i;

   for (i=0; i<2; i++) {
      if (fd[i] >= 0) {
         close(fd[i]);
         fd[i] = -1;
      }
   }
}

#else

/* Dummy implementation for system without the linux floppy disk API */

int FloppyReadSector(int drive, int density, int track, int sector, int nsects, unsigned char data[])
{
    (void)drive;
    (void)density;
    (void)track;
    (void)sector;
    (void)nsects;
    (void)data;
    return 0x10;
}

int FloppyFormatTrack(int drive, int density, int track, const unsigned char header_table[])
{
    (void)drive;
    (void)density;
    (void)track;
    (void)header_table;
    return 0x10;
}

int FloppyWriteSector(int drive, int density, int track, int sector, int nsects, const unsigned char data[])
{
    (void)drive;
    (void)density;
    (void)track;
    (void)sector;
    (void)nsects;
    (void)data;
    return 0x10;
}

int FloppyInit(struct floppy_info *fi, int enable_write_support)
{
    (void)fi;
    (void)enable_write_support;
    /* return number of drive : 0 */
    return 0;
}

void FloppyExit(void)
{
}
#endif
