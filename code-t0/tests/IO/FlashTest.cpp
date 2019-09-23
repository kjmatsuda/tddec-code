extern "C"
{
#include "Flash.h"
#include "MockIO.h"
}
#include "CppUTest/TestHarness.h"

TEST_GROUP(Flash)
{
	void setup()
	{
		MockIO_Create(4);
	}

	void teardown()
	{
		MockIO_Destroy();
	}
};

TEST(Flash, WriteSucceeds_ReadyImmediately)
{
	int result = 0;
	MockIO_Expect_Write(0, 0x40);
	MockIO_Expect_Write(0x1000, 0xBEEF);
	MockIO_Expect_ReadThenReturn(0, 1<<7);
	MockIO_Expect_ReadThenReturn(0x1000, 0xBEEF);

	result = Flash_Write(0x1000, 0xBEEF);

	MockIO_Verify_Complete();
	LONGS_EQUAL(0, result);
}
