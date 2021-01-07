#ifndef __FILEUTILS_H
#define __FILEUTILS_H

#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
* Verifies a file was opened successfully.
* Prints an error message and exits if file open failed
*/
void verifyFileOpen(FILE *fp, char *fname);

#endif // !__FILEUTILS_H