
/*				<<<<Last Modified: Thu Feb 08 15:08:32 1996>>>>
------------------------------------------------------------------------------

	=====
	CPCfs  --  t o o l s . c  --  Auxiliary functions
	=====

	Version 0.85                    (c) February '96 by Derik van Zuetphen
------------------------------------------------------------------------------
*/


#ifdef DOS
#if  !defined(WIN32) && !defined(WIN64)
#include <ctype.h>
#endif
#endif


#include "cpcfs.h"


#if defined(WIN32) || defined(WIN64)
#include <direct.h>
#include <ctype.h>
#endif

#ifdef DOS
extern long coreleft();
#endif

#ifdef LINUX
#include <errno.h>
#include <ctype.h>
#endif


/*************
  Auxiliaries
 *************/

void printm(int verb, char *fmt, ...)  {
/*   ^^^^^^ */
va_list	args;
const char	highlight[] = "\x1B[0;31;1m";	/* light red */
const char	unhighlight[] = "\x1B[0m";

	va_start (args, fmt);
	if (Verb >= verb) {
		if (verb>9) printf("%s",highlight);
		vprintf(fmt,args);
		if (verb>9) {
			printf("%s",unhighlight);
			fflush(stdout);
		}
	}
	va_end(args);
}

void putcharm(int verb, char ch)  {
/*   ^^^^^^^^ */
	if (Verb >= verb) putchar(ch);
}
 
 

 
char *lower(char *s)  {
/*    ^^^^^ */
char	*p=s;
	while (*s) {
		*s=tolower(*s);
		s++;
	}
	return p;
}

char *upper(char *s)  {
/*    ^^^^^ */
char	*p=s;

	while (*s) 
	{
		*s = toupper(*s);
		s++;
	}


	return p;
}

 
char *append_suffix (char *name, char *suffix) {
/*    ^^^^^^^^^^^^^
Appends <suffix> to <name>, but not if the name already ends with <suffix>.
<name> must point to sufficient space. Returns <name> */
char	*p, *ds, *dot;

	p=name;
	ds=dot=NULL;
	while (*p) {
		if (*p==DIRSEPARATOR)	ds=p;
		if (*p=='.')		dot=p;			
		p++;
	}
	if ((dot==NULL)||(dot<ds)) {
		strcat(name,".");
		strcat(name,suffix);
	}
	return name;
}
 
 
int errorf (bool perr,const char *fmt, ...) {
/*  ^^^^^^
Writes a formatted errormessage to stderr and appends a system errormessage
if <perr> set. The message in <fmt> should not end with \n.
Always returns -1. */

va_list	args;
	if (Verb<1) return -1;

	va_start(args,fmt);
	vfprintf(stderr,fmt,args);
	va_end(args);
	if (perr) {
#if UNIX	
		fprintf(stderr,": ");
#endif
/* DOS put always a colon before the error message, UNIX only if the argument
to perror is not empty */
		fputc(10,stderr);
		perror("");
	} else
	{
		fputc(10,stderr);
	}
	return -1;
}

/* attribute names for parsing and writing attributes */

const char	*attr_name[3][11] = {
	{"A=ON", "SYS", "R/O", "F8=ON", "F7=ON", "F6=ON", "F5=ON",   /* set */
			       "F4=ON", "F3=ON", "F2=ON", "F1=ON" },
	{"A=OFF","DIR", "R/W", "F8=OFF","F7=OFF","F6=OFF","F5=OFF",  /* reset */
			       "F4=OFF","F3=OFF","F2=OFF","F1=OFF" },
	{"A", "SYS", "R/O", "8", "7", "6", "5", "4", "3", "2", "1" } /* short */
};

const char	*attr_empty[] = {"-", "   ", " "};


const char *show_attr(int mask, int attr, bool always) {
/*          ^^^^^^^^^
Answer a representation for attribute <attr>, if it appears in <mask>,
If <always> is set, answer also a string for zero-attributes */

	if (mask & attr) {	/* attribute set */
		switch (attr) {
		case ATTR_A: return attr_name[2][0];
		case ATTR_S: return attr_name[2][1];
		case ATTR_R: return attr_name[2][2];
		case ATTR_8: return attr_name[2][3];
		case ATTR_7: return attr_name[2][4];
		case ATTR_6: return attr_name[2][5];
		case ATTR_5: return attr_name[2][6];
		case ATTR_4: return attr_name[2][7];
		case ATTR_3: return attr_name[2][8];
		case ATTR_2: return attr_name[2][9];
		case ATTR_1: return attr_name[2][10];
		}
	} else if (always) /* attribute not set and output required */
		switch (attr) {
		case ATTR_S: return attr_name[1][1];
		case ATTR_R: return attr_name[1][2];
		default:     return attr_empty[0];
		}
	else {	/* only spaces, attribute not set and no output */
		switch (attr) {
		case ATTR_R: return attr_empty[1];
		case ATTR_S: return attr_empty[1];
		default:     return attr_empty[2];
		}
	}
	errorf(FALSE,"--==>>> show_attr");
	return NULL;
}


