/*				<<<<Last Modified: Thu Feb 08 15:08:10 1996>>>>
------------------------------------------------------------------------------

	=====
	CPCfs  --  f s . c  --	Manageing the Filesystem
	=====

	Version 0.85		  	(c) February '96 by Derik van Zuetphen
------------------------------------------------------------------------------
*/

#include <sys/stat.h>

#ifdef WIN64
#include <direct.h>
#include <io.h>
#endif

#if defined(WIN32) || defined(WIN64)
#include <direct.h>
#include <io.h>
#endif

#include "cpcfs.h"

#ifdef DOS
#if  !defined(WIN32) && !defined(WIN64)
#endif
#endif


/****** Variables ******/

int	cur_trk = -1;		/* flag for no track loaded */
int	cur_hd = -1;
int	cur_blk = -1;
/* KT - added directory dirty flag - directory is written only if it has changed! */
int directory_dirty = 0;
/* KT - added track dirty flag - track is written only if it has changed! */
int track_dirty = 0;

int image_type = 0;

bool tag_ok () {
/*   ^^^^^^ */

	/* KT - 27/04/2000 - Support for Extended disk image */
	if (strncmp("EXTENDED", (signed char *)disk_header.tag,8)==0)
	{
		image_type = 1;
		return TRUE;
	}
	else
	if (strncmp("MV - CPC",(signed char*)disk_header.tag,8)==0)
	{
		image_type = 0;
		return TRUE;
	}

	return FALSE;
}

const char *accessing_invalid_block_message = "Warning, attempting to access invalid block!";

void alloc_block(int blk) {
/*   ^^^^^^^^^^^ */

	unsigned long blk_byte_offset;
	unsigned long blk_bit_offset;

  /* KT - if a disc was opened and the format was not detected correctly, then
	a invalid blk could be passed to this routine. Previously this attempted
	to read off the end of the array and cause an error. I've added this check
	to stop that from occuring and alerting this fact to the user */
	if ((blk<0) || (blk>=(dpb->DSM+1)))
	{
		errorf(FALSE, accessing_invalid_block_message);
		return;
	}

	blk_byte_offset = blk>>3;
	blk_bit_offset = blk & 0x07;

	blk_alloc[blk_byte_offset]|=(1<<blk_bit_offset);
}




void free_block(int blk) {
/*   ^^^^^^^^^^ */

	unsigned long blk_byte_offset;
	unsigned long blk_bit_offset;

	if ((blk<0) || (blk>=(dpb->DSM+1)))
	{
		errorf(FALSE, accessing_invalid_block_message);
		return;
	}

	blk_byte_offset = blk>>3;
	blk_bit_offset = blk & 0x07;

	blk_alloc[blk_byte_offset]&=~(1<<blk_bit_offset);
}


bool is_free_block(int blk) {
/*   ^^^^^^^^^^^^^ */

	unsigned long blk_byte_offset;
	unsigned long blk_bit_offset;

	if ((blk<0) || (blk>=(dpb->DSM+1)))
	{
		errorf(FALSE, accessing_invalid_block_message);
		return FALSE;
	}

	blk_byte_offset = blk>>3;
	blk_bit_offset = blk & 0x07;

	return (bool)((blk_alloc[blk_byte_offset]&(1<<blk_bit_offset))==0);
}


void calc_allocation () {
/*   ^^^^^^^^^^^^^^^ */
int	i;
	allocated_blks = 0;
	free_blks = 0;
	/* KT; removed dpb->DBL because directory blocks will already be allocated */
	for (i=0; i<dpb->DSM; i++)
		if (is_free_block(i)) free_blks++;
		else allocated_blks++;
	percentage=100*allocated_blks/(float)(allocated_blks+free_blks);
	total_blks = allocated_blks + free_blks;
}


bool inactive () {
/*   ^^^^^^^^ */
	if (*disk_header.tag==0) {
		printm(1,"No image loaded!\n");
		return TRUE;
	} else
		return FALSE;
}



/* KT - added support for different block orders */

void	calc_t_s_h(int blk,int *track, int *sector, int *head)
{
	int t,s,h;

	unsigned long byte_offset;
	unsigned long sector_offset;

	t = -1;
	s = -1;
	h = -1;

	/* calc byte offset from block index and bytes per block */
	byte_offset = blk*dpb->BLS;
	/* calc sector offset using sector size */
	sector_offset = byte_offset/dpb->BPS;

	/* calculate sector index within track by taking remainder after dividing by number of sectors
	per track (0-dpb->SECS). Sector ID is added on when sector is accessed! */
	s = sector_offset % dpb->SECS;

	/* calculate track offset by dividing by number of sectors */
	t = (sector_offset / dpb->SECS) + dpb->OFS;

	/* order system for blocks */
	switch (dpb->order)
	{
		/* track 0 side 0, track 1 side 0... track n side 0, track n side 1, track n-1 side 1 ... track 0 side 1 */
		case ORDER_CYLINDERS:
		{
		 /* calc side */
			h = t/dpb->TRKS;

			/* odd side */
			if (h & 1)
			{
				/* tracks in reverse */
				t = dpb->TRKS - (t % dpb->TRKS) - 1;
			}
			else
			{
				/* tracks in order */
				t = t % dpb->TRKS;
			}
		}
		break;

		/* track 0 side 0, track 0 side 1, track 1 side 0, track 1 side 1... track n side 0, track n side 1*/
		case ORDER_SIDES:
		{
			/* calc side */
			h = t % dpb->HDS;
			/* calc track */
			t = t/dpb->HDS;
		}
		break;

		/* track 0 side 0, track 1 side 0... track n side 0, track 0 side 1, track 1 side 1....track n side 1 */
		case ORDER_EAGLE:
		{
			/* calc side */
			h = t / dpb->TRKS;

			/* calc track */
			t = t % dpb->TRKS;
		}
		break;
	}
	*track = t;
	*sector = s;
	*head = h;
}


#if 0
/* Calculate track, sector, and head number of the first sector of block
<blk>.

A quick picture:

		secs
	   .------^-------.
	   _______________
	  / -2- - - - - >/|  } heads
	 /______________/ | }
      / | -1- - - - - > | |
     :	|		| |
     :	|		| |
     :	|		| |
trks =	|  -4- - - - - -| |
     :	|		| |
     :	| -3- - - - - > | |
     :	|		| |
      \ |_______________|/


numbering scheme:
first secs, then heads, then tracks
    %s	       /s%h	  /s/h%t	has to be applied on running sec number

s = sec/trk, h = heads, t = tracks (here infinite)

*/


int trk_calc(int blk) {
/*  ^^^^^^^^ */
	return ((long)blk*dpb->BLS/dpb->BPS + dpb->OFS*dpb->SECS)
		/ (dpb->SECS*dpb->HDS);
}

int sec_calc(int blk) {
/*  ^^^^^^^^
The returned number is not shifted by <sec_offset>! */
	return ((long)blk*dpb->BLS/dpb->BPS + dpb->OFS*dpb->SECS) % dpb->SECS;
}

int hd_calc(int blk) {
/*  ^^^^^^^ */
	return ((long)blk*dpb->BLS/dpb->BPS + dpb->OFS*dpb->SECS)
		/ dpb->SECS % dpb->HDS;
}
#endif


/* TO BE FIXED ! */

int blk_calc(int hd, int trk, int sec) {
/*  ^^^^^^^^
Return the blocknumber of position <hd>, <trk>, <sec> or -1, if it is a
reserved position
*/
#if 0
	switch (dpb->order)
	{
		/* track 0 side 0, track 1 side 0... track n side 0, track n side 1, track n-1 side 1 ... track 0 side 1 */
		case ORDER_CYLINDERS:
		{



		}
		break;

		case ORDER_EAGLE:
		{

		}
		break;

		case ORDER_SIDES:
		{



		}
		break;
	}
#endif

	if (trk*dpb->HDS+hd < dpb->OFS) return -1;
	return ((long)sec + hd*dpb->SECS
		+ trk*dpb->SECS*dpb->HDS
		- dpb->OFS*dpb->SECS)
		/ (dpb->BLS/dpb->BPS);
}

/* KT - added common free data function */
void free_image_data(void)
{
	if (blk_alloc)
	{
		free(blk_alloc);	blk_alloc=NULL;
	}

	if (track)
	{
		free(track);		track=NULL;
	}

	if (directory)
	{
		free(directory);	directory=(DirEntry*)NULL;
	}

	if (block_buffer)
	{
		free(block_buffer);	block_buffer=(uchar*)NULL;
	}

	*disk_header.tag = 0;
	dpb = NULL;

	if (imagefile >= 0)
	{
		close(imagefile);
		imagefile = -1;
	}
	cur_trk = -1;
	cur_blk = -1;
	directory_dirty = 0;
}



void abandonimage() {
/*   ^^^^^^^^^^^^ */
	free_image_data();

	errorf(FALSE,"Image \"%s\" abandoned!",imagename);
}


/********
  Tracks
 ********/

/* KT - get offset to track data. */
unsigned long get_track_offset(int track, int side)
{
	switch (image_type)
	{
		/* standard disk image */
		case 0:
		{
			/* attempt to access a invalid track and side */
			if ((track>=disk_header.nbof_tracks) || (side>=disk_header.nbof_heads))
				return 0;

			return (((track*disk_header.nbof_heads) + side) * disk_header.tracksize) + 0x0100;
		}
		/* extended disk image */
		case 1:
		{
			int i;
			unsigned long offset;

			/* attempt to access a invalid track and side */
			if ((track>=disk_header.nbof_tracks) || (side>=disk_header.nbof_heads))
				return 0;

			/* unformatted track? */
			if (disk_header.unused[((track*disk_header.nbof_heads)+side)]==0)
				return 0;

			/* for extended disk image, the disk header contains
			size of each track / 256. Use this table to calculate
			the offset to the track */

			offset = 0;

			for (i=0; i<(track*disk_header.nbof_heads)+side; i++)
			{
				offset += (disk_header.unused[i]<<8);
			}

			return offset + 0x0100;
		}

		default:
			break;
	}

	return 0;
}

unsigned long get_track_size(int track, int side)
{
	switch (image_type)
	{
		/* standard disk image */
		case 0:
		{
			/* attempt to access a invalid track and side */
			if ((track>=disk_header.nbof_tracks) || (side>=disk_header.nbof_heads))
				return 0;

			/* return size of track */
			return disk_header.tracksize;
		}

		/* extended disk image */
		case 1:
		{
			/* attempt to access a invalid track and side */
			if ((track>=disk_header.nbof_tracks) || (side>=disk_header.nbof_heads))
				return 0;

			/* return track size */
			return ((disk_header.unused[(track*disk_header.nbof_heads)+side])<<8);
		}

		default:
			break;
	}

	return 0;
}

int	is_track_header_valid(unsigned char *track)
{
	struct t_header *track_header = (struct t_header*)track;

	/* header limited to 29 sectors per track */
	if (track_header->SPT>29)
		return 0;

	/* unlikely to have a track where the BPS is 16384 bytes! */
	if (track_header->BPS>6)
		return 0;


	return 1;
}

int validate_image(void)
{
	int t,h;
	/* header is already validated */
	for (h=0; h<disk_header.nbof_heads; h++)
	{
		for (t=0; t<disk_header.nbof_tracks; t++)
		{
			int track_offset = get_track_offset(t,h);

			if (track_offset>=0)
			{
				int n;

				int track_size;

				track_size =get_track_size(t,h);

				if (track_size!=0)
				{
					/* formatted track */
					n = lseek(imagefile,track_offset,SEEK_SET);
					if (n == -1L)
					{
						return 0;
					}

					n = read(imagefile,track,track_size);
					if (n != track_size)
					{
						return 0;
					}

					/* seeked to position ok, and read data */

					/* check track header */
					if (!is_track_header_valid(track))
						return 0;
				}
			}
		}
	}

	return 1;

}



