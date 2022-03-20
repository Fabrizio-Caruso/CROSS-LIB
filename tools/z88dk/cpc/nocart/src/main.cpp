// v1.9
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <memory.h>
#include <string.h>

#ifdef WIN32
#define stringcmp _stricmp
#else
#define stringcmp strcasecmp
#endif

extern "C"
{
#include "opth.h"
};
#define SECTOR_ID_PATCH_DOS_OFFSET 0x056d
#define FILENAME_PATCH_DOS_OFFSET 0x01c04
#define DATA_XPDB_OFFSET 0x05ca /* partial; 0x00a bytes long */
#define SYSTEM_XPDB_OFFSET 0x00a43 /* full; 0x016 bytes long */
#define BINARY_CART_PAGE 21
#define FORMAT_ID 0x056d

enum
{
	DISK_IMAGE_TYPE_UNKNOWN,
	DISK_IMAGE_TYPE_STANDARD,
	DISK_IMAGE_TYPE_EXTENDED
};

static int DiskImageType = DISK_IMAGE_TYPE_UNKNOWN;

int UsesBinaryFile = 0;
int UsesBasicCommand = 0;
char BasicCommand[16];
int DskMaxChunk = 32;
int UsesExtendedFormat = 0;
int ChosenExtendedFormat = -1;

#pragma pack(1)
typedef struct
{
	unsigned char SPT_l; /* 128 byte records per track */
	unsigned char SPT_h; /* 128 byte records per track */
	unsigned char BSH;
	unsigned char BLM;
	unsigned char EXM;
	unsigned char DSM_l;
	unsigned char DSM_h;
	unsigned char DRM_l;
	unsigned char DRM_h;
	unsigned char AL_l; /* AL0 and AL1 */
	unsigned char AL_h; /* AL0 and AL1 */
	unsigned char CKS_l; /* num dir entries/4 rounded up */
	unsigned char CKS_h; /* num dir entries/4 rounded up */
	unsigned char OFF_l;
	unsigned char OFF_h;
	unsigned char SectorID;
	unsigned char SectorsPerTrack;
	unsigned char GapRW;
	unsigned char GapFormat;
	unsigned char Filler;
	unsigned char N;
	unsigned char RecordsPerSector;
} XPDB;
#pragma pack()


typedef struct
{
	const char *FormatID;
	int SectorIDMin;
	int MaxTracks;
	int SPT;
	XPDB XPDB;
} SupportedFormat;

const SupportedFormat SupportedFormats[1]=
{
	{
		"DA3A",
		0x0c1,
		60,
		9,
		{
			(0x024&0x0ff),	/* SPT */
			((0x024>>8) & 0x0ff),	/* SPT */
			0x03,	/* BSH */
			0x07,	/* BLM */
			0x0,	/* EXM */
			(254&0x0ff),	/* DSM */
			((254>>8)&0x0ff),
			(63 & 0x0ff),	/* DRM */
			((63 >> 8) & 0x0ff),
			(0x00c0 & 0x0ff),	/* AL0/AL1 */
			((0x00c0 >> 8) & 0x0ff),
			(0x010 & 0x0ff),	/* CKS */
			((0x010 >> 8) & 0x0ff),
			(0x0 & 0x0ff),	/* OFS */
			((0x0 >> 8) & 0x0ff),
			0xc1,/* Sector ID */
			0x09, /* SPT */
			0x2a, /* Gap RW */
			0x52, /* Gap Format */
			0xe5, /* Filler */
			0x02, /* N */
			0x04
		}
	}
};

typedef struct
{
	uint8_t	C;
	uint8_t	H;
	uint8_t	R;
	uint8_t	N;
	uint8_t	ST1;
	uint8_t	ST2;
	uint8_t	SectorSizeLow;
	uint8_t SectorSizeHigh;
} EXTDSKCHRN;

typedef struct
{
	char	TrackHeader[12];
	char	pad0[4];
	unsigned char	track;
	unsigned char	side;
	unsigned char	pad1[2];
	unsigned char	BPS;
	unsigned char	SPT;
	unsigned char	Gap3;
	unsigned char	FillerByte;
	EXTDSKCHRN	SectorIDs[29];
} EXTDSKTRACKHEADER;

