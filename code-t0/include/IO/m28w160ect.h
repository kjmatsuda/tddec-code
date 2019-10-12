#ifndef INCLUDE_IO_M28W160ECT_H_
#define INCLUDE_IO_M28W160ECT_H_

#define ReadyBit					(1 << 7)
#define VppErrorBit					(1 << 3)
#define ProgramErrorBit				(1 << 4)
#define ProtectedBlockErrorBit		(1 << 1)

typedef enum
{
	CommandRegister = 0x0,
	StatusRegister = 0x0
} Flash_Registers;

typedef enum
{
	ProgramCommand = 0x40,
	Reset = 0xff
} Flash_Command;

#endif /* INCLUDE_IO_M28W160ECT_H_ */
