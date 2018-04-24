
/*				<<<<Last Modified: Thu Feb 08 15:07:16 1996>>>>
------------------------------------------------------------------------------

	=====
	CPCfs -- CPCEmu Filesystem Maintenance
        =====                   especially for CPCEmu

	Version 0.85                    (c) February '96 by Derik van Zuetphen
------------------------------------------------------------------------------
*/


#include "cpcfs.h"

/***********
  Variables
 ***********/

/****** CPC Filesystem ******/

char	full_imagename[FULL_IMAGENAME_LENGTH];	/* full pathname, ... */
char	*imagename;		/* ... only name portion, and ... */
int	imagefile;		/* ... handle of imagefile */

int	cur_user	= 0;
int	cur_format;
int	BLKNR_SIZE 	= 1;
int	BLKNR 		= 16;

uchar	*track = NULL;	/* 0x100 header + track data */
unsigned char filler;	/* copied from track header */

uchar	*block_buffer = NULL;	/* Buffer for 1024 or more bytes */

uchar	*blk_alloc = NULL;	/* Block-Allocation-Table, */
				/* chars point to fcb, FF = free block */
int	allocated_blks;
int	free_blks;
int	total_blks;	/* = allocated_blks + free_blks */
float	percentage;	/* of allocated blocks */

int	mode;		/* Mode for GET-commands */
bool	force;

struct d_header disk_header;


/****** User Interface *****/
int	nbof_args;
char	*arg[INPUTLEN];
char	prompt[INPUTLEN];
int	pagelen, lineno;
bool	Break_Wish;
jmp_buf break_entry;
bool	Interactive;
char	installpath[INPUTLEN];		/* path of argv[0] */
int	Verb = 9;

DirEntry *directory = (DirEntry*)NULL;

/* KT - adjusted stamp */
char stamp[] = "Compiled: " __DATE__ " "  __TIME__ "  by Kevin Thacker.";
char copyright[] = "Original code (C) Dec. 1995 Derik van Ztphen";


/****** Disk Parameter Block ******/
#if 0
/* DPB templates for SYSTEM, DATA, IBM, VORTEX, and user defined */
#define DPB_store_size  5
DPB_type DPB_store[DPB_store_size] = {
/* SYSTEM */
	{	/*SYSTEMFORMAT,*/ /* ID */
		0x041, /* SEC1 */
		9,	/* SECS */
		40,	/* TRKS */
		1,	/* HDS */
		512,	/* BPS */

		36,	/* SPT */
		3,	/* BSH */
		7,	/* BLM */
		0,	/* EXM */
		168,	/* DSM */
		63,	/* DRM */
		0xC0,	/* AL0 */
		0x00,	/* AL1 */
		16,	/* CKS */
		2,	/* OFS */
		0,0,0
	},
/* DATA */
	{	/*DATAFORMAT,*/ /* ID */
		0x0c1, /* SEC1 */
		9,	/* SECS */
		40,	/* TRKS */
		1,	/* HDS */
		512,	/* BPS */

		36,	/* SPT */
		3,	/* BSH */
		7,	/* BLM */
		0,	/* EXM */
		177,	/* DSM */
		63,	/* DRM */
		0xC0,	/* AL0 */
		0x00,	/* AL1 */
		16,	/* CKS */
		0,	/* OFS */
		0,0,0
	},
/* IBM */
	{	/*IBMFORMAT,*/ /* ID */
		0x01, /* SEC1 */
		8,	/* SECS */
		40,	/* TRKS */
		1,	/* HDS */
		512,	/* BPS */

		36,	/* SPT */
		3,	/* BSH */
		7,	/* BLM */
		0,	/* EXM */
		153,	/* DSM */
		63,	/* DRM */
		0xC0,	/* AL0 */
		0x00,	/* AL1 */
		16,	/* CKS */
		1,	/* OFS */
		0,0,0
	},
/* VORTEX */
	{       /*VORTEXFORMAT,*/ /* ID */
		0x01,	/* SEC1 */
		9,	/* SECS */
		80,	/* TRKS */
		2,	/* HDS */
		512,	/* BPS */

		36,	/* SPT */
		5,	/* BSH */
		31,	/* BLM */
		3,	/* EXM */
		176,	/* DSM */
		127,	/* DRM */
		0x80,	/* AL0 */
		0x00,	/* AL1 */
		32,	/* CKS */
		2,	/* OFS */
		0,0,0
	}
/* user defined DPB is empty for now */
};
#endif

