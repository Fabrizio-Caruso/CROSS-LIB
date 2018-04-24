/*				<<<<Last Modified: Tue Jan 16 11:15:24 1996>>>>
*/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define LINELEN 256

int main () {

const char	help_name[] = "cpcfs.hlp";
const char	template_name[] = "template.doc";
const char	doc_name[] = "doc";

char		line[LINELEN];
char		topic[20];
FILE	*help, *template, *doc;
int	found;	/* as bool */

	help = fopen(help_name,"r");
	if (help==NULL) {
		fprintf(stderr,"I cannot open \"%s\"",help_name);
		exit(1);
	}

	template = fopen(template_name,"r");
	if (template==NULL) {
		fprintf(stderr,"I cannot open \"%s\"",template_name);
		exit(1);
	}

	doc = fopen(doc_name,"w");
	if (doc==NULL) {
		fprintf(stderr,"I cannot open \"%s\"",doc_name);
		exit(1);
	}


	while (fgets(line,LINELEN,template) != NULL) {
		if (line[0]=='~') {
			strncpy(topic,line,20);
			if (topic[strlen(topic)-1]=='\n') {
				topic[strlen(topic)-1] = 0;
			}
			fseek(help,0L,SEEK_SET);
			found = 0;
			while (fgets(line,LINELEN,help) != NULL) {
				if (found && line[0]!='~')
					fputs(line,doc);
				if (found && line[0]=='~') {
					found=0;
					continue;
				}
				if (!found&& line[0]!='~')
					continue;
				if (!found&& line[0]=='~')
					found = (strstr(line,topic)!=NULL);
			}
		} else {
			fputs(line,doc);
		}
	}

	fclose(doc);
	fclose(template);
	fclose(help);

	printf("Merged \"%s\" and \"%s\" to \"%s\"\n",
		template_name, help_name, doc_name);
	printf("You can now copy \"%s\" to \"cpcfs.doc\"\n",
		doc_name);

	return 0;	
}
