/* bbc_crc.h, by W.H.Scholten 2009-2014
 *
 * Calculates CRCs using the algorithm from the BBC microcomputer user guide p.399
 */

int bbc_fcrc(char *bestand, int verbose);
int bbc_data_crc(void *data, int length);
