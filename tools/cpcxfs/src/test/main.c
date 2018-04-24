char lorum[65536];

int main(int argc, char **argv)
{
	FILE *fh;
	int count;
	
	if (argc!=2)
		return -1;

	fh = fopen("lorum.txt","rb");
	if (fh!=NULL)
	{
		fread(lorum, sizeof(lorum), 1, fh);
		fclose(fh);
	}
	
	count = atoi(argv[2]);
	fh = fopen(argv[1],"w");
	if (fh!=NULL)
	{
		fwrite(lorum, count, 1, fh);
	
		fclose(fh);
	}

	return 0;
}

	