const char* show_all_attr(int att, bool always) {
/*          ^^^^^^^^^^^^^
Show all attribues like "R/O SYS A 1234 5678" or "R/W DIR - ---- ----"
*/

static char buf[25];
	sprintf(buf,"%s %s %s %s%s%s%s %s%s%s%s",
		show_attr(att,ATTR_R,always),
		show_attr(att,ATTR_S,always),
		show_attr(att,ATTR_A,always),
		show_attr(att,ATTR_1,always),
		show_attr(att,ATTR_2,always),
		show_attr(att,ATTR_3,always),
		show_attr(att,ATTR_4,always),
		show_attr(att,ATTR_5,always),
		show_attr(att,ATTR_6,always),
		show_attr(att,ATTR_7,always),
		show_attr(att,ATTR_8,always));
	return buf;
}


int parse_attr(char *str, int *mask, bool *set) {
/*  ^^^^^^^^^^
Parse the given string <str> as a attribute name. Set the <*mask> to the
recognized attribut, and <set> whether the attribut is requested to set.
Answer -1, if <str> is not a valid attribute name.
*/
int	i, j;
char	s[INPUTLEN];

	strcpy(s,str);
	upper(s);
	for (i=0;i<2;i++) {	/* set, reset */
		for (j=0;j<11;j++) {	/* all attributs */
			if (strcmp(s,attr_name[i][j])==0) {
				*set = (i==0);
				*mask = 0x1 << j;
				return 0;
			}
		}
	}

	return -1;
}




void do_break() {
/*   ^^^^^^^^
Jump to interactive mode or exit */
	printm(3," [Aborted]\n");
	if (Interactive)
		longjmp(break_entry,1);
	else
		exit(2);
}


void *Malloc(int bytes) {
/*    ^^^^^^
Allocate memory. If an error ocurrs, jump to input loop */
void	*p;

	p = malloc(bytes);
	if (p==NULL) {
		errorf(TRUE,"Malloc");
		abandonimage();
		do_break();
		return NULL;
	}
	return p;
}


char	valid_keys[4];	/* can be c, q, and/or r */

void newpage(char *keys) {
/*   ^^^^^^^
Set up a new paging cycle. <keys> contains a list of characters that may be
pressed. */
	strcpy(valid_keys,keys);
	lineno=1;
}


char nextline() {
/*   ^^^^^^^^
Anvance the line counter of the internal pager. Stop at a prompt, if the
screen is full. Return the pressed character or '\0' */
char	answer;
int	i;

	if (Break_Wish) do_break();
	if ((lineno==0) || (pagelen==0) || (Verb < 1)) return 0;
	lineno++;
	if (Interactive == TRUE)
	{
		if (lineno+1>pagelen) {
			printm(1,"-- M o r e --   ");
			if (*valid_keys!=0) putcharm(1,'(');
			for (i=0; i<(int)strlen(valid_keys);i++) {
				if (i>0) printm(1,", ");
				switch (valid_keys[i]) {
				case 'c': printm(1,"`c' = continous");	break;
				case 'q': printm(1,"`q' = quit");	break;
				case 'r': printm(1,"`r' = restart");	break;
				default:  errorf(FALSE,"--==>>> nextline");
				}
			}
			if (*valid_keys!=0) putcharm(1,')');

			answer = tolower(wait_for_key(0,TRUE));
			putcharm(1,10);
			if (Break_Wish) do_break();
			if (answer=='c' && strchr(valid_keys,'c')!=NULL) {
				lineno=-1;
				return 'c';
			}
			lineno=1;
			if (strchr(valid_keys,answer)!=NULL) return answer;
		}
	}
	return 0;
}


