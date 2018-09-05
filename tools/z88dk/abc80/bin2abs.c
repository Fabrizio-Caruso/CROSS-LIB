#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <inttypes.h>

unsigned char filebuf[65536];
unsigned char mem[65536];
unsigned char set[65536];

static void write_buffer(FILE *out, uint8_t *buf, int buf_len, int buf_address, int *spaceinrecord_ptr)
{
  int spaceinrecord = *spaceinrecord_ptr;
  struct abs_data_hdr {
    uint8_t type;		/* 0 for data */
    uint8_t data_bytes;
    uint8_t zero;		/* Reserved */
    uint8_t addrh;
    uint8_t addrhx;
    uint8_t addrl;
    uint8_t addrlx;
  } hdr;
  uint8_t csum;
  int i;

  memset(&hdr, 0, sizeof hdr);

  csum = 0;
  for ( i = 0 ; i < buf_len ; i++ )
    csum += buf[i];

  hdr.data_bytes = buf_len;
  hdr.addrh = buf_address >> 8;
  hdr.addrhx = ~hdr.addrh;
  hdr.addrl = buf_address;
  hdr.addrlx = ~hdr.addrl;

  fwrite(&hdr, 1, sizeof hdr, out);
  fwrite(buf, 1, buf_len, out);
  putc(csum, out);

  spaceinrecord -= (buf_len + sizeof hdr + 1);

  if ( spaceinrecord < sizeof hdr + 4 ) {
    putc(0xff, out);
    spaceinrecord--;
    putc(0x00, out);
    spaceinrecord--;

    while ( spaceinrecord-- )
      putc(0xee, out);

    spaceinrecord = 253;
  }

  *spaceinrecord_ptr = spaceinrecord;
}

static void writeabs(FILE *out, uint8_t *data, uint8_t *isset, uint16_t entry_point)
{
  int spaceinrecord = 253;
  int lastaddr      = -999;
  int addr;
  uint8_t buf[256];
  int buf_address   = 0;
  int buf_len       = 0;
  struct abs_entry_hdr {
    uint8_t type;		/* 0 for data */
    uint8_t data_bytes;		/* 0 for the entry point */
    uint8_t zero;		/* Reserved */
    uint8_t addrh;
    uint8_t addrhx;
    uint8_t addrl;
    uint8_t addrlx;
  } hdr;

  for ( addr = 0 ; addr < 65536 ; addr++ )
  {
    if ( isset[addr] )
    {
      if ( buf_len && (spaceinrecord < (buf_len+11) || addr != lastaddr+1) )
      {
			/* Write previous buffer head */
			write_buffer(out, buf, buf_len, buf_address, &spaceinrecord);
			buf_len = 0;
      }
      if ( !buf_len ) buf_address = addr;
      buf[buf_len++] = data[addr];
      lastaddr = addr;
    }
 }

 if ( buf_len ) write_buffer(out, buf, buf_len, buf_address, &spaceinrecord);

  /* Write entry point */
  write_buffer(out, NULL, 0, entry_point, &spaceinrecord);

  /* Pad to end of record */
  while ( spaceinrecord-- )
    putc(0xee, out);
}

void main(int argc,char* argv[])
{
	FILE* fi;
	int i,nrBytes,addr;

	for(i=0;i<65536;i++)
	{
		mem[i]=0;
		set[i]=0;
	}

	if(argc!=4)
	{
		printf("BIN2ABS v0.1 (C) 2018 Andreas Gustafsson\n");
		printf("Usage: BIN2ABS <infile.bin> <outfile.abs> <address>\n");
		return;
	}
	if (!(fi=fopen(argv[1],"rb")))
   {
		printf("Loading of file %s failed.\n",argv[1]);
   	return;
  	}
  	nrBytes=fread(filebuf,1,65536,fi);
  	fclose(fi);
  	addr=strtol(argv[3],NULL,0);
  	printf("Read %d bytes from %s\n",nrBytes,argv[1]);
  	printf("Writing a binary that loads at %d to %s\n",addr,argv[2]);
  	for(i=0;i<nrBytes;i++)
  	{
  		set[addr+i]=1;
  		mem[addr+i]=filebuf[i];
  	}
	if (!(fi=fopen(argv[2],"wb")))
   {
		printf("Saving of file %s failed.\n",argv[2]);
   	return;
  	}
  	writeabs(fi,mem,set,addr);
  	fclose(fi);
}