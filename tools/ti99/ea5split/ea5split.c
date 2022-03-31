#include <stdio.h>
#include <stdlib.h>

char header[128]=" TIFILES";
char empty_sector[256];

void write16(FILE* f,int v) {
	char c[2];
	c[0]=v>>8;
	c[1]=v&0xFF;
	fwrite(c,2,1,f);
}

void generate_header(int sectors,char eof_offset) {
	header[0]=7;
	int i;
	for(i=8;i<=128;i++)header[i]=0;
	header[8]=sectors/256;
	header[9]=sectors&0xFF;
	header[10]=1;
	header[12]=eof_offset;
}

void str_toupper(char* s) {
	int l=strlen(s);
	int i;
	for(i=0;i<l;i++)s[i]=toupper(s[i]);
}

int main(int argc,char** argv) {
	if(argc!=2) {
		printf("Usage:\n");
		printf("ea5split <ea5_file>\n");
		return(1);
	}

	FILE* f;
	f=fopen(argv[1],"rb");
	if(f==NULL) {
		printf("Cannot open \"%s\"\n",argv[1]);
		return(1);
	}
	fseek(f,0,SEEK_END);
	int l=ftell(f);
	fseek(f,0,SEEK_SET);
	
	unsigned char* data=malloc(l);
	fread(data,l,1,f);
	fclose(f);
	
	int load_address=data[4]*256+data[5];
	int end_address=load_address+l-6-1;
	if(end_address>0xFFFF || end_address>0x3FFF && end_address<0xA000) {
		printf("Program loaded outside expansion RAM\n");
		free(data);
		return(1);
	}
	
	char* ext=strchr(argv[1],'.');
	char* name=malloc(strlen(argv[1])+1);
	strcpy(name,argv[1]);
	if(ext!=NULL)name[ext-argv[1]]='\0';


	int files_number=(l-6)/(0x2000-6);
	if((l-6)%(0x2000-6))files_number++;
	str_toupper(name);
	int name_l=strlen(name);
	char c=name[name_l-1]+files_number-1;
	if(c>'Z' && c<'a' || c>'z') {
		name[name_l]='1';
		name[name_l+1]='\0';
	}
	
	char* filename=malloc(strlen(name));
	
	int i,l2,l3,offset,sect,i2;
	for(i=0;i<256;i++)empty_sector[i]=0;
	offset=6;
	for(i=0;i<files_number;i++) {
		strcpy(filename,name);
		
		l2=l-offset;
		if(l2>0x2000-6) {
			l2=0x2000-6;
			l3=0x2000;
		}
		else l3=l2+6;
		if(l3%256)sect=l3/256+1;
		else sect=l3/256;
		generate_header(sect,l3%256);
		
		f=fopen(filename,"wb");
		fwrite(header,128,1,f);
		if(i==files_number-1)write16(f,0x0000);
		else write16(f,0xFFFF);
		write16(f,l2);
		write16(f,load_address);
		fwrite(data+offset,l2,1,f);
		l3=(l2+6)%256;
		if(l3)fwrite(empty_sector,256-l3,1,f);
		fclose(f);
		
		load_address+=l2;
		offset+=l2;
		name[strlen(name)-1]++;
	}
	
	free(name);
	free(data);
	free(filename);

	return(0);
}