char *repstr(char c, int times) {
/*    ^^^^^^
Form a string out of <times> <c>.
There must only be one repstr in a printf statement!!!
*/
int	i;
static	char str[INPUTLEN];
	for (i=0;i<times;i++) {
		if (i==INPUTLEN) break;
		str[i]=c;
	}
	str[i]=0;
	return str;
}


const char *show_format (DPB_list_entry *entry) {
/*    ^^^^^^^^^^^ */
static char format[256];

	format[0] = '\0';

	if (entry!=NULL)
	{
		if (entry->ident!=NULL)
		{
			strcat(format, entry->ident);
		}

		strcat(format, " (");

		if (entry->description!=NULL)
		{
			strcat(format, entry->description);
		}
		
		strcat(format, ")");
		return format;
	}
	
	return "Unknown Format";
}

char *show_mode (int m)  {
/*    ^^^^^^^^^ */
static char result[10];
	switch (m) {
	case M_BIN:  strcpy(result,"Binary"); break;
	case M_TEXT: strcpy(result,"Text"); break;
	case M_AUTO: strcpy(result,"Auto"); break;
	}
	return result;
}



void reparse (int argnb) {
/*   ^^^^^^^
transform the parsed command line back to one string containing all
arguments from "argnb" to last argument.*/
int	i;
char	*p;
	for (i=argnb; i<nbof_args; i++) {
		p = arg[i];
		while (*p) p++;
		*p = ' ';	/* replace terminating 0 with space */
	}
}



void expand_percent(char *from, char *to, int max) {
/*   ^^^^^^^^^^^^^^
WARNING: this function needs sprintf to return int, and not char* ! */
bool	active = *disk_header.tag;
char	buf[256];
char	*t, *t0;

	t0 = t = Malloc(max+80);	/* 80 (>len(%V)) as overflow area */

	while (*from)  {
		if (*from!='%') *t++=*from;
		else  {
			from++;
			switch (*from) {
			case 'u':
			case 'U': if (active)
					t+=sprintf(t,"%d",cur_user);
				  else
					*t++='#';
				  break;
			case 'i': if (active)
					t+=sprintf(t,"%s",imagename);
				  else
					*t++='#';
				  break;
			case 'I': if (active)
					t+=sprintf(t,"%s",full_imagename);
				  else
					*t++='#';
				  break;
			case 'f': if (active)
					t+=sprintf(t,"%5.1f",100.00-percentage);
				  else
					*t++='#';
				  break;
			case 'F': if (active)
					t+=sprintf(t,"%ld",
						(long)free_blks*dpb->BLS);
				  else
					*t++='#';
				  break;
			case 'a': if (active)
					t+=sprintf(t,"%5.1f",percentage);
				  else
					*t++='#';
				  break;
			case 'A': if (active)
					t+=sprintf(t,"%ld",
						(long)allocated_blks*1024);
				  else
					*t++='#';
				  break;
			case 'c':
#if defined(WIN32) || defined(WIN64)
            case 'C': t+=sprintf(t,"%s",_getcwd(buf,sizeof(buf)));
#else
			case 'C': t+=sprintf(t,"%s",getcwd(buf,sizeof(buf)));
#endif
				break;
			case 'v': if (PATCHLEVEL==0)
					t+=sprintf(t,"%d.%d",
						MAJORVERSION,MINORVERSION);
				  else
					t+=sprintf(t,"%d.%dpl%d",
					MAJORVERSION,MINORVERSION,PATCHLEVEL);
				  break;
			case 'V': t+=sprintf(t,"%d.%dpl%d (%s)",
				MAJORVERSION,MINORVERSION,PATCHLEVEL,stamp);
				  break;
			case '%': *t++='%'; break;
			case '_': *t++=' '; break;
			case '#': *t++='#'; break;
			case 'e':
			case 'E': *t++=27; break;
			case 'q':
			case 'Q': *t++='"'; break;
			case 's':
			case 'S': *t++=';'; break;
/* KT - added an extra item to show copyright */
			case 'W':
				t+=sprintf(t, "%s", copyright);
				break;
			case 'M':
#if DOS 
#if defined(WIN32) || defined(WIN64)
				t+=sprintf(t,"%lu",coreleft()); break;
					/* coreleft is def'ed in unix.c */
					/* coreleft is def'ed in dos.c for WIN32 */
#else
				t+=sprintf(t,"%lu",coreleft()); break;
					/* "%lu" if model > medium */
#endif
#else
				t+=sprintf(t,"%lu",coreleft()); break;
					/* coreleft is def'ed in unix.c */
#endif
			default : *t++='%'; *t++=*from; break;
			}
		}
		if (*from) from++;

/* check on string overflow */
		if (t>t0+max) break;
	}
	*t = 0;

	strncpy(to,t0,max-1);
	to[max-1] = 0;	/* if t0 is _exactly_ of length max-1 */
	free(t0);
}


