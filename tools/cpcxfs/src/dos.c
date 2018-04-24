
/*				<<<<Last Modified: Thu Feb 08 15:07:42 1996>>>>
------------------------------------------------------------------------------

	=====
	CPCfs  --  d o s . c   ---   Borland C specific routines
	=====

	Version 0.85                    (c) February '96 by Derik van Zuetphen
------------------------------------------------------------------------------

KT - WIN32 version uses DOS code, but with some changes to compile
with MSDev 

*/

#if defined(WIN32) || defined(WIN64)
#include <stdio.h>
#include <io.h>
#include <direct.h>
#include <conio.h>
#endif

#include <stdlib.h>
#include <string.h>
#if !defined(WIN32) && !defined(WIN64)
#include <dir.h>
#endif
#include <dos.h>	/* the one in includedir of course */

#include "cpcfs.h"

#if defined(WIN32) || defined(WIN64)
#define MAXPATH		_MAX_PATH
#define MAXDRIVE	_MAX_DRIVE
#define MAXDIR		_MAX_DIR
#define MAXFILE		_MAX_FNAME
#define MAXEXT		_MAX_EXT
#endif

/* KT - added check in case min is already defined */
#ifndef min
#define min(A,B)	((A)<=(B)?(A):(B))
#endif
extern char Break_Wish;

char	cwdbuffer[256];

char	*cl_string, *cm_string;	/* Clear Screen, Cursor Motion */


#if defined(WIN32) || defined(WIN64)

char	termcap_buffer[1024];
char	*term_entries;

const char	perr_str[] = "terminal";
#if 0
void	break_handler();

void disable_break() {
/*   ^^^^^^^^^^^^^ */
	signal (SIGINT,break_handler);		/* Ctrl-C */
	/*        signal (SIGQUIT,break_handler);*/	/* Ctrl-\, hard interrupt */
/*        signal (SIGTSTP,break_handler);*/	/* Ctrl-Z, suspend */
}


void break_handler () {
/*   ^^^^^^^^^^^^^ */
	Break_Wish = 1;
	disable_break();
}
#endif






void gotoxy(int col,int lin) {
/*   ^^^^^^ */
//	if (*cm_string!=0)
//		printf ("%s",tgoto (cm_string,col-1,lin-1));
} /*gotoxy*/


/* usefull, but not necessary:
#include <sys/filio.h>
bool kbhit () {
/ *   ^^^^^ * /
long	x;  / * holds # of pending chars * /
	fflush(stdout);
	return((ioctl(0,FIONREAD,&x) < 0) ? 0 : x);
} / *kbhit* /
*/


void clrscr() {
/*   ^^^^^^ */
	fflush(stdout);
#if !defined(WIN32)
	printf("%s",cl_string);
#endif
} /*clrscr*/

#endif

#if !defined(WIN32) && !defined(WIN64)
int break_handler() {
	Break_Wish = 1; /*TRUE*/;
	return 1;   /* 0 = exit(); 1 = continue program */
}

void disable_break() {
	/* not found under DJGPP! */
	/* ctrlbrk(break_handler); */
}
#endif

/* doesn't work */

/*struct mstats core_statistics;*/

long coreleft () {
/*   ^^^^^^^^ */
/*	core_statistics = mstats();
	return core_statistics.bytes_free;*/
	return 0;
}

/*****************
  Terminal Access
 *****************/

char wait_for_key (int must_be_0, char must_be_TRUE) {
/*   ^^^^^^^^^^^^ */
char	c;
	while (!kbhit()) ;
	if((c=getch())==0)
		return getch();
	else
		return c;
} /*wait_for_key*/

int	saved_drive;
char	saved_path[MAXPATH];

void save_path () {
/*   ^^^^^^^^^
Saves current drive and directory path */
#if defined(WIN32) || defined(WIN64)
	saved_drive = _getdrive();
#else
	saved_drive = getdisk();
#endif

#if defined(WIN32) || defined(WIN64)
	_getcwd(saved_path,  MAXPATH);
#else
	getcwd(saved_path,MAXPATH);
#endif
}

void rest_path () {
/*   ^^^^^^^^^
Restores saved drive and directory path */
#if defined(WIN32) || defined(WIN64)
	_chdrive(saved_drive);
	_chdir(saved_path);
#else
	setdisk(saved_drive);
	chdir(saved_path);
#endif
	
}


