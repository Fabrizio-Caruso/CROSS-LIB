#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef DJGPP
#    include <locale.h>
#endif

/* ugly hack to support French accents */
#ifdef DJGPP
static char ucirc[] = "–";
static char agrave[] = "…";
static char eacute[] = "‚";
static char ecirc[]  = "ˆ";
#else
static char ucirc[] = "Ã»";
static char agrave[] = "Ã ";
static char eacute[] = "Ã©";
static char ecirc[]  = "Ãª";
#endif

int tab[4]={0xff,0x01,0x03c,0x01};

FILE *fw,*fr;
int noct;
static int is_fr=0;


int mygetc()
{
    int a=fgetc(fr);

    if (a==EOF)
    {
        printf(is_fr?"erreur : fichier source incomplet\n":"error : incomplete source file\n");
        exit(EXIT_FAILURE);
    }

    return a&255;
}


int myputc(int a)
{
    fputc(a,fw);

    noct++;

    if (noct==0x1800)
    {
        fclose(fw);
        printf("ok\n");
        exit(EXIT_FAILURE);
    }

    return 0;
}


int main(int argc,char **argv)
{
    int i,step;
    int c,n;
    char buf[256];

#ifdef DJGPP
#ifdef FRENCH_LANGUAGE
    is_fr = 1;
#else
    is_fr = 0;
#endif
#else
    char *lang=getenv("LANG");
    if (lang==NULL) lang="fr_FR";        
    setlocale(LC_ALL, "fr_FR.UTF8");    
    if (strncmp(lang,"fr",2)==0) 
        is_fr=-1;
#endif

    if (is_fr)
    {
        printf("GetROM 1.0 par Sylvain HUET\n");
        printf("R%scuperation de la ROM du TO7 %s partir d'un fichier .k7\n",eacute,agrave);
        printf("La ROM a d%s %stre sauvegard%se par : SAVEM\"ROM\",&HE800,&HFFFF,0\n\n",ucirc,ecirc,eacute);
    }
    else
    {
        printf("GetROM 1.0 by Sylvain HUET\n");
        printf("Get the ROM of a TO7 from a k7 file\n");
        printf("The ROM must have been saved by : SAVEM\"ROM\",&HE800,&HFFFF,0\n\n");
    }

    if ((argc<2)
      ||((strcmp(".k7",&argv[1][strlen(argv[1])-3]))
          &&(strcmp(".K7",&argv[1][strlen(argv[1])-3]))))
    {
        printf(is_fr?"usage : getrom fichier.k7\n":"usage : getrom file.k7\n");
        exit(EXIT_FAILURE);
    }

    strcpy(buf,argv[1]);
    strcpy(&buf[strlen(buf)-3],".rom");

    if (is_fr)
        printf("ouverture de %s\n",argv[1]);
    else
        printf("open %s\n",argv[1]);

    if ((fr=fopen(argv[1],"rb"))==NULL)
    {
        printf(is_fr?"fichier introuvable\n":"file not found\n");
        exit(EXIT_FAILURE);
    }

    if (is_fr)
        printf("%scriture de %s\n",eacute,buf);
    else
        printf("write %s\n",buf);

    if ((fw=fopen(buf,"wb"))==NULL)
    {
        if (is_fr)
            printf("%scriture impossible\n",eacute);
        else
            printf("can't write\n");
        exit(EXIT_FAILURE);
    }

    noct=0;
    step=0;

    while (1)
    {
        c=mygetc();
        if (c==tab[step])
            step++;
        else if (c==tab[0])
            step=1;
        else step=0;

        if (step==4)
        {
            n=mygetc();
            if (noct==0)
            {
                for(i=0;i<5;i++) mygetc();
                for(i=0;i<n-5;i++) myputc(mygetc());
            }
            else
                for(i=0;i<n;i++) myputc(mygetc());

            step=0;
        }
    }

    exit(EXIT_SUCCESS);
}