/* calculate the maximum size of a track from the disk image. Use this
to allocate a track buffer */
void malloc_track()
{
	unsigned long max_track_size;

	max_track_size = 0;

	switch (image_type)
	{
		case 0:
		{
			max_track_size = disk_header.tracksize;
		}
		break;

		case 1:
		{

			int t,h;

			for (h=0; h<disk_header.nbof_heads; h++)
			{
				for (t=0; t<disk_header.nbof_tracks; t++)
				{
					int track_size;

					track_size = get_track_size(t,h);

					if ((t==0) && (h==0))
					{
						max_track_size = track_size;
					}
					else
					{
						if (track_size>max_track_size)
						{
							max_track_size = track_size;
						}
					}
				}
			}
		}
		break;

		default:
			break;
	}

	track = Malloc(max_track_size);
}

int read_track (int hd, int trk) {
/*  ^^^^^^^^^^ */
long	n, pos;
int		track_size;

	if (trk == cur_trk && hd == cur_hd) {
		return 0;
	}

	printm(11,"[rt(%d,%d)] ",hd,trk);

	track_size = get_track_size(trk,hd);

	if (track_size == 0)
		return -1;

	pos = get_track_offset(trk, hd);

	if (pos==0)
		return -1;

	n = lseek(imagefile,pos,SEEK_SET);
	if (n == -1L) {
		errorf(TRUE,"Image corrupt! I cannot position on track %d!",
									trk);
		abandonimage();
		return -1;
	}

	n = read(imagefile,track,track_size);
	if (n != track_size) {
		errorf(TRUE,"Image corrupt! I can only read %ld bytes "
					"of track %d (instead of %d bytes)!",
					n,trk,disk_header.tracksize);
		abandonimage();
		return -1;
	}
	cur_trk = trk;
	cur_hd	= hd;
	track_dirty = 0;
	return 0;
}


int write_track() {
/*  ^^^^^^^^^^^ */
long	n,pos;
int track_size;

	if (cur_trk == -1) return 0;

	/* if track is not dirty quit */
	if (track_dirty==0) return 0;

	printm(11,"[wt(%d,%d)] ",cur_hd,cur_trk);

	track_size = get_track_size(cur_trk, cur_hd);

	if (track_size == 0)
		return -1;

	pos = (long)get_track_offset(cur_trk, cur_hd);

	if (pos==0)
		return -1;

	n = lseek(imagefile,pos,SEEK_SET);
	if (n == -1L) {
		errorf(TRUE,"Image corrupt! I cannot position on track %d!",
								cur_trk);
		abandonimage();
		return -1;
	}

	n = write(imagefile,track,track_size);
	if (n != track_size) {
		errorf(TRUE,"Something wrong! I cannot write %d bytes "
					"to track %d (only %d bytes written)!",
					disk_header.tracksize,cur_trk,n);
		abandonimage();
		return -1;
	}

	track_dirty = 0;

	return 0;
}

/* KT - added support for different block orders */
bool next_sector (int *hd, int *trk, int *sec) {
/*   ^^^^^^^^^^^
Assumes <sec> without offset. Answer TRUE if the <trk> or <hd> is changed. */

	int prev_hd;
	int prev_trk;

	prev_hd = *hd;
	prev_trk = *trk;

	switch (dpb->order)
	{
		case ORDER_CYLINDERS:
		{
			(*sec)++;

			if ((*sec) >= dpb->SECS)
			{
				(*sec) -= dpb->SECS;

				if ((*hd) & 1)
				{
					(*trk)--;

					if ((*trk)<0)
					{
	                 // not handled
					}
				}
				else
				{
					(*trk)++;

					if ((*trk)>=dpb->TRKS)
					{
						(*trk) = dpb->TRKS-1;
						(*hd)++;
					}
				}
			}
		}
		break;

		case ORDER_SIDES:
		{
			(*sec)++;
			if (*sec >= dpb->SECS)
			{
				*sec -= dpb->SECS;
				(*hd)++;
				if (*hd >= dpb->HDS)
				{
					*hd = 0;
					(*trk)++;
				}
			}
		}
		break;

		case  ORDER_EAGLE:
		{
			(*sec)++;

			if ((*sec) >= dpb->SECS)
			{
				(*sec) -= dpb->SECS;

				(*trk)++;

				if ((*trk)>dpb->TRKS)
				{
					*trk = 0;
					(*hd)++;
				}
			}
		}
		break;


	}

	/* if head, track or head and track have changed, then force a write of track */
	if ((prev_hd!=(*hd)) || (prev_trk!=(*trk)))
	{
		return TRUE;
	}

	return FALSE;
}



#if 0
bool next_sector (int *hd, int *trk, int *sec) {
/*   ^^^^^^^^^^^
Assumes <sec> without offset. Answer TRUE if the <trk> or <hd> is changed. */
	(*sec)++;
	if (*sec >= dpb->SECS) {
		*sec -= dpb->SECS;
		(*hd)++;
		if (*hd >= dpb->HDS) {
			*hd = 0;
			(*trk)++;
		}
		return TRUE;
	}
	return FALSE;
}
#endif

/* given a sector id, search through sector id's in track header
to find the position of the sector in the track. This allows
for interleaved sectors */
int	get_sector_pos_in_track(int r,int h)
{
	struct t_header *track_header = (struct t_header*)track;
	struct s_info	*sector_info = &track_header->sector[0];
	int i;
	int spt;

	/* defines number of sector id's stored */
	spt = track_header->SPT;

	for (i=0; i<spt; i++)
	{
		/* match sector id */
		if (sector_info->sector==r)
		{
			/* match head value */
			if (sector_info->head == h)
			{
				return i;
			}
		}

		sector_info++;
	}

	/* sector not found */
	return -1;
}

int get_sector_data_offset_extended(int pos)
{
	struct t_header *track_header = (struct t_header*)track;
	struct s_info	*sector_info = &track_header->sector[0];
	int i;
	int sector_offset;

	sector_offset = 0;

	for (i=0; i<pos; i++)
	{
		int sector_size;

		sector_size = sector_info->unused[0] +
					(sector_info->unused[1]<<8);

		sector_info++;
		sector_offset += sector_size;
	}

	return sector_offset;
}

/* N value used in formatting and reading/writing data. This defines
the amount of data stored in the sector */
int	get_sector_size_from_n(int n)
{
	return (1<<n)<<7;
}

uchar	*get_sector_data_ptr(int r,int h)
{
	struct t_header *track_header = (struct t_header*)track;

	/* using sector id find sector pos in track */
	int pos = get_sector_pos_in_track(r,h);
    int sector_offset = 0;

	if (pos==-1)
		return NULL;

    switch (image_type)
    {
        case 0:
        {
            /* for standard disk images, N in the track header should
            define the largest sector size */
            int sector_size = get_sector_size_from_n(track_header->BPS);

            sector_offset = 0x0100 + pos*sector_size;
        }
        break;

        case 1:
        {
            sector_offset = 0x0100 + get_sector_data_offset_extended(pos);
        }
        break;

        default:
            return NULL;
    }

	return track+sector_offset;
}

uchar *read_block (int blk) {
/*    ^^^^^^^^^^
Read block <blk> into a buffer and return its address, or NULL on error */
int	trk, sec, hd;
int	filled = 0;
uchar *sector_ptr;


	if (blk==cur_blk) return block_buffer;

	if ((blk<0) || (blk>=(dpb->DSM+1)))
		return NULL;

	printm(11,"[rb(%d)] ",blk);

/*	trk = trk_calc (blk);
	sec = sec_calc (blk);
	hd  = hd_calc (blk);*/

	calc_t_s_h(blk, &trk, &sec, &hd);

	while (filled < dpb->BLS)
	{
		int r,h;

		if (read_track(hd,trk))
		{
		 errorf(FALSE, "Warning: cannot find track, unable to read data!");

        	return NULL;
        }

		r = sec;

		if (hd==0)
		{
			h = dpb->side0_hd;
			r = dpb->SEC_side1[r];
			//r += dpb->SEC1_side1;
		}
		else
		{
			h = dpb->side1_hd;
			r = dpb->SEC_side2[r];
			//r += dpb->SEC1_side2;
		}

		sector_ptr = get_sector_data_ptr(r,h);

		if (sector_ptr==NULL)
		{
			errorf(FALSE, "Warning: cannot find sector (%d,%d,%d), unable to read data!",trk,hd,r);
          memset(block_buffer+filled, 0, dpb->BPS);
        //	return NULL;
		}
		else
		{

	    	memcpy(block_buffer+filled, sector_ptr, dpb->BPS);
         }

		filled += dpb->BPS;
		next_sector (&hd,&trk,&sec);
	}
	cur_blk = blk;
	return block_buffer;
}


uchar *write_block (int blk, uchar *buf) {
/*    ^^^^^^^^^^^
Return the written buffer or NULL on error */
int	trk, sec, hd;
int	filled = 0;
uchar *sector_ptr;

	printm(11,"[wb(%d)] ",blk);

	if ((blk<0) || (blk>=(dpb->DSM+1)))
		return NULL;

/*	trk = trk_calc (blk);
	sec = sec_calc (blk);
	hd  = hd_calc (blk);
*/

	calc_t_s_h(blk, &trk, &sec, &hd);

	while (filled < dpb->BLS)
	{
		int r,h;

		if (read_track(hd,trk))
        {
         		 errorf(FALSE, "Warning: cannot find track, unable to write data!");

        	return NULL;
         }

		r = sec;

		if (hd==0)
		{
			h = dpb->side0_hd;
			r = dpb->SEC_side1[r];
			//r+=dpb->SEC1_side1;
		}
		else
		{
			h = dpb->side1_hd;
			r = dpb->SEC_side2[r];
			//r+=dpb->SEC1_side2;
		}

		sector_ptr = get_sector_data_ptr(r,h);

		if (sector_ptr==NULL)
		{
			errorf(FALSE, "Warning: cannot find sector, unable to write data!");

			return NULL;
		}

		memcpy(sector_ptr, buf+filled, dpb->BPS);
		track_dirty = 1;

		filled += dpb->BPS;
		if (next_sector (&hd,&trk,&sec)) write_track();
	}
	write_track();
	return buf;
}

/* fill a block with 0x0e5 - nukes data in it */
void	nuke_block(int blk)
{
	if ((blk<0) || (blk>=(dpb->DSM+1)))
		return;

	if (block_buffer == NULL)
		return;

	memset(block_buffer, 0x0e5, dpb->BLS);

	write_block(blk, block_buffer);
}

/***********
  Directory
 ***********/


/* local definitions for glob_cpm_* */

#define GLOB_ENV_MAX	3
int	glob_env = 0;
	/* determines which global variable-set should <glob_cpm_*> use, */
	/* necessary for nested globs */

uchar	pattern[GLOB_ENV_MAX][INPUTLEN];
int	last_entry[GLOB_ENV_MAX];

int glob_cpm_next() {
/*  ^^^^^^^^^^^^^
Answer the next entry (or -1) with the same pattern as before */
int	i;
uchar	name[20];

	if (dpb==NULL)
		return -1;

	for (i=last_entry[glob_env]+1;i<=dpb->DRM;i++) {
		if (!directory[i].first) continue;
		build_cpm_name_32((signed char*)name, directory[i].user,
				  (signed char*)directory[i].root,
				  (signed char*)directory[i].ext);
		if (match((signed char*)pattern[glob_env],(signed char*)name)) {
			last_entry[glob_env] = i;
			return i;
		}
	}
	return -1;
}


