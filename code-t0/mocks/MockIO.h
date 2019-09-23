#ifndef MOCKS_MOCKIO_H_
#define MOCKS_MOCKIO_H_

#include "IO.h"

void MockIO_Create(int maxExpectations);
void MockIO_Destroy(void);
void MockIO_Expect_Write(ioAddress offset, ioData data);
void MockIO_Expect_ReadThenReturn(ioAddress offset, ioData data);
void MockIO_Verify_Complete(void);

#endif /* MOCKS_MOCKIO_H_ */