typedef struct
{
	char		DskHeader[34];
	char		DskCreator[14];
	unsigned char	NumTracks;
	unsigned char	NumSides;
	unsigned char	TrackSizeLow;
	unsigned char	TrackSizeHigh;
	char		TrackSizeTable[255 - 4 - 14 - 33];
} EXTDSKHEADER;


typedef struct
{
	uint8_t	C;
	uint8_t	H;
	uint8_t	R;
	uint8_t	N;
	uint8_t	ST1;
	uint8_t	ST2;
	uint8_t	Unused[2];
} DSKCHRN;

typedef struct
{
	char	TrackHeader[12];
	char	pad0[4];
	unsigned char	track;
	unsigned char	side;
	unsigned char	pad1[2];
	unsigned char	BPS;
	unsigned char	SPT;
	unsigned char	Gap3;
	unsigned char	FillerByte;
	DSKCHRN	SectorIDs[29];
} DSKTRACKHEADER;


typedef struct
{
	char		DskHeader[34];
	char		DskCreator[14];
	unsigned char	NumTracks;
	unsigned char	NumSides;
	unsigned char	TrackSizeLow;
	unsigned char	TrackSizeHigh;
	char		Unused[255 - 4 - 14 - 33];
} DSKHEADER;

const uint8_t *ExtendedGetSectorDataPtr(const uint8_t *pData, int PhysicalTrack, int SectorID)
{
	EXTDSKHEADER *pHeader = (EXTDSKHEADER *)pData;
	EXTDSKTRACKHEADER *pTrackHeader;
	size_t Offset = 0;
	int t;
	int s;
	int TrackIndex = PhysicalTrack*pHeader->NumSides;

	if (pHeader->TrackSizeTable[TrackIndex] == 0)
		return NULL;

	/* accumulate offset to desired track using size of each track */
	for (t = 0; t < TrackIndex; t++)
	{
		Offset += (pHeader->TrackSizeTable[t] & 0x0ff) << 8;
	}

	/* add main header size */
	Offset += sizeof(EXTDSKHEADER);

	pTrackHeader = (EXTDSKTRACKHEADER *)(pData + Offset);

	/* add track header */
	Offset += sizeof(EXTDSKTRACKHEADER);

	/* locate sector and accumulate sector sizes on offset */
	for (s = 0; s < pTrackHeader->SPT; s++)
	{
		size_t SectorSize = 0;

		if (pTrackHeader->SectorIDs[s].R == SectorID)
		{
			return (uint8_t *)(pData + Offset);
		}

		/* found sector id */
		SectorSize =
			(pTrackHeader->SectorIDs[s].SectorSizeLow & 0x0ff) |
			((pTrackHeader->SectorIDs[s].SectorSizeHigh & 0x0ff) << 8);
		Offset += SectorSize;
	}
	
	return NULL;

}


const uint8_t *StandardGetSectorDataPtr(const uint8_t *pData, int PhysicalTrack, int SectorID)
{
	DSKHEADER *pHeader = (DSKHEADER *)pData;
	DSKTRACKHEADER *pTrackHeader;
	size_t Offset = 0;
	int s;
	int TrackIndex = PhysicalTrack*pHeader->NumSides;

	Offset = (
		(pHeader->TrackSizeLow & 0x0ff) |
		((pHeader->TrackSizeHigh & 0x0ff)<<8)
		) * TrackIndex;

	/* add main header size */
	Offset += sizeof(DSKHEADER);

	pTrackHeader = (DSKTRACKHEADER *)(pData + Offset);

	/* add track header */
	Offset += sizeof(DSKTRACKHEADER);

	/* locate sector and accumulate sector sizes on offset */
	for (s = 0; s < pTrackHeader->SPT; s++)
	{
		size_t SectorSize = 0;

		if (pTrackHeader->SectorIDs[s].R == SectorID)
		{
			return (uint8_t *)(pData + Offset);
		}

		switch (pTrackHeader->SectorIDs[s].N)
		{
		case 0:
			SectorSize = 128;
			break;
		case 1:
			SectorSize = 256;
			break;
		case 2:
			SectorSize = 512;
			break;
		case 3:
			SectorSize = 1024;
			break;
		case 4:
			SectorSize = 2048;
			break;

		}

		Offset += SectorSize;
	}

	return NULL;
}