int glob_cpm_file(char *pat) {
/*  ^^^^^^^^^^^^^
Scan the entries for the first file that matches <pattern> and answer its
first (according to <first> flag, see DirEntry) entry number,
If the pattern contains no filename part, *.* is assumed.
Errorcode is -1, if pattern not found.
The work is mostly deferred to <glob_cpm_next>. */

int	user;
uchar	root[INPUTLEN], ext[INPUTLEN];
const char errmsg[] = "Illegal filename \"%s\"";
root[0] = '\0';
ext[0] = '\0';
	if (parse_cpm_filename(pat,&user,root,ext))
		return errorf(FALSE,errmsg,pat);
	upper(root);
	upper(ext);
	if (*root==0) {
		if (user >= 0) {
			strcpy(root,"*");
			strcpy(ext,"*");
		} else {
		return errorf(FALSE,errmsg,pat);

		}
	}
	if (user==-1) user = cur_user;
	build_cpm_name((signed char*)pattern[glob_env], user,
		(signed char*)root, (signed char*)ext);
	last_entry[glob_env] = -1;	/* thus start with 0 */
	return glob_cpm_next();
}


/* local definitions for update_directory */

struct pair {uchar en; uchar ex;};

int cmp_pair(struct pair *x, struct pair *y) {
	if (x->ex < y->ex) return -1;
	if (x->ex > y->ex) return 1;
	return 0;
}

void update_directory(int set_dirty) {
/*   ^^^^^^^^^^^^^^^^
(Re-)Organizes the directory structure (Packing the name, making a linked
list) */

int	i, j;

/* a dynamic array of (entry,extent) pairs */
struct pair	*map;

	if (set_dirty)
	{
		directory_dirty = 1;
	}


	printm(10,"[ud] ");
	map = (struct pair*)Malloc(sizeof(DirEntry)*(dpb->DRM+1));

	/* KT - added memory fail check */
	if (map==NULL)
		return;

/****** packing a name of kind "FOO	BAR" to "FOO.BAR\0" ******/
	for (i=0;i<=dpb->DRM;i++) {
		if (directory[i].user == 0xE5) continue;
		build_cpm_name_32((signed char*)directory[i].name, -1,
				  (signed char*)directory[i].root,
				  (signed char*)directory[i].ext);
	}


/****** organizing the directory structure as linked list ******/

/* set entries in the directory to "not visited"
<size> = 0 : never visit; <size> = -1 : not yet visited */
	for (i=0;i<=dpb->DRM;i++) {
		if (directory[i].user == 0xE5) /*NOT <filler>, E5 is required*/
			directory[i].size = 0;	/* never visit empty entries */
		else
			directory[i].size = -1; /* not visited */
		directory[i].first = FALSE;
		directory[i].next  = -1;
	};

/* scan the entries */
	for (i=0;i<=dpb->DRM;i++) {
		if (directory[i].size > -1) continue;

/* reset the map */
		for (j=0;j<=dpb->DRM;j++) {map[j].en=(uchar)j;map[j].ex=(uchar)0xFF;}

/* fill the map with <extent> from the directory */
		map[i].ex = directory[i].extent;
		for (j=0;j<=dpb->DRM;j++) {
			if ((directory[j].size == -1) &&
			    (directory[j].user == directory[i].user) &&
			    (i!=j) &&
			    (strcmp((signed char*)directory[i].name,
				    (signed char*)directory[j].name)==0)) {
					map[j].ex = directory[j].extent;
					directory[j].size = 0;
			}
		}
/* sort the map according to map[].ex, not necessary in most cases */
		qsort(map,dpb->DRM+1,sizeof(struct pair),
			(int(*)(const void*,const void*))cmp_pair);

/* fill <first>, <size> and <next> from the map */
		directory[map[0].en].first = TRUE;
		j=1;
		while (map[j].ex < 0xFF) {
			directory[map[j-1].en].next = map[j].en;
			j++;
		}
		directory[map[j-1].en].next = -1;

/* the filesize located in the first fileentry can be calculated from the
<extent> and <rec> fields of the last fileentry */

		/* KT: changed calculation to use extent number, and block size */
		directory[map[0].en].size =
			((long)directory[map[j-1].en].extent * EXTENTSIZE)
			+ (directory[map[j-1].en].rec * RECORDSIZE);


	} /* for i */

	free(map); map=NULL;
}


void get_directory() {
/*   ^^^^^^^^^^^^^ */
int	i,j,off;
uchar	*buf;
int	mask;

	printm(10,"[rd] ");

	for (i=0; i<=dpb->DRM; i++)
	{
		directory[i].user = 0x0e5;
	}

/* reading the directory data */
	for (i=0;i<=dpb->DRM;i++) {
		buf = read_block((signed)i*32/dpb->BLS);

		if (buf==0)
			break;

		off = i*32 % dpb->BLS;
/* user, name, ... */
		directory[i].user	= buf[off+0];
		for (j=0;j<8;j++) directory[i].root[j] = (buf[off+j+1] & 0x7F);
		for (j=0;j<3;j++) directory[i].ext[j]  = (buf[off+j+9] & 0x7F);
		directory[i].name[0]	= 0;

		directory[i].extent	= buf[off+12];
		directory[i].unused[0]	= buf[off+13];
		directory[i].unused[1]	= buf[off+14];
		directory[i].rec	= buf[off+15];

/* attributes */
		mask=0x1;
		directory[i].attr = 0;
		for (j=11;j>0;j--) {
			if (buf[off+j]&0x80) directory[i].attr |= mask;
			mask <<= 1;
		}

/* block pointer */
		for (j=0;j<16;j++) directory[i].blk[j] = 0;
		if (BLKNR_SIZE==1) {
			for (j=0;j<16;j++) {
				directory[i].blk[j] = buf[off+16+j];
			}
		} else if (BLKNR_SIZE==2) {
			for (j=0;j<8;j++) {
				directory[i].blk[j] = (buf[off+16+2*j]
							+ (buf[off+17+2*j]<<8));
			}
		}
	}

	update_directory(0);
	/*for (i=0;i<dpb->DBL;i++) alloc_block(i,0);*/ /* 0 is not correct! */

	/* marking all blocks as free */
	for (j=0;j<=dpb->DSM;j++) free_block(j);

	{
		/* mark allocated blocks from AL1, AL0 as used */
		/* AL0 and AL1 make a bitmask. AL0 is bits 15..8, AL1 is
		bits 7..0. If a bit is set, a block is allocated. Block indexs
		go from 0-15, bits 15..0 */
		unsigned long bit_mask;

		bit_mask = (dpb->AL0<<8) | dpb->AL1;

		for (i=0; i<16; i++)
		{
			/* is bit set? */
			if (bit_mask & 0x08000)
			{
				/* yes, alloc block */
				alloc_block(i);
			}
			/* shift for next bit */
			bit_mask = bit_mask<<1;

		}
	}

	/* mark blocks used by files as allocated */
	for (i=0;i<=dpb->DRM;i++)
	{
		for (j=0;j<BLKNR;j++)
		{
			if (directory[i].user!=0xE5)
			{
				/* KT - changed. will quit out at the first zero block index found */
				if (directory[i].blk[j])
				{
					alloc_block(directory[i].blk[j]);
				}
				else
					break;
			}
		}
	}
}


void put_directory() {
/*   ^^^^^^^^^^^^^ */
int	i, j, off;
uchar	*buf;
int	mask;
int	block;

	printm(10,"[wd] ");
	buf = block_buffer;	/* simply a shortcut */

	/* KT - added to prevent crash if directory is not allocated! */
    if (directory==NULL)
		return;

	if (directory_dirty==0)
		return;

	directory_dirty = 0;

	block = 0;
	for (i=0;i<=dpb->DRM;i++) {
		off=i*32 % dpb->BLS;

		buf[off] = directory[i].user;
		for (j=0;j<8;j++) buf[off+j+1] = directory[i].root[j];
		for (j=0;j<3;j++) buf[off+j+9] = directory[i].ext[j];

		buf[off+12] = directory[i].extent;
		buf[off+13] = directory[i].unused[0];
		buf[off+14] = directory[i].unused[1];
		buf[off+15] = directory[i].rec;

		mask=0x1;
		for (j=11;j>0;j--) {
			if (directory[i].attr & mask)
				buf[off+j] |= 0x80;
			mask <<= 1;
		};

		if (BLKNR_SIZE==1) {
			for (j=0;j<16;j++) {
				buf[off+16+j] = (uchar)directory[i].blk[j];
			}
		} else if (BLKNR_SIZE==2) {
			for (j=0;j<8;j++) {
				buf[off+16+2*j] = (uchar)(directory[i].blk[j] & 255);
				buf[off+17+2*j] = (uchar)(directory[i].blk[j]>>8);
			}
		}

/* if next entry is in the next block, then write the current block */
		if ((i+1)*32/(signed)dpb->BLS > block) {
			write_block(block,buf);
			block++;
		}
	}
}



/*******
  Image
 *******/

int	get_bit_count_from_mask(unsigned long mask)
{
	int bit_count;

	bit_count = 0;

	while ((mask & 1)!=0)
	{
		bit_count++;
		mask = mask>>1;
	}

	return bit_count;
}


/* at this point we should have a format which matches the disk image format */
/* TRKS in DPB is not good. In 22disk this defines the physical disk structure. */
void update_dpb(DPB_type *dpb, uchar *track) {
/*   ^^^^^^^^^^
Determine the extended DPB data out of <dpb> and the sample track <track>.
Complete the extension parts of <dpb>. <track> must be read in first!
*/
	int BLKNR_MAX;
	int sectors_per_dir;

	dpb->BLS  = 1 << (dpb->BSH + 7); /* or 2^BSH*128 */

	/* KT - already present in format description */
#if 0
/* an image must exist, do not call form <format>! */
	dpb->SEC1 = ((struct t_header*)track)->sector[0].sector;
	dpb->SECS = ((struct t_header*)track)->SPT;

	/* KT - Removed because they are already specified */
	/* the next two elements should already be set */
	dpb->TRKS = disk_header.nbof_tracks;
	dpb->HDS  = disk_header.nbof_heads;
#endif

	dpb->SYS  = (dpb->OFS>0) && (*(track+0x100)) != filler;
	dpb->DBL  = 32 * (dpb->DRM+1) / dpb->BLS; /* or often CKS/8 */

	/* KT - Removed because this is already specified */
#if 0
	/* DSM will be specified, so no need to calculate */

	dpb->DSM = (dpb->TRKS*dpb->HDS*dpb->SECS) / (dpb->BLS/dpb->BPS);
/* subtract reserved tracks */
	dpb->DSM -= dpb->OFS * dpb->SECS / (dpb->BLS/dpb->BPS);
	dpb->DSM--;
#endif

	/* KT - Added because it is no longer stored! */
	dpb->SPT = (dpb->SECS*dpb->BPS)/RECORDSIZE;

	sectors_per_dir = (((dpb->DRM+1)<<5)+(dpb->BPS-1))/dpb->BPS;
	sectors_per_dir = sectors_per_dir * dpb->BPS;

	dpb->CKS = sectors_per_dir/RECORDSIZE;


 	if (dpb->DSM>=256) {
/* 2 byte pointer and 8 pointer per entry max */
		BLKNR_SIZE = 2;
		BLKNR_MAX = 8;
	}
	else
	{
/* 1 byte pointer and 16 pointer per entry max */
		BLKNR_SIZE = 1;
		BLKNR_MAX = 16;
	}

	/* if the extent mask is a real mask, then we can calculate the number of 1 bits in it.
	From this we can calculate the number of extents */
	/* the function will only work if EXM is 1,3,7,15.... i.e. if bit 3 is set, bits 2,1 and 0 must be set */
	/* it will not handle isolated bits! e.g. EXM=8*/
	/* get number of extents from exm bit mask */
	dpb->num_extents = 1<<get_bit_count_from_mask(dpb->EXM);


	/* I hope this should correctly get the number of blocks per directory entry */
	/* a directory entry can only describe a maximum of 16384 bytes */
	{
		unsigned long max_size;

		/* calculate the max size based on the max number of blocks that
		can be stored in the directory entry. The max number of blocks
		is dependant on DSM. If DMS<=255, then 16 blocks can be stored,
		else 8 blocks can be stored */
		max_size = BLKNR_MAX * dpb->BLS;

		/* divide max size by EXM+1. If this is greater than 16384,
		calculate the number of blocks we can store */
		if ((max_size/dpb->num_extents)>EXTENTSIZE)
		{
			BLKNR = (EXTENTSIZE*dpb->num_extents)/dpb->BLS;
		}
		else
		{
			/* set to max allowed */
			BLKNR = BLKNR_MAX;
		}
	}
}