#if defined(WIN32) || defined(WIN64)
struct _finddata_t glob_buffer;
long	find_handle;
#else
struct ffblk glob_buffer;
#endif


char	glob_dir[MAXPATH];

char *glob_file (char *pattern, int *type) {
/*    ^^^^^^^^^ */
static char
	n[MAXPATH];

#if  !defined(WIN32) && !defined(WIN64)
int flags;
char	drive[MAXDRIVE];
char	dir[MAXDIR];
char	name[MAXFILE];
char	ext[MAXEXT];
#endif
char *find_name;
int is_dir = 0;
	*glob_dir=0;

#if  !defined(WIN32) && !defined(WIN64)
	flags=fnsplit(pattern,drive,dir,name,ext);
	if (DRIVE & flags) {
		strcpy(glob_dir,drive);
		strcat(glob_dir,":");
	}
	if (DIRECTORY & flags) {
		strcat(glob_dir,dir); 	/* included trailing "\" */
	}
#else
	/* KT - MSDev doesn't have fnsplit function */
	
	/* pattern defines full path and filename with extension */
	if (pattern!=NULL)
	{
		int i;
		int pos;

		/* searching from end of string find first directory seperator */
		pos = 0;

		for (i=strlen(pattern)-1; i>=0; i--)
		{
			if (pattern[i] == '\\')
			{
				pos = i+1;
				break;	
			}
		}

		/* copy string */
		strncpy(glob_dir, pattern, MAXPATH);
		glob_dir[MAXPATH-1] = '\0';
	
		/* if directory seperator is inside buffer, end string after seperator */
		if (pos<MAXPATH)
		{
			glob_dir[pos] = '\0';
		}
	
	}
#endif
	
#if defined(WIN32) || defined(WIN64)
	if (pattern == NULL)
		return NULL;

	find_handle = _findfirst(pattern, &glob_buffer);


	if (find_handle == -1)
	{
		return NULL;
	}
	
	find_name = glob_buffer.name;
	is_dir = glob_buffer.attrib & _A_SUBDIR;

#else
	if (findfirst(pattern,&glob_buffer,0)) {
		return NULL;
	}

	find_name = glob_buffer.ff_name;
	is_dir = 0;
#endif

	*type = is_dir;

	/* KT - changed strcpy to stop overwriting end of buffer */
	strncpy(n,glob_dir, MAXPATH);
	n[MAXPATH-1] = '\0';
	
	{
		int BytesRemaining;

		BytesRemaining = MAXPATH - strlen(n)-1;

		if (BytesRemaining!=0)
		{
#if defined(WIN32) || defined(WIN64)
			strncat(n,glob_buffer.name, BytesRemaining);
#else
            strncat(n,glob_buffer.ff_name, BytesRemaining);
#endif
            n[MAXPATH-1] = '\0';
		}
	}

	return n;
}


char *glob_next (int *type) {
/*    ^^^^^^^^^ */
static char
	n[MAXPATH];
char *find_name;
int is_dir;

#if defined(WIN32) || defined(WIN64)
	if (_findnext(find_handle, &glob_buffer)==-1)
	{
		_findclose(find_handle);

		return NULL;
	}

	find_name = glob_buffer.name;
	is_dir = glob_buffer.attrib & _A_SUBDIR;
#else
	if (findnext(&glob_buffer)) {
		return NULL;
	}

	find_name = glob_buffer.ff_name;
	is_dir = glob_buffer.ff_attrib & FA_DIREC;
#endif

	*type = is_dir;

	/* KT - changed strcpy to stop overwriting end of buffer */
	strncpy(n,glob_dir, MAXPATH);
	n[MAXPATH-1] = '\0';
	
	{
		int BytesRemaining;

		BytesRemaining = MAXPATH - strlen(n)-1;

		if (BytesRemaining!=0)
		{
#if defined(WIN32) || defined(WIN64)
			strncat(n,glob_buffer.name, BytesRemaining);
#else
            strncat(n,glob_buffer.ff_name, BytesRemaining);
#endif

            n[MAXPATH-1] = '\0';
		}
	}
	 
	return n;
}


