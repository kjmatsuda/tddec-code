#include "Flash.h"

static FlashStatus writeError(status);

void Flash_Create()
{

}
void Flash_Destroy()
{

}

int Flash_Write(ioAddress address, ioData data)
{
	ioData status = 0;

	IO_Write(CommandRegister, ProgramCommand);
	IO_Write(address, data);

	while (!(status & ReadyBit))
	{
		status = IO_Read(StatusRegister);
	}
	if (status != ReadyBit)
	{
		return writeError(status);
	}

	// Check read back error
	if (IO_Read(address) != data)
	{
		return FLASH_READ_BACK_ERROR;
	}

	return FLASH_SUCCESS;
}

static FlashStatus writeError(status)
{
	IO_Write(CommandRegister, Reset);

	if (status & VppErrorBit)
	{
		return FLASH_VPP_ERROR;
	}
	else if (status & ProgramErrorBit)
	{
		return FLASH_PROGRAM_ERROR;
	}
	else if (status & ProtectedBlockErrorBit)
	{
		return FLASH_PROTECTED_BLOCK_ERROR;
	}
	else
	{
		return FLASH_UNKNOWN_PROGRAM_ERROR;
	}
}
