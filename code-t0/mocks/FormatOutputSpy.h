#ifndef D_FormatOutputSpy_H
#define D_FormatOutputSpy_H

#include "Utils.h"

int FormatOutputSpy(const char * format, ...);
void FormatOutputSpy_Create(int size);
void FormatOutputSpy_Destroy(void);
const char * FormatOutputSpy_GetOutput(void);

#endif  /* D_FormatOutput_H */
