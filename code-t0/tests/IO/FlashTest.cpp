#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

extern "C"
{
#include "CppUTestExt/MockSupport_c.h"
#include "Flash.h"
#include "m28w160ect.h"
#include "FakeMicroTime.h"

void IO_Write(ioAddress addr, ioData value)
{
	mock_c()->actualCall("IO_Write")
		->withIntParameters("addr", addr)
		->withIntParameters("value", value);
}

ioData IO_Read(ioAddress addr)
{
	mock_c()->actualCall("IO_Read")
		->withIntParameters("addr", addr);
	return mock_c()->returnValue().value.intValue;
}
}

TEST_GROUP(Flash)
{
	ioAddress address;
	ioData data;
	int result;

	void setup()
	{
		address = 0xfeed;
		data = 0x1dea;
		result = -1;
	}

	void teardown()
	{
		mock().checkExpectations();
		mock().clear();
	}
};

TEST(Flash, WriteSucceeds_ReadyImmediately)
{
	mock().expectOneCall("IO_Write")
			.withParameter("addr", CommandRegister)
			.withParameter("value", ProgramCommand);
	mock().expectOneCall("IO_Write")
			.withParameter("addr", address)
			.withParameter("value", data);
	mock().expectOneCall("IO_Read")
			.withParameter("addr", StatusRegister)
			.andReturnValue(ReadyBit);
	mock().expectOneCall("IO_Read")
			.withParameter("addr", address)
			.andReturnValue(data);

	int result = Flash_Write(address, data);

	LONGS_EQUAL(FLASH_SUCCESS, result);
}

//TEST(Flash, ProgramSucceedsNotImmediatelyReady)
//{
//	MockIO_Expect_Write(CommandRegister, ProgramCommand);
//	MockIO_Expect_Write(address, data);
//	MockIO_Expect_ReadThenReturn(StatusRegister, 0);
//	MockIO_Expect_ReadThenReturn(StatusRegister, 0);
//	MockIO_Expect_ReadThenReturn(StatusRegister, 0);
//	MockIO_Expect_ReadThenReturn(StatusRegister, ReadyBit);
//	MockIO_Expect_ReadThenReturn(address, data);
//
//	result = Flash_Write(address, data);
//
//	LONGS_EQUAL(FLASH_SUCCESS, result);
//}
//
//TEST(Flash, WriteFails_VppError)
//{
//	MockIO_Expect_Write(CommandRegister, ProgramCommand);
//	MockIO_Expect_Write(address, data);
//	MockIO_Expect_ReadThenReturn(StatusRegister, ReadyBit | VppErrorBit);
//	MockIO_Expect_Write(CommandRegister, Reset);
//
//	result = Flash_Write(address, data);
//
//	LONGS_EQUAL(FLASH_VPP_ERROR, result);
//}
//
//TEST(Flash, WriteFails_ProgramError)
//{
//	MockIO_Expect_Write(CommandRegister, ProgramCommand);
//	MockIO_Expect_Write(address, data);
//	MockIO_Expect_ReadThenReturn(StatusRegister, ReadyBit | ProgramErrorBit);
//	MockIO_Expect_Write(CommandRegister, Reset);
//
//	result = Flash_Write(address, data);
//
//	LONGS_EQUAL(FLASH_PROGRAM_ERROR, result);
//}
//
//TEST(Flash, WriteFails_ProtectedBlockError)
//{
//	MockIO_Expect_Write(CommandRegister, ProgramCommand);
//	MockIO_Expect_Write(address, data);
//	MockIO_Expect_ReadThenReturn(StatusRegister, ReadyBit | ProtectedBlockErrorBit);
//	MockIO_Expect_Write(CommandRegister, Reset);
//
//	result = Flash_Write(address, data);
//
//	LONGS_EQUAL(FLASH_PROTECTED_BLOCK_ERROR, result);
//}
//
//TEST(Flash, WriteFails_FlashReadBackError)
//{
//	MockIO_Expect_Write(CommandRegister, ProgramCommand);
//	MockIO_Expect_Write(address, data);
//	MockIO_Expect_ReadThenReturn(StatusRegister, ReadyBit);
//	MockIO_Expect_ReadThenReturn(address, data - 1);
//
//	result = Flash_Write(address, data);
//
//	LONGS_EQUAL(FLASH_READ_BACK_ERROR, result);
//}
//
//TEST(Flash, WriteSucceeds_IgnoresOtherBitsUntilReady)
//{
//	MockIO_Expect_Write(CommandRegister, ProgramCommand);
//	MockIO_Expect_Write(address, data);
//	MockIO_Expect_ReadThenReturn(StatusRegister, ~ReadyBit);
//	MockIO_Expect_ReadThenReturn(StatusRegister, ReadyBit);
//	MockIO_Expect_ReadThenReturn(address, data);
//
//	result = Flash_Write(address, data);
//
//	LONGS_EQUAL(FLASH_SUCCESS, result);
//}
//
//TEST(Flash, WriteFails_Timeout)
//{
//	FakeMicroTime_Init(0, 500);
//	Flash_Create();
//
//	MockIO_Expect_Write(CommandRegister, ProgramCommand);
//	MockIO_Expect_Write(address, data);
//
//	for (int ii = 0; ii < 10; ii++)
//	{
//		MockIO_Expect_ReadThenReturn(StatusRegister, ~ReadyBit);
//	}
//
//	result = Flash_Write(address, data);
//	LONGS_EQUAL(FLASH_TIMEOUT_ERROR, result);
//}
//
//TEST(Flash, WriteFails_TimeoutAtEndOfTime)
//{
//	FakeMicroTime_Init(0xffffffff, 500);
//	Flash_Create();
//	MockIO_Expect_Write(CommandRegister, ProgramCommand);
//	MockIO_Expect_Write(address, data);
//	for (int ii = 0; ii < 10; ii++)
//	{
//		MockIO_Expect_ReadThenReturn(StatusRegister, ~ReadyBit);
//	}
//	result = Flash_Write(address, data);
//	LONGS_EQUAL(FLASH_TIMEOUT_ERROR, result);
//}
