#include "../include/core/core.h"

wchar_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH];

#ifdef _WIN32

LARGE_INTEGER frequency, start, end;

#else

struct timespec start, end;

// Set up the signal handler for SIGWINCH
struct sigaction sa;

#endif

double elapsed_time = 0.0f;

// Initialize the global `core` struct and assign function pointers
core CORE = {
    .TERMINAL =
        {
                   .init_console  = init_console,
                   .hide_cursor   = hide_cursor,
                   .show_cursor   = show_cursor,
                   .render_buffer = render_buffer,
                   .clear_buffer  = clear_buffer,
                   },
    .UTILS = {
                   .draw_edges             = draw_edges,
                   .draw_coordinate_system = draw_coordinate_system,
                   .draw_stats             = draw_stats,
                   .init_measurement       = init_measurement,
                   .measure_start          = measure_start,
                   .measure_end            = measure_end,
                   .measure_diff           = measure_diff}
};