#define min(a,b) \
	(a<b) ? a : b

const char *RIFFTag = "RIFF";
const char *AmsTag = "AMS!";
static int SectorIDMin = 0x0c1;
static int SectorIDMax = 0x0c9;

#define NUM_CHUNKS 32
#define CART_CHUNK_LENGTH 16384
#define RIFF_CHUNK_HEADER_SIZE (sizeof(uint32_t)*2)
#define SECTOR_SIZE 512
#define MAX_TRACKS 40
static int MaxTracks = MAX_TRACKS;
static int NumTracks = MAX_TRACKS;
static int MaxSPT = 9;
static int MinID = 0x0c1;
static int MaxID = 0x0c9;

static void WriteTag(FILE *fh, const char *sTag)
{
	fwrite(sTag, 4, 1, fh);
}

typedef struct 
{
	uint8_t *pData;
	size_t DataLength;
} DataBlock;


static int CheckDiskValid(DataBlock *pBlock)
{
	int result = -1;
	switch (DiskImageType)
	{
		case DISK_IMAGE_TYPE_EXTENDED:
		{
			int t;
			int TrackSizeWanted;
			int Offset;
			
			result = 0;
			EXTDSKHEADER *pHeader = (EXTDSKHEADER *)pBlock->pData;
			if (pHeader->NumSides!=1)
			{
				fprintf(stderr,"Double sided discs are not supported\n");
				return -1;
			}
			fprintf(stdout,"Single sided disc detected\n");
			
			NumTracks = pHeader->NumTracks;
			if (NumTracks>MaxTracks)
			{
				fprintf(stderr,"Disk image has %d tracks. Only %d will be used\n", pHeader->NumTracks, MaxTracks);
				NumTracks = MaxTracks;
			}
			else
			{
				fprintf(stderr,"Disk image has %d tracks.\n", pHeader->NumTracks);
			}
			
			TrackSizeWanted = ((512*MaxSPT)+sizeof(EXTDSKTRACKHEADER))>>8;
			
			/* check track sizes */
			for (t=0; t<NumTracks*pHeader->NumSides; t++)
			{
				if (pHeader->TrackSizeTable[t]!=TrackSizeWanted)
				{
					fprintf(stderr,"Track %d has size %d. Not supported\n", t, (int)(pHeader->TrackSizeTable[t]&0x0ff)<<8);
					result = -1;
				}
			}
			
			Offset = sizeof(EXTDSKHEADER);
			SectorIDMin = 255;
			SectorIDMax = 0;
			for (t=0; t<NumTracks; t++)
			{
				int s;
				int TrackSize = 0;
				
				EXTDSKTRACKHEADER *pTrackHeader = (EXTDSKTRACKHEADER *)(pBlock->pData + Offset);
				
				if (pTrackHeader->SPT!=MaxSPT)
				{
					fprintf(stderr,"Track %d has %d sectors. Not supported\n", t,(int)pTrackHeader->SPT);
					result = -1;
				}
				
				for (s=0; s<pTrackHeader->SPT; s++)
				{
					if ( 
						(pTrackHeader->SectorIDs[s].C!=t) ||
						(pTrackHeader->SectorIDs[s].H!=0) ||
						(pTrackHeader->SectorIDs[s].N!=2) ||
						(pTrackHeader->SectorIDs[s].ST1!=0) ||
						(pTrackHeader->SectorIDs[s].ST2!=0) ||
						(pTrackHeader->SectorIDs[s].SectorSizeLow!=0) ||
						(pTrackHeader->SectorIDs[s].SectorSizeHigh!=2)
					)
					{
						fprintf(stderr,"Sector %d on Track %d has unsupported ID or protection (ID: %02x %02x %02x %02x).\n", s,t,pTrackHeader->SectorIDs[s].C, pTrackHeader->SectorIDs[s].H, pTrackHeader->SectorIDs[s].R, pTrackHeader->SectorIDs[s].N);
						result = -1;
					}

					/* capture id */
					if (pTrackHeader->SectorIDs[s].R < SectorIDMin)
					{
						SectorIDMin = pTrackHeader->SectorIDs[s].R;
					}
						
					if (pTrackHeader->SectorIDs[s].R > SectorIDMax)
					{
						SectorIDMax = pTrackHeader->SectorIDs[s].R;
					}
				}
				
				TrackSize = (pHeader->TrackSizeTable[t] & 0x0ff)<<8;
									
				Offset+=TrackSize;
			}
		}
		break;

		case DISK_IMAGE_TYPE_STANDARD:
		{
			int t;
			int TrackSize = 0;
			size_t Offset;

			result = 0;
			DSKHEADER *pHeader = (DSKHEADER *)pBlock->pData;
			if (pHeader->NumSides!=1)
			{
				fprintf(stderr,"Double sided discs are not supported\n");
				return -1;
			}
			fprintf(stdout,"Single sided disc detected\n");
			
			NumTracks = pHeader->NumTracks;
			if (NumTracks>MaxTracks)
			{
				fprintf(stderr,"Disk image has %d tracks. Only %d will be used\n", pHeader->NumTracks, MaxTracks);
				NumTracks = MaxTracks;
			}
			else
			{
				fprintf(stderr,"Disk image has %d tracks.\n", pHeader->NumTracks);
			}
			
			
			TrackSize = (
			(pHeader->TrackSizeLow & 0x0ff) | 
			((pHeader->TrackSizeHigh & 0x0ff)<<8)
			);
			
			Offset = sizeof(DSKHEADER);
			SectorIDMin = 255;
			SectorIDMax = 0;
			for (t=0; t<NumTracks; t++)
			{
				int s;
				
				DSKTRACKHEADER *pTrackHeader = (DSKTRACKHEADER *)(pBlock->pData + Offset);
				
				if (pTrackHeader->SPT!= MaxSPT)
				{
					fprintf(stderr,"Track %d has %d sectors. Not supported\n", t,pTrackHeader->SPT);
					result = -1;
				}
				
				for (s=0; s<pTrackHeader->SPT; s++)
				{
					if ( 
						(pTrackHeader->SectorIDs[s].C!=t) ||
						(pTrackHeader->SectorIDs[s].H!=0) ||
						(pTrackHeader->SectorIDs[s].N!=2) ||
						(pTrackHeader->SectorIDs[s].ST1!=0) ||
						(pTrackHeader->SectorIDs[s].ST2!=0)
					)
					{
						fprintf(stderr,"Sector %d on Track %d has unsupported ID or protection (ID: %02x %02x %02x %02x).\n", s,t,pTrackHeader->SectorIDs[s].C, pTrackHeader->SectorIDs[s].H, pTrackHeader->SectorIDs[s].R, pTrackHeader->SectorIDs[s].N);
						result = -1;
					}


					/* capture id */
					if (pTrackHeader->SectorIDs[s].R < SectorIDMin)
					{
						SectorIDMin = pTrackHeader->SectorIDs[s].R;
					}
						
					if (pTrackHeader->SectorIDs[s].R > SectorIDMax)
					{
						SectorIDMax = pTrackHeader->SectorIDs[s].R;
					}
				}
					
				Offset+=TrackSize;
			}

			/* if result=0, then all is ok and either data or system detected */
			
		}	
		break;

		default:
			return -1;
	}
	
	return result;
}

