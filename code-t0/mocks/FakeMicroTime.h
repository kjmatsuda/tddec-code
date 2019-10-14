#ifndef D_FakeMicroTime_H
#define D_FakeMicroTime_H

#include "MicroTime.h"

void FakeMicroTime_Init(uint32_t start, uint32_t increment);
uint32_t FakeMicroTime_GetDelayDuration(void);

#endif  /* D_FakeMicroTime_H */
