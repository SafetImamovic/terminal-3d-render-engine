#pragma once

#include <wchar.h>
#include "constants.h"
#ifdef _WIN32
#include <windows.h>
#endif

extern wchar_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH];

extern double elapsed_time;

#ifdef _WIN32

extern LARGE_INTEGER frequency, start, end;

#else

extern struct timespec start, end;

// Set up the signal handler for SIGWINCH
extern struct sigaction sa;

#endif