char* tmp_nam(char* buf) {
/*    ^^^^^^^
Calls tmpnam() and prepends the value of the %TEMP environment variable.
Contrary to tmpnam(), <buf> must not be NULL! */
char	name[FULL_IMAGENAME_LENGTH];

/* KT: Under DJGPP, and Microsoft MSDEV, 
tmpnam generates a path and filename for the temporary file! */

#if 0
	temp = getenv("TEMP");
	if (temp==NULL)	strcpy(buf,".");
	else		strcpy(buf,temp);
	if (temp[strlen(temp)]!='\\') strcat(buf,"\\");
#endif
	/* this has a path prefix! */
	tmpnam(name);
	strcat(buf,name);
	return buf;
}


void os_init() {
/*   ^^^^^^^
Nothing to do for DOS */
}


/**********************************************************************
				History
 **********************************************************************/

#define MaxHistSize 100

int	hist_size = 0;  /* number of entries in history */
int	hist_last = 0;  /* number of last	ed entry */
char    *history[MaxHistSize];	/* filled with NULLS */

int add_history(char *line) {
/*  ^^^^^^^^^^^
Add <line> to the history list */
char	*str;

	str = (char*)Malloc(strlen(line)+1);
	strcpy(str,line);
	if (hist_size < MaxHistSize) {		/* append */
		history[hist_size] = str;
		hist_last = hist_size++;
	} else {       				/* overwrite eldest entry */
		hist_last = (hist_last+1) % MaxHistSize;
		free(history[hist_last]);
		history[hist_last] = str;
	}
	return 0;
}

#if  !defined(WIN32) && !defined(WIN64)
/**********************************************************************
	The next lines in this file are insertions from ACTlib 1.7
	(slightly modified)
	former name: KEY.H
 **********************************************************************/
/*
 *  Copyright (C) 1993   Marc Stern  (internet: stern@mble.philips.be)
 *
 * File         : key.h
 *
 * Description  : key code definitions.
 *
 */


#ifndef __Key_H
#define __Key_H


#define RETURN			0x0d
#ifndef ENTER
#define ENTER                   RETURN
#endif
#define SPACE			0x20
#define ESC			0x1b
#define BKSP			0x08
#define BACKSPACE		0x08

#define UP              	328
#define DOWN                    336
#define LEFT                    331
#define RIGHT                   333
#define PGUP		        329
#define PGDN	                337
#define HOME                    327
#define END  	       	        335
#define INSERT 	       	        338
#define INS                     INSERT
#define DELETE 	       	        339
#define DEL                     DELETE

enum { F1 = 315, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11 = 389, F12 };

#define CTRL_UP              	397
#define CTRL_DOWN               401
#define CTRL_LEFT               371
#define CTRL_RIGHT              372
#define CTRL_PGUP		388
#define CTRL_PGDN	        374
#define CTRL_HOME               375
#define CTRL_END  	       	373
#define CTRL_INSERT             402
#define CTRL_INS                CTRL_INSERT
#define CTRL_DELETE             403
#define CTRL_DEL                CTRL_DELETE

enum { CTRL_F1 = 350, CTRL_F2, CTRL_F3, CTRL_F4, CTRL_F5,
       CTRL_F6, CTRL_F7, CTRL_F8, CTRL_F9, CTRL_F10,
       CTRL_F11 = 393, CTRL_F12
     };

#define CTRL_A                  1
#define CTRL_B                  2
#define CTRL_C                  3
#define CTRL_D                  4
#define CTRL_E                  5
#define CTRL_F                  6
#define CTRL_G                  7
#define CTRL_H                  8
#define CTRL_I                  9
#define CTRL_J                  10
#define CTRL_K                  11
#define CTRL_L                  12
#define CTRL_M                  13
#define CTRL_N                  14
#define CTRL_O                  15
#define CTRL_P                  16
#define CTRL_Q                  17
#define CTRL_R                  18
#define CTRL_S                  19
#define CTRL_T                  20
#define CTRL_U                  21
#define CTRL_V                  22
#define CTRL_W                  23
#define CTRL_X                  24
#define CTRL_Y                  25
#define CTRL_Z                  26

