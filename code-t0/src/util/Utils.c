#include "Utils.h"

static int FormatOutput_Impl(const char *format, ...)
{
	// TODO 実装 FormatOutput_Impl
}

int (*FormatOutput)(const char *format, ...) = FormatOutput_Impl;
