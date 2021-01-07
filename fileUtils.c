#include "fileUtils.h"

void verifyFileOpen(FILE *fp, char *fname)
{
	if (!fp)
	{
		printf("error opening file %s. error number: %d , error message: %s. Exiting", fname, errno, strerror(errno));
		exit(1);
	}
}