typedef struct
{
	int CurrentPhysicalTrack;
	int CurrentSectorID;
} DiskPosition;

static int ReadFile(const char *sFilename, DataBlock *pBlock)
{
	int result = 0;

	pBlock->pData = NULL;
	pBlock->DataLength = 0;

	FILE *fhIn = fopen(sFilename, "rb");
	if (fhIn)
	{
		size_t FileLength = 0;
		fseek(fhIn, 0, SEEK_END);

		FileLength = (size_t)ftell(fhIn);

		fseek(fhIn, 0, SEEK_SET);

		if (FileLength != 0)
		{
			pBlock->DataLength = FileLength;
			pBlock->pData = (uint8_t *)malloc(FileLength);
			if (pBlock->pData)
			{
				if (fread(pBlock->pData, FileLength, 1, fhIn)==1)
				{
					fprintf(stdout,"Successfully read '%s'\n", sFilename);
					result = 1;
				}
			}
		}

		fclose(fhIn);
	}
	else
	{
		fprintf(stderr, "Failed to open file: '%s'\n", sFilename);
	}

	return result;
}

static void PadPage(FILE *fh, int Remaining)
{
	/* pad to end if size is smaller than cart chunk length */
	while (Remaining != 0)
	{
		fputc(0x0ff, fh);
		Remaining--;
	}
}

