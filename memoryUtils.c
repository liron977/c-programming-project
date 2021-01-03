#include "memoryUtils.h"


void *ver_malloc(size_t size)
{
	void *res = malloc(size);
	verifyAllocation(res);
	return res;
}


void *ver_realloc(void *data, size_t size)
{
	void *res = realloc(data, size);
	verifyAllocation(res);
	return res;
}


void verifyAllocation(void *arr)
{
	if (!arr)
	{
		printf("Memory allocation failed - exiting!");
		exit(1);
	}
}