void close_image() {
/*   ^^^^^^^^^^^ */
	if (*disk_header.tag) {
		printm(10,"[ci] ");
		if (cur_trk > -1) write_track();
		put_directory();
	}
	free_image_data();
}

int sector_exists(uchar *track, ushort SEC1)
{
 	struct t_header *track_header = (struct t_header *)track;
 	int i;
	struct s_info *sector_info = &track_header->sector[0];
	int spt;

	spt = track_header->SPT;

	/* check if SEC1 exists in sectors available on track 0 */
	for (i=0; i<spt; i++)
	{
		if (sector_info->sector == SEC1)
		{
			return 1;
		}

		sector_info++;
	}

	return 0;
}

/* KT - added select format from those available */
int	select_format(uchar *track)
{
	struct t_header *track_header = (struct t_header *)track;
	int spt;
	DPB_list_entry *cur_entry;
	DPB_list_entry *dpb_entry_found = NULL;
	int found_count = 0;
	spt = track_header->SPT;

	/* go through list of supported formats */
	cur_entry = dpb_list.first;

	while (cur_entry!=NULL)
	{
		/* check if first sector exists in list of sectors in track */
		if (sector_exists(track, cur_entry->dpb.SEC_side1[0]))
		{
			/* sector exists */

			/* correct sectors per track? */
			if (spt == cur_entry->dpb.SECS)
			{
				/* correct number of sides? */
				if (disk_header.nbof_heads==cur_entry->dpb.HDS)
				{
					if (disk_header.nbof_tracks == cur_entry->dpb.TRKS)
					{
						// found a format that appears to match the format on the disc
						if (dpb_entry_found==NULL)
						{
							dpb_entry_found = cur_entry;
						}

						errorf(FALSE, "Found format \"%s\" matching disk image format", cur_entry->ident);

						found_count++;
					}
				}

			}

		}

		cur_entry = cur_entry->next;
	}

	/* no entry found */
	if (dpb_entry_found==NULL)
	{
		/* format not recognised */

		/* no format found */
		return -1;
	}

	if (found_count==1)
	{
		dpb = &dpb_entry_found->dpb;
		dpb_list_entry = dpb_entry_found;

		/* found it - update it */
		update_dpb(dpb,track);

		return 0;
	}

	/* too many formats to choose from */
	return -2;
}

DPB_list_entry *get_dpb_entry_from_format_name(const char *format_name)
{
	DPB_list_entry *cur_entry;

	/* go through list of supported formats */
	cur_entry = dpb_list.first;

	while (cur_entry!=NULL)
	{
		if (strcmp(cur_entry->ident, format_name)==0)
		{
			return cur_entry;
		}

		cur_entry = cur_entry->next;
	}

	return 0;
}


/* KT - added select format from those available */
void print_formats_available()
{
	DPB_list_entry *cur_entry;

	newpage("c");

	/* go through list of supported formats */
	cur_entry = dpb_list.first;

	while (cur_entry!=NULL)
	{
		if (cur_entry->description!=NULL)
		{
			printm(2, "%s (%s)",cur_entry->ident,cur_entry->description);
			putcharm(0,10);
			nextline();
		}

		cur_entry = cur_entry->next;
	}
}

const char *disc_format_not_recognised_string = "Disc format not recognised!";

void initialise(void)
{
	/* KT - initialise some data */

	cur_trk = -1;
	cur_hd = -1;
	cur_blk = -1;
	directory_dirty = 0;
	image_type = 0;
	*disk_header.tag = 0;
	track = NULL;
	blk_alloc = NULL;
	directory = NULL;
	block_buffer = NULL;
	imagefile = -1;
}

int open_image(char *name, int auto_detect) {
/*  ^^^^^^^^^^
alloc track buffer, blk_alloc, buffer, read directory */

int	n;
char	dirsep[2] = {DIRSEPARATOR, 0};
int result;
#ifdef LINUX
char *p;
#endif
/* open file */
	printm(10,"[oi] ");
	imagefile = open(name,O_RDWR|O_BINARY,0);
	imagename = name;	/* temporary, for <abandonimage> */
	if (imagefile < 0) {
		return errorf(TRUE,"Cannot open \"%s\"",name);
	}
	n = read(imagefile,&disk_header,0x100);
	if (n!=0x100) {
		errorf(FALSE,"Image corrupt! I cannot read image header "
							"(only %d bytes)!",n);
		abandonimage();
		return -1;
	}
	if (!tag_ok()) {
		errorf(FALSE,"\"%s\" is not a DSK image!",name);
		abandonimage();
		return -1;
	}
	if ((disk_header.nbof_heads<1) || (disk_header.nbof_tracks<1)) {
		errorf(FALSE,"--==>>> open_image: \"%s\"",name);
		abandonimage();
		return -1;
	}

	/* added function to malloc block which is size of the biggest track */
	malloc_track();

	/* check malloc succeded */
	if (track==NULL)
	{
		abandonimage();
		return -1;
	}



	if (!validate_image())
	{
		errorf(FALSE, "Image file is corrupted!");
		abandonimage();
		return -1;
	}



/* set up varaibles */
	filler = 0xE5;
	cur_user=0;

#if defined(WIN32) || defined(WIN64)
	/* potential to exceed full_imagename buffer size */
	_getcwd(full_imagename, FULL_IMAGENAME_LENGTH);

	/* if image name was got and it exceeded buffer, then it
	will be truncated, but there will be no ending NULL. Force it
	just in case */
	full_imagename[FULL_IMAGENAME_LENGTH-1] = '\0';
#else
	/* potential to exceed full_imagename buffer size */
	p = getcwd(full_imagename, sizeof(full_imagename));
	full_imagename[FULL_IMAGENAME_LENGTH-1] = '\0';
#endif

#if  !defined(WIN32) && !defined(WIN64)
	/* not required by WIN32 version, since _getcwd returns a pointer
	to the buffer passed if it succeeded. In this case returning
	full_buffer */
	if (p)
	{
		/* copy as much of the string as would fit in the buffer */
		strncpy(full_imagename,p,FULL_IMAGENAME_LENGTH);
		/* force NULL just in case there isn't one */
		full_imagename[FULL_IMAGENAME_LENGTH-1] = '\0';
	}
#endif

	if (full_imagename[strlen(full_imagename)-1]==DIRSEPARATOR)
		full_imagename[strlen(full_imagename)-1]=0;

	/* KT - added this to stop strcat overflowing buffer */
	{
		int BytesRemaining;

		BytesRemaining = FULL_IMAGENAME_LENGTH - strlen(full_imagename) - 1;

		if (BytesRemaining!=0)
		{
			strncat(full_imagename,dirsep, BytesRemaining);
			full_imagename[FULL_IMAGENAME_LENGTH-1] = '\0';
		}

		BytesRemaining = FULL_IMAGENAME_LENGTH - strlen(full_imagename) - 1;

		if (BytesRemaining!=0)
		{
			strncat(full_imagename,name, BytesRemaining);
			full_imagename[FULL_IMAGENAME_LENGTH-1] = '\0';
		}
	}

#if DOS
	lower(full_imagename);
#endif
	if((imagename=strrchr(full_imagename,DIRSEPARATOR)))
		imagename++;
	else	imagename=full_imagename;

	/* determine disk format */
	if (read_track(0,0))
	{
		abandonimage();
		return -1;
	}



	/* auto-detect image format */
	if (auto_detect)
	{

		result = select_format(track);

		if (result==-2)
		{
			errorf(FALSE, "Multiple formats found!");
			abandonimage();
			return -1;
		}

		if (result==-1)
		{
			errorf(FALSE, disc_format_not_recognised_string);
			abandonimage();
			return -1;
		}
	}
	else
	{
		/* found it - update it */
		update_dpb(dpb,track);
	}

	if (dpb==NULL)
	{
		errorf(FALSE, disc_format_not_recognised_string);
		abandonimage();
		return -1;
	}



/* calculate number of blocks and allocate memory */
	blk_alloc = (uchar *)Malloc(((dpb->DSM+1)+7)>>3);

	/* KT - added memory allocate check */
	if (blk_alloc==NULL)
	{
		abandonimage();

		return -1;
	}

	directory = (DirEntry*)Malloc(sizeof(DirEntry)*(dpb->DRM+1));

	/* KT - added memory allocate check */
	if (directory==NULL)
	{
		abandonimage();
		return -1;
	}

/* allocate block buffer */
	block_buffer = (uchar*)Malloc(dpb->BLS);

	if (block_buffer==NULL)
	{
		abandonimage();
		return -1;
	}


/* get directory information */
	get_directory();
	calc_allocation();

	return 0;
}


int comment_image(const char *text) {
/*  ^^^^^^^^^^^^^
Place <text> in the comment field of the image and save the image
48 bytes tag = 8 bytes required + 40 bytes free */

int     i;
	memset(disk_header.tag+8,0,40);
	i=0;
	while (i<40 && text[i]) {
		*(disk_header.tag+8+i) = text[i];
		i++;
	}

	lseek(imagefile,0L,SEEK_SET);
	if (write(imagefile,&disk_header,sizeof(disk_header)) < 0) {
		return errorf(TRUE,"--==>>> comment_image");
	}
	return 0;
}



/********
  Blocks
 ********/

int get_free_block() {
/*  ^^^^^^^^^^^^^^ */
static int next = 0;
int	i;

	if (next > dpb->DSM) next = 0;
/* try to allocate next block, if there was a previos one (next!=0) */
	if ((next != 0) && (is_free_block(next))) return next++;
/* try to find the first free block */
	for (i=dpb->DBL;i<=dpb->DSM;i++) {
		if (is_free_block(i)) return i;
	}
	return -1;
}


/*****************
  FS Maintenance
 *****************/


struct {
	int	flag;
	uchar	type;
	ushort	load;
	ushort	jump;
	ushort	size;
	ushort	checksum;

} amsdos_header;


void get_amshead(int ent) {
/*   ^^^^^^^^^^^
Read the first 128 bytes from the file and store them in the structure
<amsdos_header>. Set the <flag> to 2, if it's a valid header; to 1, if it's
invalid; and to 0 if the files is empty.
*/

int	i;
ushort	sum = 0;
uchar	*buf;

	if (directory[ent].blk[0] == 0) {
		amsdos_header.flag = 0;
		return;
	}
	buf = read_block((signed)directory[ent].blk[0]);

	if (buf==0)
	{
		amsdos_header.flag = 1;
		return;
	}
	amsdos_header.type	= buf[18];
	amsdos_header.load	= buf[21] + (buf[22]<<8);
	amsdos_header.jump	= buf[26] + (buf[27]<<8);
	amsdos_header.size	= buf[64] + (buf[65]<<8);
	amsdos_header.checksum	= buf[67] + (buf[68]<<8);

	for (i=0;i<=66;i++) {
		sum += buf[i];
	}
	if (sum==amsdos_header.checksum)
		amsdos_header.flag = 2;
	else
		amsdos_header.flag = 1;

	return;
}