#if 0
/* DPB templates for SYSTEM, DATA, IBM, VORTEX, and user defined */
#define DPB_store_size  5
DPB_type DPB_store[DPB_store_size] = {
/* SYSTEM */
	{	/*SYSTEMFORMAT,*/ /* ID */
		0x041, /* SEC1 */
		9,	/* SECS */
		40,	/* TRKS */
		1,	/* HDS */
		512,	/* BPS */

		36,	/* SPT */
		3,	/* BSH */
		7,	/* BLM */
		0,	/* EXM */
		168,	/* DSM */
		63,	/* DRM */
		0xC0,	/* AL0 */
		0x00,	/* AL1 */
		16,	/* CKS */
		2,	/* OFS */
		0,0,0
	},
/* DATA */
	{	/*DATAFORMAT,*/ /* ID */
		0x0c1, /* SEC1 */
		9,	/* SECS */
		40,	/* TRKS */
		1,	/* HDS */
		512,	/* BPS */

		36,	/* SPT */
		3,	/* BSH */
		7,	/* BLM */
		0,	/* EXM */
		177,	/* DSM */
		63,	/* DRM */
		0xC0,	/* AL0 */
		0x00,	/* AL1 */
		16,	/* CKS */
		0,	/* OFS */
		0,0,0
	},
/* IBM */
	{	/*IBMFORMAT,*/ /* ID */
		0x01, /* SEC1 */
		8,	/* SECS */
		40,	/* TRKS */
		1,	/* HDS */
		512,	/* BPS */

		36,	/* SPT */
		3,	/* BSH */
		7,	/* BLM */
		0,	/* EXM */
		153,	/* DSM */
		63,	/* DRM */
		0xC0,	/* AL0 */
		0x00,	/* AL1 */
		16,	/* CKS */
		1,	/* OFS */
		0,0,0
	},
/* VORTEX */
	{       /*VORTEXFORMAT,*/ /* ID */
		0x01,	/* SEC1 */
		9,	/* SECS */
		80,	/* TRKS */
		2,	/* HDS */
		512,	/* BPS */

		36,	/* SPT */
		5,	/* BSH */
		31,	/* BLM */
		3,	/* EXM */
		176,	/* DSM */
		127,	/* DRM */
		0x80,	/* AL0 */
		0x00,	/* AL1 */
		32,	/* CKS */
		2,	/* OFS */
		0,0,0
	}
/* user defined DPB is empty for now */
};
#endif

DPB_type system_dpb=
	{	
		{0x041, 0x042, 0x043, 0x044, 0x045, 0x046, 0x047, 0x048, 0x049},
		{0x041, 0x042, 0x043, 0x044, 0x045, 0x046, 0x047, 0x048, 0x049}, 
		9,	/* SECS */
		40,	/* TRKS */
		1,	/* HDS */
		512,	/* BPS */

		36,	/* SPT */
		3,	/* BSH */
		7,	/* BLM */
		0,	/* EXM */
		170,	/* DSM */
		63,	/* DRM */
		0xC0,	/* AL0 */
		0x00,	/* AL1 */
		16,	/* CKS */
		2,	/* OFS */
		0,0,0,
		ORDER_SIDES,0,0,5,0,NULL
	};

DPB_type data_dpb = 
	{	
		{0x0c1, 0x0c2, 0x0c3, 0x0c4, 0x0c5,0x0c6,0x0c7,0x0c8,0x0c9}, 
		{0x0c1, 0x0c2, 0x0c3, 0x0c4, 0x0c5, 0x0c6, 0x0c7, 0x0c8, 0x0c9},
		9,	/* SECS */
		40,	/* TRKS */
		1,	/* HDS */
		512,	/* BPS */

		36,	/* SPT */
		3,	/* BSH */
		7,	/* BLM */
		0,	/* EXM */
		179,	/* DSM */
		63,	/* DRM */
		0xC0,	/* AL0 */
		0x00,	/* AL1 */
		16,	/* CKS */
		0,	/* OFS */
		0,0,0,
		ORDER_SIDES,0,0,5,0,NULL
	};


DPB_list_entry *dpb_list_entry = NULL;
DPB_type *dpb = NULL;	/* pointer to current DPB */
DPB_list dpb_list;

void	dpb_list_add_entry(DPB_list *list, DPB_list_entry *entry)
{
	DPB_list_entry *cur_entry;

	/* if list empty add as first item */
	if (list->first==NULL)
	{
		list->first = entry;
	}
	else
	{
		/* add to end of list */
		cur_entry = list->first;

		while (cur_entry->next!=NULL)
		{
			cur_entry = cur_entry->next;
		}

		cur_entry->next = entry;
	}
}


/* remove entry from list */
void	dpb_list_remove_entry(DPB_list *list,DPB_list_entry *entry)
{
	DPB_list_entry *cur_entry;

	/* entry at start of list? */
	if (list->first==entry)
	{
		list->first = entry->next;
		return;
	}

	/* find entry which points to this one and update it's next pointer
	to point to next of entry we are removing */
	cur_entry = list->first;

	while (cur_entry!=NULL)
	{
		if (cur_entry->next == entry)
		{
			cur_entry->next = entry->next;
			return;
		}
	}
}

/* delete an entry from the list */
void	dpb_list_entry_delete(DPB_list *list,DPB_list_entry *entry)
{
	/* remove it */
	dpb_list_remove_entry(list, entry);

	/* free label data */
	if (entry->dpb.label_data!=NULL)
	{
		free(entry->dpb.label_data);
	}


	/* delete entry data */
	if (entry->description!=NULL)
	{
		free(entry->description);
	}

	if (entry->ident!=NULL)
	{
		free(entry->ident);
	}

	/* delete entry */
	free(entry);
}

