#ifndef __MEMORY_UTILS_H
#define __MEMORY_UTILS_H

#include <stdlib.h>
#include <stdio.h>

/*
* Allocates memory using malloc, verifying allocation succeeded.
* If allocation fails, prints an error message and exits the program
*/
void* ver_malloc(size_t size);

/*
* Re-allocates memory using realloc, verifying allocation succeeded.
* If allocation fails, prints an error message and exits the program
*/
void* ver_realloc(void *data, size_t size);

/*
* verifies an array is allocated (not null).
* throws error message and exits if array isn't allocated.
* should be used after allocating new memory to a variable
*/
void verifyAllocation(void *arr);

#endif // !__MEMORY_UTILS_H

