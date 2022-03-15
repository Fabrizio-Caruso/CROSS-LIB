/*
 *
 *  FD2SAP conversion FD => SAP
 *
 */

#include <stdio.h>
#include <strings.h>


#define NBSIDE 4
#define NBTRACK 80
#define NBSECT 16
#define SECTSIZE 256

unsigned char disk[NBSIDE][NBTRACK][NBSECT][SECTSIZE];

#define SIDESIZE (NBTRACK*NBSECT*SECTSIZE)

int LoadFD(char * name)
{
   FILE * f;

   int sector,track,side;
   int nb_side,size;

   f=fopen(name,"rb");
   if (f==NULL) return -1;
   
   fseek(f, 0L, SEEK_END);
   size = ftell(f);
   fseek(f, 0L, SEEK_SET);

   if (size==SIDESIZE) {
   	for (track=0;track<NBTRACK;track++)
   	for (sector=0;sector<NBSECT;sector++)
   	{
       		fread(disk[0][track][sector],sizeof(char),256,f);
    	}
	return 1;
   } else 
   if (size==2*SIDESIZE) {
	for (side=0;side<2;side++)
   	for (track=0;track<NBTRACK;track++)
   	for (sector=0;sector<NBSECT;sector++)
   	{
       		fread(disk[side][track][sector],sizeof(char),256,f);
    	}
	return 2;
   }else
   if (size==4*SIDESIZE) {
	for (side=0;side<4;side++)
   	for (track=0;track<NBTRACK;track++)
   	for (sector=0;sector<NBSECT;sector++)
   	{
       		fread(disk[side][track][sector],sizeof(char),256,f);
    	}
	return 4;
   }

   fclose(f);
   return 0;
}

/*
  ecriture au format TDS
*/
void SaveTDS(char * name,int side)
{
   FILE * f;
   char descript[0xA0];

   int i,j;

   /* lecture du descripteur TDS */
   f=fopen(name,"wb");
   if (f==NULL) return;

   /* construction du header */
   for(i=0;i<0xA0;i++)
   {
     descript[i]=0;
     for(j=0;j<8;j++)
     {
       if (disk[side][20][1+(i*8)/SECTSIZE][(i*8)%SECTSIZE]!=0xFF)
       descript[i]|=(1<<j);
     }
   }
   fwrite(descript,sizeof(char),(size_t)0xA0,f);


   for (i=0;i<0xA0;i++)
   for (j=0;j<8;j++)
   {
     if (   ( descript[i]&(1<<j) )!=0   )
     {
       int SEC,TRK;
       SEC=(i%2)*8+j;
       TRK=i/2;

       fwrite(disk[side][TRK][SEC],sizeof(char),(size_t)SECTSIZE,f);
     }
   }

   fclose(f);
}

short crcpuk_temp;
static const short magic_puk[]={

   0x0000, 0x1081, 0x2102, 0x3183,
   0x4204, 0x5285, 0x6306, 0x7387,
   0x8408, 0x9489, 0xa50a, 0xb58b,
   0xc60c, 0xd68d, 0xe70e, 0xf78f

 };

char * puk_header="\x01SYSTEME D'ARCHIVAGE PUKALL S.A.P. (c) Alexandre PUKALL Avril 1998";

crc_pukall(short c)
 {
    register short puk;

    /* on repete le code pour aller plus vite. Pas de boucle ici */

    puk = (crcpuk_temp ^ c) & 0xf;
    crcpuk_temp = ((crcpuk_temp>>4) & 0xfff) ^ magic_puk[puk];

    c >>= 4;

    puk = (crcpuk_temp ^ c) & 0xf;
    crcpuk_temp = ((crcpuk_temp>>4) & 0xfff) ^ magic_puk[puk];
 }


void SaveSAP(char * name,int side)
{

	FILE *f;
	unsigned char format,protection,piste,secteur;

	char *buffer;
	int fois;
	format=0;
	protection=0;  /* variables de test */

	f=fopen(name,"wb");
	if (f==NULL) return;

/*
 *	Ecriture du header
 */

	fwrite(puk_header,strlen(puk_header),sizeof(char),f);

/*
 *	Ecriture des donnees
 */

	for (piste=0;piste<80;piste++)
	for (secteur=1;secteur<=16;secteur++)
	{

		crcpuk_temp = 0xffff; /* toujours remettre cette variable a 0xffff */
			  /* pour calculer un nouveau CRC */

	    	crc_pukall(format);   /* calcul du CRC d'un secteur */
    		crc_pukall(protection);
    		crc_pukall(piste);
    		crc_pukall(secteur);

		buffer=disk[side][piste][secteur-1];
    		for (fois=0;fois<=255;fois++) /* on admet que buffer contient les */
     		{                            /* donnees d'un secteur S.A.P. */
       			crc_pukall(buffer[fois]);
       			buffer[fois]=buffer[fois]^0xb3;
	     	}

		fputc(format & 0xFF,f);
		fputc(protection & 0xFF,f);
		fputc(piste & 0xFF,f);
		fputc(secteur & 0xFF,f);

		fwrite(buffer,256,sizeof(char),f);

		fputc((crcpuk_temp >> 8) & 0xFF,f);
		fputc(crcpuk_temp & 0xFF,f);
	
		
	}	

		

	fclose(f);
}


int main(int argc,char * argv[])
{
	char str[1024];
	char str2[1024];
	char str3[32];

	int i,nb_side;

	if (argc!=3)
	{
		fprintf(stderr,"syntaxe : %s <fichier fd source> <fichier SAP dest>\n",argv[0]);
		exit (-1);
   	}

   	nb_side=LoadFD(argv[1]);

	if (nb_side<=0) {
		fprintf(stderr,"Bad FD file");
		exit(-1);
	}

	strcpy(str,argv[2]);

	if (strlen(str)>4)
	if (strcmp(&str[strlen(str)-4],".sap")==0) str[strlen(str)-4]='\0';

	for (i=0;i<nb_side;i++) {
		strcpy(str2,str);
		strcat(str2,"_side");
		sprintf(str3,"%d",i);
		strcat(str2,str3);
		strcat(str2,".sap");	
   		SaveSAP(str2,i);
	}  
}