#define ALT_UP              	408
#define ALT_DOWN                416
#define ALT_LEFT                411
#define ALT_RIGHT               413
#define ALT_PGUP	        409
#define ALT_PGDN	        417
#define ALT_HOME                407
#define ALT_END                 415
#define ALT_INSERT              418
#define ALT_INS                 ALT_INSERT
#define ALT_DELETE              419
#define ALT_DEL                 ALT_DELETE

enum { ALT_F1 = 360, ALT_F2, ALT_F3, ALT_F4, ALT_F5,
       ALT_F6, ALT_F7, ALT_F8, ALT_F9, ALT_F10,
       ALT_F11 = 395, ALT_F12
     };

#define ALT_A                   286
#define ALT_B                   304
#define ALT_C                   302
#define ALT_D                   288
#define ALT_E                   274
#define ALT_F                   289
#define ALT_G                   290
#define ALT_H                   291
#define ALT_I                   279
#define ALT_J                   292
#define ALT_K                   293
#define ALT_L                   294
#define ALT_M                   306
#define ALT_N                   305
#define ALT_O                   280
#define ALT_P                   281
#define ALT_Q                   272
#define ALT_R                   275
#define ALT_S                   287
#define ALT_T                   276
#define ALT_U                   278
#define ALT_V                   303
#define ALT_W                   273
#define ALT_X                   301
#define ALT_Y                   277
#define ALT_Z                   300

#define SHIFT_UP              	328
#define SHIFT_DOWN              336
#define SHIFT_LEFT              331
#define SHIFT_RIGHT             333
#define SHIFT_PGUP	        329
#define SHIFT_PGDN	        337
#define SHIFT_HOME              327
#define SHIFT_END               335
#define SHIFT_INSERT            338
#define SHIFT_INS               SHIFT_INSERT
#define SHIFT_DELETE 	       	339
#define SHIFT_DEL               SHIFT_DELETE

enum { SHIFT_F1 = 340, SHIFT_F2, SHIFT_F3, SHIFT_F4, SHIFT_F5,
       SHIFT_F6, SHIFT_F7, SHIFT_F8, SHIFT_F9, SHIFT_F10,
       SHIFT_F11 = 391, SHIFT_F12
     };

#endif


/**********************************************************************
	former name: INPUTS.C (history access added)
 **********************************************************************/

/*  Copyright (C) 1993   Marc Stern  (internet: stern@mble.philips.be)   */

#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define True  	1
#define False	0

int insert_mode = True;


/***
 *  Function    :   inputs
 *
 *  Description :   Input a string from keyboard
 *
 *  Parameters  :   in/out   char * data        default and result
 *                  in       int    maxLen      maximum length to accept
 *                  in       int    timeout     maximum time (seconds) waiting
 *                                              before a key is pressed
 *
 *
 *  Decisions   :   Valid keys:   Home         Begin of line
 *		                  End          End of line
 *                                Left/Right   One character left/right
 *				  Up/Down      Browse thru history
 *		                  Insert       Toggle insert on/off
 *                                Delete       Delete current  character
 *                                BackSpace    Delete previous character
 *		                  Ctrl-home    Erase to begin-of-line
 *		                  Ctrl-end     Erase to end-of-line
 *                                Escape       Erase whole input
 *                                Enter        Accept input
 *		    plus EMACS bindings
 *
 *                  Default string is erase if another input is given.
 *                  (if any non-editing character is hit).
 *
 *                  If no key is hit before timeout (in seconds)
 *                  the function returns.
 *                  timeout -1 means no timeout.
 *                  timeout  0 means check for type-ahead.
 *
 *  Return      :   length of input
 *                  -1 if time-out
 *                  -2 if Ctrl-Z
 *
 *  OS/Compiler :   MS-DOS & Turbo-C
 ***/