static int WriteROM(FILE *fh, const char *sFilename)
{
	int resultCode = 0;

	DataBlock block;
	if (ReadFile(sFilename, &block))
	{
    			size_t Size = block.DataLength;
		size_t Remaining = 0;

		if (Size > CART_CHUNK_LENGTH)
		{
			fprintf(stderr, "File '%s' is larger than cart page size %d. It will be cropped\n", sFilename, CART_CHUNK_LENGTH);
		}
		Size = min(Size, CART_CHUNK_LENGTH);
		Remaining = CART_CHUNK_LENGTH - Size;

		fwrite(block.pData, Size, 1, fh);

		PadPage(fh, Remaining);
	}
	else
	{
		PadPage(fh, CART_CHUNK_LENGTH);
		resultCode = 1;
	}

	if (block.pData!=NULL)
	{
		free(block.pData);
		block.pData = NULL;
	}
	return resultCode;
}



static int NumFiles = 0;
static const char *Filenames[2];
static const char *os = NULL;
static const char *pBinaryFilename;

int	NonOptionHandler(const char *pOption)
{
	if (NumFiles<2)
	{
		Filenames[NumFiles] = pOption;
		NumFiles++;
	}

	return OPTION_OK;
}

int SetCommandHandler(ARGUMENT_DATA *pData)
{
	const char *command = ArgumentList_GetNext(pData);

	if (command==NULL)
		return OPTION_MISSING_PARAMETER;
	
  UsesBasicCommand = 1;
  strncat(BasicCommand, command, 15);
  BasicCommand[15]='\0';
	return OPTION_OK;
}

int SetBinaryFileHandler(ARGUMENT_DATA *pData)
{
	const char *filename = ArgumentList_GetNext(pData);

	if (filename == NULL)
		return OPTION_MISSING_PARAMETER;

	DskMaxChunk = 21;
	UsesBinaryFile = 1;
	pBinaryFilename = filename;
	return OPTION_OK;
}

int SetAllowExtendedFormatHandler(ARGUMENT_DATA *pData)
{
	int i;
	const char *formatName = ArgumentList_GetNext(pData);

	if (formatName == NULL)
	{
		/* list supported formats */
		
		printf("Supported extended formats: ");
		for (i=0; i<sizeof(SupportedFormats)/sizeof(SupportedFormats[0]); i++)
		{
			if (i != 0)
			{
				printf(",");
			}
			printf("%s",SupportedFormats[i].FormatID);
		}	
		printf("\n");
		return OPTION_OK;
	}
	
	/* choose format */
	for (i=0; i<sizeof(SupportedFormats)/sizeof(SupportedFormats[0]); i++)
	{
		if (stringcmp(SupportedFormats[i].FormatID,formatName)==0)
		{
			UsesExtendedFormat = 1;
			ChosenExtendedFormat = i;
			MaxTracks = SupportedFormats[i].MaxTracks;
			MaxSPT = SupportedFormats[i].SPT;
			MinID = SupportedFormats[i].SectorIDMin;
			MaxID = SupportedFormats[i].SectorIDMin + (SupportedFormats[i].SPT-1);
			fprintf(stdout,"Extended format '%s' chosen\n", SupportedFormats[i].FormatID);
			return OPTION_OK;
		}
	}
	fprintf(stderr,"Extended format '%s' not found\n",formatName);
	return OPTION_BAD_VALUE;
}


void OutputDetails()
{
	printf("NoCart v1.9\n");
#ifdef WIN32
	printf("Usage: [options] <disk image> <output cart>\ne.g. nebulus.dsk nebulus.cpr -c RUN\"\"\"disc\n\n");
#else
  printf("Usage: [options] <disk image> <output cart>\ne.g. nebulus.dsk nebulus.cpr -c 'RUN\"disc'\n\n");
#endif

  printf("[options]:\n");
	printf("-c <string> = BASIC command to execute. Limited to 16 characters\n");
	printf("          e.g. RUN\"disc or |CPM\n");
	printf("-f <format> = Choose extended format\n");
	printf("-f = List supported extended formats\n");
	printf("-b <filename> = add binary data. (cart block %d-31/&%02x-&ff. Max size %d)\n", BINARY_CART_PAGE, BINARY_CART_PAGE|0x080, (32-BINARY_CART_PAGE)*16384);
}


