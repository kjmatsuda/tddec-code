extern "C"
{
#include "Flash.h"
#include "MockIO.h"
#include "FakeMicroTime.h"
}
#include "CppUTest/TestHarness.h"

TEST_GROUP(Flash)
{
	ioAddress address;
	ioData data;
	int result;

	void setup()
	{
		address = 0x1000;
		data = 0xBEEF;
		result = -1;

		MockIO_Create(20);
		Flash_Create();
	}

	void teardown()
	{
		Flash_Destroy();
		MockIO_Verify_Complete();
		MockIO_Destroy();
	}
};

TEST(Flash, WriteSucceeds_ReadyImmediately)
{
	int result = 0;
	MockIO_Expect_Write(CommandRegister, ProgramCommand);
	MockIO_Expect_Write(address, data);
	MockIO_Expect_ReadThenReturn(StatusRegister, ReadyBit);
	MockIO_Expect_ReadThenReturn(address, data);

	result = Flash_Write(address, data);

	LONGS_EQUAL(FLASH_SUCCESS, result);
}

TEST(Flash, ProgramSucceedsNotImmediatelyReady)
{
	MockIO_Expect_Write(CommandRegister, ProgramCommand);
	MockIO_Expect_Write(address, data);
	MockIO_Expect_ReadThenReturn(StatusRegister, 0);
	MockIO_Expect_ReadThenReturn(StatusRegister, 0);
	MockIO_Expect_ReadThenReturn(StatusRegister, 0);
	MockIO_Expect_ReadThenReturn(StatusRegister, ReadyBit);
	MockIO_Expect_ReadThenReturn(address, data);

	result = Flash_Write(address, data);

	LONGS_EQUAL(FLASH_SUCCESS, result);
}

TEST(Flash, WriteFails_VppError)
{
	MockIO_Expect_Write(CommandRegister, ProgramCommand);
	MockIO_Expect_Write(address, data);
	MockIO_Expect_ReadThenReturn(StatusRegister, ReadyBit | VppErrorBit);
	MockIO_Expect_Write(CommandRegister, Reset);

	result = Flash_Write(address, data);

	LONGS_EQUAL(FLASH_VPP_ERROR, result);
}

TEST(Flash, WriteFails_ProgramError)
{
	MockIO_Expect_Write(CommandRegister, ProgramCommand);
	MockIO_Expect_Write(address, data);
	MockIO_Expect_ReadThenReturn(StatusRegister, ReadyBit | ProgramErrorBit);
	MockIO_Expect_Write(CommandRegister, Reset);

	result = Flash_Write(address, data);

	LONGS_EQUAL(FLASH_PROGRAM_ERROR, result);
}

TEST(Flash, WriteFails_ProtectedBlockError)
{
	MockIO_Expect_Write(CommandRegister, ProgramCommand);
	MockIO_Expect_Write(address, data);
	MockIO_Expect_ReadThenReturn(StatusRegister, ReadyBit | ProtectedBlockErrorBit);
	MockIO_Expect_Write(CommandRegister, Reset);

	result = Flash_Write(address, data);

	LONGS_EQUAL(FLASH_PROTECTED_BLOCK_ERROR, result);
}

TEST(Flash, WriteFails_FlashReadBackError)
{
	MockIO_Expect_Write(CommandRegister, ProgramCommand);
	MockIO_Expect_Write(address, data);
	MockIO_Expect_ReadThenReturn(StatusRegister, ReadyBit);
	MockIO_Expect_ReadThenReturn(address, data - 1);

	result = Flash_Write(address, data);

	LONGS_EQUAL(FLASH_READ_BACK_ERROR, result);
}

TEST(Flash, WriteSucceeds_IgnoresOtherBitsUntilReady)
{
	MockIO_Expect_Write(CommandRegister, ProgramCommand);
	MockIO_Expect_Write(address, data);
	MockIO_Expect_ReadThenReturn(StatusRegister, ~ReadyBit);
	MockIO_Expect_ReadThenReturn(StatusRegister, ReadyBit);
	MockIO_Expect_ReadThenReturn(address, data);

	result = Flash_Write(address, data);

	LONGS_EQUAL(FLASH_SUCCESS, result);
}

TEST(Flash, WriteFails_Timeout)
{
	FakeMicroTime_Init(0, 500);
	Flash_Create();

	MockIO_Expect_Write(CommandRegister, ProgramCommand);
	MockIO_Expect_Write(address, data);

	for (int ii = 0; ii < 10; ii++)
	{
		MockIO_Expect_ReadThenReturn(StatusRegister, ~ReadyBit);
	}

	result = Flash_Write(address, data);
	LONGS_EQUAL(FLASH_TIMEOUT_ERROR, result);
}

TEST(Flash, WriteFails_TimeoutAtEndOfTime)
{
	FakeMicroTime_Init(0xffffffff, 500);
	Flash_Create();
	MockIO_Expect_Write(CommandRegister, ProgramCommand);
	MockIO_Expect_Write(address, data);
	for (int ii = 0; ii < 10; ii++)
	{
		MockIO_Expect_ReadThenReturn(StatusRegister, ~ReadyBit);
	}
	result = Flash_Write(address, data);
	LONGS_EQUAL(FLASH_TIMEOUT_ERROR, result);
}
