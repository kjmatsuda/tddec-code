extern "C"
{
#include <stdio.h>
#include <string.h>
}
#include "CppUTest/TestHarness.h"

enum { BOUND=30 };

TEST_GROUP(RandomMinute)
{
	int minute;

	void setup()
	{
		RandomMinite_Create(BOUND);
		srand(1);
	}

	void teardown()
	{
	}

	void AssertMinuteIsInRange()
	{
		if (minute < -BOUND || minute > BOUND)
		{
			printf("bad minute value: %d\n", minute);
			FAIL("Minute out of range");
		}
	}
};

TEST(RandomMinute, GetIsInRange)
{
	for (int ii = 0; ii < 100; ii++)
	{
		minute = RandomMinute_Get();
		AssertMinuteIsInRange();
	}
}

TEST(RandomMinute, AllValuesPossible)
{
	int hit[2*BOUND + 1];
	memset(hit, 0, sizeof(hit));
	int ii;
	for (ii = 0; ii < 225; ii++)
	{
		minute = RandomMinute_Get();
		AssertMinuteIsInRange();
		hit[minute + BOUND]++;
	}
	for (ii = 0; ii < 2* BOUND + 1; ii++)
	{
		CHECK(hit[ii] > 0);
	}
}


