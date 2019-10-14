#ifndef INCLUDE_IO_FLASH_H_
#define INCLUDE_IO_FLASH_H_

#include "IO.h"
#include "m28w160ect.h"
#include "MicroTime.h"

typedef enum
{
	FLASH_SUCCESS = 0,
	FLASH_VPP_ERROR,
	FLASH_PROGRAM_ERROR,
	FLASH_PROTECTED_BLOCK_ERROR,
	FLASH_UNKNOWN_PROGRAM_ERROR,
	FLASH_READ_BACK_ERROR,
	FLASH_TIMEOUT_ERROR
} FlashStatus;

#define FLASH_WRITE_TIMEOUT_IN_MICROSECONDS		(5000)

void Flash_Create();
void Flash_Destroy();

int Flash_Write(ioAddress address, ioData data);

#endif /* INCLUDE_IO_FLASH_H_ */
