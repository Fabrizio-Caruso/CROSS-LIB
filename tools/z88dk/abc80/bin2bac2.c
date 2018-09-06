#include <stdio.h>
#include <stdlib.h>

unsigned char mem[16384];
unsigned char out[32768];
unsigned char block[512];
int offs=0,pos=0,left=0;

void writeblock(int len)
{
	int i;
	
	//printf("len=%d\n",len);
	for(i=0;i<253;i++)
	{
		if(i<len) out[offs++]=block[i];
		else out[offs++]=0;		
	}
	pos=0;
}


void main(int argc,char* argv[])
{
	FILE* fi;
	int i,nrBytes,addr,call_addr,len,r=0,mempos=0,pfxlen;
   unsigned char r1[20] = {0x83,0xc1,0xf1,0x5a,0x00,0xbb,0xc7,0x1e,0xfe,0xce,0x36,0xc7,0x1f,0xfe,0xce,0x36,0xce,0x34,0xf5,0xc7};
   unsigned char r1_2[4] = {0xf8,0xce,0x3a,0xb7};
	unsigned char bld_0[7] = {0xcb,0x22,0x1c,0x2a,0x1c,0xfe,0x01};
	unsigned char bld_1[2] = {0x09,0x11};
	unsigned char bld_2[6] = {0x06,0x00,0x3e,0xf8,0x86,0xd2};
	unsigned char bld_3[12] = {0x0e,0x06,0x09,0x4f,0xed,0xb0,0x23,0x23,0x18,0xf0,0xbb,0x0d};
	
	if(argc!=4) 
	{
		printf("BIN2BAC2 v0.1 (C) 2014 Andreas Gustafsson\n");
		printf("Usage: BIN2BAC2 <infile.bin> <outfile.bac> <address>\n");
		return;
	}
	if (!(fi=fopen(argv[1],"rb")))
   {
		printf("Loading of file %s failed.\n",argv[1]);	
   	return;
  	}
  	nrBytes=fread(mem,1,16384,fi);
  	fclose(fi);  	
  	addr=strtol(argv[3],NULL,0);
  	printf("Read %d bytes from %s\n",nrBytes,argv[1]);
  	printf("Writing a binary that loads at %d to %s\n",addr,argv[2]);  	
  	call_addr=addr;
  	pos=0;
  	block[pos++]=0x82;
  	block[pos++]=0x1e;
  	r++;
  	block[pos++]=r&255;
  	block[pos++]=r>>8;
   for(i=0;i<20;i++)
  	{
  		block[pos++]=r1[i];  			
  	}		
  	block[pos++]=30;
  	block[pos++]=0;
   for(i=0;i<4;i++)
  	{
  		block[pos++]=r1_2[i];  			
  	}		
  	block[pos++]=0x0d;
  	pfxlen=pos-1;
  	do
  	{
  		if(pos+8>252) writeblock(pos); 
  		left=252-pos;
	  	len=left-8;
  		if(len>nrBytes-mempos) len=nrBytes-mempos;
  		block[pos++]=len+8;
	  	r++;
  		//printf("line is now %d, outputting %d bytes\n",r,len);
	  	block[pos++]=r&255;
	  	block[pos++]=r>>8;
	  	block[pos++]=0xcb;
	  	block[pos++]=0x22;
	  	block[pos++]=len;
	  	for(i=0;i<len;i++)
	  	{
	  		block[pos++]=mem[mempos++];
	  	}
	  	block[pos++]=0xbb;
	  	block[pos++]=0x0d;
  	} while(mempos<nrBytes);
  	if(pos-6>252) writeblock(pos); 
	block[pos++]=6;
   r++;
   block[pos++]=r&255;
  	block[pos++]=r>>8;
  	block[pos++]=0x86;
  	block[pos++]=0x8a;
  	block[pos++]=0x0d;
  	if(pos-0x24>252) writeblock(pos); 
	block[pos++]=0x24;
   r++;
   block[pos++]=r&255;
  	block[pos++]=r>>8;
   for(i=0;i<7;i++)
  	{
  		block[pos++]=bld_0[i];  			
  	}		
   block[pos++]=pfxlen&255;
  	block[pos++]=pfxlen>>8;
   for(i=0;i<2;i++)
  	{
  		block[pos++]=bld_1[i];  			
  	}		
   block[pos++]=addr&255;
  	block[pos++]=addr>>8;
   for(i=0;i<6;i++)
  	{
  		block[pos++]=bld_2[i];  			
  	}		
   block[pos++]=call_addr&255;
  	block[pos++]=call_addr>>8;
   for(i=0;i<12;i++)
  	{
  		block[pos++]=bld_3[i];  			
  	}		
  	if(pos-2>252) writeblock(pos); 
   block[pos++]=1;
	writeblock(pos);   		
	if(!(fi=fopen(argv[2],"wb")))
   {
		printf("Writing of file %s failed.\n",argv[2]);	
   	return;
  	}
  	nrBytes=fwrite(out,1,offs,fi);
  	fclose(fi);
  	
}