int inputs( char *data, int maxLen, int timeout )
{
 int Xpos = wherex(), Ypos = wherey(), len = strlen( data ), oldLen = 0;
 char *curPos = data + len;
 int firstkey = True, x, y, c;
 struct text_info ti;
 int hist = -1;			/* not in history yet */
 char hist_used = False;        /* flag to signify line redraw */

 gettextinfo( &ti );

 for (;;)
    {
     if ( hist_used || len != oldLen )
	{
	 hist_used = False;
	 gotoxy( Xpos, Ypos );
	 cputs( data );
	 for ( ; oldLen > len; oldLen-- ) cputs(" ");
	 if ( oldLen < len ) oldLen = len;
	 Ypos = min( Ypos, wherey() - (Xpos + len - 1) / ti.screenwidth );
	}

     if ( firstkey && (timeout >= 0) )
	{
	 time_t now = time( NULL );  /* Gets system time */

	 do if ( kbhit() ) timeout = -1;
	 while ( difftime(time(NULL), now) < timeout );

	 if ( timeout >= 0 ) return -1;
	}

 _setcursortype( insert_mode ? _NORMALCURSOR : _SOLIDCURSOR );

#ifndef _WIN32
#pragma warn -sig
#endif
     for ( x = curPos - data + Xpos, y = Ypos;
	   x > ti.screenwidth;
	   x -= ti.screenwidth, y++
	 );
     gotoxy( x, y );
#ifndef _WIN32
#pragma warn .sig
#endif

     switch( c = getkey() )
     {
      case LEFT:
      case CTRL_B:
	 if ( curPos > data ) curPos--;
	 break;

      case RIGHT:
      case CTRL_F:
	 if ( curPos < data + len ) curPos++;
	 break;

      case UP:
      case CTRL_P:
	 if (hist==-1) hist = hist_last;
	 else          {hist--; hist = (hist<0? hist_size-1 : hist);}
	 strcpy(data,history[hist]);
	 len = strlen(data);
	 curPos = data+len;
	 hist_used = True;
	 break;

     case DOWN:
     case CTRL_N:
	 if (hist==-1) break;
	 else          hist = (hist+1)%hist_size;
	 strcpy(data,history[hist]);
	 len = strlen(data);
	 curPos = data+len;
	 hist_used = True;
	 break;

      case HOME:
      case CTRL_A:
	 curPos =  data;
	 break;

      case END:
      case CTRL_E:
	 curPos = data + len;
	 break;

      case BACKSPACE:
	 if ( curPos > data )
	    {
	     strcpy( curPos - 1, curPos );
	     curPos--;
	     len--;
	    }
	 break;

      case CTRL_D:
      case DEL:
	 if ( curPos < data + len )
	    {
	     strcpy( curPos , curPos + 1 );
	     len--;
	    }
	 break;

      case INSERT:
      case CTRL_V:
	 insert_mode = ! insert_mode;
	 _setcursortype( insert_mode ? _NORMALCURSOR : _SOLIDCURSOR );
	 continue; /* break; */

      case ESC :
      case CTRL_U:
	 *data = '\0';
	 curPos = data;
	 len = 0;
	 break;

      case CTRL_Z:
      case ENTER:
	 cputs( "\r\n" ); clreol();
	 _setcursortype( _NORMALCURSOR );
	 if (c==CTRL_Z) return -2;
		   else return len;

      case CTRL_END:
      case CTRL_K:
	 *curPos = '\0';
	 len = strlen( data );
	 break;

      case CTRL_HOME:
	 strcpy( data , curPos );
	 curPos = data;
	 len = strlen( data );
	 break;

      default:
	 if ( c > 255 )
	    {
	     break;
	    }

         if ( firstkey )
            {
             *data = '\0';
             curPos = data;
             len = 0;
             ungetch( c );
             break;
            }

         if ( ! insert_mode )
            {
             cprintf( "%c", c );
             *curPos++ = c;
             if ( curPos >= data + len )
                {
                 *curPos = '\0';
                 len++;
                }
             break;
            }

         if ( len == maxLen )
            {
	     break;
            }

         memmove( curPos + 1, curPos, strlen(curPos) + 1 );
         *curPos++ = c;
         len++;
         break;
      }

      firstkey = False;
    }
}

#endif

/**********************************************************************
	former name: GETKEY.C
 **********************************************************************/
 
/*  Copyright (C) 1993   Marc Stern  (internet: stern@mble.philips.be)  */

#include <conio.h>

/***
 *
 * Function   : getkey
 *
 * Description: return a 2-bytes key pressed
 *              (extended characters are added to 256).
 *
 */

int getkey( void )

{ int car;

 if ( ! (car = getch()) ) car = 256 + getch();

 return car;
}