int OutputDetailsOption(ARGUMENT_DATA *pData)
{
	OutputDetails();
	return OPTION_OK;
}
OPTION OptionTable[]=
{
	{"c",SetCommandHandler},
	{ "b", SetBinaryFileHandler},
	{ "f", SetAllowExtendedFormatHandler},
	{ "?", OutputDetailsOption },
	{NULL, NULL},
};
	

static void WriteCartTag(FILE *fh, int index)
{
	char CartTag[4];
	CartTag[0] = 'c';
	CartTag[1] = 'b';
	CartTag[2] = (index / 10) + '0';
	CartTag[3] = (index % 10) + '0';
	WriteTag(fh, CartTag);
}


static void UpdateDiskPosition(DiskPosition *position)
{
	position->CurrentSectorID++;
	if (position->CurrentSectorID > SectorIDMax)
	{
		position->CurrentSectorID = SectorIDMin;
		position->CurrentPhysicalTrack++;
	}
}

static int IsDiskDone(DiskPosition *position)
{
	return (
		(position->CurrentSectorID == SectorIDMin) &&
		(position->CurrentPhysicalTrack == NumTracks)
		);
}

static void WriteChunkLengthLittleEndian(FILE *fh, uint32_t Length)
{
	unsigned char LengthBuffer[4];
	LengthBuffer[0] = Length & 0x0ff;
	LengthBuffer[1] = (Length >> 8) & 0x0ff;
	LengthBuffer[2] = (Length >> 16) & 0x0ff;
	LengthBuffer[3] = (Length >> 24) & 0x0ff;

	fwrite(LengthBuffer, 4, 1, fh);
}

static void WriteSectorDataToCartridgePage(FILE *fh, uint8_t *pDiskImageData, DiskPosition *position)
{
	size_t Remaining = CART_CHUNK_LENGTH;

	do
	{
		const uint8_t *pSectorPtr = NULL;
		
		switch (DiskImageType)
		{
			case DISK_IMAGE_TYPE_EXTENDED:
				pSectorPtr = ExtendedGetSectorDataPtr(pDiskImageData, position->CurrentPhysicalTrack, position->CurrentSectorID);
				break;

			case DISK_IMAGE_TYPE_STANDARD:
				pSectorPtr = StandardGetSectorDataPtr(pDiskImageData, position->CurrentPhysicalTrack, position->CurrentSectorID);
				break;

			default:
				break;
		}

		if (pSectorPtr==NULL)
		{
			int s;
			fprintf(stderr, "Sector data not found (%d/%02x)\n",position->CurrentPhysicalTrack, position->CurrentSectorID);
			for (s=0; s<SECTOR_SIZE; s++)
			{
				fputc(0x0ff, fh);
			}
		}
		else
		{
			fwrite(pSectorPtr, SECTOR_SIZE, 1, fh);
		}
		Remaining -= SECTOR_SIZE;

		UpdateDiskPosition(position);
	} 
	while ((Remaining >= SECTOR_SIZE) && (!IsDiskDone(position)));

	PadPage(fh, Remaining);
}

