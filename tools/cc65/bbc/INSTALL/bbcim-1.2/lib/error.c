int bbcim_set_error(int errorno, int verbose) {
	switch (errorno) {
	case BBCIM_ERROR__OPEN_FAILED:
		bbcim_error = "Open file failed";
		break;
	case BBCIM_ERROR__OUT_OF_MEMORY:
		bbcim_error = "Out of memory";
		break;

	case BBCIM_ERROR__GENERIC:
	default:
		bbcim_error = "Unspecified error";
		errorno = -1;
		break;
	}
	bbcim_errno = errorno;

	if (verbose) fprintf(stderr, "%s\n", bbcim_error);

	return bbcim_errno;
}
