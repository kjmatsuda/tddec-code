#include "FakeRandomMinute.h"

static int m_CurrentMinute = 0;
static int m_Increment = 0;

void FakeRandomMinute_SetFirstAndIncrement(int first, int increment)
{
	m_CurrentMinute = first;
	m_Increment = increment;
}

int FakeRandomMinute_Get(void)
{
	int retMinute = m_CurrentMinute;
	m_CurrentMinute += m_Increment;
	return retMinute;
}
