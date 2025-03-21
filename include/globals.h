#pragma once

#include <wchar.h>
#include "constants.h"
#ifdef _WIN32
#include <windows.h>
#endif

extern wchar_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH];

#ifdef _WIN32
extern LARGE_INTEGER frequency, start, end;
#endif