void echom (int v, char *p) {
/*   ^^^^^ */

char	buf[256]; /* 3 lines should be enough */
	if (Verb >= v) {
		expand_percent(p,buf,256);
		printf("%s",buf);
	}
}


bool confirmed()  {
/*   ^^^^^^^^^ */
char	answer;
	printm(1,"[y/N] _");
	if (force||(Verb<1)) {
		printm(3," [Forced]\n");
		return TRUE;
	} else {
		answer = wait_for_key(0,TRUE);
		putcharm(1,10);
		if (Break_Wish) return FALSE;
		return (tolower(answer) == 'y');
	}
}


int parse_cpm_filename (char *name, int *user, char *root, char *ext) {
/*  ^^^^^^^^^^^^^^^^^^
Split the CP/M conformant <name> "UU:RRRRRRRR.EEE" into its ASCIIZ parts.
<user> will be -1, if no UU: found; -2, if UU: is *:
<ext> will be "", if no .EEE found.
<root> = "" is allowed and signals a missing filename (e.g. dir 1:).
Filenames of kind ".xxx" are not possible.
The <root> or <ext> parts can be longer than 8 or 3 due to "[]" wildcards!
Errorcode is 1 on error.
*/
char	*p, *q, *r;		/* temp pointer */
int	i;

	*user = -1;
	*root = 0;
	*ext = 0;

/* scan usernumber */
	p = strchr(name,':');
	if (p!=NULL) {
		if ((name[0]=='*') && (name[1]==':')) {
			*user = -2;
			name += 2;
		} else {
			*user = (int)strtoul(name,&q,0);
			if ((*user<0) || (*user>255) || (*q!=':')
							|| (errno==ERANGE))
				return 1;
			name = p+1;
		}
	}

/* scan root part */
	p = name;
	i = 0;
	r = root;
	while ((*p!='.') && *p && i<8) {
		*r++ = *p++;
		i++;
	}
	*r=0;
	q=strchr(p,'.');
	if (q) p=q;

/* scan extension part */
	if (*p) strcpy(ext,p+1);

	
/* convert to upper case */
	upper(root);
	upper(ext);

	if (*root==0 && *ext!=0) return 1;
	return 0;
}



int parse_filename (char *name, int *drive, char *path, char *root, char *ext) {
/*  ^^^^^^^^^^^^^^
Split the DOS conformant <name> "D:\PP\..\PP\RRRRRRRR.EEE" into its parts.
<drive> will be 0, if no D: was found, and 1 for A:, 2 for B:, and so on;
<path> contains a trailing backslash (i.e. at least "\").
On UNIX systems <root> may contain dots and <ext> is always empty.
Errorcode is 1 on error
*/
char	*p;	/* temp pointer */
#if DOS
char	*r, *q;
int	i;
#endif

	*drive = 0;
	*path = 0;
	*root = 0;
	*ext = 0;

#if DOS
/* scan drive */
	if ((*name!=0) && (name[1]==':')) {
		*drive = tolower(*name)-'a'+1;
		if ((*drive<1) || (*drive>26)) return 1;
		name += 2;
	}
#endif

/* scan path */
	p = strrchr(name,DIRSEPARATOR);
	if (p!=NULL) {			/* else no "\" found, path is empty */
		strncpy(path,name,p-name+2);
		name = p+1;
	}

/* scan root part */
	if (*name == 0) return 1;

#if UNIX
	strcpy(root,name);
#else
	r = root;
	i = 0;
	while ((*name!='.') && *name && i<8) {
		if (*name==':') return 1;	/* possibly mislead drive */
		*r++ = *name++;
	}
	*r=0;
	if (p!=NULL)
	{
		q=strchr(p,'.');
		if (q) p=q;
	}

/* scan extension part */
	if (*name) strncpy(ext,name+1,4);

#endif


#if DOS
/* convert to upper case */
	upper(path);
	upper(root);
	upper(ext);
#endif

	return 0;
}


