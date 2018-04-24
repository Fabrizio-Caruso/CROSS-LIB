
/*				<<<<Last Modified: Thu Feb 08 15:07:55 1996>>>>
------------------------------------------------------------------------------

        =====
        CPCfs  --  d o s . h   ---   Borland C specific header
        =====

	Version 0.85                    (c) February '96 by Derik van Zuetphen
------------------------------------------------------------------------------
*/

#ifndef DOS_H_INCLUDED
#define DOS_H_INCLUDED


#if  !defined(WIN32) && !defined(WIN64)
#include <unistd.h>
#include <dir.h>
#endif

#include <string.h>
#include <fcntl.h>
#include <setjmp.h>
#include <errno.h>
#if USE_READLINE
#include <conio.h>
#endif

#define SHELLVAR	"COMSPEC"
#define SHELLDEFAULT	"c:\\command"
#define PAGERDEFAULT	"more"
#define LDIRCOMMAND	"dir"
#define DIRSEPARATOR	'\\'
#define FIRST_OPTIND	1
#ifndef F_OK
#define	F_OK		0		/* for access */
#endif
#ifndef R_OK
#define R_OK		4		/* dto. */
#endif
#ifndef ENTER
#define ENTER		13
#endif

extern char Break_Wish;

/* this getwd() is not exactly the same as Unix' one, but it works */
extern char	cwdbuffer[256];
#define getwd(DUMMY)		getcwd(cwdbuffer,256)

int break_handler();
void disable_break();

void save_path();
void rest_path();
char wait_for_key(int must_be_0, char must_be_TRUE);
void os_init();

char *glob_file(char *pattern, int *);
char *glob_next(int *);

int add_history(char*);

char* tmp_nam(char*);

/* prototypes of getopt.c */
extern int	optind;
extern char	*optarg;
extern int	opterr;

/* DOS DJGPP has getopt in unistd.h */
#if defined(WIN32) || defined(WIN64)
int getopt(int argc, char *argv[], char *optionS);
#endif


#endif
