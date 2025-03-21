#pragma once

#include "../../include/core/terminal.h"
#include "../../include/core/utils.h"

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
        void (*draw_stats)(double);
        void (*init_measurement)(void);
        void (*measure)(double *);

} utils;

// The CORE struct holds the TERMINAL struct as a member
typedef struct
{
        terminal TERMINAL; // Sub-struct for terminal-related functions
        utils    UTILS;

} core;

extern core CORE;