/* delete all items in list */
void	dpb_list_delete(DPB_list *list)
{
	DPB_list_entry *entry;

	entry = list->first;

	while (entry!=NULL)
	{
		/* get next entry */
		DPB_list_entry *next = entry->next;

		/* delete the entry */
		dpb_list_entry_delete(list,entry);

		entry = next;
	}
}

bool	dpb_compare_sector_id(DPB_type *a, DPB_type *b)
{
	/* number of heads must be the same */
	if (a->HDS != b->HDS)
		return FALSE;

	/* if one head, check id on first side */
	if (a->HDS == 1)
	{
		return (a->SEC_side1[0] == b->SEC_side1[0]);
	}
	
	/* if 2 sides, check id on second side too */
	return ((a->SEC_side1[0] == b->SEC_side1[0]) && (a->SEC_side2[0] == b->SEC_side2[0]));
}


/* compare two dpb's. This comparison checks the values that are not calculated,
because they may not be setup in one or both of the definitions */
bool	dpb_match(DPB_type *a, DPB_type *b)
{
	if (dpb_compare_sector_id(a,b) &&
		(a->SECS == b->SECS) &&
		(a->TRKS == b->TRKS) &&
		(a->BPS == b->BPS) &&
		(a->BSH == b->BSH) &&
		(a->BLM == b->BLM) &&
		(a->EXM == b->EXM) &&
		(a->DSM == b->DSM) &&
		(a->DRM == b->DRM) &&
		(a->AL0 == b->AL0) &&
		(a->AL1 == b->AL1) &&
		(a->OFS == b->OFS)&&
		(a->order == b->order) &&
		(a->side0_hd == b->side0_hd) && 
		(a->side1_hd == b->side1_hd) &&
		(a->skew == b->skew))
	{
		return TRUE;
	}
	
	return FALSE;
}



/* go through all entries. if a entry has a dpb which matches the dpb specified, then return
a pointer to this entry */
DPB_list_entry	*dpb_check_if_matching_format_exists(DPB_list *list, DPB_type *type)
{
	DPB_list_entry *cur_entry;

	cur_entry = list->first;

	while (cur_entry!=NULL)
	{
		if (dpb_match(&cur_entry->dpb, type))
		{
			return cur_entry;
		}

		cur_entry = cur_entry->next;
	}

	return NULL;
}

void	dpb_list_add_item(DPB_list *list,char *description, char *ident, DPB_type *dpb)
{
	DPB_list_entry *entry = (DPB_list_entry *)malloc(sizeof(DPB_list_entry));

	if (entry!=NULL)
	{
		if (description==NULL)
		{
			entry->description = NULL;
		}
		else
		{
			entry->description = (char *)malloc(strlen(description)+1);

			if (entry->description!=NULL)
			{
				strcpy(entry->description, description);
			}
		}

		if (ident==NULL)
		{
			entry->ident = NULL;
		}
		else
		{
			entry->ident = (char *)malloc(strlen(ident)+1);

			if (entry->ident!=NULL)
			{
				strcpy(entry->ident, ident);
				upper(entry->ident);
			}
		}


		/* copy dpb */
		memcpy(&entry->dpb, dpb, sizeof(DPB_type));

		/* if cylinders is 40, but used in a 80 track drive, then the drive
		head is automatically double stepped */
		/* want to use cylinders from .def because this is good for discs
		where only 60 tracks are used */
#if 0
		/* The 22disk definition uses CYLINDERS to define, from what I can
		see the physical number of tracks on the disc media. So, you can
		have a 80 track 5.25" drive reading a 40 track format with
		or without double step - calculate a decent TRKS from DSM
		which specifies the total number of blocks on the disc. */
		{
			DPB_type *dpb = &entry->dpb;

			dpb->BLS  = 1 << (dpb->BSH + 7); /* or 2^BSH*128 */

			/* calculate the number of tracks from DSM */
			dpb->TRKS = (((dpb->DSM+1)*dpb->BLS)+(dpb->OFS*dpb->SECS*dpb->BPS) + ((dpb->DRM+1)<<5))/(dpb->SECS*dpb->BPS*dpb->HDS);
		}
#endif

		entry->next = NULL;
	}

	dpb_list_add_entry(list, entry);
}


/* KT - moved to here because it was causing problems with MSDev */
int	ui_main (int,char**);


void parse_def_file(char *);

int main (int argc, char **argv) {
	/* initialise list */
	dpb_list.first = NULL;

	initialise();

	/* KT - add default formats */
	
	/* data */
	dpb_list_add_item(&dpb_list,"Data Format", "data", &data_dpb);
	
	/* system */
	dpb_list_add_item(&dpb_list,"System Format", "system", &system_dpb);


	parse_def_file("cpmdisks.def");
										  
	ui_main (argc,argv);
	
	dpb_list_delete(&dpb_list);

	return 0;
}
