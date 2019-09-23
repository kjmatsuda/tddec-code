#include "Flash.h"


void Flash_Create()
{

}
void Flash_Destroy()
{

}

int Flash_Write(ioAddress address, ioData data)
{
	IO_Write(CommandRegister, ProgramCommand);
	IO_Write(address, data);
	IO_Read(StatusRegister);
	IO_Read(address);

	return FLASH_SUCCESS;
}
