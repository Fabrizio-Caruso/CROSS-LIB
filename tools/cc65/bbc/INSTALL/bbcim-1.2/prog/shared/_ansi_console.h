#ifndef _whs_console_h
#define _whs_console_h

/* Comment out if your console has no ANSI colours or you don't want to use colours: */
#ifndef USE_ANSI_COLOURS
#  error "define USE_ANSI_COLOURS 1 or 0"
#endif

#if USE_ANSI_COLOURS==1

/* ANSI colour sequences */
#define CONSOLE_RESTORE       "\033[0m"
#define CONSOLE_BOLD          "\033[1m"
#define CONSOLE_ITALIC        "\033[3m" /* Usually not supported */
#define CONSOLE_UNDERLINE     "\033[4m" /* Not supported on standard FreeBSD terminal for example */
#define CONSOLE_INVERT        "\033[7m"
#define CONSOLE_BLINK         "\033[5m"
#define CONSOLE_BLINK_OFF     "\033[25m"


#define CONSOLE_BLACK         "\033[30m"
#define CONSOLE_RED           "\033[31m"
#define CONSOLE_GREEN         "\033[32m"
#define CONSOLE_YELLOW        "\033[33m"
#define CONSOLE_BLUE          "\033[34m"
#define CONSOLE_MAGENTA       "\033[35m"
#define CONSOLE_CYAN          "\033[36m"
#define CONSOLE_WHITE         "\033[37m"


#define CONSOLE_GRAY          "\033[1;30m" /* bright black */


#define CONSOLE_BG_BLACK      "\033[40m"
#define CONSOLE_BG_RED        "\033[41m"
#define CONSOLE_BG_GREEN      "\033[42m"
#define CONSOLE_BG_YELLOW     "\033[43m"
#define CONSOLE_BG_BLUE       "\033[44m"
#define CONSOLE_BG_MAGENTA    "\033[45m"
#define CONSOLE_BG_CYAN       "\033[46m"
#define CONSOLE_BG_WHITE      "\033[47m"



#else /* No ANSI colours */
#define CONSOLE_RESTORE       ""
#define CONSOLE_BOLD          ""
#define CONSOLE_ITALIC        ""
#define CONSOLE_UNDERLINE     ""
#define CONSOLE_INVERT        ""
#define CONSOLE_BLINK         ""
#define CONSOLE_BLINK_OFF     ""

#define CONSOLE_BLACK         ""
#define CONSOLE_RED           ""
#define CONSOLE_GREEN         ""
#define CONSOLE_YELLOW        ""
#define CONSOLE_BLUE          ""
#define CONSOLE_MAGENTA       ""
#define CONSOLE_CYAN          ""
#define CONSOLE_WHITE         ""
#define CONSOLE_GRAY          ""


#define CONSOLE_GRAY          ""


#define CONSOLE_BG_BLACK      ""
#define CONSOLE_BG_RED        ""
#define CONSOLE_BG_GREEN      ""
#define CONSOLE_BG_YELLOW     ""
#define CONSOLE_BG_BLUE       ""
#define CONSOLE_BG_MAGENTA    ""
#define CONSOLE_BG_CYAN       ""
#define CONSOLE_BG_WHITE      ""


#endif

#endif /*  _whs_console_h */
