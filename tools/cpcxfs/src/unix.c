/*				<<<<Last Modified: Mon Feb 12 17:31:21 1996>>>>
------------------------------------------------------------------------------

        =====
        CPCfs  --  u n i x . c   ---   Unix specific routines
        =====

	Version 0.85                    (c) February '96 by Derik van Zuetphen
------------------------------------------------------------------------------
*/


#include <signal.h>
#include <glob.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <termcap.h>
#include <termios.h>
#include <unistd.h>
#include "unix.h"
#include <memory.h>


char	termcap_buffer[1024];
char	*term_entries;
char	*cl_string, *cm_string;	/* Clear Screen, Cursor Motion */

const char	perr_str[] = "terminal";

void break_handler();

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



glob_t	glob_buffer;
int	current_glob;


char *glob_file (char *pattern, int *type) {
/*    ^^^^^^^^^ */
	current_glob=0;
	if (glob(pattern, 0, NULL, &glob_buffer)!=0) {
		return NULL;
	}
	return glob_buffer.gl_pathv[current_glob++];
}


char *glob_next (int *type) {
/*    ^^^^^^^^^ */

	if (current_glob<glob_buffer.gl_pathc) {
		return glob_buffer.gl_pathv[current_glob++];
	} else {
		globfree(&glob_buffer);
		return NULL;
	}
}


/* doesn't work */

/*struct mstats core_statistics;*/

long coreleft () {
/*   ^^^^^^^^ */
/*	core_statistics = mstats();
	return core_statistics.bytes_free;*/
	return 0;
}


char*	tmp_nam(char* buf) {	return tmpnam(buf);	}


int raw () {
/*  ^^^ */
struct termios tio;
	if (tcgetattr(0,&tio)) perror(perr_str);	/* 0 is stdin */
	tio.c_lflag &= ~(ICANON|ISIG);
	if (tcsetattr(0,TCSANOW,&tio)) perror(perr_str);
	return 0;
}

char wait_for_key (int dsec, char/*bool*/ intr) {
/*   ^^^^^^^^^^^^
dsec = 0	wait and answer the next char
dsec > 0	answer the next char or 0 after `dsec' 1/10 seconds
dsec < 0	wait `-dsec' 1/10 seconds and answer then 0
intr = TRUE	^C interrupts
constraints:	-MAXINT < d(ezi)sec < 256
Characters > 127 may cause trouble! (resetting the terminal??)
I am using the POSIX termios structure, SYSV needs termio, BSD needs sgtty 
*/

struct termios	tio, save;
char	answer=0;	/* default answer */
int	tty = 0;	/* stdin (must be console) */

/* open terminal and set timeout */
	if (tcgetattr(tty,&tio)) perror(perr_str); /* also test stdin */
	memcpy(&save,&tio,sizeof(tio));

	tio.c_lflag &= ~(ISIG | ICANON | ECHO);	/* raw mode */
	if (dsec == 0) {
		tio.c_cc[VMIN] = 1;		/* one character */
		tio.c_cc[VTIME] = 0;		/* and don't wait */
	} else if (dsec > 0) {
		tio.c_cc[VMIN]=0;		/* no inter-character delay */
		tio.c_cc[VTIME]=dsec;		/* wait for ... seconds */
	}
	if (intr) {
		tio.c_lflag |= ISIG;
		tio.c_cc[VQUIT]=3;		/* 3=^C invokes Quit signal */
	}
	if (tcsetattr(tty,TCSANOW,&tio)) perror(perr_str);

/* read a character */
	if (dsec >= 0) {
		fflush (stdout);
		read (tty,&answer,1);
	} else {
/* or simply wait */
		sleep((-dsec+5)/10);
		tcflush(tty,TCIFLUSH);		/* flush pending input */
	}
		
/* reset terminal */
	if (tcsetattr(tty,TCSANOW,&save)) perror(perr_str);
	return answer;
} /* wait_for_key */


/*****************
  Terminal Access
 *****************/

void clrscr() {
/*   ^^^^^^ */
	fflush(stdout);
	printf("%s",cl_string);
} /*clrscr*/


void gotoxy(int col,int lin) {
/*   ^^^^^^ */
	if (*cm_string!=0)
		printf ("%s",tgoto (cm_string,col-1,lin-1));
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


/****************
  Initialization
 ****************/

void os_init () {
/*   ^^^^ */
int     err;
char    *term;
struct termios tio;


/* Init TERMCAP: */
	if ((term = getenv("TERM")) == NULL) err = 2;
	else err = tgetent(termcap_buffer,term);
	switch (err) {
	case -1:	fprintf(stderr,"Can't access TERMINFO Directory!\n");
			break;
	case 0:		fprintf(stderr,"No entry for terminal '%s'!\n",term);
			break;
	case 2:		fprintf(stderr,"No $TERM Variable!\n");
			break;
	} /*switch*/
	if (err !=1) exit(1);
	term_entries = (char*) malloc(100);
	if ((cl_string=tgetstr("cl",&term_entries))==NULL) {
		fprintf (stderr,"\n'%s' can't clear the screen!\n",term);
		cl_string="";
	}
	if ((cm_string=tgetstr("cm",&term_entries))==NULL) {
		fprintf(stderr,"\n'%s' can't move the cursor!\n",term);
		cm_string="";
	}

/* get attributes of stdin and stdout; if failed, output was redirected! */
	if (tcgetattr(0,&tio) || tcgetattr(1,&tio)) {
		fprintf(stderr,"Redirection of stdin/stdout is not allowed!");
	}

} /*os_init*/

