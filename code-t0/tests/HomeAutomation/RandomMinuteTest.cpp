extern "C"
{
#include <stdio.h>
#include <string.h>
#include "RandomMinute.h"
}
#include "CppUTest/TestHarness.h"

enum { BOUND=30 };

TEST_GROUP(RandomMinute)
{
	int minute;

	void setup()
	{
		RandomMinute_Create(BOUND);
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
