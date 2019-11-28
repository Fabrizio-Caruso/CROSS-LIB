int bbc_error__errno;
char *bbc_error__message;


struct bbc_error {
	int type;
	char *message;
	int verbose;
}

int bbc_error__set_error(int errorno, int verbose) {
	switch (errorno) {
	case BBC_ERROR__OPEN_FAILED:
		bbc_error__message = "Open file failed";
		break;
	case BBC_ERROR__OUT_OF_MEMORY:
		bbc_error__message = "Out of memory";
		break;

	case BBC_ERROR__GENERIC:
	default:
		bbc_error__message = "Unspecified error";
		errorno = -1;
		break;
	}
	bbc_error__errno = errorno;

	if (verbose) fprintf(stderr, "%s\n", bbc_error__message);

	return 0;
}
