#pragma once

#include <wchar.h>
#include "constants.h"
#include "core/terminal.h"
#include "core/utils.h"
#ifdef _WIN32
#include <windows.h>
#endif

// The TERMINAL struct holds pointers to terminal functions
typedef struct
{
        void (*init_console)(void);
        void (*hide_cursor)(void);
        void (*show_cursor)(void);
        void (*render_buffer)(void);
        void (*clear_buffer)(void);

} terminal;

typedef struct
{
        void (*draw_edges)(wchar_t, wchar_t, wchar_t);
        void (*draw_coordinate_system)(void);
        void (*draw_stats)(void);
        void (*init_measurement)(void);
        void (*measure_start)(void);
        void (*measure_end)(void);
        void (*measure_diff)(void);

} utils;

// The CORE struct holds the TERMINAL struct as a member
typedef struct
{
        terminal TERMINAL; // Sub-struct for terminal-related functions
        utils    UTILS;

} core;

extern core CORE;

extern wchar_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH];

extern double elapsed_time;

#ifdef _WIN32

extern LARGE_INTEGER frequency, start, end;

#else

extern struct timespec start, end;

// Set up the signal handler for SIGWINCH
extern struct sigaction sa;

#endif
