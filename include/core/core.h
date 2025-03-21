#pragma once

#include "../../include/core/terminal.h"

// The TERMINAL struct holds pointers to terminal functions
typedef struct
{
        void (*init_console)(void);
        void (*hide_cursor)(void);
        void (*show_cursor)(void);
        void (*render_buffer)(void);
        void (*clear_buffer)(void);
        void (*draw_edges)(wchar_t, wchar_t, wchar_t);
        void (*draw_coordinate_system)(void);
} terminal;

// The CORE struct holds the TERMINAL struct as a member
typedef struct
{
        terminal TERMINAL; // Sub-struct for terminal-related functions

} core;

extern core CORE;