/* DOS DJGPP has getopt defined in unistd.h */
#if defined(WIN32) || defined(WIN64)
/**********************************************************************
	The next lines of this file are insertions from Borland C++ 2.0
	(slightly modified)
	former name: GETOPT.C
 **********************************************************************/

/*
	Copyright (c) 1986,1991 by Borland International Inc.
	All Rights Reserved.
*/

#include <errno.h>
#include <string.h>
#include <dos.h>
#include <stdio.h>

int	optind	= 1;	/* index of which argument is next	*/
char   *optarg;		/* pointer to argument of current option */
int	opterr	= 1;	/* allow error message	*/

static	char   *letP = NULL;	/* remember next option char's location */
static	char	SW = '-';	/* DOS switch character, either '-' or '/' */

/*
  Parse the command line options, System V style.

  Standard option syntax is:

    option ::= SW [optLetter]* [argLetter space* argument]

  where
    - SW is '-'
    - there is no space before any optLetter or argLetter.
    - opt/arg letters are alphabetic, not punctuation characters.
    - optLetters, if present, must be matched in optionS.
    - argLetters, if present, are found in optionS followed by ':'.
    - argument is any white-space delimited string.  Note that it
      can include the SW character.
    - upper and lower case letters are distinct.

  There may be multiple option clusters on a command line, each
  beginning with a SW, but all must appear before any non-option
  arguments (arguments not introduced by SW).  Opt/arg letters may
  be repeated: it is up to the caller to decide if that is an error.

  The character SW appearing alone as the last argument is an error.
  The lead-in sequence SWSW ("--" or "//") causes itself and all the
  rest of the line to be ignored (allowing non-options which begin
  with the switch char).

  The string *optionS allows valid opt/arg letters to be recognized.
  argLetters are followed with ':'.  Getopt () returns the value of
  the option character found, or EOF if no more options are in the
  command line.	 If option is an argLetter then the global optarg is
  set to point to the argument string (having skipped any white-space).

  The global optind is initially 1 and is always left as the index
  of the next argument of argv[] which getopt has not taken.  Note
  that if "--" or "//" are used then optind is stepped to the next
  argument before getopt() returns EOF.

  If an error occurs, that is an SW char precedes an unknown letter,
  then getopt() will return a '?' character and normally prints an
  error message via perror().  If the global variable opterr is set
  to false (zero) before calling getopt() then the error message is
  not printed.

  For example, if the MSDOS switch char is '/' (the MSDOS norm) and

    *optionS == "A:F:PuU:wXZ:"

  then 'P', 'u', 'w', and 'X' are option letters and 'F', 'U', 'Z'
  are followed by arguments.  A valid command line may be:

    aCommand  /uPFPi /X /A L someFile

  where:
    - 'u' and 'P' will be returned as isolated option letters.
    - 'F' will return with "Pi" as its argument string.
    - 'X' is an isolated option.
    - 'A' will return with "L" as its argument.
    - "someFile" is not an option, and terminates getOpt.  The
      caller may collect remaining arguments using argv pointers.
*/

int	getopt(int argc, char *argv[], char *optionS)
{
	unsigned char ch;
	char *optP;

	if (argc > optind) {
		if (letP == NULL) {
			if ((letP = argv[optind]) == NULL ||
				*(letP++) != SW)  goto gopEOF;
			if (*letP == SW) {
				optind++;  goto gopEOF;
			}
		}
		if (0 == (ch = *(letP++))) {
			optind++;  goto gopEOF;
		}
		if (':' == ch  ||  (optP = strchr(optionS, ch)) == NULL)  
			goto gopError;
		if (':' == *(++optP)) {
			optind++;
			if (0 == *letP) {
				if (argc <= optind)  goto  gopError;
				letP = argv[optind++];
			}
			optarg = letP;
			letP = NULL;
		} else {
			if (0 == *letP) {
				optind++;
				letP = NULL;
			}
			optarg = NULL;
		}
		return ch;
	}
gopEOF:
	optarg = letP = NULL;  
	return EOF;
 
gopError:
	optarg = NULL;
	errno  = EINVAL;
	if (opterr)
		perror ("get command line option");
	return ('?');
}

#endif