int main(int argc, const char **argv)
{
	int nResultCode = 1;
	DskMaxChunk = 32;
  UsesBasicCommand = 0;
	memset(BasicCommand, 0, sizeof(BasicCommand));
  UsesBinaryFile = 0;
UsesExtendedFormat = 0;

	if (ArgumentList_Execute(argc, argv, OptionTable, printf, NonOptionHandler)==OPTION_OK)
	{
		if ((NumFiles < 2) || (NumFiles > 2))
		{
			OutputDetails();
			if (NumFiles != 0)
			{
				nResultCode = 1;
			}
			else
			{
				nResultCode = 0;
			}
		}
		else
		{	
			nResultCode = 0;
      DataBlock DiskImageDataBlock;
      if (!ReadFile(Filenames[0], &DiskImageDataBlock))
      {
        fprintf(stderr, "Failed to read disk image '%s'\n", Filenames[0]);
        nResultCode = 1;
      }
      else
      {
        DiskImageType = DISK_IMAGE_TYPE_UNKNOWN;
        if (memcmp(DiskImageDataBlock.pData, "MV - CPC",8)==0)
        {
          DiskImageType = DISK_IMAGE_TYPE_STANDARD;

          fprintf(stdout,"Standard disk image detected\n");
        }
        else if (memcmp(DiskImageDataBlock.pData, "EXTENDED", 8) == 0)
        {
          DiskImageType = DISK_IMAGE_TYPE_EXTENDED;

          fprintf(stdout,"Extended disk image detected\n");
        }
        else 
        {
          fprintf(stderr,"Disk image not supported\n");
           nResultCode = 1;
        }
        
		if (DiskImageType != DISK_IMAGE_TYPE_UNKNOWN)
		{
			if (CheckDiskValid(&DiskImageDataBlock) == 0)
			{
				DiskPosition position;

				if (UsesExtendedFormat)
				{
					if ((SectorIDMin == MinID) && (SectorIDMax == MaxID))
					{
						fprintf(stdout, "Extended format detected\n");
					}
					else
					{
						fprintf(stderr, "Extended format not detected (found min ID: &%02x max ID: &%02x. Wanted min &%02x and max &%02x)\n", SectorIDMin, SectorIDMax, MinID, MaxID);
						nResultCode = 1;
					}
				}
				else
				{
					if (SectorIDMin == 0x041)
					{
						fprintf(stdout, "SYSTEM format disc detected\n");
					}
					else if (SectorIDMin == 0x0c1)
					{
						fprintf(stdout, "DATA format disc detected\n");
					}
					else
					{
						fprintf(stderr, "format not detected\n");
						nResultCode = 1;
					}
				}

				if (!UsesBasicCommand)
				{
					fprintf(stdout, "No BASIC command specified\n");
				}
				else
				{
					fprintf(stdout, "BASIC command: %s\n", BasicCommand);
				}

				if (nResultCode == 0)
				{
					position.CurrentPhysicalTrack = 0;
					position.CurrentSectorID = SectorIDMin;

					FILE *fhOut = fopen(Filenames[1], "wb");
					if (fhOut)
					{
						FILE *fhBinary = NULL;
						long BinaryFileLength = 0;

						int Chunk = 0;
						uint32_t CartLength =
							/* all chunks and the riff chunks for each */
							(NUM_CHUNKS*(CART_CHUNK_LENGTH + RIFF_CHUNK_HEADER_SIZE)) +
							/* main header*/
							RIFF_CHUNK_HEADER_SIZE +
							/* Ams! marker */
							sizeof(uint32_t);

						WriteTag(fhOut, RIFFTag);
						WriteChunkLengthLittleEndian(fhOut, CartLength - RIFF_CHUNK_HEADER_SIZE);
						WriteTag(fhOut, AmsTag);

						if (pBinaryFilename)
						{
							fhBinary = fopen(pBinaryFilename, "rb");
							if (fhBinary != NULL)
							{
								fseek(fhBinary, 0, SEEK_END);
								BinaryFileLength = ftell(fhBinary);
								fseek(fhBinary, 0, SEEK_SET);
								printf("Using Binary File '%s' length %d\n", pBinaryFilename, BinaryFileLength);
							}
						}



						for (Chunk = 0; Chunk < NUM_CHUNKS; Chunk++)
						{
							/* header */
							WriteCartTag(fhOut, Chunk);
							WriteChunkLengthLittleEndian(fhOut, CART_CHUNK_LENGTH);

							/* data */

							switch (Chunk)
							{
								/* OS ROM */
							case 0:
							{
								if (WriteROM(fhOut, "os.rom") != 0)
								{
									nResultCode = 1;
								}
							}
							break;

							/* BASIC ROM */
							case 1:
							{
								if (WriteROM(fhOut, "basic.rom") != 0)
								{
									nResultCode = 1;
								}
							}
							break;

							/* 464/6128 Plus has disc rom at page 3. GX4000 doesn't
							have disc page. We are therefore to change the location of the
							disc rom page */

							/* AMSDOS ROM */
							case 2:
							{
								size_t currentPosition;


								if (WriteROM(fhOut, "amsdos.rom") != 0)
								{
									nResultCode = 1;
								}


								currentPosition = ftell(fhOut);

								fseek(fhOut, currentPosition, SEEK_SET);
								fseek(fhOut, -16384 + FILENAME_PATCH_DOS_OFFSET - 1, SEEK_CUR);

								if (UsesBasicCommand)
								{
									/* marker to indicate basic command follows */
									fputc(0x0aa, fhOut);
								}
								else
								{
									/* marker indicates no basic command follows */
									fputc(0x00, fhOut);
								}
								fwrite(BasicCommand, sizeof(BasicCommand), 1, fhOut);

								/* write XPDB for extended format */
								if (UsesExtendedFormat)
								{
									fseek(fhOut, currentPosition, SEEK_SET);
									fseek(fhOut, -16384 + SYSTEM_XPDB_OFFSET, SEEK_CUR);
									fwrite(&SupportedFormats[ChosenExtendedFormat].XPDB, sizeof(XPDB), 1, fhOut);
								}

								/* write sector id to support data or system */
								fseek(fhOut, currentPosition, SEEK_SET);
								fseek(fhOut, -16384 + SECTOR_ID_PATCH_DOS_OFFSET, SEEK_CUR);
								if (UsesExtendedFormat)
								{
									/* extended format replaces system */
									fputc((uint8_t)0x041, fhOut);
								}
								else
								{
									fputc((uint8_t)SectorIDMin, fhOut);
								}
								fseek(fhOut, currentPosition, SEEK_SET);


							}
							break;

							default:
							{
								int BeforePosition = ftell(fhOut);
								int AfterPosition;
								int bIsDiskDone = IsDiskDone(&position);


								if (bIsDiskDone)
								{
									PadPage(fhOut, CART_CHUNK_LENGTH);
								}
								else
								{
									WriteSectorDataToCartridgePage(fhOut, DiskImageDataBlock.pData, &position);
								}
								AfterPosition = ftell(fhOut);

								if (Chunk >= BINARY_CART_PAGE)
								{
									if (fhBinary)
									{

										if (BinaryFileLength != 0)
										{
											if (!bIsDiskDone)
											{
												fprintf(stdout, "Warning binary file is overwriting disc data in chunk %d\n", Chunk);
											}
											fseek(fhOut, BeforePosition, SEEK_SET);

											{
												/* we have data to write, write it */
												int b = 0;
												long LengthToWrite = BinaryFileLength;
												if (LengthToWrite > CART_CHUNK_LENGTH)
												{
													LengthToWrite = CART_CHUNK_LENGTH;
												}
												for (b = 0; b < LengthToWrite; b++)
												{
													char ch = fgetc(fhBinary);
													fputc(ch, fhOut);
												}
												BinaryFileLength -= LengthToWrite;
												if ((BinaryFileLength == 0) && (LengthToWrite != 0))
												{
													/* pad to end of page if required */
													PadPage(fhOut, CART_CHUNK_LENGTH - LengthToWrite);
												}
												fseek(fhOut, AfterPosition, SEEK_SET);
											}
										}
									}
								}
							}
							break;

							}
						}

						// report if binary file is too long
						if (fhBinary && BinaryFileLength != 0)
						{
							fprintf(stderr, "Binary file is too large to write to cartridge. %d bytes remaining\n", BinaryFileLength);
							nResultCode = 1;
						}

						// report if disc image is too long
						if (!IsDiskDone(&position))
						{
							fprintf(stderr, "Disc image is too large to write to cartridge (finished at track %d sector &%02x\n", position.CurrentPhysicalTrack, position.CurrentSectorID);
							nResultCode = 1;
						}

						if (fhBinary)
						{
							fclose(fhBinary);
							fhBinary = NULL;
						}

						fclose(fhOut);

						fprintf(stdout, "Cartridge file written to '%s'\n", Filenames[1]);
					}
					else
					{
						fprintf(stderr, "Failed to open output file '%s'", Filenames[1]);
						nResultCode = 1;
					}
				}
			}
		}
      }
         if (DiskImageDataBlock.pData!=NULL)
    {
      free(DiskImageDataBlock.pData);
    }
    }
  }
   return nResultCode;
}
