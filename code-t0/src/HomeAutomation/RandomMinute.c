#include "RandomMinute.h"
#include <stdlib.h>
#include <string.h>

static int bound = 0;

void RandomMinute_Create(int b)
{
	bound = b;
}

int RandomMinute_GetImpl(void)
{
	return bound - rand() % (bound * 2 + 1);
}

int (*RandomMinute_Get)(void) = RandomMinute_GetImpl;
