#include "Flash.h"

int Flash_Write(ioAddress address, ioData data)
{
	IO_Write(0, 0x40);
	IO_Write(address, data);
	IO_Read(0);
	IO_Read(address);

	return FLASH_SUCCESS;
}