/****** local function for dir() ******/

int cmp_array(int *x, int *y) {
/*  ^^^^^^^^^
Compares two filenames, whose entry numbers are <x> and <y> */
int	res;

	if (directory[*x].user < directory[*y].user)		res = -1;
	else if (directory[*x].user > directory[*y].user)	res = 1;
	else {
		res = strncmp((signed char*)directory[*x].root,
			      (signed char*)directory[*y].root, 8);
		if (res==0)
			res = strncmp((signed char*)directory[*x].ext,
				      (signed char*)directory[*y].ext, 3);
	}
	return res;
}


int dir(char *pat, int mask) {
/*  ^^^
<mask> is a "bit-or" of the following bits:
	DIR_DOUBLE	default 					--00
	DIR_WIDE	only names and sizes				--01
	DIR_AMSHEAD	incl. AMSDOS Header				--10
	DIR_LONG	incl. entries, records, all attributes		--11

	DIR_SORT	sorted						-1--
*/


long	total_bytes = 0;
int	used_entries = 0;
int	i,j;
int	ent;
int	files;
int	mode;
int	*array; 		/* temporary dynamic array */
char	upbuffer[INPUTLEN];	/* buffer for upper case conversion*/
uchar	*buf;
int	user;
char	root[INPUTLEN];
char	ext[INPUTLEN];
upbuffer[0] = '\0';
root[0] = '\0';
ext[0] = '\0';
	array = (int*)Malloc(sizeof(int)*max(256,dpb->DRM+1));

	/* KT - added check for memory allocation fail */
	if (array==NULL)
	{
		return 0;
	}

	parse_cpm_filename(pat,&user,root,ext);
	if (user==-1) user = cur_user;

/* calculate active users, entries */
	files = 0;
	for (i=0;i<256;i++) array[i]=0;
	for (i=0;i<=dpb->DRM;i++) {
		if (directory[i].user != 0xE5) used_entries++;
		if (directory[i].first) {
			array[directory[i].user]++;
			files++;
		}
	}

	newpage("c");

/*** header ***/

	/* KT - changed strcpy because it could overflow buffer */
	strncpy(upbuffer, imagename, INPUTLEN);
	upbuffer[INPUTLEN-1]='\0';

	upper(upbuffer);
	printm(0,"Directory of Image: %s , User ", upbuffer);
	if (user==-2)	printm(0,"ALL\n\n");
	else		printm(0,"%-d\n\n",user);
	nextline(); nextline();

/*** used users ***/

	/* KT - modified because the display looked bad! */
	if (files>0) {
		printm(0,"Used Users:\n ");
		for (i=0;i<256;i++) {
			if (array[i]!=0) {
				printm(0,"%d with %d file%s\n",i,array[i],
							plural(array[i]));
			}
		}
		printm(0,"\n\n");	nextline(); nextline();
	}
/*** files ***/

/* fetch all needed files */
	for (i=0;i<=dpb->DRM;i++) array[i]=0xFFF; /* large but not negative */

	ent=glob_cpm_file(pat);
	files=0;
	while (ent>=0) {
		array[files] = ent;
		total_bytes += directory[ent].size;
		ent=glob_cpm_next();
		files++;

	}

	if (mask & DIR_SORT) qsort(array,files,sizeof(int),
		(int(*)(const void*,const void*))cmp_array);

/* <array> contains now the entry numbers of the requested files in
the requested order */


/* output of the filenames */

	if (files==0) {
		printm(0,"No files\n");
		goto footer;
	}

	i=0;
	switch (mask&0x3) {
	case DIR_DOUBLE:
		printm(0," U Name            Size  Attr    Ent   "
		       "%c U Name            Size  Attr    Ent\n",vert);
		nextline();
		printm(0,"%s%c",repstr(hori,39),cross);
		printm(0,"%s\n",repstr(hori,37));
		nextline();
		while (array[i]<0xFFF) {
			j=array[i]; ent=1;
			while(directory[j].next>-1) {
				ent++;
				j=directory[j].next;
			}

			printm(0," %u %-12s  %6lu  %3s %c  %3d   ",
				directory[array[i]].user,
				directory[array[i]].name,
				directory[array[i]].size,
				show_attr(directory[array[i]].attr,ATTR_R,FALSE),
				( (directory[array[i]].attr&(~ATTR_R))? '+' : ' '),
				ent);
			if (i%2==0)	printm(0," %c",vert);
			else		{putcharm(0,10); nextline();}
			i++;
		};
		break;
	case DIR_WIDE:
		printm(0,"Name          Size %c"
			 "Name          Size %c"
			 "Name          Size %c"
			 "Name          Size\n",vert,vert,vert);
		nextline();
		printm(0,"%s%c",repstr(hori,19),cross);
		printm(0,"%s%c",repstr(hori,19),cross);
		printm(0,"%s%c",repstr(hori,19),cross);
		printm(0,"%s\n",repstr(hori,19));
		nextline();
		while (array[i]<0xFFF) {
			printm(0,"%-12s%6lu ",
				directory[array[i]].name,
				directory[array[i]].size);
			if (i%4!=3)	printm(0,"%c",vert);
			else		{putcharm(0,10); nextline();}
			i++;
		};
		break;
	case DIR_AMSHEAD:
		printm(0," U Name           Size Attr     Amsdos-Header\n");
		nextline();
		printm(0,"%s\n",repstr(hori,74));
		nextline();
		while (array[i]<0xFFF) {
			printm(0,"%2u %-12s %6lu %3s %c    ",
				directory[array[i]].user,
				directory[array[i]].name,
				directory[array[i]].size,
				show_attr(directory[array[i]].attr,ATTR_R,FALSE),
				( (directory[array[i]].attr&(~ATTR_R))? '+' : ' '));

			get_amshead(array[i]);
			if (amsdos_header.flag==0) {
				printm(0,"Empty");
			} else if (amsdos_header.flag==1) {
				if (strncmp((signed char*)
				  directory[array[i]].ext,"COM",3)==0)
					printm(0,"CP/M Program");
				else
					printm(0,"---");
			} else {
				if (amsdos_header.type & 0x1)
					printm(0,"protected ");
				switch ((amsdos_header.type>>1) & 0x7) {
				case 0: printm(0,"BASIC "); break;
				case 1: printm(0,"Binary"); break;
				case 2: printm(0,"Screen"); break;
				case 3: printm(0,"ASCII "); break;
				default:printm(0,"Type=&%2X",
							amsdos_header.type);
				}

				printm(0,"  Load=&%-4X, Jump=&%-4X, Size=&%-4X",
					amsdos_header.load,
					amsdos_header.jump,
					amsdos_header.size);
			}

			putcharm(0,10); 	nextline();

			i++;
		};
		break;
	case DIR_LONG:
		printm(0,"User  Name            Size  Attr    Ext. Attr. "
			 "Detect Entries Records Blocks\n");
		nextline();
		printm(0,"%s\n",repstr(hori,77));
		nextline();
		while (array[i]<0xFFF) {
			/* KT - changed strcpy so it doesn't overrun end of buffer */
			strncpy(upbuffer,
				(signed char*)directory[array[i]].name, INPUTLEN);
			upbuffer[INPUTLEN-1]='\0';

			j=array[i]; ent=1;
/* detect mode */
			if (directory[array[i]].blk[0]==0)
				mode = -1;
			else {
/* the only other function using <block_buffer> here is <get_amshead>,
but they do not interfere! */
				buf = read_block(directory[array[i]].blk[0]);

				if (buf==0)
				{
					break;
				}

				mode = detectmode(buf,dpb->BLS);
			}
/* count entries */
			while(directory[j].next>-1) {
				ent++;
				j=directory[j].next;
			}

			printm(0,"%2u    %-12s  %6lu  %s %-6s%5u%8lu%7lu",
				directory[array[i]].user,
				upbuffer,
				directory[array[i]].size,
				show_all_attr(directory[array[i]].attr,TRUE),
				mode==-1? "Empty"
					: (mode==M_TEXT? "Text" : "Bin"),
				ent,
				(directory[array[i]].size+RECORDSIZE-1)/RECORDSIZE,
				(directory[array[i]].size+(dpb->BLS-1))/dpb->BLS);
			putcharm(0,10); 	nextline();
			i++;
		};
		break;
	} /* switch */
	printm(0,"\n"); 	nextline();


/*** footer ***/
footer:

	printm(0,"\n%d file%s in %lu Bytes\n",
		files, plural(files),
		total_bytes);
	nextline(); nextline();
	printm(0,"(%lu Bytes free, %lu Bytes allocated, %d entr%s of %d)\n",
		(long)free_blks*dpb->BLS,
		(long)allocated_blks*dpb->BLS,
		used_entries, plural_y(used_entries),
		dpb->DRM+1);
	nextline();

	free(array);
	return 0;
}


static void nuke_dir_entry(DirEntry *dir_entry)
{
	/* nuke user */
	dir_entry->user = 0x0E5;
	/* nuke name */
	memset(&dir_entry->root,0x0e5, 8);
	/* nuke extension */
	memset(&dir_entry->ext,0x0e5, 3);
	/* nuke rec */
	dir_entry->rec = 0x0e5;
	/* clear attr's */
	dir_entry->attr = 0;
	/* nuke extent */
	dir_entry->extent = 0x0e5;
	/* nuke blk's */
	memset(&dir_entry->blk,0x0e5, sizeof(int)*16);
	/* nuke unused */
	memset(&dir_entry->unused, 0x0e5, 2);
}

/* this command will clean a disk image. It will fill all blocks (not used
by valid files in the directory) with 0x0e5, and nuke all directory entries
for deleted files. USE WITH CARE!

It works by going through the directory, entry by entry, and if the user number
is not 0x0e5, it marks the blocks as used in a big array.

  It then goes through the big array, and if the block is not used, it will nuke it.
  Finally, it goes through the directory, and if any entry has 0x0e5 for the user
  (i.e. it is deleted or unused), it will nuke it!
*/

void clean(void)
{
	int i;
	unsigned short allocated_blocks;
	unsigned short *block_allocation;

	printm(1,"Nuke all deleted files ? ");
	if (!confirmed()) return;

	allocated_blocks = ((dpb->AL0<<8) | dpb->AL1);

	block_allocation = (ushort *)Malloc(sizeof(ushort)*(dpb->DSM+1));

	if (block_allocation==NULL)
		return;

	memset(block_allocation, 0, sizeof(ushort)*(dpb->DSM+1));


	/* fill block allocation array. If !=0, this block is allocated to a valid file,
	or is allocated with AL0/AL1 */
	for (i=0; i<16; i++)
	{
		if ((allocated_blocks & (1<<(15-i)))!=0)
		{
			block_allocation[i] = 1;
		}
	}

	/* mark all blocks used by valid files */
	for (i=0;i<=dpb->DRM;i++)
	{
		if (directory[i].user!=0x0e5)
		{
			int j;

			for (j=0;j<BLKNR;j++)
			{
				if (directory[i].blk[j])
				{
					block_allocation[directory[i].blk[j]] = 1;
				}
			}
		}
	}

	/* go through all blocks */
	for (i=0; i<dpb->DSM+1; i++)
	{
		/* is it allocated to a valid file? */
		if (block_allocation[i]==0)
		{
			/* no. NUKE IT! - note: block is already free! */
			nuke_block(i);
		}
	}

	/* find all deleted directory entries... */
	for (i=0; i<=dpb->DRM; i++)
	{
		if (directory[i].user==0x0e5)
		{
			/* nuke it */
			nuke_dir_entry(&directory[i]);
		}
	}

	free(block_allocation);
	update_directory(1);
	calc_allocation();
}


