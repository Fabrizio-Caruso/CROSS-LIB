/* bbc_filenames.h, by W.H.Scholten 2009-2014
 *
 * This is about renaming files due to issues with BBC micro file names
 * having characters that can't be represented on other systems, etc.
 */

void bbc_replace_bad_chars(char *unix_bbcfile_naam);
int bbc_printable_bbcfilename(char *name);
int bbc_rename_sequential(char *name);

#if !defined(True)
# define True  1
# define False 0
#endif
