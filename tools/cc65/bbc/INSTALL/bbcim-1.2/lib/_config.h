/* Libbcim CONFIGURATION
 */

#ifndef _bbcim__config_h
#define _bbcim__config_h


/*************** CONFIGURATION ************************/
/* Comment out for english messages from the library */
//#define LANGUAGE_DUTCH

/*************** END OF CONFIGURATION *****************/


/* Characters to be replaced in filenames for BBC filename -> host filename
 * which could give problems, such as '/','\'. Add more if needed for you OS.
 * The following is for unix:
 */
char bbcim_filename_chars_bad[]     = { '\\', '/', ':', '*', '?', '\"', '<', '>', '|', '`', '\'', ' ' };
char bbcim_filename_chars_replace[] = { '_' , '_', '_', '_', '_', '_' , '_', '_', '_', '_', '_' , '_' };


#ifdef LANGUAGE_DUTCH
#  define TEKST_NL(x) x
#  define TEKST_ENG(x)
#else
#  define TEKST_NL(x)
#  define TEKST_ENG(x) x
#endif

#endif /* _bbcim__config_h */