/* erase file, no undelete possible! */
long nuke(bool silent, char *pat) {
/*   ^^^^^^
Delete all files that match <pat>.
Answer the amount of deleted bytes (at least 0) */
long	freed = 0;
long	total_freed = 0;
int	ent, i;


/* warn, if <pat> contains *.* or is only usernumber */
	if (match("*\\*.\\**",pat) || match("*:",pat)) {
		if (!silent && Verb > 0) {
			printm(1,"Nuke all in \"%s\"? ",pat);
			if (!confirmed()) return 0;
		}
	}

	ent = glob_cpm_file(pat);
	if (ent<0) {
		if (!silent) errorf(FALSE,"\"%s\" not found",pat);
		return 0;
	}


	while (ent>=0) {
		freed = 0;
		if (directory[ent].attr & ATTR_R) {
			if (!silent && Verb > 0) {
				printm(1,"\"%u:%s\" readonly. Nuke? ",
				    directory[ent].user,directory[ent].name);
				if (!confirmed()) {
					ent = glob_cpm_next();
					continue;
				}
			}
		}

		if (!silent) printm(3,"Nukeing \"%u:%s\": ",
				directory[ent].user,directory[ent].name);
		freed += directory[ent].size;
		while (ent>=0)
		{
			int next;

			next = directory[ent].next;

			for (i=0;i<BLKNR;i++) {
				if (directory[ent].blk[i]==0) break;
				/* free it */
				free_block(directory[ent].blk[i]);
				/* nuke it */
				nuke_block(directory[ent].blk[i]);
			}

			nuke_dir_entry(&directory[ent]);
			ent = next;
		};

		if (!silent) printm(3,"%ld Bytes\n",freed);
		total_freed += freed;
		ent = glob_cpm_next();
	}

	update_directory(1);
	calc_allocation();
	return total_freed;
}


long delete(bool silent, char *pat) {
/*   ^^^^^^
Delete all files that match <pat>.
Answer the amount of deleted bytes (at least 0) */
long	freed = 0;
long	total_freed = 0;
int	ent, i;


/* warn, if <pat> contains *.* or is only usernumber */
	if (match("*\\*.\\**",pat) || match("*:",pat)) {
		if (!silent && Verb > 0) {
			printm(1,"Delete all in \"%s\"? ",pat);
			if (!confirmed()) return 0;
		}
	}

	ent = glob_cpm_file(pat);
	if (ent<0) {
		if (!silent) errorf(FALSE,"\"%s\" not found",pat);
		return 0;
	}


	while (ent>=0) {
		freed = 0;
		if (directory[ent].attr & ATTR_R) {
			if (!silent && Verb > 0) {
				printm(1,"\"%u:%s\" readonly. Delete? ",
				    directory[ent].user,directory[ent].name);
				if (!confirmed()) {
					ent = glob_cpm_next();
					continue;
				}
			}
		}

		if (!silent) printm(3,"Deleting \"%u:%s\": ",
				directory[ent].user,directory[ent].name);
		freed += directory[ent].size;
		while (ent>=0) {
			directory[ent].user = 0xE5;
			for (i=0;i<BLKNR;i++) {
				if (directory[ent].blk[i]==0) break;
				free_block(directory[ent].blk[i]);
			}

			ent = directory[ent].next;
		};

		if (!silent) printm(3,"%ld Bytes\n",freed);
		total_freed += freed;
		ent = glob_cpm_next();
	}

	update_directory(1);
	calc_allocation();
	return total_freed;
}


int change_attrib(char* pattern, int set, int reset) {
/*  ^^^^^^^^^^^^^
Change all attributes, masked by <mask> to <set> in all files matching
<pattern>. Answer -1 on error.
*/
int	ent, ent0;

	ent = glob_cpm_file(pattern);
	if (ent<0)
		return errorf(FALSE,"\"%s\" not found",pattern);
	while (ent>=0) {
		printm(3,"Changing \"%s\" from \"%s\"",
			directory[ent].name,
			show_all_attr(directory[ent].attr,TRUE));
		ent0=ent;
		do {
			directory[ent].attr |= set;
			directory[ent].attr &= ~reset;
			ent = directory[ent].next;
		} while (ent>=0);
		printm(3," to \"%s\"\n",
			show_all_attr(directory[ent0].attr,TRUE));
		ent=glob_cpm_next();
	}


	return 0;
}

#if 0
int sysgen(char *filename) {
/*  ^^^^^^
Copies the CP/M system from <filename> to the first two tracks of the image.
Does not test on Data-Format or sufficient space!
Answer -1 on error
*/
char	buf[INPUTLEN];
int	cpm;
int	t;
short	tracksize = dpb->BPS*dpb->SECS; /* = Disk_header.tracksize - 0x100 */
long	n;

	/* KT - changed strcpy so it doesn't overwrite buffer */
	strncpy(buf,filename, INPUTLEN);
	buf[INPUTLEN-1] = '\0';

	if (strchr(buf,'.')==NULL) strcat(buf,".cpm");

	cpm = open(buf,O_RDONLY|O_BINARY,0);
	if (cpm < 0) {
		return errorf(TRUE,"Cannot open \"%s\" for reading",buf);
	}

	for (t=0;t<dpb->OFS;t++) {
/* skip AMSDOS header and position on track <t> */
		n = lseek(cpm,(long)128+t*tracksize,SEEK_SET);
		if (n == -1L) {
			close(cpm);
			return errorf(TRUE,"CP/M Image currupt! "
				"I cannot position on track %d",t);
		}
		read_track(t%dpb->HDS,t/dpb->HDS);
		n = read(cpm,track+0x100,tracksize);
		if (n != tracksize) {
			close(cpm);
			return errorf(TRUE,"CP/M Image currupt! "
			    "I can only read %d bytes (instead of %d bytes) ",
			    n, tracksize);
		}
		write_track();
	}
	printm(2,"CP/M \"%s\" copied to CPC filesystem\n",buf);

	close(cpm);
	dpb->SYS = TRUE;

	return 0;
}
#endif

void LoadLabelFile(char *LabelFilename, unsigned char **ppData, unsigned long *pLength)
{
	FILE *fh;
	char *LabelData = NULL;

	*ppData = NULL;
	*pLength = 0;

	if (strlen(LabelFilename)==0)
		return;
	
	fh = fopen(LabelFilename,"rb");

	if (fh!=NULL)
	{
		char *LoadedLabelData;
		unsigned long Length;

		/* seek to end of file */
		fseek(fh, 0, SEEK_END);

		/* report current position/length of file */
		Length = ftell(fh);

		/* seek back to start of file */
		fseek(fh, 0, SEEK_SET);

		/* allocate memory to store file data */
		LoadedLabelData = malloc(Length);

		if (LoadedLabelData!=NULL)
		{
			/* read label data */
			if (fread(LoadedLabelData,1, Length,fh)==Length)
			{
				int offset = 0;

				/* check for AMSDOS header only if length is greater than 128 bytes */
				if (Length>128)
				{
					int i;
					ushort	sum = 0;
					ushort checksum;

					/* get checksum byte */
					checksum = ((LoadedLabelData[67] & 0x0ff) | ((LoadedLabelData[68] & 0x0ff)<<8));

					for (i=0;i<=66;i++) {
						sum += LoadedLabelData[i];
					}

					if (sum==checksum)
					{
						/* set offset to copy from */
						offset = 128;
					}
				}

				/* copy data into label data buffer */
				LabelData = malloc(Length-offset);

				if (LabelData!=NULL)
				{
					memcpy(LabelData, LoadedLabelData+offset, Length-offset);

					*ppData = LabelData;
					*pLength = Length-offset;
				}

			}

			free(LoadedLabelData);
		}

		fclose(fh);
	}
	else
	{
		errorf(TRUE, "Label file \"%s\" not found!", LabelFilename);
	}
}

int	write_label(DPB_type *dpb,char *label_data, unsigned long label_data_length)
{
	int hd,trk,sec;
	unsigned long LengthRemaining;
	unsigned long Offset;
	int r,h;
	int result = 0;

	LengthRemaining = label_data_length;
	Offset = 0;

	/* not sure how 22disk labels a disc when the defintion is defined
	to access side 2 of a disc */
	/* does it use TRKS and OFS to calculate number of tracks used by system */
	/* e.g. if OFS is 42 and TRKS is 40, then system tracks could be 2.
	does it write at track 0 on side 2 or track 0 side 0? */
	trk = 0;
	sec = 0;
	hd = 0;

	while (LengthRemaining!=0)
	{
		unsigned char *sector_ptr;

		/* read the track */
		result = read_track(hd, trk);
		if (result != 0)
			return result;
		
		r = sec;

		if (hd==0)
		{
			h = dpb->side0_hd;
			r = dpb->SEC_side1[r];
			//r+=dpb->SEC1_side1;
		}
		else
		{
			h = dpb->side1_hd;
			r = dpb->SEC_side2[r];
			//r+=dpb->SEC1_side2;
		}

		sector_ptr = get_sector_data_ptr(r,h);

		if (sector_ptr!=NULL)
		{
			if (LengthRemaining<dpb->BPS)
			{
				memcpy(sector_ptr, label_data + Offset, LengthRemaining);
				memset(sector_ptr+LengthRemaining, label_data[Offset + LengthRemaining-1], dpb->BPS-LengthRemaining);
				LengthRemaining = 0;
			}
			else
			{
				memcpy(sector_ptr, label_data + Offset, dpb->BPS);
				Offset+=dpb->BPS;
				LengthRemaining -= dpb->BPS;
			}
			track_dirty = 1;
		}

		if (next_sector(&hd, &trk, &sec)) write_track();
	}
	return write_track();
}