int internal_pager(char *filename) {
/*  ^^^^^^^^^^^^^^
Display the file with stops after each screen */
FILE	*file;
char	line[256];
char	key;

	file = fopen(filename,"r");
	if (file==NULL) return errorf(TRUE,"I can't display \"%s\"",filename);

	newpage("cqr");
	while (fgets(line,256,file)) {
		printm(0,"%s",line);
		key = nextline();
		if (key=='r') {
			fseek(file,0L,SEEK_SET);
		}
		if (key=='q') break;	
	}
	fclose(file);
	return 0;
}

int pager(char *filename) {
/*  ^^^^^
Display the file <filename> with the lister program from the environment
variable %PAGER or with the internal_pager()
*/
char	buf[INPUTLEN];
char	*pag;
int	err;

	pag = getenv("PAGER");
	if (pag==NULL)	{
		/* strcpy(buf,PAGERDEFAULT);*/
		internal_pager(filename);
		return 0;
	} else {
		strcpy(buf,pag);
	}
	strcat(buf," ");
	strcat(buf,filename);

	err = system(buf);
	if ((err==-1) || (err==127)) return -1;

	return 0;
}


void str2mem(char *mem, char *str, int spc) {
/*   ^^^^^^^
Copy the ASCIIZ string at <str> to position <mem> without trailing 0.
Fill the memory at <mem> with <spc> spaces.
*/
int	i;

	memset(mem,' ',spc);
	i=0;
	while (str[i]) {
		mem[i] = str[i];
		i++;
	}
}



void build_cpm_name(char *buf, int user, char *root, char *ext) {
/*   ^^^^^^^^^^^^^^
Construct a CP/M name out of the given stuff.
If <user> = -1, no usernumber is prepended,
if <user> = -2, a * (wild user) is prepended.
If <ext> = "", the last char is "."!
<buf> must point to memory of at least 3+1+8+1+3+1=17 Byte (UUU:RRRRRRRR.EEE0).
*/
	
	*buf = 0;
	if (user==-2) strcpy(buf,"*:");
	if (user>=0) sprintf(buf,"%d:",user);

	strcat(buf,root);
	strcat(buf,".");
	if (*ext) {
		strcat(buf,ext);
	}
}


void build_cpm_name_32(char *buf, int user, char *root, char *ext) {
/*   ^^^^^^^^^^^^^^^^^
The same function as <build_cpm_name> except, that <root> and <ext> are
considered as padded with ' ' (= ASCII 32) (as in <DirEntry>)
*/
int	j;

	*buf = 0;
	if (user==-2)	{strcpy(buf,"*:"); buf +=2;}
	if (user>=0)	buf += sprintf(buf,"%d:",user);

	memcpy(buf,root,8);
	j=7; while (buf[j] == ' ') {j--;}; j++;
	buf[j]='.'; j++;
	if (strncmp(ext,"   ",3)!=0) {
		memcpy(&buf[j],ext,3); j+=2;
		while (buf[j] == ' ') {j--;}; j++;
	}
	buf[j]=0;
}


bool has_wildcards(char os_tag, char *name) {
/*   ^^^^^^^^^^^^^
Checks whether <name> contains wildcard characters according to match.h
(if <os_tag> == 'c' ( *?[] )) or to DOS standards (if <os_tag> == 'd'
( *? )).
*/
char	wild[10];
	if (os_tag=='c')	strcpy(wild,"*?[]");	/* !^- ??? */
	else if (os_tag == 'd') strcpy(wild,"*?");
	else return errorf(FALSE,"--==>>> has_wildcards");

	while (*name) {
		if (strchr(wild,*name) != NULL) return TRUE;
		name++;
	}
	return FALSE;
}


char *show_hex(int nr, uchar *buf, int size) {
/*    ^^^^^^^^
Formats a hexdump line out out <size> bytes at <buf>. Prefix the line with
<nr> as address. This function uses a static variable as temporary buffer, so
don't call it twice in a function.
*/
static char line[INPUTLEN];
int	i;
char	*p;
uchar	c;

	p = line;
	p += sprintf(p,"%6X %c ",nr,vert);
	for (i=0;i<size;i++) {
		p += sprintf(p,"%2X ",buf[i]);
	}
	p += sprintf(p," %c ",vert);
	for (i=0;i<size;i++) {
		if (buf[i]<32)		c=' ';
		else if (buf[i]>=127)	c='~';
		else			c=buf[i];
		p += sprintf(p,"%c",c);
	}
/*	*(p++) = '\n';*/
	return line;
}	
