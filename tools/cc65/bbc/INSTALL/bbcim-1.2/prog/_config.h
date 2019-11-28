/* bbcim configuration
 */

/*************** CONFIGURATION ************************/
/* Comment out for english messages */
//#define LANGUAGE_DUTCH

/* Console output: */
#define USE_ANSI_COLOURS 1

/* See lib/config.h for all other configuration */
/*************** END OF CONFIGURATION *****************/

#ifdef LANGUAGE_DUTCH
#  define TEKST_NL(x) x
#  define TEKST_ENG(x)
#else
#  define TEKST_NL(x)
#  define TEKST_ENG(x) x
#endif