int format(char* name, DPB_list_entry *entry, int extended) {
/*  ^^^^^^
Creates a new image with name <name> and format <dpb>. <dpb> must be out of
the <DPB_store>, not processed by <update_dpb>!
Answers -1 on error.
*/
int	format_file;
int	i,h,j;
struct t_header *trhd;
time_t	now;
DPB_type *dpb= &entry->dpb;
int track_size;
int sec_base;

#if defined(WIN32) || defined(WIN64)
//	format_file= open(name,O_RDWR|O_BINARY,0);

format_file = creat(name, _S_IREAD | _S_IWRITE);
#else
	format_file = creat(name,0644);
#endif


	if (format_file<0) {
		return errorf(TRUE,"Cannot open \"%s\" for writing",name);
	}


/* fill disk_header */
	printm(3,"Formatting (%s) ",show_format(entry));
	for (j=0;j<0x2F;j++) disk_header.tag[j] = 0;

	if (extended)
	{
		strcpy ((signed char*)disk_header.tag,"EXTENDED    / ");
	}
	else
	{
		strcpy ((signed char*)disk_header.tag,"MV - CPCEMU / ");
	}
	memset((disk_header.tag)+14,' ',20);
	now = time(NULL);
	strftime(((signed char*)disk_header.tag)+14,20,"%d %b %y %H:%M",
							localtime(&now));
	disk_header.nbof_tracks	= (unsigned char)dpb->TRKS;
	disk_header.nbof_heads	= (unsigned char)dpb->HDS;

	track_size = 0x0100 + (dpb->BPS * dpb->SECS);

	if (extended)
	{
		disk_header.tracksize = 0;
	}
	else
	{
		disk_header.tracksize	= track_size;
	}
	memset(disk_header.unused,0,0xCC);

	if (extended)
	{
		unsigned char TrackSizeByte = track_size>>8;

		/* extended disk images have a size for each track. This is stored /256 */
		memset(disk_header.unused, TrackSizeByte, dpb->TRKS*dpb->HDS);
	}

#if defined(WIN32) || defined(WIN64)
	if (_write(format_file,&disk_header,0x100) < 0) {
#else
	if (write(format_file,&disk_header,0x100) < 0) {
#endif
		return errorf(TRUE,"FORMAT");
	}

	track = Malloc(track_size);

	trhd = (struct t_header*)track;
	for (i=0;i<disk_header.nbof_tracks;i++)
	    for (h=0;h<disk_header.nbof_heads;h++)
		{
		int sec_index;

		if (Break_Wish) {
			close(format_file);
			abandonimage();
			do_break();
		}
/* fill track_header */
		putcharm(3,'.'); fflush(stdout);
		strncpy((signed char*)trhd->tag,"Track-Info\r\n",0x10);
		trhd->track	= i;
		trhd->head	= h;
		trhd->unused[0] = 0;
		trhd->unused[1] = 0;
		trhd->BPS	= dpb->BPS/0x100;
		trhd->SPT	= (unsigned char)dpb->SECS;
		trhd->GAP3	= 0x4E;
		trhd->filler	= 0xE5;

		sec_index = 0;
		sec_base = 0;

		for (j=0;j<dpb->SECS;j++) {
			trhd->sector[j].track	= i;
			if (h==0)
			{
				trhd->sector[j].head	= dpb->side0_hd;
				trhd->sector[j].sector	= dpb->SEC_side1[sec_index];
			}
			else
			{
				trhd->sector[j].head	= dpb->side1_hd;
				trhd->sector[j].sector	= dpb->SEC_side2[sec_index]; //dpb->SEC1_side2 + sec_index;
			}
			trhd->sector[j].BPS	= dpb->BPS/0x100;
			trhd->sector[j].status1 = 0;
			trhd->sector[j].status2 = 0;
			trhd->sector[j].unused[0]=0;
			trhd->sector[j].unused[1]=0;

			/* update index */
			sec_index += dpb->skew;

			/* overflow? */
			if (sec_index>=dpb->SECS)
			{
				/* increment base */
				sec_base++;
				/* restart count at base value */
				sec_index = sec_base;
			}
		}
		for (j=dpb->SECS; j<29; j++)  {
			memset(&(trhd->sector[j]),0,8);
		}

		memset(track+0x100,trhd->filler,track_size-0x100);
#if defined(WIN32) || defined(WIN64)
		if (_write(format_file,track,track_size) < 0) {
#else
		if (write(format_file,track,track_size) < 0) {

#endif
			return errorf(TRUE,"FORMAT");
		}
	} /* end for i,h */
	printm(3,"   done\n");
	free(track);
	track = NULL;
#if defined(WIN32) || defined(WIN64)
	_commit(format_file);
	_close(format_file);
#else
	close(format_file);
#endif
	*disk_header.tag = 0;
	cur_trk = -1;
	cur_hd = -1;
	directory_dirty =0;

	return 0;
}


int ren_file(char *from, char *to) {
/*  ^^^^^^^^
Renames a file.
Wildcards are allowed and complete filenames must be given.
See ren_wild().
If a file is renamed to itself, do nothing.
The directory must be updated and written afterwards!
Answer -1 on error.
*/
int	to_user;
char	to_root[INPUTLEN];
char	to_ext[INPUTLEN];
char	to_full[INPUTLEN];
int	from_user;
char	from_root[INPUTLEN];
char	from_ext[INPUTLEN];
char	from_full[INPUTLEN];
int	ent;
const char wild_fmt[] = "\"%s\" may not contain wildcards";
to_root[0] = '\0';
to_ext[0] = '\0';
to_full[0] = '\0';
from_root[0] = '\0';
from_ext[0] = '\0';
from_full[0] = '\0';

	upper(to);
	upper(from);
	if (has_wildcards('c',from)) {
		return errorf(FALSE,wild_fmt,from);
	}
	if (has_wildcards('c',to)) {
		return errorf(FALSE,wild_fmt,to);
	}

	parse_cpm_filename(from,&from_user,from_root,from_ext);
	if (from_user==-1) from_user = cur_user;
	if (from_user==-2) return errorf(FALSE,"--==>>> ren_file: wild user");
	if (*from_root==0)
		return errorf(FALSE,"No name in \"%s\"",from);
	build_cpm_name((signed char*)from_full, from_user,
		(signed char*)from_root, (signed char*)from_ext);

	parse_cpm_filename(to,&to_user,to_root,to_ext);
	if (to_user==-1) to_user = cur_user;
	if (*to_root==0) {
		strcpy(to_root,from_root);
		strcpy(to_ext,from_ext);
	}
	build_cpm_name((signed char*)to_full, to_user,
		(signed char*)to_root, (signed char*)to_ext);

/* test on identity of <to> and <from> */
	if (strcmp(to_full,from_full)==0) {
		printm(2,"Renaming \"%s\" to itself\n",from_full);
		return 0;
	}

/* check if already exists */
	if (glob_cpm_file(to_full)>=0) {
		if (Verb > 0) {
			printm(1,"\"%s\" already exists! Overwrite? ",to_full);
			if (confirmed())	delete(TRUE,to_full);
			else			return 0;
		} else return errorf(FALSE,"\"%s\" already exists",to_full);
	}

       	ent = glob_cpm_file(from_full);
	if (ent<0) return errorf(FALSE,"\"%s\" not found",from_full);

	printm(2,"Renaming \"%u:%s\" to \"%s\"\n",
		directory[ent].user, directory[ent].name, to_full);

	do {
		directory[ent].user = to_user;
		str2mem((signed char*)directory[ent].root,
			(signed char*)to_root, 8);
		str2mem((signed char*)directory[ent].ext,
			(signed char*)to_ext, 3);
		ent = directory[ent].next;
	} while (ent>=0);

	return 0;
}


int ren_wild(char *pat, int us) {
/*  ^^^^^^^^
Renames all files that match <pat> to <user>.
The directory must be updated and written afterwards!
Answer -1 on error. */
int	ent;
char	src[20], trg[20];

	ent=glob_cpm_file(pat);
	if (ent<0) {
		errorf(FALSE,"\"%s\" not found",pat);
		return -1;
	}
	while (ent>=0) {
		sprintf(src,"%u:%s",directory[ent].user,directory[ent].name);
		sprintf(trg,"%u:%s",us,directory[ent].name);
		glob_env++;
		ren_file(src,trg);
		glob_env--;
		ent=glob_cpm_next();
	}
	return 0;
}


int copy_file(char *from, char *to) {
/*  ^^^^^^^^^
Copies a file to a new file with another name.
This preliminary version goes the way through put() and get() with a
temporary file.
Answer -1 on error. */

char	tempname[INPUTLEN];
int	err;
tempname[0] = '\0';
	tmp_nam(tempname);

	printm(3,"Copying \"%s\" to ",from);
	err=get(from,tempname);
	if (err==-1) {
		unlink(tempname);
		return -1;
	}
	err=put(tempname,to);
	if (err < 0) {
		unlink(tempname);
		return -1;
	}
	printm(3,"\"%s\"\n",to);

	unlink(tempname);
	return 0;
}


int copy_wild(char *pat, int us) {
/*  ^^^^^^^^^
Copies all files that match <pat> to <user>.
Answer -1 on error. */
int	ent;
char	src[20], trg[20];

	ent=glob_cpm_file(pat);
	if (ent<0) {
		errorf(FALSE,"\"%s\" not found",pat);
		return -1;
	}
	while (ent>=0) {
		int result;
		
		sprintf(src,"%u:%s",directory[ent].user,directory[ent].name);
		sprintf(trg,"%u:%s",us,directory[ent].name);
		glob_env++;
		result = copy_file(src,trg);
		if (result!=0)
			return result;
		
		glob_env--;
		ent=glob_cpm_next();
	}
	return 0;
}



/*********
  Dumping
 *********/


int dumpdir (FILE* file) {
/*  ^^^^^^^ */
int	i,j;
char	n[INPUTLEN],e[INPUTLEN];
n[0] = '\0';
e[0] = '\0';
	fprintf(file," #  U NAME         EX RE ATR BLOCKS\t\t\t\t\t    NEX\n");
	for (i=0;i<=dpb->DRM;i++) {
		strncpy(n,(signed char*)directory[i].root,8); 	n[8] = 0;
		strncpy(e,(signed char*)directory[i].ext,3);	e[3] = 0;
		fprintf(file,"%2X%c%2X %s.%s %2X %2X ",
			i, (directory[i].first?'>':' '),
			directory[i].user,
			n, e,
			directory[i].extent, directory[i].rec);
		fprintf(file,"%2X%1X",(directory[i].attr&~0x7)>>3,
			directory[i].attr&0x7);
		for (j=0;j<BLKNR;j++) {
			if (directory[i].blk[j])
				fprintf(file," %2X",directory[i].blk[j]);
			else
				fprintf(file," --");
		}

		if (directory[i].next>=0)
			fprintf(file,">%2X",directory[i].next);
		else
			fprintf(file,"<<<");
		putc(10,file);
		if (fflush(file)!=0)
		{
			return errorf(TRUE,"DUMP -D");
		}
	}


	return 0;
}


int dump(FILE *file, int block, int h, int t, int s) {
/*  ^^^^
Dump the contents of block <block> to <file> or if <block> = -1, the contents
of <h>,<t>,<s> to <file>.
*/

int	hd, trk, sec;
int	i, j;
int	secs, k;
uchar	*p, *q;

	if (block == -1) {
		hd = h; trk = t; sec = s;
		secs=1;
		block = blk_calc(hd,trk,sec);
	} else {
/*		hd  = hd_calc(block);
		trk = trk_calc(block);
		sec = sec_calc(block); */
		calc_t_s_h(block, &trk, &sec, &hd);
		secs=dpb->BLS/dpb->BPS;
	}


	for (k=0;k<secs;k++) {
		int sector;

		read_track(hd,trk);

		fprintf(file,
			"\nBlock %d/Part %d   Head %d Track %d Sector %d\n\n",
			block,k,  hd,trk,sec);
		i = 0;

		if (hd==0)
		{
			sector = dpb->SEC_side1[sec]; //+dpb->SEC1_side1;
		}
		else
		{
			sector = dpb->SEC_side2[sec]; //sec+dpb->SEC1_side2;
		}

		p = (uchar *)get_sector_data_ptr(sector,hd);
		while (i<dpb->BPS) {
			fprintf(file,"%3X %c ",i,vert);
			q=p;
			for (j=0;j<16;j++) fprintf(file,"%2X ",*p++);
			fprintf(file," %c ",vert);
			p=q;
			for (j=0;j<16;j++) {
				if (*p<32)
					putc(' ',file);
				else if (*p>=127)
					putc('~',file);
				else
					putc(*p,file);
				p++;
			}
			i+=16;
			putc(10,file);
			if (fflush(file)!=0)
				return errorf(TRUE,"DUMP");
		}

		next_sector(&hd,&trk,&sec);
	};
	return 0;
}


int map(FILE *file) {
/* Writes the disk allocation map on <file>. */
int	h, t, s, b;
int i;
char	*str;
unsigned short allocated_blocks;
unsigned short *block_allocation;

	allocated_blocks = ((dpb->AL0<<8) | dpb->AL1);


	block_allocation = (ushort *)Malloc(sizeof(ushort)*(dpb->DSM+1));

	if (block_allocation==NULL)
		return errorf(TRUE, "DUMP -D");

	memset(block_allocation, 0, sizeof(ushort)*(dpb->DSM+1));


	/* fill block allocation array with details of which file
	is allocated to which block */
	for (i=0;i<=dpb->DRM;i++)
	{
		if (directory[i].user!=0x0e5)
		{
			int j;

			for (j=0;j<BLKNR;j++)
			{
				if (directory[i].blk[j])
				{
					block_allocation[directory[i].blk[j]] = i;
				}
			}
		}
	}

	str = repstr(' ',max(0,(dpb->SECS*5+2)-9));
						/* 9 = len("%c Head %-2d") */

	fprintf(file,"      ");
	for (h=0;h<dpb->HDS;h++) {
		fprintf(file,"%c Head %-2d%s",vert,h,str);
	}
	fprintf(file,"\n");

	fprintf(file,"Track ");
	fprintf(file,"%c ",vert);

	for (s=0;s<dpb->SECS;s++) {
		fprintf(file,"%-4d ",s);
	}
	fprintf(file,"\n");
	fprintf(file,"& Head");
	fprintf(file,"%c ",vert);
	fprintf(file,"\n");

	str=repstr(hori,6); fprintf(file,"%s",str);

	str = repstr(hori,dpb->SECS*5+1);
	fprintf(file,"%c%s",cross,str);
	fprintf(file,"\n");

	for (t=0;t<dpb->TRKS;t++) {
		for (h=0;h<dpb->HDS;h++) {
			fprintf(file,"%-4d %1x ",t,h);
			for (s=0;s<dpb->SECS;s++) {
				b = blk_calc(h,t,s);

				if (b>=(dpb->DSM+1))
				{
					fprintf(file,"-NA- ");
				}
				else
				{

					if (h+dpb->HDS*t < dpb->OFS) {
						if (dpb->SYS) {
							fprintf(file,"$$$$ ");
						} else {
							fprintf(file,"---- ");
						}
						continue;
					}

					if (
						(b<16) &&
						((allocated_blocks & (1<<(15-b)))!=0)
						)
					{
						fprintf(file,"DDDD ");
						continue;
					}


					if (is_free_block(b)) {
						fprintf(file,"---- ");
					} else {
						fprintf(file,"%4X ",block_allocation[b]);
					}
				}
			}
			fprintf(file,"\n");
		}
		fprintf(file,"\n");
		if (fflush(file)!=0)
		{
			free(block_allocation);
			return errorf(TRUE,"DUMP -M");
		}
	}

	free(block_allocation);

	return 0;
}


/**********
  Transfer
 **********/


int detectmode (char *buf, int size)  {
/*  ^^^^^^^^^^
Count <size> characters in <buf>. If more than 70% are printable, the
file is considered as Text, otherwise Binary.
Returns M_TEXT or M_BIN.
*/
long	printable, total;
int	k;

	printable = total = 0;
	for (k=0;k<=size; k++)	{
		if (buf[k] == CPM_EOF) break;
		if ((buf[k]==10)||(buf[k]==13)||
		    ((buf[k]>=32)&&(buf[k]<=126)))  {
			printable++;
		}
		total++;
	}
	if (total==0) return M_BIN;	/* i.e. first char = ^Z */
	if ((100*printable/total) > 70)
		return M_TEXT;
	else
		return M_BIN;
}


long get(char *src, char *target) {
/*   ^^^
Get a file from CPC filesystem to local filesystem.
<src> is the CPC filename
<target> is the local filename
Returns number of bytes read or -1 if not found
*/
int	ent, i, k;
int	file;
long	bytes = 0;	/* sum of bytes copied */
int	size;		/* size to copy at a chunk */
bool	last;		/* in last entry? */
int	localmode;
int	err;
uchar	*buf, *p;

/* open CP/M file */
	if (has_wildcards('c',src)) {
		return errorf(FALSE,"\"%s\" may not contain wildcards",src);
	}
	ent = glob_cpm_file(src);
	if (ent<0)	return errorf(FALSE,"\"%s\" not found",src);


/* open DOS file */
#if defined(WIN32) || defined(WIN64)
	if (_access(target, F_OK)==0) {
#else
	if (access(target,F_OK)==0) {
#endif
		if (Verb > 0) {
			printm(1,"\"%s\" already exists! "
					"Overwrite? ",target);
			if (!confirmed())  {
				return -1;
			}
		}
	}
#if defined(WIN32) || defined(WIN64)
	file = _creat(target, _S_IREAD | _S_IWRITE);
#else
	file=creat(target,0644);
#endif
	if (file<0) return errorf(TRUE,"Cannot open \"%s\" for writing",target);


	localmode = -1; /* i.e. unknown */
	do {
		last = directory[ent].next == -1;	/* last entry? */
		for (i=0;i<BLKNR;i++) {
			if (directory[ent].blk[i]==0) {
				if (!last) {
					errorf(FALSE,"Directory entry for "
					  "\"%u:%s\" corrupt",
					  directory[ent].user,
					  directory[ent].name);
					close(file);
					return -1;
				} else
					break;
			}

			buf = read_block((signed)directory[ent].blk[i]);
			if (buf == NULL)
			{
				errorf(FALSE, "Failed to read block %d for "
					"\"%u:%s\"",
					directory[ent].blk[i],
					directory[ent].user,
					directory[ent].name);
				close(file);
				return -1;
			}
			if (localmode == -1) {
				if (mode!=M_AUTO)
					localmode = mode;
				else {
					localmode = detectmode(buf,
							(signed)dpb->BPS);
/*					printm(3,"%s detected.\n",
						show_mode(localmode));
*/
				}
			}

/* copy a whole block, except if on last blockpointer in last entry, then
   copy records */
			/* if we are accessing the last directory entry for this file.. */
			if (last &&
			    /* and indexing last block index in directory entry */
				/* e.g. 16 block index's can fit in a directory entry and we
				are accessing index 15 */
				((i==BLKNR-1) ||
				/* or indexing a block which is last in file */
				/* e.g. 16 block index's can fit in a directory entry and we
				are accessing index 5, index 6 is 0 indicating no more blocks */
			     directory[ent].blk[i+1]==0)) {
				size =
					/* number of extents used by this directory */
					(((directory[ent].extent & dpb->EXM)*EXTENTSIZE) +
					/* number of bytes used by all records used by all blocks in the directory
					entry */
					(directory[ent].rec*RECORDSIZE))
					-
					/* number of bytes used by blocks to this point */
					(i*dpb->BLS);
			} else {
				size = dpb->BLS;
			}

			if (size>=0)
			{
				if (localmode==M_BIN) {
					err=write(file,buf,size);
					bytes += size;
				} else  {	/* up to ^Z */
					p = memchr(buf,CPM_EOF,size);
					if (p==NULL) {/* no ^Z */
						err=write(file,buf,size);
						bytes += size;
					} else {/* get only a fraction of block */
						k = p-buf;
						err=write(file,buf,k);
						bytes += k;
					}
				}
			}
			else
			{
				err = size;
			}

			if (err<0) {
				close(file);
				return errorf(TRUE,"GET");
			}
		}
		ent = directory[ent].next;
	} while (ent>=0);

	close(file);
	return bytes;
}



long put(char *src, char *trg) {
/*   ^^^
Writes the DOS file <src> as CP/M File <trg>.
Returns number of bytes written or -1 if skipped, -2 if error */

uchar	*buf;
int	file;
int	entry,			/* current dir entry */
	blk,			/* pointer in data space */
	i;
long    size = 0;
long	total;			/* total bytes */
long	entry_total;		/* total bytes for one entry */
int	used_entries;

int	usr;
char	rootname[INPUTLEN];
char	extension[INPUTLEN];
const char wild_fmt[] = "\"%s\" may not contain wildcards";
struct stat stat_buf;
rootname[0] = '\0';
extension[0] = '\0';
	buf = block_buffer;	/* simply a shortcut */

	if (has_wildcards('d',src)) {
		errorf(FALSE,wild_fmt,src);
		return -2;
	}
	if (has_wildcards('c',trg)) {
		errorf(FALSE,wild_fmt,trg);
		return -2;
	}
	upper(trg);


/* test on existence and size of DOS file */
	if ((file = open(src, O_RDONLY|O_BINARY, 0)) == -1)  {
		errorf(TRUE,"Cannot read \"%s\"",src);
		return -2;
	}
	if (fstat(file,&stat_buf)) {
		errorf(TRUE,"--==>>> put: cannot stat \"%s\"",src);
		return -2;
	}
	if (stat_buf.st_size > (long)(dpb->DSM+1)*dpb->BLS) {
		errorf(FALSE,"\"%s\" is bigger than image",src);
		return -1;
	}

/* spilt the <trg> into name and extension */
	if (parse_cpm_filename(trg,&usr,rootname,extension)) return -1;
	if (usr==-1) usr = cur_user;
	if (*rootname==0) {
		errorf(FALSE,"No filename in \"%s\"",trg);
		return -2;
	}


/* test on existence in CP/M directory */
	if (glob_cpm_file(trg) >= 0) {
		if (Verb > 0) {
			printm(1,"\"%s\" already exists! Overwrite? ",trg);
			if (!confirmed())  {
				close(file);
				return -1;
			}
		}
		delete (TRUE,trg);
	}

/* walk thru the directory */
	total		= 0;
	used_entries	= 0;
	for (entry=0;entry<=dpb->DRM;entry++) {
		if (directory[entry].user != 0xE5) continue;

/* preread the first part, necessary if filesize multipe of 16k */
		size = read(file,buf,dpb->BLS);
		if (size==0) break;

/* fill name, user ... */
		directory[entry].user = usr;
		str2mem((signed char*)directory[entry].root,
			(signed char*)rootname, 8);
		str2mem((signed char*)directory[entry].ext,
			(signed char*)extension, 3);
		directory[entry].attr	= 0x00;
		directory[entry].unused[0]	= 0;	/* reserved for CP/M */
		directory[entry].unused[1]	= 0;

		/* KT - initialise blocks - if an error was caused during writing
		the remaining blocks could be 0x0e5. It would then attempt to access
		invalid memory */
		for (i=0; i<16; i++)
		{
			directory[entry].blk[i] = 0;
		}

/* walk thru the block pointer area */
		entry_total = 0;
		for (i=0;i<BLKNR;i++) {
			if (i>0) size = read(file,buf,dpb->BLS);
			total += size;
			entry_total += size;

			if (size==0) break;
/* get a free block and copy the data */
			blk = get_free_block();
			if (blk==-1)  {
				errorf(FALSE,"CPC Disk full!");
				update_directory(1);
				close(file);
				delete(TRUE,trg);
				return -2;
			}
			directory[entry].blk[i] = blk;
///* add a ^Z at the end */
//			if (size<dpb->BLS) {
//				buf[size] = CPM_EOF;
//				size++;
//			}
/* write the block */
			alloc_block(blk);
			if (write_block(blk,(signed char*)buf)==NULL) {
				errorf(FALSE,"Write error!");
				close(file);
				return -2;
			}

		}

		/* KT - changed calculation, and added brackets */
		/* there was a few bugs */

		/* assumption: with normal ext = 0, numbers are: 0,1,2,3, for each dir entry */
		/* for ext = 1, numbers are 1,3,5,7.. for each dir entry */

		{
			int num_extents;
			int num_records;

			num_extents = (entry_total+16383)>>14;

			if ((entry_total & 0x03fff)!=0)
			{
				num_records = ((entry_total & 0x03fff)+RECORDSIZE-1)>>7;
			}
			else
			{
				num_records = 0x080;
			}

			/* store num extents */
			directory[entry].extent = (num_extents-1)+(used_entries*dpb->num_extents);
			directory[entry].rec = num_records;
		}

		used_entries++;
		if (size==0) break;
	}

	close(file);
	update_directory(1);

/* uncopied data left */
	if (used_entries == 0)
	{
		errorf(FALSE, "CPC Directory full!");
		delete(TRUE, trg);
		return -2;
	}
	if (size>0) {
		errorf(FALSE,"CPC Directory full!");
		delete(TRUE,trg);
		return -2;
	}

/* <put_directory> needs <block_buffer> too, but it's available now after
<put> is nearly complete */
	put_directory();
	calc_allocation();
	return total;
}



